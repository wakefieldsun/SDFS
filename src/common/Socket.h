/*
 * NetworkHelper.h
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#ifndef NETWORKHELPER_H_
#define NETWORKHELPER_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include "../common/common_define.h"

namespace sdfs {


typedef struct _ServerConfig{
	char	strIp[IP4_STR_LEN];
	int		nPort;
	char	strName[SRV_NAME_LEN];
}ServerConfig;

enum SocketStatus{CLOSED = 0, ESTABLISHED, LISTENING, CREATEFAILURE};

typedef struct _Socket{
	struct sockaddr_in	s_addr;
	SocketStatus		status;
	int					sockfd;
	int					port;
	bool				bBlock;
}SocketBody;

class Socket {
public:
	Socket(const ServerConfig &conf, bool isBlock = false);
	virtual ~Socket();

	int CreateListenSocket();

	int CreateConnectSocket(ServerConfig &conf);

	int Close();

private:

	SocketBody	m_sBody;

};

} /* namespace sdfs */
#endif /* NETWORKHELPER_H_ */
