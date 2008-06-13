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
/** \file GraphNode.h
    \author Kornel Kisielewicz kornel.kisielewicz@gmail.com
    \brief Defines an GraphNode class model for the PlanarGraph class.
*/

#ifndef __GRAPHNODE_H__
#define __GRAPHNODE_H__

#include <vector>
#include "Point2D.h"

// I'll maybe think later of adding namespaces
//namespace bzwgen {
//namespace geometry {

/** \class GraphNode
    \brief Class defining an node in a double linked planar graph. 

    Due to the fact that the PlanarGraph is dual linked, each GraphNode holds a 
    set of pointers to other GraphNodes. The initial implementation uses vector
    for holding the GraphNode pointers, but that probably will be changed to
    sets in the future. Also, currently I use pointers, but as there will be heavy 
    work on node addition, and maybe removal, I'll switch to boost::weak_ptr later.

    The graph node inherits Point2D, cause the main definition of it's existence is 
    it's place on the 2D planar graph.

*/
class GraphNode : public Point2D
{
public:
  /** A graph node pointer. Currently it's a C++ pointer, will be a boost::weak_ptr 
      in the future. */
  typedef GraphNode*		    GraphNodePtr;
  /** A list of GraphNodePtr's. Should be at least a set, or an ordered list for 
      more easy Shape extraction.*/
  typedef std::vector<GraphNodePtr> GraphNodeList;
private:
  /** The list of GraphNode s that this node is connected to. Currently it's
      unordered, will be probably sorted clockwise for easier shape retrival 
      later. */
  GraphNodeList connections;
public:
  /** Default constructor. Sets the underlying point to (0,0) and initializes
      an empty list of connections.*/
  GraphNode() {} 

};


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __GRAPHNODE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8