/*
 * Observer.h
 *
 *  Created on: 2013年7月12日
 *      Author: sun
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

namespace sdfs {

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void notify(Observer *o)=0;
};

} /* namespace sdfs */
#endif /* OBSERVER_H_ */
