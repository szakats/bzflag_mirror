#include "MessageUtilities.h"

template <>
bool MessageUtilities::parse(const char *string, bool &dest)
{
  if (strcasecmp(string, "true") == 0 || strcasecmp(string, "1") == 0)
    dest = true;
  else if (strcasecmp(string, "false") == 0 || strcasecmp(string, "0") == 0)
    dest = false;
  else
    return false;

  return true;
}

template <>
bool MessageUtilities::parse(const char *string, float &dest)
{
	if (sscanf(string,"%f",&dest) != 1)
		return false;
#ifdef _OLD_CRAP
	#ifndef _WIN32
	char *endptr;
	dest = strtof(string, &endptr);
	if (endptr == string)
		return false;
	#else
		dest = (float)atof(string);
	#endif
#endif
  /* We don't want NaN no matter what - it's of no use in this scenario.
   * (And strtof will allow the string "NAN" as NaN) */
  if (isnan(dest))
    dest = 0.0f;

  return true;
}

template <>
bool MessageUtilities::parse(const char *string, std::string &dest)
{
  dest = string;
  return true;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
