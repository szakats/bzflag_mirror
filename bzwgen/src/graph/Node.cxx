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

#include "graph/Node.h"
#include "graph/PlanarGraph.h"

namespace graph {

void Node::orphanize() {
  for ( SortedEdgeList::iterator itr = incoming.begin(); itr!= incoming.end(); ++itr) {
    graph->removeEdge((*itr)->ID);
  }
  for ( SortedEdgeList::iterator itr = outgoing.begin(); itr!= outgoing.end(); ++itr) {
    graph->removeEdge((*itr)->ID);
  }
  incoming.clear();
  outgoing.clear();
}


}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
