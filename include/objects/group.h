#ifndef GROUP_H_
#define GROUP_H_

#include "bz2object.h"
#include "../render/RGBA.h"
#include "../model/BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class define;		// pre-declare define class

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
		
		
	private:
		// member data
		RGBA tintColor;
		bool driveThrough, shootThrough;
		int team;
		
		// reference to the definition
		define* def;
		
		// the ring structure around the groups
		osg::ref_ptr< osg::Geode > ring;
		
		// build the ring-like structure around the objects
		void buildGeometry();
		
		// update the child objects
		void updateObjects();
		
};

#include "define.h"

#endif /*GROUP_H_*/
