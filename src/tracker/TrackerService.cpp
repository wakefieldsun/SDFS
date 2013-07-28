/*
 * TrackerService.cpp
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#include "TrackerService.h"
#include "../common/Log.h"
#include <errno.h>
#include <stdlib.h>

namespace sdfs {

TrackerService::TrackerService() {
}

TrackerService::~TrackerService() {
}

int TrackerService::Stop()
{
	int result = CloseListenSocket();
	if(result != 0)
	{
		Log::Error("can not close listen socket");
	}
	result = CloseFileTable();
	if(result != 0)
	{
		Log::Error("can not close FileTable");
	}
	result = CloseConnectionPool();
	if(result != 0)
	{
		Log::Error("can not close connection pool");
	}
	StopWorkers();
	if(result != 0)
	{
		Log::Error("can not stop worker threads");
	}
	return result;
}

int TrackerService::Start()
{
	int result = 0;
	do{
		result = ReadConfiguration();
		if(result != 0)
		{
			Log::Error("can not initialize configuration");
			return -1;
		}
		result = OpenListenSocket();
		if(result < 0)
		{
			Log::Error("can not open listen socket");
			return -1;
		}
		ConnectBackendServer();
		if(result != 0)
		{
			Log::Error("can not connect backed servers");
			return -1;
		}
		InitFileTable();
		if(result != 0)
		{
			Log::Error("can not initialize FileTable");
			return -1;
		}
		InitConnectionPool();
		if(result != 0)
		{
			Log::Error("can not initialize connection pool");
			return -1;
		}
		StartWorkers();
		if(result != 0)
		{
			Log::Error("can not create worker threads");
			return -1;
		}
	}while(false);
	return 0;
}

} /* namespace sdfs */
