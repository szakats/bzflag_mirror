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

#include "BZWGenerator.h"
#include "time.h"
#include "Output.h"
#include "GridGenerator.h"
#include "FaceGenerator.h"
#include <sstream>
#include <iostream>

typedef std::ostringstream OutStringStream;

extern int yyparse(RuleSet*);
extern int yylineno;
extern FILE* yyin;

#ifdef _USE_LIB_RULES_
std::vector<void*> handleList;

const char* getLibExtension ( void )
{
#ifdef _WINt 2
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

void BZWGenerator::loadConfig(const char* configFile) {
  cmd.Set(COSFile(configFile));
}

bool BZWGenerator::getOptionI ( int &val, const char* shortName, const char* longName )
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

bool BZWGenerator::getOptionS ( String &val, const char* shortName, const char* longName )
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

int BZWGenerator::setup( ) {
  ruledir.SetStdDir( "./rules" );
  String temp;

  int detail = 3;
  int debugLevel = 2;
  int fileDebugLevel = -1;

  getOptionI( debugLevel,     "d", "debug" );
  getOptionI( fileDebugLevel, "f", "filedebug" );

  Logger.setOutputLogLevel( debugLevel );
  if ( fileDebugLevel >= 0 ) {
    Logger.setFileLogLevel( fileDebugLevel );
  }

  getOptionI( detail,      "l", "detail" );
  getOptionS( texturepath, "t", "texture" );
  if ( getOptionS( temp, "r", "rulesdir" ) )
    ruledir.SetOSDir( temp.c_str() );

  COSFile file;
  ruleset = new RuleSet();

  while ( ruledir.GetNextFile( file, "*.set", false ) ) {
    Logger.log( 1, "BZWGenerator : loading %s... ", file.GetOSName() );
    file.Open( "r" );
    yyin = file.GetFile();
    if ( yyparse( ruleset ) == 0) {
      Logger.log( 3, "BZWGenerator : loading done." );
    } else {
      Logger.log( "BZWGenerator : loading %s failed!", file.GetOSName() );
      return 1;
    }
    yylineno = 1;
    file.Close();
  }

  loadPlugIns();

  bool passsidewalk = ( cmd.Exists( "w" ) || cmd.Exists( "sidewalk" ) );
  experimental      = ( cmd.Exists( "e" ) || cmd.Exists( "experimental" ) );

  ruleset->initialize();
  ruleset->addAttr( "DETAIL",            double( detail ) );
  ruleset->addAttr( "PASSABLE_SIDEWALK", double( passsidewalk ? 1.0 : -1.0 ) );

  srand( (unsigned int) time( NULL ) );

  return 0;
}

void BZWGenerator::generate( OutStream* outstream ) {
  Logger.log( 1, "BZWGenerator : initializing... " );
  Generator* gen;
  if ( experimental )
    gen = new FaceGenerator( ruleset );
  else
    gen = new GridGenerator( ruleset );

  Logger.log( 1, "BZWGenerator : parsing options... " );
  gen->parseOptions( &cmd );

  Logger.log( 1, "BZWGenerator : generating... " );
  gen->run( );

  Logger.log( 1, "BZWGenerator : outputing... " );
  Output os( outstream, texturepath );
  os.info( BZWGMajorVersion, BZWGMinorVersion, BZWGRevision );
  gen->output( os );
  os.footer( );

  Logger.log( 1, "BZWGenerator : generation done. ");
  delete gen;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

