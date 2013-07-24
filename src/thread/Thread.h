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
#include "ThreadCondition.h"
#include <pthread.h>

namespace sdfs {

typedef union thread_data
{
	int fd;
	void *ptr;
}thread_data_t;

typedef struct ThreadConfig
{
	IRunnable	*runner;
	thread_data_t arg;
	bool		bIsContinue;
	bool		bIsIdle;
	bool		bIsClosed;
}ThreadConfig;

//CThreadCondition ThreadConfig::cond = new CThreadCondition;

void *thread_entrance(void *arg);

class Thread {
public:
	Thread(IRunnable *func = NULL,void *arg = NULL, bool joinable=false, thread_func entrance = thread_entrance);
	virtual ~Thread();

	int Start();
	void setStackSize(const int stacksize);
	int Killself();
	bool isAlive();
	pthread_t getPid();
	int Join(void **result);
	virtual void setRunnable(IRunnable &runner, void *arg);
	virtual void setRunnable(IRunnable &runner, int fd);
	bool IsIdle();
	void Stop();

	//virtual void * Run(void *arg) = 0;

private:
	ThreadConfig m_pArg;
	pthread_t	m_pid;
	int m_bJoinable;
	int m_nStackSize;
	thread_func m_fEntrance;
//protected:
//	thread_func m_pfunc;
};



} /* namespace sdfs */
#endif /* THREAD_H_ */
