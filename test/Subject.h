#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <iostream>
#include <vector>
#include "Observer.h"

using namespace std;
namespace sdfs {
class Subject
{
public:
	Subject();
	virtual ~Subject();

	virtual void notifyObserver()
	{
		vector<Observer *>::iterator it;
		for(it = observers->begin() ; it != observers->end(); it++)
			(*it)->notify(*it);
	}
	virtual void addObserver(Observer* o)
	{
		observers->push_back(o);
	}
	virtual void deleteObserver(Observer *o)
	{
		vector<Observer*>::iterator it;
		int index=0;
		for(it = observers->begin() ; *it != o && it < observers->end(); it++)
			index++;
		if(*it == o)
			observers->erase(it);
	}

private:
	std::vector<Observer *> *observers;
};
}

#endif
