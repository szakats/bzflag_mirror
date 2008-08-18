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
 * @file SortedEdgeList.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an EdgeList class model for the Graph class.
 */

#ifndef __SORTEDEDGELIST_H__
#define __SORTEDEDGELIST_H__

#include <set>
#include <cassert>
#include "graph/forward.h"

// The EdgeList class is a part of Graph class
namespace graph {

/**
 * @class SortedEdgeList
 * @brief A sorted edge list class
 *
 * Edges are sorted clockwise, comparing the angle to the (1.0,0.0)
 * vector.
 */
class SortedEdgeList
{
private:
  /**
   * Structure for edge comparison.
   */
  struct EdgeCompare {
    /**
    * Edge comparision function. Comparision is done by comparing
    * angles.
    */
    bool operator()( Edge* s1, Edge* s2) const;
  };
public:
  /**
   * Type definition for the EdgeSet
   */
  typedef std::set<Edge*,EdgeCompare> EdgeSet;
  /**
   * Type definition for the EdgeSet iterator
   */
  typedef EdgeSet::iterator iterator;
private:
  /**
   * This is the set used to actually store the Edge pointers.
   */
  EdgeSet edgeSet;
public:
	/**
   * Default constructor.
   */
	SortedEdgeList() {}
  /**
   * Adds an edge to the list.
   */
  void insert( Edge* edge ) {
    edgeSet.insert(edge);
  }
  /**
   * Removes an edge from the list.
   */
  void remove( Edge* edge ) {
    edgeSet.erase(edge);
  }
  /**
   * Returns the begin iterator. Note that the iterator is NOT cyclic.
   */
  iterator begin() {
    return edgeSet.begin();
  }
  /**
   * Returns the end iterator. Note that the iterator is NOT cyclic.
   */
  iterator end() {
    return edgeSet.end();
  }
  /**
   * Returns the next edge stored after the one passed. Due to the clockwise
   * ordering, the next one will be the next in clockwise order. Also, the
   * list is treated as a cyclic list, so there is always a next element,
   * unless there are no elements.
   *
   * If the passed element is not in the list (hence also if it's empty) then
   * NULL is returned.
   */
  Edge* next( Edge* edge ) {
    iterator edgeItr = edgeSet.find( edge );
    if ( edgeItr == edgeSet.end( ) ) return NULL;
    edgeItr++;
    if ( edgeItr == edgeSet.end( ) ) edgeItr = edgeSet.begin();
    return (*edgeItr);
  }
  /**
   * Returns the previous edge stored after the one passed. Due to the
   * clockwise ordering, the next one will be the next in clockwise order.
   * Also, the list is treated as a cyclic list, so there is always a next
   * element, unless there are no elements.
   *
   * If the passed element is not in the list (hence also if it's empty) then
   * NULL is returned.
   */
  Edge* prev( Edge* edge ) {
    iterator edgeItr = edgeSet.find( edge );
    if ( edgeItr == edgeSet.end( ) ) return NULL;
    edgeItr--;
    if ( edgeItr == edgeSet.end( ) ) edgeItr = edgeSet.begin();
    return (*edgeItr);
  }
  /**
   * Clears the list. Does not dispose of the edges.
   */
  void clear() {
    edgeSet.clear();
  }
  /**
   * Clears the list. Does not dispose of the edges.
   */
  size_t size() const {
    return edgeSet.size();
  }
  /**
   * Returns the "first" (smallest) edge in the list.
   */
  Edge* first( ) {
    return (*edgeSet.begin());
  }
private:
  /**
   * Blocked copy constructor.
   */
  SortedEdgeList(const Edge& ) {}
};


} // namespace end Graph

#endif // __SORTEDEDGELIST_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
