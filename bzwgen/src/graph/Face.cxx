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

#include "graph/Face.h"
#include "graph/Edge.h"
#include "graph/PlanarGraph.h"

namespace graph {

NodeVector Face::getNodes() {
  NodeVector result;
  for (EdgeVectorIterator itr = edges.begin(); itr!= edges.end(); ++itr) {
    result.push_back((*itr)->getSource());
  }
  return result;
}

size_t Face::addEdge( Edge* edge ) {
  edges.push_back( edge );
  edge->setFace( this );
  return edges.size() - 1;
}


}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8