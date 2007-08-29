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

#ifndef COMMON_CONTROLS_H
#define COMMON_CONTROLS_H

#include "BZWBAPI.h"

#include <map>
#include <vector>

typedef std::vector<bzwb_BaseCommonControlHandler*> controlHanderList;

controlHanderList getControlHandlers ( bzwb_eCommonControlType type );

void addControlHandler ( bzwb_eCommonControlType type, bzwb_BaseCommonControlHandler* handler );
void removeControlHandler ( bzwb_eCommonControlType type, bzwb_BaseCommonControlHandler* handler );

bool callOpenFileDialog ( std::string  &result, const char* file, const char *dir, const char *extension, const char* description );
bool callSaveFileDialog ( std::string  &result, const char* file, const char *dir, const char *extension, const char* description );



#endif /*COMMON_CONTROLS_H*/
