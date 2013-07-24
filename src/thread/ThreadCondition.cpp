/*
 * ThreadCondition.cpp
 *
 *  Created on: 2013年7月23日
 *      Author: sun
 */

#include "ThreadCondition.h"
#include<sys/time.h>

namespace sdfs {

CThreadCondition::CThreadCondition() {
	m_cond = PTHREAD_COND_INITIALIZER;
	m_mutex = PTHREAD_MUTEX_INITIALIZER;
}

CThreadCondition::~CThreadCondition() {
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}

int CThreadCondition::Lock()
{
	return pthread_mutex_lock(&m_mutex);
}

int CThreadCondition::Unlock()
{
	return pthread_mutex_unlock(&m_mutex);
}

int CThreadCondition::Signal()
{
	return pthread_cond_signal(&m_cond);
}

int CThreadCondition::Wait()
{
	return pthread_cond_wait(&m_cond, &m_mutex);
}

int CThreadCondition::WaitTime(int sec, long nanosec)
{
	struct timespec spce;
	struct timeval now;

	gettimeofday(&now, NULL);

	spce.tv_sec=now.tv_sec+sec;
	spce.tv_nsec = now.tv_usec*1000 + nanosec;
	return pthread_cond_timedwait(&m_cond, &m_mutex, &spce);
}

int CThreadCondition::Broadcast()
{
	return pthread_cond_broadcast(&m_cond);
}

} /* namespace sdfs */
