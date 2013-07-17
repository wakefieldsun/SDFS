/*
 * Thread.h
 *
 *  Created on: 2013-7-17
 *      Author: sun
 */

#ifndef THREAD_H_
#define THREAD_H_
#include "../common/common_define.h"
#include <pthread.h>

namespace sdfs {

class Thread {
public:
	Thread(thread_func func, void *arg);
	virtual ~Thread();

	int Start();
	void setStackSize(const int stacksize);
	int Killself();
	pthread_t getPid();

private:
	thread_func m_func;
	void *m_pArg;
	pthread_t m_pid;
	bool isStart;
	int m_nStackSize;
};

} /* namespace sdfs */
#endif /* THREAD_H_ */
