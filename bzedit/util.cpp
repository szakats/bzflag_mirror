#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

bool getLine(char **data, char *line) {
  line[0] = '\0';
  char *ptr = *data, *out = line;

  while(!(*ptr == '\n' || *ptr == '\r')) {
    *out = *ptr;
    if(*ptr == '\0')
      return false;
    ptr++;
    out++;
  }
  ptr++;
  *data = ptr;
  *out = '\0';
  return true;
}

bool getNextElement(char **data, char *element) {
  if(!data || !element)
    return false;

  element[0] = '\0';
  char *in = *data, *out = element, line[255];

  bool done = false, eof = false;

  while(!done) {
    if(!getLine(&in, line)) {
      done = true;
      eof = true;
    }
    switch(line[0]) {
      case '#':
      case '\n':
      case '\t':
      case ' ':
      case '\0':
	break;
      default:
	strcat(element, line);
	if(stricmp(line, "end") == 0) {
	  done = true;
	}
	strcat(element, "\n");
	break;
    }
  }
  *data = in;
  
  return eof;
}
