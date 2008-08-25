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
/**
 * @file Rule.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Grammar Rule class for BZWGen.
 */

#ifndef __RULE_H__
#define __RULE_H__

#include "globals.h"
#include "Product.h"
#include "Mesh.h"

/**
 * @class Rule
 * @brief Class representing a BZWGen grammar rule.
 *
 * Rules constitute of products, products constitute of Operations.
 * A rule may be just a single product, or it may constitue of several ones
 * with different conditions and rarieties.
 */
class Rule {
  /**
   * Name of the rule, by which it is executed.
   */
  String name;
  /**
   * A vector of Product pointers, that this rule is made of.
   */
  ProductVector* products;
public:
  /**
   * Standard constructor, only sets up members.
   */
  Rule( const String& _name, ProductVector* _products )
    : name( _name ), products( _products ) { };
  /**
   * Runs the rule on the passed mesh, on the passed face.
   */
  int runMesh( Mesh* mesh, int face );
  /**
   * Returns the name of the rule.
   */
  String& getName() {
    return name;
  }
  /**
   * Standard destructor, deallocates all the stored products, and the
   * product vector itself.
   */
  ~Rule() {
    deletePointerVector( products );
  };
private:
  /**
   * Returns a product for the given mesh/face. The choice of product
   * is dependent on the rule, and may be based on a random pick, and/or
   * on conditions to the passed mesh/face.
   */
  Product* getProduct( Mesh* mesh, int face );
};

typedef std::map <String, Rule*> RuleMap;
typedef RuleMap::iterator RuleMapIter;

#endif /* __RULE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
