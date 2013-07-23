/*
 * ThreadPool.h
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Thread.h"
#include "TaskQueue.h"
#include "Pipe.h"
#include "IRunnable.h"
#include <signal.h>
#include <map>
#include <sys/epoll.h>

namespace sdfs {

class ThreadPool : public IRunnable{
public:
	ThreadPool(IRunnable& runner, int size = THREAD_POOL_SIZE);
	virtual ~ThreadPool();

	int addTask(int sockfd);

	void *Run(void *arg);

	void Stop();

	int TaskSize();

protected:
	Thread* SelectThread();
private:
	Thread** m_pThreads;
	int		m_nSize;
	int		m_epollfd;
	Pipe 	m_pipe;
	bool	m_bContinue;
	int		m_nTasknum;
	std::vector<int> *m_pFds;
};

} /* namespace sdfs */
#endif /* THREADPOOL_H_ */
