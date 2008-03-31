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

#ifndef FTOA_H_
#define FTOA_H_

#include <string>
#include "TextUtils.h"

using namespace std;

inline string ftoa(float f) {
	return TextUtils::format("%f", f);
}

inline string itoa(int i) {
	return TextUtils::format("%i", i);	
}

#endif /*FTOA_H_*/
