/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef _ANSI_CODES_H_
#define _ANSI_CODES_H_

#include "common.h"

// system headers
#include <assert.h>
#include <string.h>

// common headers
#include "vectors.h"

// Escape character to begin ANSI codes
#define ESC_CHAR	((char) 0x1B)

// ANSI (ISO 6429) colors codes

#define ANSI_STR_RESET		"\033[0;1m"	// reset & bright
#define ANSI_STR_RESET_FINAL	"\033[0m"	// only reset
#define ANSI_STR_BRIGHT		"\033[1m"
#define ANSI_STR_DIM		"\033[2m"
#define ANSI_STR_UNDERLINE	"\033[4m"
#define ANSI_STR_NO_UNDERLINE	"\033[24m"
#define ANSI_STR_PULSATING	"\033[5m"
#define ANSI_STR_NO_PULSATE	"\033[25m"
#define ANSI_STR_REVERSE	"\033[7m"
#define ANSI_STR_NO_REVERSE	"\033[27m"

#define ANSI_STR_FG_BLACK	"\033[30m"	// grey
#define ANSI_STR_FG_RED		"\033[31m"
#define ANSI_STR_FG_GREEN	"\033[32m"
#define ANSI_STR_FG_YELLOW	"\033[33m"
#define ANSI_STR_FG_BLUE	"\033[34m"
#define ANSI_STR_FG_MAGENTA	"\033[35m"	// purple
#define ANSI_STR_FG_CYAN	"\033[36m"
#define ANSI_STR_FG_WHITE	"\033[37m"

#define ANSI_STR_FG_ORANGE	"\033[130m"	// orange (custom; not defined in ISO 6429)

// Color definitions
typedef enum {
  // the first 8 codes line up with the TeamColor enum from global.h
  RogueColor		= 0,	// team (yellow)
  RedColor		= 1,	// team
  GreenColor		= 2,	// team
  BlueColor		= 3,	// team
  PurpleColor		= 4,	// team
  WhiteColor		= 5,	// observer
  GreyColor		= 6,	// rabbit
  OrangeColor		= 7,    // hunter
  CyanColor		= 8,

  LastColor		= 8,	// last of the actual colors, the rest are modifiers

  ResetColor		= 9,
  FinalResetColor       = 12,
  BrightColor		= 13,
  DimColor		= 14,
  PulsatingColor	= 10,
  NonPulsatingColor	= 15,
  UnderlineColor	= 11,
  NonUnderlineColor	= 16,

  LastCode		= 16,	// last of the codes

  // aliases
  YellowColor		= 0,	// aka RogueColor
  DefaultColor		= 6	// default to grey
} ColorCodes;

// These enum values have to line up with those above
static const char* ColorStrings[17] = {
  ANSI_STR_FG_YELLOW,   // 0  Rogue     (yellow)
  ANSI_STR_FG_RED,      // 1  Red
  ANSI_STR_FG_GREEN,    // 2  Green
  ANSI_STR_FG_BLUE,     // 3  Blue
  ANSI_STR_FG_MAGENTA,  // 4  Purple
  ANSI_STR_FG_WHITE,    // 5  White
  ANSI_STR_FG_BLACK,    // 6  Grey      (bright black is grey)
  ANSI_STR_FG_ORANGE,	// 7  Orange	(nonstandard)
  ANSI_STR_FG_CYAN,     // 8  Cyan
  ANSI_STR_RESET,       // 9  Reset
  ANSI_STR_PULSATING,   // 10 Pulsating
  ANSI_STR_UNDERLINE,   // 11 Underline
  ANSI_STR_RESET_FINAL, // 12 Really reset (no brightness added)
  ANSI_STR_BRIGHT,	// 13 Bright mode
  ANSI_STR_DIM,		// 14 Dim mode
  ANSI_STR_NO_PULSATE,  // 15 No Pulsating
  ANSI_STR_NO_UNDERLINE // 16 No Underlining
};
static int ColorStringsLength = sizeof(ColorStrings);
static bool quellAnsiCodesWarning = ColorStringsLength > 0 ? quellAnsiCodesWarning : false;

// These RGB float values have to line up with the colors above
static const fvec4 BrightColors[9] = {
  fvec4(1.0f, 1.0f, 0.0f, 1.0f), // yellow
  fvec4(1.0f, 0.0f, 0.0f, 1.0f), // red
  fvec4(0.0f, 1.0f, 0.0f, 1.0f), // green
  fvec4(0.1f, 0.2f, 1.0f, 1.0f), // blue
  fvec4(1.0f, 0.0f, 1.0f, 1.0f), // purple
  fvec4(1.0f, 1.0f, 1.0f, 1.0f), // white
  fvec4(0.5f, 0.5f, 0.5f, 1.0f), // grey
  fvec4(1.0f, 0.5f, 0.0f, 1.0f), // orange (nonstandard)
  fvec4(0.0f, 1.0f, 1.0f, 1.0f)  // cyan
};


// strip ANSI codes from a string
inline const char* stripAnsiCodes(const char* text)
{
  static char* str = NULL;
  static size_t strSize = 0;

  if (text == NULL) {
    delete[] str;
    strSize = 0;
    str = NULL;
    return NULL;
  }

  const size_t length = strlen(text);
  if ((length + 1) > strSize) {
    delete[] str;
    strSize = length + 1;
    str = new char[strSize];
  }

  size_t j = 0;
  for (size_t i = 0; i < length; i++) {
    if (text[i] == ESC_CHAR) {
      i++;
      if ((i < length) && (text[i] == '[')) {
	i++;
	while ((i < length) && ((text[i] == ';') ||
	       ((text[i] >= '0') && (text[i] <= '9')))) {
	  i++;
	}
      }
    } else {
      str[j] = text[i];
      j++;
    }
  }
  str[j] = '\0';

  return str;
}


#endif //_ANSI_CODES_H_

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
