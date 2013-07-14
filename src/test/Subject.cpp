/*
 * Observer.cpp
 *
 *  Created on: 2013年7月12日
 *      Author: sun
 */

#include <iostream>
#include <vector>
#include "Subject.h"

using namespace std;

namespace sdfs {

Subject::Subject() {
	observers = new std::vector<Observer *>;
}

Subject::~Subject() {
	delete observers;
}

} /* namespace sdfs */
