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
#include "../common/common_define.h"

namespace sdfs {


typedef struct _ServerConfig{
	char	strIp[SOCK_STR_LEN];
	int		nPort;
	char	strName[SRV_NAME_LEN];
}ServerConfig;

enum SocketStatus{CLOSED = 0, ESTABLISHED, LISTENING};

typedef struct _Socket{
	struct sockaddr_in	s_addr;
	SocketStatus		status;
}Socket;

class NetworkHelper {
public:
	NetworkHelper(const ServerConfig &conf, bool isAsyn = true);
	virtual ~NetworkHelper();

	int CreateListenSocket();

	int CreateConnectSocket();

	int Close();

	int Receive(void *data, int size);

	int Send(void *data, int size);

private:
	ServerConfig	m_sConf;
	int				m_fd;
	bool			m_bAsyn;
	Socket			m_sSocket;
};

} /* namespace sdfs */
#endif /* NETWORKHELPER_H_ */
