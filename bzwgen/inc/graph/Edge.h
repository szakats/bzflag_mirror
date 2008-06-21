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
 * @file Edge.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Edge class model for the Graph class.
 */

#ifndef __EDGE_H__
#define __EDGE_H__

#include <vector>
#inlu

// The Edge class is a part of Graph class
namespace Graph {

/** 
 * @class Edge
 * @brief Class defining an Edge in a double linked planar graph. 
 */
class Edge 
{
public:
  /** 
   * A graph edge pointer. Currently it's a C++ pointer, may be a boost::weak_ptr 
   * in the future. 
   */
  typedef Edge* EdgePtr;
  /** 
   * A list of EdgePtr's. Should be at least a set, or an ordered list for 
   * more easy Shape extraction.
   */
  typedef std::vector<EdgePtr> EdgeList;
private:
  /**
   * The ID of the given edge. Should match the index in the owning Graph
   * class.
   */
  int ID;
public:
};


}; // namespace end Graph

#endif // __EDGE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
