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
/** 
 * @file Logger.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a Logger class for bzwgen.
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h> 
#include <stdarg.h>
#include <iostream> 
#include <fstream> 

#ifdef _WIN32
  #pragma warning (push)
  #pragma warning (disable:4996)
#endif

/**
 * @class Logger
 * @brief Logger class for the generator
 *
 * The logger class is a utility class for debugging BZWGen, it handles
 * information, error and warning messages for BZWGen. 
 */
class LoggerSingleton {
  /**
   * Buffer for formating of the message using vsnprintf
   */
  char buffer[160];
  /** 
   * Log level of messages going to the standard output (cout)
   */
  int outputLogLevel;
  /** 
   * Log level of messages going to the file output.
   */
  int fileLogLevel;
  /**
   * Stream for file output. Initialized when setFileLogLevel is called.
   */
  std::ostream* fileStream;
public:
  /** 
   * Singleton instance accessor.
   */
  static LoggerSingleton& getInstance() {
    static LoggerSingleton singleton;
    return singleton;
  }
  /** 
   * Message logging, takes level and printf-like syntax.
   */
  void log( int level, const char *str, ... ) {
    if ( !needsLogging( level ) ) return;
    va_list va;
    va_start( va, str );
    if ( vsnprintf( buffer, 159, str, va ) == -1 )
      std::cerr << "LoggerSingleton::log, buffer is too small!\n" ;
    va_end( va );
    if ( level <= outputLogLevel ) std::cout << buffer << "\n";
    if ( level <= fileLogLevel && fileStream ) (*fileStream) << buffer << "\n";
  }
  /** 
   * Message logging, shortcut for logging without level (meaning always).
   */
  void log( const char *str, ... ) {
    va_list va;
    va_start( va, str );
    if ( vsnprintf( buffer, 159, str, va ) == -1 )
      std::cerr << "LoggerSingleton::log, buffer is too small!\n" ;
    va_end( va );
    std::cout << buffer << "\n";
    if ( fileStream ) (*fileStream) << buffer << "\n";
  }
  /** 
   * Sets the level of output for the console.
   */
  void setOutputLogLevel( int level ) {
    outputLogLevel = level;
  }
  /** 
   * Sets the level of output for the console.
   */
  void setFileLogLevel( int level ) {
    fileLogLevel = level;
    if (!fileStream) fileStream = new std::ofstream("log.txt");
  }
  ~LoggerSingleton() {
    if (fileStream) delete fileStream;    
  }
private:
  /** 
   * Returns true if either outputLogLevel or fileLogLevel is greater then
   * the passed argument.
   */
  bool needsLogging( int level ) {
    return ( level <= outputLogLevel || level <= fileLogLevel );
  }
  /** 
   * Blocked constructor.
   */
  LoggerSingleton() {
    outputLogLevel = 2;
    fileLogLevel = 0;
    fileStream = NULL;
  }
  /** 
   * Blocked copy constructor.
   */
  LoggerSingleton( LoggerSingleton& ) {}
};

#define Logger LoggerSingleton::getInstance()

#ifdef _WIN32
  #pragma warning (pop)
#endif

#endif // __LOGGER_H__


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
