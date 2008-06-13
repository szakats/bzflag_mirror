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
/** \file PlanarGraph.h
    \author Kornel Kisielewicz kornel.kisielewicz@gmail.com
    \brief Defines an PlanarGraph class for road construction and/or zoning.
*/

#ifndef __PLANARGRAPH_H__
#define __PLANARGRAPH_H__

#include "GraphNode.h"

// I'll maybe think later of adding namespaces
//namespace bzwgen {
//namespace geometry {

/** \class PlanarGraph
    \brief Class defining double linked planar graph. 

    The PlanarGraph is implemented as a list of GraphNode's each of which
    holds a list of it's unique neighbours. 
*/
class PlanarGraph
{
private:
  /** The list of all GraphNodes. */
  GraphNodeList GraphNode::GraphNodeList;
public:
  /** Default constructor. Creates an empty planar graph.*/
  PlanarGraph() {} 

};


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __PLANARGRAPH_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8