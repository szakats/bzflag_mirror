/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <iostream>
#include <cstring>
#include "BZWGeneratorStandalone.h"
#include "Output.h"


void BZWGeneratorStandalone::printHelpCommand ( const char* shortName, const char* longName, const char* description )
{
  if (shortName && strlen(shortName))
  {
    std::cout << argumentDeliminator;
    std::cout << shortName;
    std::cout << " (";
  }
  std::cout << argumentDeliminator;
  std::cout << longName;
  if (shortName && strlen(shortName))
    std::cout << ") ";
  std::cout << description;
  std::cout << "\n";
}

void BZWGeneratorStandalone::printHelp() {
  std::cout << "\nBZWGen by Kornel 'Epyon' Kisielewicz\n";
  printf("Version %d.%d.%d(%s)\nCopyright 2007-2008 BZFlag Project and Tim Riker\n\n",BZWGMajorVersion,BZWGMinorVersion,BZWGRevision,BZWGBuildState);
  std::cout << "Command line arguments:\n";
  printHelpCommand("h","help","                shows help");
  printHelpCommand("d","debug","integer        sets debug level (0-4)(default: 2)");
  printHelpCommand("f","filedebug","integer    sets log.txt output debug level (0-4)(default: no output)");
  printHelpCommand("o","output","filename      sets output filename (default: map.bzw)");
  printHelpCommand("r","rulesdir","directory   sets rules directory (default: rules)");;
  printHelpCommand("s","size","integer         sets world size (default: 800)");
  printHelpCommand("g","gridsize","integer     sets grid size (default: 42)");
  printHelpCommand("p","gridsnap","integer     sets the grid snap (default: 3)");
  printHelpCommand("f","fullslice","integer    sets the number of full slices (default: 8)");
  printHelpCommand("v","subdiv","integer       sets the number of subdivisions (default: 120)");
  printHelpCommand("b","bases","integer        sets number of bases (0/2/4)(default: 0)");
  printHelpCommand("","ctfsafe","                   turns flag safety zones on for CTF maps");
  printHelpCommand("l","detail","integer       sets the level of detail (1-3)(default: 3)");
  printHelpCommand("w","sidewalk","            sets sidewalks passable (default: false)");
  printHelpCommand("t","texture","URL          sets the URL for textures");
  printHelpCommand("e","experimental","        turns on experimental generator\n");
}

int BZWGeneratorStandalone::parseCommandLine(int argc, char* argv[]) {
  cmd.SetDelimnator(argumentDeliminator);
  cmd.Set(argc,argv);
  if (cmd.Exists("h") || cmd.Exists("help"))	  { printHelp(); return 1; }

  if (cmd.Exists("c"))		    cmd.Set(COSFile(cmd.GetDataS("c")));
  else if (cmd.Exists("config"))    cmd.Set(COSFile(cmd.GetDataS("config")));

  outname = "map.bzw";
  getOptionS(outname,"o","output");
  return 0;
}



// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

