/*
 * Timer.h
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

namespace sdfs {

class Timer {
public:
	Timer();
	virtual ~Timer();

	void Start()
	{
		clock1 = clock();
	}

	double Stop()
	{
		clock_t clock2 = clock();
		return (double)(clock2   -   clock1)   /   CLOCKS_PER_SEC;
	}
private:
	clock_t clock1;
};

} /* namespace sdfs */
#endif /* TIMER_H_ */
