/*
 * IRunnable.cpp
 *
 *  Created on: 2013年7月18日
 *      Author: sun
 */

#include "IRunnable.h"
#include <pthread.h>

namespace sdfs {

IRunnable::IRunnable() {
	// TODO Auto-generated constructor stub

}

IRunnable::~IRunnable() {
	// TODO Auto-generated destructor stub
}

void *IRunnable::Run(void *arg)
{
	pthread_exit((void *)0);
}

} /* namespace sdfs */
