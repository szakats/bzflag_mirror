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

/* interface header */
#include "Authentication.h"

/* system implementation headers */
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#include "DirectoryNames.h"


bool	   Authentication::authentication = false;

Authentication::Authentication() : trusted(false), globallyAuthenticated(false)
{
}

void Authentication::cleanUp()
{
}


void Authentication::init(const char *, int , const char *)
{
}

void Authentication::setPrincipalName(char *buf, int len)
{
  if (len > 1023)
    return;

  // Saving principal name
  char name[1024];
  memcpy(name, buf, len);
  name[len] = 0;
  principalName = name;

  if (!authentication)
    return;
}

void Authentication::verifyCredential(char *, int)
{
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
