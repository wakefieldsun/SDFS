/*
 * Thread.cpp
 *
 *  Created on: 2013-7-17
 *      Author: sun
 */

#include "Thread.h"
#include <signal.h>
#include <sys/epoll.h>
#include <errno.h>
#include "../common/ThreadHelper.h"
#include "TaskQueue.h"
#include "../common/Log.h"

namespace sdfs {


void *thread_entrance(void *arg)
{
	Log::Debug("thread: 0x%x entered", pthread_self());
	ThreadConfig *targ = (ThreadConfig *)arg;
	targ->bIsClosed = false;
	IRunnable *runner = targ->runner;
	void *result = NULL;
	while(targ->bIsContinue)
	{
		Log::Debug("thread:0x%x is working...", pthread_self());
		targ->bIsIdle = false;
		void *arg = targ->arg.ptr;
		result = runner->Run(arg);
		Log::Debug("thread:0x%x task end", pthread_self());
	}
	Log::Debug("thread: 0x%x exited", pthread_self());
	targ->bIsClosed = true;
	pthread_exit(NULL);
}

Thread::Thread(IRunnable *runner, void *arg, bool joinable, thread_func entrance) {
	int result;
	this->m_nStackSize = DEFAULT_STACK_SIZE;
	this->m_pArg.runner = runner;
	if(arg != NULL)
		this->m_pArg.arg.ptr = arg;
	this->m_pArg.bIsContinue = true;
	this->m_pArg.bIsIdle = true;
	this->m_pid = (unsigned long int)0L;

	m_bJoinable = joinable;
	m_fEntrance = entrance;
}

Thread::~Thread() {
}

void Thread::setStackSize(const int stacksize)
{
	this->m_nStackSize = stacksize;
}

int Thread::Start()
{
	int result;
	if(isAlive())
	{
		Log::Warning("thread has already startup");
		return 0;
	}
	int type = m_bJoinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED;
	//void *IRunnable::(*Run)(void *arg) func = m_core->Run;
	result = ThreadHelper::create(&m_pid, m_fEntrance, &m_pArg, m_nStackSize, type);
	return result;
}

int Thread::Killself()
{
	if(!isAlive())
		return 0;
	return ThreadHelper::kill(&m_pid);
}

pthread_t Thread::getPid()
{
	return m_pid;
}

bool Thread::isAlive()
{
	Log::Debug("isAlive, isClosed: %d", m_pArg.bIsClosed);
	if(m_pid <= (unsigned long int)0L || m_pArg.bIsClosed)
		return false;
	int result = pthread_kill(m_pid, 0);
	if(result == ESRCH)
	{
		return false;
	}
	else if(result == EINVAL)
	{
		Log::Warning("File: "__FILE__", line: %d, " \
				"call pthread_kill, Unsupport Signal");
		return false;
	}
	return true;
}

int Thread::Join(void **result)
{
	if(m_bJoinable)
	{
		int err = pthread_join(m_pid, result);
		if(err != 0)
		{
			Log::Error("File: "__FILE__", line: %d, " \
				"call pthread_join, errno: %d, info: %s",
				__LINE__, err, STRERROR(err));
		}
		return err;
	}
	*result = NULL;
	return 0;
}

void Thread::setRunnable(IRunnable &runner, void *arg)
{
	m_pArg.runner = &runner;
	m_pArg.arg.ptr = arg;
}

void Thread::setRunnable(IRunnable &runner, int fd)
{
	m_pArg.runner = &runner;
	m_pArg.arg.fd = fd;
}

bool Thread::IsIdle()
{
	return m_pArg.bIsIdle;
}

void Thread::Stop()
{
	if(isAlive()){
		m_pArg.runner->Stop();
		m_pArg.bIsContinue = false;
	}
}

} /* namespace sdfs */
