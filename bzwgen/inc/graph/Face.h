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
   * that is to allow a clockwise full traversal of the face.
   */
  EdgeList edges;
  /**
   * Pointer to the graph of which this face is part of.
   */
  GraphPtr graph;
public:
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
