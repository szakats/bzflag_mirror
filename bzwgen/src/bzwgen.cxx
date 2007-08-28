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

#define MajorVersion "0"
#define MinorVersion "1"
#define Revision     "2"
#define BuildState "development"

extern int yyparse(RuleSet*);
extern int yylineno;
extern FILE* yyin;

int debugLevel = 2;
CCommandLineArgs  cmd;
COSDir ruledir;
std::string texturepath;

#ifdef _USE_GNU_DELIMS
std::string argumentDeliminator = "--";
#else
std::string argumentDeliminator = "-";
#endif //_USE_GNU_DELIMS

void printHelpCommand ( const char* shortName, const char* longName, const char* description )
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

void printHelp() {
  std::cout << "\nBZWGen by Kornel 'Epyon' Kisielewicz\n";
  printf("Version %s.%s.%s(%s)\ncopyright 2007 BZFlag Project and Tim Rikker\n\n",MajorVersion,MinorVersion,Revision,BuildState);
  std::cout << "Command line arguments:\n";
  printHelpCommand("h","help","                shows help");
  printHelpCommand("d","debug","integer        sets debug level (0-4)(default: 2)");
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
  printHelpCommand("t","texture","URL          sets the URL for textures\n");
}

std::vector<void*> handleList;

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
  bool *init ( int* ) = 0;
#ifdef _WIN32
    HANDLE hLib = LoadLibrary(file);
    if (!hLib)
      return;
    
    init = GetProcAddress(hLib,"bzwgen_init_plugin");
    if (!init)
    {
      FreeLibrary(hLib);
      return;
    }
    handleList.push_back((void*)hLib);
#else

#endif
  
  if (init && *init(0))
  {
    // do something
  }
}

void freeOnePlugIn ( void *handle )
{
#ifdef _WIN32
  FreeLibrary((HMODULE)handle);
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

void freePlugins ( void )
{
 #ifdef _USE_LIB_RULES_
  for ( int i = 0; i < (int)handleList.size(); i++ )
    freeOnePlugIn(handleList[i]);
#endif // _USE_LIB_RULES_
}

bool getOptionI ( int &val, char* shortName, char* longName )
{
  if (cmd.Exists(shortName))
  {
    val = cmd.GetDataI(shortName);
    return true;
  }

  if (cmd.Exists(longName))
  {
    val = cmd.GetDataI(longName);
    return true;
  }
  return false;
}

bool getOptionS ( std::string &val, char* shortName, char* longName )
{
  if (cmd.Exists(shortName))
  {
    val = cmd.GetDataS(shortName);
    return true;
  }

  if (cmd.Exists(longName))
  {
    val = cmd.GetDataS(longName);
    return true;
  }
  return false;
}

int main (int argc, char* argv[]) {

  cmd.SetDelimnator(argumentDeliminator);
  cmd.Set(argc,argv);

  if (cmd.Exists("h") || cmd.Exists("help"))	  { printHelp(); return 0; }

  if (cmd.Exists("c"))		    cmd.Set(COSFile(cmd.GetDataS("c")));
  else if (cmd.Exists("config"))    cmd.Set(COSFile(cmd.GetDataS("config")));

  ruledir .SetStdDir("./rules");
  std::string temp,outname = "map.bzw";

  int detail = 3;

  getOptionI(debugLevel,"d","debug");
  getOptionS(outname,"o","output");
  getOptionI(detail,"l","detail");
  getOptionS(texturepath,"t","texture");
  if(getOptionS(temp,"r","rulesdir"))
    ruledir.SetOSDir(temp.c_str());
  
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
      return 1;
    }
    yylineno = 1;
    file.Close();
  }

  loadPlugIns();

  bool passsidewalk = false;
  if (cmd.Exists("w") || cmd.Exists("sidewalk")) { passsidewalk = true; }

  ruleset->initialize();
  ruleset->addAttr("DETAIL",double(detail));
  ruleset->addAttr("PASSABLE_SIDEWALK",double(passsidewalk ? 1.0 : -1.0));

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
  os << "#\n# BZWGen (" << MajorVersion << "." << MinorVersion << "." Revision << ") generated map file\n#\n\n";
  gen.output(os);
  os.footer();
  std::cout << "done.\n";
  std::cout << "\n" << os.vertices << " vertices, " << os.texcoords << " texcoords and " << os.faces << " faces generated.\n";

}

	
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
