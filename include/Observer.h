#ifndef OBSERVER_H_
#define OBSERVER_H_

// forward declare Observable
class Observable;

/**
 * This is like Java's Observer class--it is designed to be registered with an Observable
 * and has a method to be called when the Observable is updated
 */
 
class Observer {
	
public:

	virtual void update(Observable* obs, void* data) = 0;
	virtual ~Observer() { };
};

#endif /*OBSERVER_H_*/
