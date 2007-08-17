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

#ifndef __RULE_H__
#define __RULE_H__

#include <string>
#include <map>
#include "Product.h"
#include "globals.h"
#include "Mesh.h"

class Rule {
  std::string name;
  ProductVector* products;
public:
  Rule(const std::string& _name, ProductVector* _products) : name(_name), products(_products) {};
  Product* getProduct() {
    int size = products->size();
    if (size == 0) return NULL;
    ProductVectIter itr = products->begin();
    if (size == 1) return (*itr);
    float roll = randomFloat01();
    do {
      float rarity = (*itr)->getRarity();
      if (rarity < roll) return (*itr);
      roll -= rarity;
      ++itr;
    } while (itr!= products->end());
    return NULL;
  }
  int runMesh(Mesh* mesh, int face) {
    Product* product = getProduct();
    if (product == NULL) return -1;
    return product->runMesh(mesh,face);
  }
  ~Rule() { 
    ProductVectIter itr; 
    for (itr = products->begin(); itr!= products->end(); ++itr) delete (*itr);
    delete products; 
  }
};

typedef std::map <std::string, Rule*> RuleMap;
typedef RuleMap::iterator RuleMapIter;

#endif /* __RULE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
