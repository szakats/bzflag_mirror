/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* win32/config.h.  Generated by hand by Jeff Myers 6-12-03 */
/* this config is just for visual C++ since it doesn't use automake*/

/* Building regex */
#define BUILD_REGEX 1

/* Time Bomb expiration */
/* #undef TIME_BOMB */

/* Debug Rendering */
/* #undef DEBUG_RENDERING */

/* Enabling Robots */
#define ROBOT 1

/* Enabling Snapping */
#define SNAPPING 1

/* Enabling lubcurl */
#ifndef HAVE_CURL
#define HAVE_CURL 1
#endif

/* On windows, strcasecmp is really strcmp */
#define HAVE_STRICMP 1

/* Use modern template for std::count */
#define HAVE_STD_COUNT 1

/* We have float math functions */
#define HAVE_ASINF 1
#define HAVE_ATAN2F 1
#define HAVE_ATANF 1
#define HAVE_COSF 1
#define HAVE_EXPF 1
#define HAVE_FABSF 1
#define HAVE_FLOORF 1
#define HAVE_FMODF 1
#define HAVE_LOGF 1
#define HAVE_POWF 1
#define HAVE_SINF 1
#define HAVE_SQRTF 1
#define HAVE_TANF 1

/* but we don't have this one */
/* #undef HAVE_HYPOTF */

/* Define to 1 if you have regex stuff available */
/* undef HAVE_REGEX_H */

// define our OS

#ifndef BZ_BUILD_OS
	#ifdef _DEBUG
		#define DEBUG
		#define BZ_BUILD_OS			"W32VC71D"
	#else
		#define BZ_BUILD_OS			"W32VC71"
	#endif //_DEBUG
#endif //BZ_BUILD_OS

#include <stdio.h>
#include <Windows.h>


/* Define to 1 if you have the `WaitForSingleObject' function. */
#define HAVE_WAITFORSINGLEOBJECT 1

/* Define to 1 if you have the `Sleep' function. */
#define HAVE_SLEEP 1

#ifndef DEBUG_TRACE
#define DEBUG_TRACE
inline void W32_DEBUG_TRACE (const char* buffer ) {printf("%s",buffer);}
#endif
