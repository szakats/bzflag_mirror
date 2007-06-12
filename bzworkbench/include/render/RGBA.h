#ifndef RGBA_H_
#define RGBA_H_

#include "../model/BZWParser.h"
#include "../ftoa.h"

#include <string>
#include <vector>

using namespace std;

// simple RGBA construct
class RGBA {
public:
	float r, g, b, a;
	
	RGBA() {
		this->r = this->g = this->b = this->a = 0;	
	}
	
	RGBA(float r, float g, float b, float a) {
		this->r = r; this->g = g; this->b = b; this->a = a;	
	}
	
	RGBA(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		
		// only initialize from the string if there are at least 4 elements
		if(points.size() >= 4) {
			this->r = atof( points[0].c_str() );
			this->g = atof( points[1].c_str() );
			this->b = atof( points[2].c_str() );
			this->a = atof( points[3].c_str() );
		}
		else {
			this->r = this->g = this->b = this->a = 0;	
		}
	}
	
	string toString(void) {
		return string(ftoa(this->r)) + " " + string(ftoa(this->g)) + " " + string(ftoa(this->b)) + " " + string(ftoa(this->a)) + "\n";
	}
};
#endif /*RGBA_H_*/
