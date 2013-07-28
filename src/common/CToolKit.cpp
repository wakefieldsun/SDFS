/*
 * CToolKit.cpp
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#include "CToolKit.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

namespace sdfs {

CToolKit::CToolKit() {
	// TODO Auto-generated constructor stub

}

CToolKit::~CToolKit() {
	// TODO Auto-generated destructor stub
}

int CToolKit::setNoblock(int fd)
{
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1)
	{
		return -1;
	}
	return 0;
}

char *CToolKit::GetIpByName(const char *name, char* buf, int size)
{
	struct hostent *phost;
	char 	*ptr;
	char	**pptr;
	if(strlen(name)<0)
		return NULL;
	phost = gethostbyname(name);
	if(phost == NULL)
	{
		Log::Warning("can not find host by name: %s", name);
		return NULL;
	}
	switch(phost->h_addrtype)
	{
	case AF_INET:
		 pptr=phost->h_addr_list;
		 for(;*pptr!=NULL;pptr++)
		 {
			 return inet_ntop(phost->h_addrtype, *pptr, buf, size);
		 }
		 break;
	default:
		Log::Warning("unknown address type");
		return NULL;
	}
	return NULL;
}

} /* namespace sdfs */
