/*
 * ThreadHelper.cpp
 *
 *  Created on: 2013-7-16
 *      Author: sun
 */

#include "ThreadHelper.h"
#include "common_define.h"
#include "Log.h"
#include <signal.h>

namespace sdfs {

ThreadHelper::ThreadHelper() {

}

ThreadHelper::~ThreadHelper() {

}

int ThreadHelper::init_lock(pthread_mutex_t &mutex, int type)
{
	int result=0;
	pthread_mutexattr_t attr;
	if((result = pthread_mutexattr_init(&attr))!= 0)
	{
		Log::Error("File: "__FILE__", Line: %d, call phread_mutexattr_init,"\
				"errno: %d, info: %s", __LINE__, errno, STRERROR(errno));
		return result;
	}
	if((result = pthread_mutexattr_settype(&attr, type))!= 0)
	{
		Log::Error("File: "__FILE__", Line: %d, call pthread_mutexattr_settype,"\
						"errno: %d, info: %s", __LINE__, errno, STRERROR(errno));
		return result;
	}
	if((result = pthread_mutex_init(&mutex, &attr)) != 0)
	{
		Log::Error("File: "__FILE__", Line: %d, call pthread_mutex_init,"\
						"errno: %d, info: %s", __LINE__, errno, STRERROR(errno));
		return result;
	}
	if((result = pthread_mutexattr_destroy(&attr)) != 0)
	{
		Log::Error("File: "__FILE__", Line: %d, call pthread_mutexattr_destroy,"\
						"errno: %d, info: %s", __LINE__, errno, STRERROR(errno));
		return result;
	}
	return result;
}

int ThreadHelper::init_attr(pthread_attr_t &attr, const int stack_size, int type)
{
	size_t old_stack_size;
	size_t new_stack_size;
	int result;

	if ((result=pthread_attr_init(&attr)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
			"call pthread_attr_init fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	if ((result=pthread_attr_getstacksize(&attr, &old_stack_size)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
			"call pthread_attr_getstacksize fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	if (stack_size > 0)
	{
		if (old_stack_size != stack_size)
		{
			new_stack_size = stack_size;
		}
		else
		{
			new_stack_size = 0;
		}
	}
	else if (old_stack_size < 1 * 1024 * 1024)
	{
		new_stack_size = 1 * 1024 * 1024;
	}
	else
	{
		new_stack_size = 0;
	}

	if (new_stack_size > 0)
	{
		if ((result=pthread_attr_setstacksize(&attr, \
				new_stack_size)) != 0)
		{
			Log::Error("file: "__FILE__", line: %d, " \
				"call pthread_attr_setstacksize fail, " \
				"errno: %d, error info: %s", \
				__LINE__, result, STRERROR(result));
			return result;
		}
	}

	if ((result=pthread_attr_setdetachstate(&attr, \
			type)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
			"call pthread_attr_setdetachstate fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	return 0;
}

int ThreadHelper::create(pthread_t *pid, void *(*startfunc)(void *), \
		void *arg, const int stacksize, int type)
{
	int result;
	pthread_attr_t attr;
	if((result = init_attr(attr, stacksize, type)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
				"call init_attr fail, " \
				"errno: %d, error info: %s", \
				__LINE__, result, STRERROR(result));
		return result;
	}
	Log::Debug("create pthread_attr_t successful");
	if((result = pthread_create(pid, &attr, startfunc, arg)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
				"call pthread_create fail, " \
				"errno: %d, error info: %s", \
				__LINE__, result, STRERROR(result));
	}
	Log::Debug("pthread_create successful");
	pthread_attr_destroy(&attr);
	return result;
}

int ThreadHelper::kill(pthread_t *tid)
{
	int result;
	Log::Debug("to kill: %x", *tid);
	if ((result=pthread_kill(*tid, SIGINT)) != 0)
	{
		Log::Error("file: "__FILE__", line: %d, " \
			"kill thread failed, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
	}
	Log::Debug("kill successful: %x", *tid);
	return 0;
}

} /* namespace sdfs */
