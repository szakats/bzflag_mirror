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
 * @file forward.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief File with forward definitions for all the Graph classes
 *
 * Created to prevent unneccessary includes, and to avoid header
 * file recursion
 */

#ifndef __FORWARD_H__
#define __FORWARD_H__

#include <vector>
#include <string>
#include <cassert>
#include "ObjectList.h"

// The e class is a part of Graph class
namespace graph {

class PlanarGraph;
class Node;
class Edge;
class Face;
class SortedEdgeList;

typedef TObjectList<Edge*> EdgeList;
typedef TObjectList<Node*> NodeList;
typedef TObjectList<Face*> FaceList;

typedef std::vector<Edge*> EdgeVector;
typedef std::vector<Node*> NodeVector;
typedef std::vector<Face*> FaceVector;

typedef EdgeVector::iterator EdgeVectorIterator;
typedef NodeVector::iterator NodeVectorIterator;
typedef FaceVector::iterator FaceVectorIterator;

} // namespace end graph

#endif // __FORWARD_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

