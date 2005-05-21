/* bzflag
 * Copyright (c) 1993 - 2005 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "PyEvent.h"
#include "PyTeam.h"
#include <Python.h>

#ifndef __PYTHON_BZFLAG_H__
#define __PYTHON_BZFLAG_H__

namespace Python
{

class BZFlag
{
public:
	static BZFlag *GetInstance ();
	static int References;
	static void DeRef ();
protected:
	BZFlag ();
private:
	static BZFlag *instance;
	Event *event_sub;
	Team  *team_sub;
	PyObject *module;
};

};

#endif
