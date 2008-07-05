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
* @file SortedEdgeList.h
* @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
* @brief Defines an EdgeList class model for the Graph class.
*/

#ifndef __SORTEDEDGELIST_H__
#define __SORTEDEDGELIST_H__

#include <vector>
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
		/** Structure for edge comparison. */
		struct EdgeCompare {
        /** 
         * Edge comparision function. Comparision is done by comparing 
         * angles. 
         */
        bool operator()( Edge* s1, Edge* s2) const {
            return s1->getAngle() > s2->getAngle();
        }
		};
	public:
		/** Default constructor */
		SortedEdgeList() {}
	private:
		/** Blocked copy constructor */
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
