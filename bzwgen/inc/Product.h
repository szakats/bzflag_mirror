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
 * @file Product.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Grammar Product class for BZWGen.
 */
#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "Operation.h"
#include "Expression.h"
#include "Mesh.h"

/**
 * @class Product
 * @brief Class representing a BZWGen grammar product.
 *
 * Rules constitute of products, products constitute of Operations.
 * A rule may be just a single product, or it may constitue of several ones
 * with different conditions and rarieties.
 */
class Product {
  /**
   * A vector of operations for the given product, that are to be executed
   * sequentially.
   */
  OperationVector* operations;
  /**
   * Rarity of the given product. The rarities of all the products in a
   * given rule should sum up to 1.0.
   */
  double rarity;
  /**
   * The condition under which the product may be executed. Checked with
   * each invocation at run-time.
   */
  Expression* condition;
public:
  /**
   * Constructor, to be called by the parser. Takes the vector of operations,
   * a rarity and condition. Initializes data, does nothing.
   */
  Product( OperationVector* _operations, double _rarity, Expression* _condition = NULL )
    : operations( _operations ), rarity( _rarity ), condition( _condition ) {};
  /**
   * Run the sequence of operations on the given mesh and the given face.
   * returns the face ID of the last result, or -1 if an error is encountered.
   */
  int runMesh( Mesh* mesh, int face ) {
    for ( size_t i = 0; i < operations->size(); i++ ) {
      face = operations->at( i )->runMesh( mesh, face );
      if ( face == -1 ) return -1;
    }
    return face;
  }
  /**
   * Checks wether the conditions that were assigned to this product are
   * met. If no conditions are assigned, returns true.
   */
  bool conditionsMet( Mesh* mesh, int face ) {
    return condition == NULL ? true : ( condition->calculate( mesh,face ) >= 0.0 );
  }
  /**
   * Rarity value accessor.
   */
  double getRarity() {
    return rarity;
  }
  /**
   * Destructor, frees all operations, and disposes of the operation
   * vector. Frees condition if present.
   */
  ~Product() {
    deletePointerVector( operations );
    deletePointer( condition );
  }
private:
  /**
   * Blocked default constructor. Product may only be instantiated by the
   * grammar.
   */
  Product( ) {}
  /**
   * Blocked copy constructor. Product has pointers, so it's unsafe for
   * copy.
   */
  Product( const Product& ) {}
};

typedef std::vector <Product*> ProductVector;
typedef ProductVector::iterator ProductVectIter;

#endif /* __PRODUCT_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
