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

#include "Rule.h"
#include "Random.h" 

Product* Rule::getProduct(Mesh* mesh, int face) {
  int size = products->size();
  if (size == 0) return NULL;
  ProductVectIter itr = products->begin();
  double roll = Random::double01();
  do {
    if ((*itr)->conditionsMet(mesh,face)) {
      double rarity = (*itr)->getRarity();
      if (rarity >= roll) return (*itr);
      roll -= rarity;
    }
    ++itr;
  } while (itr!= products->end());
  Logger.log( 1, "Warning : Rule '%s' returned no product!\n", name.c_str() );
  return NULL;
}
int Rule::runMesh(Mesh* mesh, int face) {
  Product* product = getProduct(mesh,face);
  return product == NULL ? -1 : product->runMesh(mesh,face);
}

Rule::~Rule() { 
  ProductVectIter itr; 
  for (itr = products->begin(); itr!= products->end(); ++itr) delete (*itr);
  delete products; 
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
