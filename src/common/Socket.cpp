/*
 * NetworkHelper.cpp
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#include "Socket.h"
#include "../common/CToolKit.h"
#include "../common/Log.h"

namespace sdfs {

Socket::Socket(const ServerConfig &conf, bool isBlock)
{
	memset(&m_sBody, 0, sizeof(struct SocketBody));
	m_sBody.bBlock = isBlock;
	m_sBody.status = CLOSED;
	int result;
	m_sBody.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_sBody.sockfd < 0)
	{
		m_sBody.status = CREATEFAILURE;
		return;
	}
	if(!m_sBody.bBlock)
	{
		result = CToolKit::setNoblock(m_sBody.sockfd);
		if(result < 0)
		{
			Log::Error("file :"__FILE__", line: %d,"
					"call setNoblock, "
					"errno:%d, info: %s, bind", __LINE__,
					result, STRERROR(result));
		}
	}
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

int Socket::CreateListenSocket()
{
	if(m_sBody.status != CLOSED || m_sBody.status == CREATEFAILURE)
	{
		Log::Warning("socket: %d has already opened, please close first", m_sBody.sockfd);
		return -1;
	}
	int result;
	memset(&m_sBody.s_addr, 0, sizeof(struct sockaddr_in));
	m_sBody.s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sBody.s_addr.sin_family = AF_INET;
	m_sBody.s_addr.sin_port = htons(m_sBody.port);
	int len = sizeof(struct sockaddr);
	result = bind(m_sBody.sockfd, (struct sockaddr *)&m_sBody.s_addr, len);
	if(result != 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call bind, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	result = listen(m_sBody.sockfd, LISTENQ);
	if(result != 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
				"call listen, "
				"errno:%d, info: %s, bind", __LINE__,
				result, STRERROR(result));
		return -1;
	}
	Log::Info("start listening on Port: %d", m_sBody.port);
	m_sBody.status = LISTENING;
	return m_sBody.sockfd;
}

int Socket::CreateConnectSocket(ServerConfig &conf)
{
	if(m_sBody.status != CLOSED || m_sBody.status == CREATEFAILURE)
	{
		Log::Warning("socket: %d has already opened, please close first", m_sBody.sockfd);
		return -1;
	}
	int result;
	memset(&m_sBody.s_addr, 0, sizeof(struct sockaddr_in));
	m_sBody.s_addr.sin_family = AF_INET;
	m_sBody.s_addr.sin_port = htons(conf.nPort);
	inet_pton(AF_INET, conf.strIp, &m_sBody.s_addr);
	if((result = connect(m_sBody.sockfd,(struct sockaddr*)&m_sBody.s_addr,sizeof(m_sBody.s_addr)))<0)
	{
		Log::Error("file :"__FILE__", line: %d,"
					"call connect, "
					"errno:%d, info: %s, bind", __LINE__,
					result, STRERROR(result));
		return -1;
	}
	m_sBody.status = ESTABLISHED;
	return m_sBody.sockfd;
}

int Socket::Close()
{
	if(m_sBody.status == CLOSED)
		return -1;
	int result = close(m_sBody.sockfd);
	if(result != 0)
	{
		Log::Error("file :"__FILE__", line: %d,"
					"call close, "
					"errno:%d, info: %s, bind", __LINE__,
					errno, STRERROR(errno));
		return -1;
	}
	m_sBody.status = CLOSED;
	return 0;
}




} /* namespace sdfs */
