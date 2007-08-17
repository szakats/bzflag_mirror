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

#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include <vector>
#include "Operation.h"
#include "Mesh.h"

class Product {
  OperationVector* ops;
  float rarity;
public:
  Product(OperationVector* _ops, float _rarity = 1.0f) : ops(_ops), rarity(_rarity) {};
  int runMesh(Mesh* mesh, int face) {
    for (size_t i = 0; i < ops->size(); i++) {
      face = ops->at(i)->runMesh(mesh,face);
      if (face == -1) return -1;
    }
    return face;
  }
  float getRarity() { return rarity; }
  ~Product() {
    OperationVectIter itr; 
    for (itr = ops->begin(); itr!= ops->end(); ++itr) delete (*itr);
    delete ops; 
  }
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
