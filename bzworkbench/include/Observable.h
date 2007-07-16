#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <vector>
#include "Observer.h"

using namespace std;

/**
 *  An interface a class must implement to be considered a Model in MVC.
 *  This is similar to the Observable interface in Java.
 * 
 *  NOTE: it is NOT the job of the Observables to delete Observers
 */

 
class Observable {
	
public:

	Observable() {
		observers = vector<Observer*>();
	}
	
	virtual ~Observable() { }
	
	vector<Observer*>& getObservers() { return this->observers; }
	
	// add an observer
	virtual void addObserver( Observer* obs ) {
		observers.push_back( obs );
	}
	
	// remove an observer
	virtual void removeObserver( Observer* obs ) {
		if(observers.size() > 0) {
			vector<Observer*>::iterator start = observers.begin();
			for(unsigned int i = 0; i < observers.size(); i++, start++) {
				if( observers[i] == *start )
					observers.erase( start );
			}	
		}	
	}
	
	// notify observers
	virtual void notifyObservers(void* data) {
		// call each observer's update() method
		if(observers.size() > 0) {
			for(vector<Observer*>::iterator i = observers.begin(); i != observers.end(); i++) {
				(*i)->update(this, data);
			}
		}
	}
	
private:

	// the observers
	vector<Observer*> observers;
};

#endif /*OBSERVABLE_H_*/
