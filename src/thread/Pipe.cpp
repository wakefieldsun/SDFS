/*
 * Pipe.cpp
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#include "Pipe.h"
#include "../common/Log.h"
#include "../common/CToolKit.h"
#include "../common/common_define.h"
#include <stdlib.h>

namespace sdfs {

Pipe::Pipe() {
	m_pipefds[0] = -1;
	m_pipefds[1] = -1;
	if(pipe(m_pipefds) < 0)
	{
		Log::Crit("file:"__FILE__", line: %d, "
				"failed to call pipe, errno: %d, "
				"info: %s", __LINE__, errno, STRERROR(errno));
		exit(0);
	}
	CToolKit::setNoblock(m_pipefds[0]);
	CToolKit::setNoblock(m_pipefds[1]);
}

Pipe::~Pipe() {
	if(m_pipefds[0] != -1)
		close(m_pipefds[0]);
	if(m_pipefds[1] != -1)
		close(m_pipefds[1]);
}

int Pipe::GetReadDescriptor()
{
	return m_pipefds[0];
}

int Pipe::GetWriteDescriptor()
{
	return m_pipefds[1];
}

int Pipe::CloseWrite()
{
	return close(m_pipefds[1]);
}

int Pipe::CloseRead()
{
	return close(m_pipefds[0]);
}

} /* namespace sdfs */
