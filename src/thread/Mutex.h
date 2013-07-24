/*
 * Mutex.h
 *
 *  Created on: 2013年7月17日
 *      Author: sun
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

namespace sdfs{
class Mutex {
public:
	Mutex();
	virtual ~Mutex();

	void Lock();
	void Unlock();

	pthread_mutex_t *GetMutex();
private:
	pthread_mutex_t m_mutex;
};
}

#endif /* MUTEX_H_ */
