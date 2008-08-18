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
  PlanarGraph* graph;
  /**
  * Pointer to a subgraph. Unless explicitly created is always null.
  */
  PlanarGraph* subgraph;
public:
  /**
   * Constructor, takes only a pointer to the Graph that the Face
   * belongs to. Edges need to be added manually via addEdge in clockwise order.
   */
  Face( PlanarGraph* _graph ) : graph( _graph ), subgraph( NULL ) {}
  /**
   * Adds an Edge to the face. Note that edges NEED to be added in clockwise
   * order, or else most of the algorithms here won't work. Edges are still
   * belonging to the Graph class, Face just holds pointers to them. Returns
   * the index of the added Edge. Also sets the edge face owner to self.
   */
  size_t addEdge( Edge* edge );
  /**
   * Returns the size (amount of stored Edge s) of the Face.
   */
  size_t size( ) const {
    return edges.size( );
  }
  /**
   * Returns a vector of Node's that are a part of this face.
   */
  NodeVector getNodes( ) const;
  /**
   * Initializes subgraph. The subgraph initially will be made of a copy of
   * the face that is it's parent. As a convenience, returns a pointer
   * to the created subgraph. If the graph created would be "degenerate"
   * (i.e. either a point or a single double-edge) then the method returns
   * NULL.
   */
  PlanarGraph* initializeSubgraph( );
  /**
   * Returns a pointer to the subgraph, NULL if no subgraph is stored.
   */
  PlanarGraph* getSubgraph( ) const {
    return graph;
  }
  /**
   * Calculates the area of the polygon created by the faces edges.
   */
  float area( ) const;
  /**
   * Destructor. Destroys the underlying subgraph if one exists.
   */
  ~Face( );
  /**
   * Returns a string representation of the Face.
   */
  std::string toString() const;
  /**
   * Returns wether face is convex.
   */
  bool isConvex();
private:
  /** Blocked default constructor */
  Face( ) {}
  /** Blocked copy constructor */
  Face( const Face& ) {}

};


} // namespace end graph

#endif // __FACE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
