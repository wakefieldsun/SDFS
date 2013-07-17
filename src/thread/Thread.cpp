/*
 * Thread.cpp
 *
 *  Created on: 2013-7-17
 *      Author: sun
 */

#include "Thread.h"
#include "../common/ThreadHelper.h"
#include "../common/Log.h"

namespace sdfs {

Thread::Thread(thread_func func, void *arg ) {
	this->m_func = func;
	this->m_nStackSize = DEFAULT_STACK_SIZE;
	this->m_pArg = arg;
	this->isStart =false;
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
	if(isStart)
	{
		Log::Warning("thread has already startup");
		return 0;
	}
	Log::Debug("Thread Start");
	result = ThreadHelper::create(&m_pid, m_func, m_pArg, m_nStackSize);
	isStart = result == 0 ? true : false;
	return result;
}

int Thread::Killself()
{
	return ThreadHelper::kill(&m_pid);
}

pthread_t Thread::getPid()
{
	return m_pid;
}

} /* namespace sdfs */
