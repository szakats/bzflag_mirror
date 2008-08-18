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
 * @file ObjectList.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an ObjectList template for the Graph class.
 *
 * The purpose of IObject and TObjectList is to create a std::vector 
 * wrapper which will expose only the methods we need in a more readable
 * fashion. However it's main purpose is to maintain strict relation 
 * between the ID of the classes held by it, and the index in the
 * vector.
 */

#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__

#include <cassert>
#include <vector>

// The ObjectList class is a part of Graph class
namespace graph {

class IObject 
{
public:
  /**
   * The ID of the given face. Should match the index in the owning class.
   */
  int ID;
};


/** 
 * @class TObjectList
 * @brief Template for creating object lists. 
 *
 * The template assumes that T is a pointer to a class that inherits
 * IObject.
 */
template <class T>
class TObjectList {
  std::vector<T> list;
public:
  /** Standard constructor */
  TObjectList() {}
  /** Returns list size */
  size_t size() const {
    return list.size();
  }
  /** Adds a new element to the end of the list */
  int add(T object) {
    list.push_back(object);
    object->ID = size()-1;
    return object->ID;
  }
  /** Returns element by index */
  T get(size_t index) const {
    assert(index < size());
    return list.at(index);
  }
  /** Returns element by index. Note that *not* a reference is returned */
  T operator[](size_t index) {
    assert(index < size());
    return list.at(index);
  } 
  void set(size_t index, T object) {
    list[index] = object;
    if (object) object->ID = index;
  }
  void clear(size_t index) {
    list[index] = NULL;
  }
  /**
   * Returns an unmanaged copy of the internal vector of elements. The
   * copy however is without "holes".
   */
  std::vector<T> getCopy() const {
    std::vector<T> result;
    for (size_t i = 0; i < list.size(); i++) {
      if (list[i]) result.push_back(list[i]);
    }
    return result;
  }

};


} // namespace end graph

#endif // __OBJECTLIST_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
