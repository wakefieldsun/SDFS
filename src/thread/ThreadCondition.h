/*
 * ThreadCondition.h
 *
 *  Created on: 2013年7月23日
 *      Author: sun
 */

#ifndef THREADCONDITION_H_
#define THREADCONDITION_H_

#include <pthread.h>
#include "Mutex.h"
#include "../common/common_define.h"

namespace sdfs {

class CThreadCondition {
public:
	CThreadCondition();
	virtual ~CThreadCondition();

	int Lock();

	int Unlock();

	int Signal();

	int Wait();

	int WaitTime(int sec = THREAD_COND_WAIT_SEC, long nanosec = THREAD_COND_WAIT_NSEC);

	int Broadcast();

private:
	pthread_cond_t	m_cond;
	pthread_mutex_t	m_mutex;
};

} /* namespace sdfs */
#endif /* THREADCONDITION_H_ */
