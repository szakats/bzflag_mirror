/* bzflag
 * Copyright (c) 1993 - 2002 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * common definitions
 */

#ifndef BZF_COMMON_H
#define BZF_COMMON_H

// Might we be BSDish? sys/param.h has BSD defined if so
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#if defined(_WIN32)
// turn off bogus `this used in base member initialization list'
#pragma warning(disable: 4355)
// turn off `decorated name length exceeded'
#pragma warning(disable: 4503)
// turn off stupid `identifier truncated in debug info'
#pragma warning(disable: 4786)
// turn off `forcing value to bool'
#pragma warning(disable: 4800)
#endif /* defined(_WIN32) */

#include <assert.h>
#include <stddef.h>
#include <string>
#include <stdarg.h>
#include <stdio.h>

// some platforms don't have float versions of the math library
#if defined(_old_linux_) || defined(sun)
#define asinf			(float)asin
#define atanf			(float)atan
#define atan2f  		(float)atan2
#define ceilf			(float)ceil
#define cosf			(float)cos
#define expf			(float)exp
#define fabsf			(float)fabs
#define floorf  		(float)floor
#define fmodf			(float)fmod
#define hypotf  		(float)hypot
#define logf			(float)log
#define powf			(float)pow
#define sinf			(float)sin
#define sqrtf			(float)sqrt
#define tanf			(float)tan
#endif

// random number stuff
#include <stdlib.h>
#define bzfrand()		((double)rand() / ((double)RAND_MAX + 1.0))
#define bzfsrand(_s)	srand(_s)


#if !defined(_WIN32) & !defined(macintosh)

#ifndef BSD
#include <values.h>
#endif
#include <sys/types.h>

#if defined(__linux) || (defined(__sgi) && !defined(__INTTYPES_MAJOR))
typedef u_int16_t		uint16_t;
typedef u_int32_t		uint32_t;
#endif
#if defined(sun)
typedef signed short	int16_t;
typedef ushort_t		uint16_t;
typedef signed int		int32_t;
typedef uint_t			uint32_t;
#endif

#endif

typedef unsigned char	uint8_t;

#if defined( macintosh )

// need some integer types
#include <inttypes.h>

#endif /* defined( macintosh ) */

#if defined(_WIN32)

// work around for statement scoping bug
#define for				if (false) { } else for

// missing float math functions
#define hypotf			(float)hypot

// missing types

typedef signed short	int16_t;
typedef unsigned short	uint16_t;
typedef signed int		int32_t;
typedef unsigned int	uint32_t;

#define snprintf	_snprintf

#endif /* !defined(_WIN32) */

#ifdef countof
#undef countof
#endif
#define countof(__x)	(sizeof(__x) / sizeof(__x[0]))

// missing constants
#ifndef MAXFLOAT
#define MAXFLOAT		3.402823466e+38f
#endif

#include <math.h>
// replace constants with float versions
#ifdef M_PI
#undef M_PI
#endif
#ifdef M_SQRT1_2
#undef M_SQRT1_2
#endif
#define M_PI			3.14159265358979323846f
#define M_SQRT1_2		0.70710678118654752440f

class string_util
{
public:
	static std::string	format(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		std::string result = vformat(fmt, args);
		va_end(args);
		return result;
	}

	static std::string	vformat(const char* fmt, va_list args)
	{
		// FIXME -- should prevent buffer overflow in all cases
		// not all platforms support vsnprintf so we'll use vsprintf and a
		// big temporary buffer and hope for the best.
		char buffer[8192];
		vsprintf(buffer, fmt, args);
		return std::string(buffer);
	}
};

#include <ctype.h>
inline
int strnocasecmp(const char* s1, const char* s2)
{
	while (tolower(*s1) == tolower(*s2) && *s1 != 0) {
		++s1;
		++s2;
	}
	return static_cast<int>(*s1) - static_cast<int>(*s2);
}

inline
int strnnocasecmp(const char* s1, const char* s2, int n)
{
	while (n > 0 && tolower(*s1) == tolower(*s2) && *s1 != 0) {
		++s1;
		++s2;
		--n;
	}
	return static_cast<int>(*s1) - static_cast<int>(*s2);
}

#endif // BZF_COMMON_H
// ex: shiftwidth=4 tabstop=4
