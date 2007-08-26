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

#ifndef DEFINES_H_
#define DEFINES_H_

#define DEFAULT_TEXTSIZE 14

#define BASE_RED 1
#define BASE_GREEN 2
#define BASE_BLUE 3
#define BASE_PURPLE 4

// define some keypress values (i.e. for transformations)
#define BZ_ROTATE_KEY		'r'
#define	BZ_SCALE_KEY		's'
#define BZ_SHEAR_KEY		'l'
#define BZ_SHIFT_KEY		't'

// for windows users...
#define _CRT_SECURE_NO_DEPRECATE

#ifdef _NEED_VNSPRINTF
 #define vsnprintf _vsnprintf
#endif
// uncomment the above line to ensure that BZWB build properly

#endif /* DEFINES_H_ */
