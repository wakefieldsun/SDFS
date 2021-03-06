/*
 * CToolKit.h
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#ifndef CTOOLKIT_H_
#define CTOOLKIT_H_

namespace sdfs {

class CToolKit {
public:
	CToolKit();
	virtual ~CToolKit();

	static int setNoblock(int fd);

	static char *GetIpByName(const char *name, char* buf, int size);
};

} /* namespace sdfs */
#endif /* CTOOLKIT_H_ */
