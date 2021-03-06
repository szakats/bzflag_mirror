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

#ifndef TEXCOORD2D_H_
#define TEXCOORD2D_H_

#include <string>
#include <vector>
#include "model/BZWParser.h"

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
			u = atof( vals[0].c_str() );
			v = atof( vals[1].c_str() );	
		}
		else {
			u = v = 0.0f;	
		}
	}
	
	TexCoord2D(float u, float v) {
		this->u = u; this->v = v;
	}
	
	TexCoord2D(double u, double v) {
		this->u = (float)u; this->v = (float)v;
	}
	
	string toString(void) {
		return string(ftoa(u)) + " " + string(ftoa(v));	
	}
};

#endif /*TEXCOORD2D_H_*/
