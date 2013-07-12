/*
 * Customer.h
 *
 *  Created on: 2013-7-12
 *      Author: sun
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <iostream>
#include <vector>
#include "Subject.h"

using namespace std;

namespace sdfs {

class Customer : public Subject{
public:
	Customer(char *name);
	virtual ~Customer();

	//virtual void addObserver(Observer *o);

private:
	string name;
};

} /* namespace sdfs */
#endif /* CUSTOMER_H_ */
