/*
 * ThreadHelper.h
 *
 *  Created on: 2013-7-16
 *      Author: sun
 */

#ifndef THREADHELPER_H_
#define THREADHELPER_H_
#include <pthread.h>

namespace sdfs {

class ThreadHelper {
private:
	ThreadHelper();
	~ThreadHelper();
public:
	/**
	 * pthread_mutexattr_t: configure pthread_mutex_t
	 *  pthread_mutexattr_init
	 *  pthread_mutexattr_destroy
	 *  pthread_mutexattr_getshared
	 *  pthread_mutexattr_setshared | PTHREAD_PROCESS_SHEARED,PTHREAD_PROCESS_PRIVATE
	 *  pthread_mutexattr_get/settype | PTHREAD_MUTEX_NORMAL,PTHREAD_MUTEX_ERRCHECK
	 *  								PTHREAD_MUTEX_RECURSIVE,PTHREAD_MUTEX_DEFAULT
	 */
	static int init_lock(pthread_mutex_t &mutex, int type=PTHREAD_MUTEX_ERRORCHECK);
	/**
	 * pthread_attr_t: configure default attribute of thread
	 * 	pthread_attr_init
	 * 	pthread_attr_destroy
	 * 	pthread_attr_getdetachstate
	 * 	pthread_attr_setdetachstate | PTHREAD_CREATE_DETACHED,PTHREAD_CREATE_JOINABLE
	 * 	pthread_attr_getstacksize
	 * 	pthread_attr_setstacksize
	 */
	static int init_attr(pthread_attr_t &attr, const int stacksize, int type=PTHREAD_CREATE_DETACHED);
	static int create(pthread_t *pid, void *(*startfunc)(void *), \
			void *arg, const int stacksize, int type);
	static int kill(pthread_t *tid);
};

} /* namespace sdfs */
#endif /* THREADHELPER_H_ */
