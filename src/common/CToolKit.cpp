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

} /* namespace sdfs */
