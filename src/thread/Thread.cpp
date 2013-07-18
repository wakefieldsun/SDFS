/*
 * Thread.cpp
 *
 *  Created on: 2013-7-17
 *      Author: sun
 */

#include "Thread.h"
#include <signal.h>
#include "../common/ThreadHelper.h"
#include "../common/Log.h"

namespace sdfs {

void *thread_runner(void *arg)
{
	ThreadArg *targ = (ThreadArg *)arg;

	IRunnable *runner = targ->runner;
	return runner->Run(targ->arg);
}

Thread::Thread(IRunnable &runner, void *arg, bool joinable) {
	//this->m_pfunc = func;
	this->m_nStackSize = DEFAULT_STACK_SIZE;
	this->m_pArg.runner = &runner;
	this->m_pArg.arg = arg;
	this->isStart =false;
	m_pid = (unsigned long int)0L;
	m_bJoinable = joinable;
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
	result = ThreadHelper::create(&m_pid, &thread_runner, &m_pArg, m_nStackSize, type);
	isStart = result == 0 ? true : false;
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
	if(m_pid <= (unsigned long int)0L)
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

} /* namespace sdfs */
