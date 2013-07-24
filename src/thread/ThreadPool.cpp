/*
 * ThreadPool.cpp
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#include "ThreadPool.h"
#include "../common/Log.h"
#include "../common/common_define.h"
#include <errno.h>
#include <stdlib.h>

namespace sdfs {

void *pool_thread_entrance(void *arg)
{
	Log::Debug("pool_thread_entrance");
	Log::Debug("thread: 0x%x entered", pthread_self());
	ThreadConfig *targ = (ThreadConfig *)arg;
	targ->bIsClosed = false;
	IRunnable *runner = targ->runner;
	void *result = NULL;
	int err = 0;
	Task task;
	while(targ->bIsContinue)
	{
		Log::Debug("thread: 0x%x is getting a task...", pthread_self());
		err = TaskQueue::GetInstance()->GetTask(task);
		if(err != 0)
		{
			continue;
		}
		Log::Debug("thread:0x%x is working, task id: %d...", pthread_self(), task.taskid);
		result = runner->Run(&task);
		Log::Debug("thread:0x%x task end", pthread_self());
	}
	Log::Debug("thread: 0x%x exited", pthread_self());
	targ->bIsClosed = true;
	pthread_exit(NULL);
}

ThreadPool::ThreadPool(IRunnable &runner, int size) {
	m_nSize = size;
	m_nTasknum = 0;
	m_pThreads = new Thread*[size];
	for(int i = 0 ; i < size ; ++i)
		m_pThreads[i] = new Thread(&runner, NULL, false, pool_thread_entrance);
	m_pFds = new std::vector<int>;
	m_epollfd = epoll_create(EPOLL_MAX_SIZE);
	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = m_pipe.GetReadDescriptor();
	m_bContinue = true;
	int result = epoll_ctl(m_epollfd, EPOLL_CTL_ADD, \
			m_pipe.GetReadDescriptor(), &event);
	if(result != 0)
	{
		Log::Crit("file:"__FILE__", line: %d, "
				"failed to call epoll_ctl, errno: %d, "
				"info: %s", __LINE__, result, STRERROR(result));
		exit(0);
	}
	for(int i = 0 ; i < size ; ++i)
	{
		result = m_pThreads[i]->Start();
		if(result != 0)
		{
			Log::Error("file:"__FILE__", line: %d, "
					"failed to call Start, errno: %d, "
					"info: %s", __LINE__, result, STRERROR(result));
			continue;
		}

	}
}

int ThreadPool::addTask(int sockfd)
{
	int result =  write(m_pipe.GetWriteDescriptor(), &sockfd, sizeof(int));
	if(result <= 0)
	{
		Log::Error("file:"__FILE__", line: %d, "
				"failed to call write, errno: %d, "
				"info: %s", __LINE__, errno, STRERROR(errno));
	}
	return result;
}

Thread* ThreadPool::SelectThread()
{
	for(int i = 0 ; i < m_nSize ; ++i)
	{
		Log::Debug("look for idle thread...");
		if(m_pThreads[i]->IsIdle())
		{
			Log::Debug("return: 0x%x", m_pThreads[i]->getPid());
			return m_pThreads[i];
		}
	}
	return NULL;
}

void* ThreadPool::Run(void *arg)
{
	epoll_event events[EPOLL_MAX_SIZE];
	int result;
	int sockfd;
	Task task;
	while(m_bContinue){
		//Log::Debug("Task Thread is waiting...");
		int nfds = epoll_wait(m_epollfd, events, EPOLL_MAX_SIZE, THREAD_POOL_WAITTIME);

		if(nfds <= 0)
		{
			continue;
		}
		for(int i = 0 ; i < nfds ; ++i)
		{
			//Log::Debug("New event...");
			int readfd = events[i].data.fd;
			if(readfd == m_pipe.GetReadDescriptor())
			{
				//Log::Debug("New Connection...");
				//new connection
				result = read(readfd, &sockfd, sizeof(int));
				if(result < 0)
				{
					Log::Error("file:"__FILE__", line: %d, "
						"failed to call epoll_ctl, errno: %d, "
						"info: %s", __LINE__, result, STRERROR(result));
					continue;
				}

				epoll_event event;
				event.events = EPOLLIN|EPOLLET;
				event.data.fd = sockfd;

				result = epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &event);
				if(result != 0)
				{
					Log::Error("file:"__FILE__", line: %d, "
						"failed to call epoll_ctl, errno: %d, "
						"info: %s", __LINE__, result, STRERROR(result));
				}
				m_pFds->push_back(sockfd);
			}
			else
			{
				//Log::Debug("New Task...");
				task.sockfd = sockfd;
				task.taskid = TaskQueue::GetInstance()->GetSize()+1;
				TaskQueue::GetInstance()->Add(task);
			}

		}
	}
	pthread_exit(NULL);
}


void ThreadPool::Stop()
{
	Log::Debug("ThreadPool: Stop");
	m_bContinue = false;
	epoll_event event = {0,{0}};
	Log::Debug("ThreadPool: remove event");
	int result = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, m_pipe.GetReadDescriptor(), &event);
	if(result != 0)
	{
		Log::Error("file:"__FILE__", line: %d, "
			"failed to call epoll_ctl, errno: %d, "
			"info: %s", __LINE__, errno, STRERROR(errno));
	}
	for(int i = 0 ; i < m_pFds->size() ; ++i)
	{
		int result = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, m_pFds->at(i), &event);
		if(result != 0)
		{
			Log::Error("file:"__FILE__", line: %d, "
				"failed to call epoll_ctl, errno: %d, "
				"info: %s", __LINE__, errno, STRERROR(errno));
		}
	}
	Log::Debug("ThreadPool: stop thread pool");
	for(int i = 0 ; i < m_nSize ; ++i)
	{
		Log::Debug("ThreadPool: stopping thread: %d", i);
		m_pThreads[i]->Stop();
	}
	result = TaskQueue::GetInstance()->StopWaiting();
	if(result != 0)
	{
		Log::Error("file:"__FILE__", line: %d, "
			"failed to call StopWaiting, errno: %d, "
			"info: %s", __LINE__, result, STRERROR(result));
	}
	Log::Debug("ThreadPool: waiting close...");
	for(int i = 0 ; i < m_nSize ; ++i)
	{
		Log::Debug("ThreadPool: waiting thread: %d", i);
		while(m_pThreads[i]->isAlive())
			sleep(1);
	}
	Log::Debug("ThreadPool: Stop all");

}

int ThreadPool::TaskSize()
{
	return TaskQueue::GetInstance()->GetSize();
}

ThreadPool::~ThreadPool() {
	delete m_pThreads;
}

} /* namespace sdfs */

