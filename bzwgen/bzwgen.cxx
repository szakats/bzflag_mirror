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

#include "Output.h"
#include "Generator.h"
#include "GridGenerator.h"
#include "Material.h"
#include "commandArgs.h"
#include "Zone.h"
#include "RuleSet.h"

extern int yyparse(RuleSet*);
extern FILE* yyin;

int debugLevel = 1;

int main (int argc, char* argv[]) {
  CCommandLineArgs* cmd = new CCommandLineArgs(argc,argv);

  COSDir ruledir;
  ruledir = "rules";
  std::string outname = "test.bzw";
  if (cmd->Exists("d"))         { debugLevel = cmd->GetDataI("d"); }
  if (cmd->Exists("-rulesdir")) { ruledir    = cmd->GetDataS("-rulesdir"); }
  if (cmd->Exists("o"))         { outname    = cmd->GetDataS("o"); }
  if (cmd->Exists("-output"))   { outname    = cmd->GetDataS("-output"); }
  
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
      return 0;
    }
    file.Close();
  }

  ruleset->initialize();

  srand((unsigned int)time(NULL));

  std::cout << "Initializing... ";
  GridGenerator gen(ruleset);
  std::cout << "done.\n";

  std::cout << "Parsing options... ";
  gen.parseOptions(cmd);
  std::cout << "done.\n";

  std::cout << "Generating... ";
  gen.run();
  std::cout << "done.\n";

  Output os(outname.c_str());
  std::cout << "Outputing... ";
  gen.output(os);
  std::cout << "done.\n";

  delete cmd;
}

	
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
