#ifndef TEXCOORD2D_H_
#define TEXCOORD2D_H_

#include <string>
#include <vector>
#include "../model/BZWParser.h"

class TexCoord2D {
	public:
	
	float u, v;
	
	TexCoord2D() {
		u = v = 0.0;	
	}
	
	TexCoord2D(const char* data) {
		// process text form
		vector<string> vals = BZWParser::getLineElements(data);
		
		if(vals.size() == 2) {
			this->u = atof( vals[0].c_str() );
			this->v = atof( vals[1].c_str() );	
		}
		else {
			this->u = this->v = 0.0f;	
		}
	}
	
	TexCoord2D(float u, float v) {
		this->u = u; this->v = v;
	}
	
	TexCoord2D(double u, double v) {
		this->u = (float)u; this->v = (float)v;
	}
	
	string toString(void) {
		return string(ftoa(this->u)) + " " + string(ftoa(this->v));	
	}
};

#endif /*TEXCOORD2D_H_*/
