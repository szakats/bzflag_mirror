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

#pragma warning (push)
#pragma warning (disable:4996)

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
  void LoggerSingleton::log( int level, char *str, ... ) {
    if ( level <= outputLogLevel ) {
      va_list va;
      va_start( va, str );
      if ( vsnprintf( buffer, 159, str, va ) == -1 )
        std::cerr << "LoggerSingleton::log, buffer is too small!\n" ;
      va_end( va );
      std::cout << buffer << "\n";
    }
  }
  /** 
   * Message logging, shortcut for logging without level (meaning always).
   */
  void LoggerSingleton::log( char *str, ... ) {
    va_list va;
    va_start( va, str );
    if ( vsnprintf( buffer, 159, str, va ) == -1 )
      std::cerr << "LoggerSingleton::log, buffer is too small!\n" ;
    va_end( va );
    std::cout << buffer << "\n";
  }
  /** 
   * Sets the level of output for the console.
   */
  void setOutputLogLevel( int level ) {
    outputLogLevel = level;
  }
private:
  /** 
   * Blocked constructor.
   */
  LoggerSingleton() {
    outputLogLevel = 2;
  }
  /** 
   * Blocked copy constructor.
   */
  LoggerSingleton( LoggerSingleton& ) {}
};

#define Logger LoggerSingleton::getInstance()


#pragma warning (pop)

#endif // __LOGGER_H__


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
