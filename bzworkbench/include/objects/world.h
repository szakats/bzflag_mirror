#ifndef WORLDOPTIONSDATA_H_
#define WORLDOPTIONSDATA_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "../DataEntry.h"
#include "../model/BZWParser.h"

class world : public DataEntry {
	public:
		// default constructor
		world();
		
		// constructor with data
		world(string& data);
		
		// send the data
		string get(void);
		
		// receive the data
		int update(string& data);
		
		// toString method
		string toString(void);
		
		// render method
		int render(void);
		
	private:
		string worldName;
		float size;
		float flagHeight;
		bool noWalls;
};

#endif /*WORLDOPTIONSDATA_H_*/
