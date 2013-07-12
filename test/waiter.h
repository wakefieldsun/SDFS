/*
 * waiter.h
 *
 *  Created on: 2013-7-12
 *      Author: sun
 */

#ifndef WAITER_H_
#define WAITER_H_

#include <iostream>
#include "Observer.h"

namespace sdfs {

class waiter : public Observer{
public:
	waiter();
	virtual ~waiter();
	void notify(Observer *o)
	{
		std::cout<<"I get it"<<std::endl;
	}

};

} /* namespace sdfs */
#endif /* WAITER_H_ */
