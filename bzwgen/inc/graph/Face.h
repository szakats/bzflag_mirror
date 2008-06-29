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
 * @file Face.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Face class model for the Graph class.
 *
 */

#ifndef __FACE_H__
#define __FACE_H__

#include <vector>
#include "graph/forward.h"

// The Face class is a part of Graph class
namespace graph {

/** 
 * @class Face
 * @brief Class defining an Face in a double linked planar graph. 
 *
 * As the planar graph is dual-linked we assume that a face is made 
 * up from clockwise edges around it. Hence, each edge belongs to 
 * only ONE face. Let's also note, that with the edge list we can 
 * retrieve the vertices list without much problems.
 */
class Face : public IObject
{
private:
  /**
   * List of edges belonging to this face. The edges should be stored in order,
   * that is to allow a clockwise full traversal of the face. Stored are only
   * pointers, disposing of edges is the responsibility of either Graph or Node
   * class.
   */
  EdgeVector edges;
  /**
   * Pointer to the graph of which this face is part of.
   */
  PlanarGraphPtr graph;
public:
  /**
   * Constructor, takes only a pointer to the Graph that the Face 
   * belongs to. Edges need to be added manualy via addEdge in clockwise order.
   */
  Face(PlanarGraphPtr _graph) : graph(_graph) {}
  /** 
   * Adds an Edge to the face. Note that edges NEED to be added in clockwise 
   * order, or else most of the algorithms here won't work. Edges are still
   * belonging to the Graph class, Face just holds pointers to them. Returns
   * the index of the added Edge.
   */
  size_t addEdge(EdgePtr edge) {
    edges.push_back(edge);
    return edges.size()-1;
  }
  /** 
   * Returns the size (amount of stored Edge s) of the Face.
   */
  size_t size() const {
    return edges.size();
  }
private:
  /** Blocked default constructor */
  Face() {}
  /** Blocked copy constructor */
  Face(const Face& ) {}

};


}; // namespace end graph

#endif // __FACE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
