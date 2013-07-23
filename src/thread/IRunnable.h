/*
 * IRunnable.h
 *
 *  Created on: 2013年7月18日
 *      Author: sun
 */

#ifndef IRUNNABLE_H_
#define IRUNNABLE_H_

namespace sdfs {

class IRunnable
{
public:
	IRunnable();
	virtual ~IRunnable();

	virtual void* Run(void *arg);

	virtual void Stop() = 0;

};


} /* namespace sdfs */
#endif /* IRUNNABLE_H_ */
