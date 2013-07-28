/*
 * Tracker.cpp
 *
 *  Created on: 2013年7月27日
 *      Author: sun
 */

#include "Tracker.h"
#include "../common/common_define.h"
#include "../common/NetworkHelper.h"

namespace sdfs {

Tracker::Tracker() {

}

Tracker::~Tracker() {

}

int Tracker::ReadConfiguration()
{
	Log::Debug("ReadConfiguration");
	//TODO: read from configuration file
	char strbuf[SRV_NAME_LEN];
	char *ptr;
	//init tracker
	strcpy(m_sConf.tracker.strIp, LOCAL_HOST);
	m_sConf.tracker.strName = "Tracker01";
	m_sConf.tracker.nPort	= TRACKER_BACK_PORT;
	ptr =
	for(int i = 0 ;i < BACKED_SERVER_SIZE; ++i)
	{
		snprintf(m_sConf.backend[i].strName, SRV_NAME_LEN, "storage0%d", i+1);
		m_sConf.backend[i].nPort = STORAGE_PORT;
	}
}

int Tracker::OpenListenSocket()
{
	Log::Debug("OpenListenSocket");
}

int Tracker::ConnectBackendServer()
{
	Log::Debug("ConnectBackendServer");
}

int Tracker::InitFileTable()
{
	Log::Debug("InitFileTable");
}

int Tracker::InitConnectionPool()
{
	Log::Debug("InitConnectionPool");
}

int Tracker::StartWorkers()
{
	Log::Debug("StartWorkers");
}

int Tracker::CloseConnectionPool()
{
	Log::Debug("CloseConnectionPool");
}

int Tracker::StopWorkers()
{
	Log::Debug("StopWorkers");
}

int Tracker::CloseFileTable()
{
	Log::Debug("CloseFileTable");
}

int Tracker::CloseListenSocket()
{
	Log::Debug("CloseListenSocket");
}

} /* namespace sdfs */
