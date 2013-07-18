/*
 * Thread.h
 *
 *  Created on: 2013-7-17
 *      Author: sun
 */

#ifndef THREAD_H_
#define THREAD_H_
#include "../common/common_define.h"
#include "IRunnable.h"
#include <pthread.h>

namespace sdfs {

struct ThreadArg
{
	IRunnable	*runner;
	void		*arg;
};


class Thread {
public:
	Thread(IRunnable &func,void *arg, bool joinable=false);
	virtual ~Thread();

	int Start();
	void setStackSize(const int stacksize);
	int Killself();
	bool isAlive();
	pthread_t getPid();
	int Join(void **result);

	//virtual void * Run(void *arg) = 0;

private:
	struct ThreadArg m_pArg;
	pthread_t m_pid;
	bool isStart;
	int m_bJoinable;
	int m_nStackSize;
//protected:
//	thread_func m_pfunc;
};



} /* namespace sdfs */
#endif /* THREAD_H_ */
