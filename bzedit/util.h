#ifndef __UTIL_H__
#define __UTIL_H__

#ifndef WIN32
// win32 and AIX use stricmp
// POSIX uses strcasecmp
// we can be pretty sure nobody will be using
// this on AIX, since bzflag doesn't support it
#define stricmp strcasecmp
#endif

bool getLine(char **data, char *line);
bool getNextElement(char **data, char *element);

#endif
