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
 * @file Random.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Random number generation helper functions.
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <algorithm>

/** 
 * @class Random
 * @brief Random number generation helper functions class.
 *
 * If the need arises, this file will also hold the mt1997 random number
 * generator implementation. The class is completely static as for the moment. 
 */
class Random 
{
public:
  /** @name Integer randomization */
  /** Returns either 0 or 1 with a 50/50 chance. */
  static inline int number01() { 
    return rand()%2; 
  }
  /** Returns a number from the 0..range-1 range. */
  static inline int numberMax( int range ) { 
    return (range == 0) ? 0 : rand()%range; 
  }
  /** Returns a number from the min..max-1 range. */
  static inline int numberRange( int min, int max ) { 
    return numberMax(max-min)+min; 
  }
  /** 
   * Returns a number from the min..max-1 range, the resultant number will be one 
   * that is a multiple of step in the given range. 
   */
  static inline int numberRangeStep( int min, int max, int step ) { 
    if (step == 0) return 0; 
    int steps = int((max-min) / step);  
    return numberMax(steps+1)*step+min; 
  }

  /** @name Floating point randomization */
  /** Returns a floating point number from the 0..1 range. */
  static inline double double01() { 
    return (double)(rand()) / (double)(RAND_MAX); 
  }
  /** Returns a floating point number from the 0..range range. */
  static inline double doubleMax( double range ) { 
    return double01()*range; 
  }
  /** Returns a floating point number from the min..max range. */
  static inline double doubleRange( double min, double max ) { 
    return doubleMax(max-min)+min; 
  }
  /** 
   * Returns a floating point number from the min..max-1 range, the resultant number will be one 
   * that is a multiple of step in the given range. 
   */
  static inline double doubleRangeStep( double min, double max, double step ) { 
    if (step == 0) return 0.0; 
   int steps = int((max-min) / step); 
    return numberMax(steps+1)*step + min; 
  }

  /** @name Boolean randomization */
  /** Returns a 50/50 chance as a boolean. */
  static inline bool coin() { 
    return rand()%2 == 0; 
  }
  /** Returns a percentage chance randomization as a boolean. */
  static inline bool chance(int chance) { 
    return numberMax(100) < chance; 
  }
  /**
   * Returns a random sign, that is either 1 or -1 with a 50/50 
   * probability.
   */
  static inline float sign() { 
    return rand()%2 == 0 ? 1.0f : -1.0f; 
  }
};

#endif /* __RANDOM_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
