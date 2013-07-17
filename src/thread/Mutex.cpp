/*
 * Mutex.cpp
 *
 *  Created on: 2013年7月17日
 *      Author: sun
 */

#include "Mutex.h"
#include "../common/ThreadHelper.h"
#include "../common/Log.h"
#include "../common/common_define.h"

namespace sdfs{
Mutex::Mutex() {
	int result = \
	ThreadHelper::init_lock(m_mutex);
	if(result != 0)
	{
		Log::Debug("file:"__FILE__", line: %d, " \
				"call init_lock fail, errno: %d, info: %s",
				__LINE__, result, STRERROR(result));
	}
	else
	{
		Log::Debug("init_lock successful");
	}
}

Mutex::~Mutex() {
	int result = pthread_mutex_destroy(&m_mutex);
	if(result != 0)
	{
		Log::Debug("file:"__FILE__", line: %d, " \
				"call pthread_mutex_destroy fail, errno: %d, info: %s",
				__LINE__, result, STRERROR(result));
	}
	else
	{
		Log::Debug("pthread_mutex_destroy successful");
	}
}

void Mutex::Lock()
{
	int result = pthread_mutex_lock(&m_mutex);
	if(result != 0)
	{
		Log::Debug("file:"__FILE__", line: %d, " \
				"call pthread_mutex_lock fail, errno: %d, info: %s",
				__LINE__, result, STRERROR(result));
	}
	else
	{
		Log::Debug("pthread_mutex_lock successful");
	}
}

void Mutex::Unlock()
{
	int result = pthread_mutex_unlock(&m_mutex);
	if(result != 0)
	{
		Log::Debug("file:"__FILE__", line: %d, " \
				"call pthread_mutex_unlock fail, errno: %d, info: %s",
				__LINE__, result, STRERROR(result));
	}
	else
	{
		Log::Debug("pthread_mutex_unlock successful");
	}
}
}
