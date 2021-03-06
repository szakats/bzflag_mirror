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

#include "MessageUtilities.h"

template <>
bool MessageUtilities::parse(const char *str, bool &dest)
{
  if (strcasecmp(str, "true") == 0 || strcasecmp(str, "1") == 0)
    dest = true;
  else if (strcasecmp(str, "false") == 0 || strcasecmp(str, "0") == 0)
    dest = false;
  else
    return false;

  return true;
}

template <>
bool MessageUtilities::parse(const char *str, double &dest)
{
  if (sscanf(str,"%lf",&dest) != 1)
    return false;

  /* We don't want NaN no matter what - it's of no use in this
   * scenario.  (And strtof will allow the string "NAN" as NaN)
   */
  if (isnan(dest))
    dest = 0.0;

  return true;
}

template <>
bool MessageUtilities::parse(const char *str, float &dest)
{
  double tmp;

  // Use double parsing and convert
  if (!MessageUtilities::parse(str, tmp))
    return false;

  dest = (float)tmp;

  return true;
}

template <>
bool MessageUtilities::parse(const char *str, uint64_t &dest)
{
#if   (SIZEOF_LONG_INT == 8)
  static const char* fmt = "%lu";
#elif (SIZEOF_LONG_LONG_INT == 8)
  static const char* fmt = "%Lu";
#else
#  error uint64_t sscanf format was not found
#endif
  return (sscanf(str, fmt, &dest) == 1);
}

template<>
bool MessageUtilities::parse(const char *str, uint32_t &dest)
{
  uint64_t tmp;

  if (!MessageUtilities::parse(str, tmp))
    return false;

  dest = (uint32_t) tmp;

  return true;
}

template <>
bool MessageUtilities::parse(const char *str, std::string &dest)
{
  dest = str;
  return true;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
