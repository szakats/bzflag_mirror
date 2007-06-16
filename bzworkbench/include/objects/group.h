#ifndef GROUP_H_
#define GROUP_H_

#include "bz2object.h"
#include "../render/RGBA.h"
#include "../model/BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class group : public bz2object {
	
	public:
	
		// constructor
		group();
		
		// constructor with data
		group(string& data);
		
		static DataEntry* init() { return new group(); }
		static DataEntry* init(string& data) { return new group(data); }
		
		// getter
		string get(void);
		
		// setter
		int update(string& data);
		
		// toString
		string toString(void);
		
		// render
		int render(void);
		
	private:
		// member data
		RGBA tintColor;
		bool driveThrough, shootThrough;
		int team;
		
};

#endif /*GROUP_H_*/
