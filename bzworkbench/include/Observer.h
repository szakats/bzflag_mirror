/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

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
