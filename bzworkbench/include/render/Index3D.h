/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef INDEX3D_H_
#define INDEX3D_H_

#include "model/BZWParser.h"

class Index3D {
	
	public:
	
	int a, b, c,		// vertex indices
	    t1, t2, t3;	// texture coordinate indices
	
	Index3D() {
		a = b = c = 0;
		t1 = t2 = t3 = -1;	
	}
	
	Index3D(int _a, int _b, int _c) {
		a = _a;
		b = _b;
		c = _c;
		t1 = _a;
		t2 = _b;
		t3 = _c;
	}
	
	Index3D(int _a, int _b, int _c, int _t1, int _t2, int _t3) {
		a = _a;
		b = _b;
		c = _c;
		t1 = _t1;
		t2 = _t2;
		t3 = _t3;
	}
	
	Index3D(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		
		// only initialize from the string if there are at least 3 elements
		if(points.size() == 3) {
			a = atoi( points[0].c_str() );
			b = atoi( points[1].c_str() );
			c = atoi( points[2].c_str() );
		}
		else if(points.size() == 6) {
			a = atoi( points[0].c_str() );
			b = atoi( points[1].c_str() );
			c = atoi( points[2].c_str() );
			t1 = atoi( points[3].c_str() );
			t2 = atoi( points[4].c_str() );
			t3 = atoi( points[5].c_str() );
		}
		else {
			a = b = c = 0;
			t1 = t2 = t3 = -1;	
		}
	}
	
	string toString() {
		string tIndices("");
		if(t1 >= 0 && t2 >= 0 && t3 >= 0)
			tIndices += string(itoa(t1)) + " " + string(itoa(t2)) + " " + string(itoa(t3));
			
		return string("") + string(itoa(a)) + " " + string(itoa(b)) + " " + string(itoa(c)) + " " + tIndices + "\n";
	}
	
	virtual ~Index3D() {}
};


#endif /*INDEX3D_H_*/
