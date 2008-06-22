/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
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
 * @file Graph.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Graph class.
*/

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "graph/forward.h"
#include "graph/Node.h"
#include "graph/Edge.h"

namespace graph {

/** 
 * @class Graph
 * @brief Class defining double linked graph. 
 */
class Graph
{
private:
  /** The list of all Nodes. */
  NodeList nodeList;
  /** The list of all Edges. */
  EdgeList edgeList;
public:
  /** Default constructor. Creates an empty graph.*/
  Graph() {} 
};


};// end namespace graph


#endif // __GRAPH_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
