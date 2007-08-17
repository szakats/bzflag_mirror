/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <fstream>
#include <iostream>
#include "time.h"

// TODO list for simple grid based generator
// ------------------------------------------
// PARTIAL DONE TODO: textures
// PARTIAL DONE TODO: random empty areas (parks?)
// TODO: railway/highway
// DONE TODO: cleanup, file split
// TODO: team bases!
// TODO: proper command line handling
// TODO: implement option handling
// TODO: doxygen 
// DONE TODO: building recoloring
// TODO: zone subdivision
// TODO: object wrappers for basic objects
// DONE TODO: zones need to use world coordinates



#include "Output.h"
#include "Generator.h"
#include "GridGenerator.h"
#include "Material.h"
#include "commandArgs.h"
#include "Zone.h"
#include "RuleSet.h"

extern int yyparse(RuleSet*);
extern FILE* yyin;

int main (int argc, char* argv[]) {
  COSDir ruledir("rules");
  COSFile file;
  RuleSet* ruleset = new RuleSet();
  
  while (ruledir.GetNextFile(file,"*.set",false)) {
    std::cout << "Loading " << file.GetOSName() << "... ";
    file.Open("r");
    yyin = file.GetFile();
    if (yyparse(ruleset) == 0) {
      std::cout << "done.\n";
    } else {
      std::cout << "failed!\n";
    }
    file.Close();
  }

  srand((unsigned int)time(NULL));

  CCommandLineArgs cmd(argc,argv);

  std::cout << "Initializing...\n";
  GridGenerator gen(ruleset);
  std::cout << "done.\n";

  std::cout << "Parsing options...\n";
  gen.parseOptions(0);
  std::cout << "done.\n";

  std::cout << "Generating...\n";
  gen.run();
  std::cout << "done.\n";

  Output os("test.bzw");
  std::cout << "Outputing... ";
  gen.output(os);
  std::cout << "done.\n";
}

	
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
