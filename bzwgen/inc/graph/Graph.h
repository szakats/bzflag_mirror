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

#include "graph/Node.h"
#include "graph/Edge.h"

/** 
 * @class Graph
 * @brief Class defining double linked graph. 
 */
class Graph
{
private:
  /** The list of all Nodes. */
  nodeList Node::NodeList;
  /** The list of all Edges. */
  edgeList Edge::EdgeList;
public:
  /** Default constructor. Creates an empty planar graph.*/
  Graph() {} 
};


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __GRAPH_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
