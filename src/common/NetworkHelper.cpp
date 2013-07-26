/*
 * NetworkHelper.cpp
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#include "NetworkHelper.h"
#include "../common/CToolKit.h"
#include "../common/Log.h"

namespace sdfs {

NetworkHelper::NetworkHelper(const ServerConfig &conf, bool isAsyn = true) {
	memcpy(&m_sConf, &conf, sizeof(ServerConfig));
	memset(&m_sSocket, 0, sizeof(Socket));
	m_bAsyn = isAsyn;
	m_fd = 0;
}

NetworkHelper::~NetworkHelper() {
	// TODO Auto-generated destructor stub
}

int NetworkHelper::CreateListenSocket()
{
	if(m_sSocket.status != CLOSED)
	{
		Log::Warning("socket: %d has already opened, please close first", m_fd);
		return -1;
	}
	int result;
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	result = CToolKit::setNoblock(m_fd);
	if(result < 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call setNoblock, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	memset(&m_sSocket.s_addr, 0, sizeof(struct sockaddr_in));
	m_sSocket.s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sSocket.s_addr.sin_family = AF_INET;
	m_sSocket.s_addr.sin_port = htons(m_sConf.nPort);
	int len = sizeof(struct sockaddr);
	result = bind(m_fd, (struct sockaddr *)&m_sSocket.s_addr, len);
	if(result != 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call bind, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	result = listen(m_fd, LISTENQ);
	if(result != 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call listen, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	Log::Info("start listening on Port: %d", m_sConf.nPort);
	m_sSocket.status = LISTENING;
	return m_fd;
}

int NetworkHelper::CreateConnectSocket()
{
	if(m_sSocket.status != CLOSED)
	{
		Log::Warning("socket: %d has already opened, please close first", m_fd);
		return -1;
	}
	int result;
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	result = CToolKit::setNoblock(m_fd);
	if(result < 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call setNoblock, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	memset(&m_sSocket.s_addr, 0, sizeof(struct sockaddr_in));
	m_sSocket.s_addr.sin_family = AF_INET;
	m_sSocket.s_addr.sin_port = htons(m_sConf.nPort);
	inet_pton(AF_INET, m_sConf.strIp, &m_sSocket.s_addr);
	if((result = connect(m_fd,(struct sockaddr*)&m_sSocket.s_addr,sizeof(m_sSocket.s_addr)))<0)
	{
		Log::Error("file :"__FILE__", line: %d,"
					"call connect, "
					"errno:%d, info: %s, bind", __LINE__,
					result, STRERROR(result));
		return -1;
	}
	m_sSocket.status = ESTABLISHED;
	return m_fd;
}

int NetworkHelper::Close()
{
	if(m_sSocket.status == CLOSED)
		return -1;

}

} /* namespace sdfs */
