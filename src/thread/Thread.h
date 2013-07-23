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

struct ThreadConfig
{
	IRunnable	*runner;
	thread_data_t arg;
	bool		bIsContinue;
	bool		bIsWait;
	bool		bIsIdle;
	bool		bIsClosed;
	CThreadCondition	cond;
};


class Thread {
public:
	Thread(IRunnable *func = NULL,void *arg = NULL, bool joinable=false);
	virtual ~Thread();

	int Start();
	void setStackSize(const int stacksize);
	int Killself();
	bool isAlive();
	pthread_t getPid();
	int Join(void **result);
	virtual void setRunnable(IRunnable &runner, void *arg);
	virtual void setRunnable(IRunnable &runner, int fd);
	void setIsWait(bool flag);
	bool IsIdle();
	void Stop();
	void Notify(int sockfd);

	//virtual void * Run(void *arg) = 0;

private:
	struct ThreadConfig m_pArg;
	pthread_t	m_pid;
	int m_bJoinable;
	int m_nStackSize;
//protected:
//	thread_func m_pfunc;
};



} /* namespace sdfs */
#endif /* THREAD_H_ */
