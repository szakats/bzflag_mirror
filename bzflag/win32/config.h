/* bzflag
 * Copyright (c) 1993 - 2003 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* win32/config.h.  Generated by hand by Jeff Myers */
/* this config is just for visual C++ since it donsn't use automake*/

/* see if our headers have the socketlen thing some do some don't*/
#if defined socklen_t
#define HAVE_SOCKELEN_T
#endif

/* Version number of package */
#define VERSION "1.7g1"

/* Bzflag internal version */
#define BZVERSION 10707001

/* Time Bomb expiration */
/* #undef TIME_BOMB */

/* Debug Rendering */
/* #undef DEBUG_RENDERING */

/* Enabling Robots */
#define ROBOT 1
