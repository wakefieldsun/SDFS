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

ThreadPool::ThreadPool(IRunnable &runner, int size) {
	m_nSize = size;
	m_nTasknum = 0;
	m_pThreads = new Thread*[size];
	for(int i = 0 ; i < size ; ++i)
		m_pThreads[i] = new Thread(&runner, NULL, false);
	m_pFds = new std::vector<int>;
	m_epollfd = epoll_create(EPOLL_MAX_SIZE);
	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = m_pipe.GetReadDescriptor();
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
			m_pThreads[i]->Start();
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
	Log::Debug("Task Thread is working...");
	epoll_event events[EPOLL_MAX_SIZE];
	int result;
	int sockfd;
	while(m_bContinue){
		//Log::Debug("Task Thread is waiting...");
		int nfds = epoll_wait(m_epollfd, events, EPOLL_MAX_SIZE, THREAD_POOL_WAITTIME);

		if(nfds <= 0)
		{
			if(TaskQueue::GetInstance()->IsEmpty())
				continue;
			Log::Debug("No new event, deal with task in queue...");
			Thread* pThread = SelectThread();
			while(pThread != NULL)
			{
				Task* task = TaskQueue::GetInstance()->GetTask();
				pThread->Notify(task->sockfd);
				pThread = SelectThread();
			}
			continue;
		}
		for(int i = 0 ; i < nfds ; ++i)
		{
			Log::Debug("New event...");
			int readfd = events[i].data.fd;
			if(readfd == m_pipe.GetReadDescriptor())
			{
				Log::Debug("New Connection...");
				//new connection
				result = read(readfd, &sockfd, sizeof(int));

				Task task;
				task.sockfd = sockfd;
				task.taskid = TaskQueue::GetInstance()->GetSize()+1;

				TaskQueue::GetInstance()->Add(task);

				epoll_event event;
				event.events = EPOLLIN|EPOLLET;
				event.data.fd = sockfd;

				result = epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &event);
				if(result != 0)
				{
					Log::Error("file:"__FILE__", line: %d, "
						"failed to call epoll_ctl, errno: %d, "
						"info: %s", __LINE__, errno, STRERROR(errno));
				}
				m_pFds->push_back(sockfd);
			}
			else
			{
				Log::Debug("New Task...");
				//TODO:existing connection, point a thread work on it
				Task task;
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

