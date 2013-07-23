/*
 * Pipe.h
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#ifndef PIPE_H_
#define PIPE_H_

#include <signal.h>

namespace sdfs {

class Pipe {
public:
	Pipe();
	virtual ~Pipe();

	int GetReadDescriptor();
	int GetWriteDescriptor();

	int CloseWrite();
	int CloseRead();
private:
	int m_pipefds[2];
};

} /* namespace sdfs */
#endif /* PIPE_H_ */
