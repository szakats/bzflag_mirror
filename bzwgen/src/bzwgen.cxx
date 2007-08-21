/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
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

int debugLevel = 2;
CCommandLineArgs  cmd;
COSDir ruledir;

#ifdef _USE_GNU_DELIMS
std::string argumentDeliminator = "--";
#else
std::string argumentDeliminator = "-";
#endif //_USE_GNU_DELIMS

void printHelpCommand ( const char* shortName, const char* longName, const char* description )
{
  std::cout << argumentDeliminator;
  std::cout << shortName;
  std::cout << " (";
  std::cout << argumentDeliminator;
  std::cout << ") ";
  std::cout << description;
  std::cout << "\n";
}
  
void printHelp() {
  std::cout << "\nBZWGen by Kornel 'Epyon' Kisielewicz\n\n";
  std::cout << "Command line arguments:\n";
  printHelpCommand("h","help","               shows help");
  printHelpCommand("d","debug","integer       sets debug level (0-4)(default: 2)");
  printHelpCommand("o","output","filename     sets output filename (default: map.bzw)");
  printHelpCommand("r","rulesdir","directory  sets rules directory (defualt: rules)");;
  printHelpCommand("s","size","integer        sets world size (defualt: 800)");
  printHelpCommand("g","gridsize","integer    sets grid size (defualt: 42)");
  printHelpCommand("p","gridsnap","integer    sets the grid snap (defualt: 3)");
  printHelpCommand("f","fullslice","integer   sets the number of full slices (defualt: 8)");
  printHelpCommand("v","subdiv","integer      sets the number of subdivisions (defualt: 120)");
  printHelpCommand("b","bases)","integer      sets number of bases (0/2/4)(defualt: 0)\n");
}

#ifdef _USE_LIB_RULES_
const char* getLibExtension ( void )
{
#ifdef _WIN32
  return "*.dll";
#else
  return "*.so";
#endif
}

void loadOnePlugIn ( const char* file )
{
#ifdef _WIN32

#else

#endif
}
#endif //_USE_LIB_RULES_

void loadPlugIns ( void )
{
#ifdef _USE_LIB_RULES_

  while (ruledir.GetNextFile(file,getLibExtension(),false))
    loadOnePlugIn(file.GetOSName());

#endif // _USE_LIB_RULES_
}

int main (int argc, char* argv[]) {

  cmd.SetDelimnator(argumentDeliminator);
  cmd.Set(argc,argv);

  ruledir = "rules";
  std::string outname = "map.bzw";

  if (cmd.Exists("c"))        { COSFile f = COSFile(cmd.GetDataS("c"));      cmd.Set(f); }
  if (cmd.Exists("config"))   { COSFile f = COSFile(cmd.GetDataS("config")); cmd.Set(f); }

  if (cmd.Exists("h"))        { printHelp(); return 0; }
  if (cmd.Exists("help"))     { printHelp(); return 0; }
  if (cmd.Exists("d"))        { debugLevel = cmd.GetDataI("d"); }
  if (cmd.Exists("debug"))    { debugLevel = cmd.GetDataI("debug"); }
  if (cmd.Exists("r"))        { ruledir    = cmd.GetDataS("r"); }
  if (cmd.Exists("rulesdir")) { ruledir    = cmd.GetDataS("rulesdir"); }
  if (cmd.Exists("o"))        { outname    = cmd.GetDataS("o"); }
  if (cmd.Exists("output"))   { outname    = cmd.GetDataS("output"); }
  
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

  loadPlugIns();

  ruleset->initialize();

  srand((unsigned int)time(NULL));

  std::cout << "Initializing... ";
  GridGenerator gen(ruleset);
  std::cout << "done.\n";

  std::cout << "Parsing options... ";
  gen.parseOptions(&cmd);
  std::cout << "done.\n";

  std::cout << "Generating... ";
  gen.run();
  std::cout << "done.\n";

  Output os(outname.c_str());
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
