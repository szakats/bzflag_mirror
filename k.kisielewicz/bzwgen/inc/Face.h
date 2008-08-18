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
 * @brief Defines a Face class for BZWGen.
 */
#ifndef __FACE3D_H__
#define __FACE3D_H__

#include "globals.h"
#include "Output.h"

/**
 * @class Face
 * @brief Class representing a mesh face.
 *
 * The Face consists of indices to vertex and texture coordinates, and
 * some additional output info.
 */
class Face {
  /** Material ID of the face */
  int materialID;
  /**
   * States wether the face should be outputed. Used by MultiFace and
   * for reusing geometry data of faces that should not exist.
   */
  bool output;
protected:
  /**
   * Vector holding the indices of texture coords held by the Mesh
   * class.
   */
  IntVector tcd;
  /**
   * Vector holding the indices of vertex coords held by the Mesh
   * class.
   */
  IntVector vtx;
public:
  /**
   * Standard and only constructor. Doesn't take any data, addVertex and
   * addTexCoord should be used for defining the face.
   */
  Face() : materialID(0), output(true) {}
  /**
   * Destructor.
   */
  virtual ~Face() {}
  /**
   * Clears the vertex index vector.
   */
  void clearVertices( ) {
    vtx.clear();
  }
  /**
   * Clears the texture coordinate index vector.
   */
  void clearTexCoords( ) {
    tcd.clear();
  }
  /**
   * Adds a vertex index to the face.
   */
  void addVertex( int vertexID ) {
    vtx.push_back(vertexID);
  }
  /**
   * Adds both a vertex and texture coordinate index to the face.
   */
  void addVertex( int vertexID, int texCoordID ) {
    vtx.push_back( vertexID );
    tcd.push_back( texCoordID );
  }
  /**
   * Adds both a texture coordinate index to the face.
   */
  void addTexCoord( int texCoordID ) {
    tcd.push_back( texCoordID );
  }
  /**
   * Returns true, if the given vertex index is a part of this face.
   */
  bool hasVertex( int vertexID ) const {
    for ( size_t i = 0; i < vtx.size(); i++ )
      if ( vtx.at(i) == vertexID )
	return true;
    return false;
  }
  /**
   * Acts like getVertex except that any input is valid -- the list of
   * vertices is treated like a cyclic list.
   */
  int getCyclicVertex( int index ) const {
    int size = vtx.size();
    if (index >= 0) {
      return vtx.at(index % size);
    } else {
      return vtx.at((index + size*int(-index/size+1)) % size);
    }
  }
  /**
   * Returns the vertex ID specified by the given index in the face's list.
   */
  int getVertex( const int index ) const {
    assert( index < int(vtx.size()) );
    return vtx.at(index);
  }
  /**
   * Returns the texture coordinate ID specified by the given index in the
   * face's list.
   */
  int getTexCoord( int index ) const {
    return vtx.at(index);
  }
  /**
   * Inserts a vertex ID after the position specified.
   */
  void insertVertexAfter( int index, int vertexID ) {
    vtx.insert( vtx.begin()+index+1, vertexID );
  }
  /**
   * Inserts a vertex ID before the position specified.
   */
  void insertVertexBefore( int index, int vertexID ) {
    vtx.insert( vtx.begin()+index, vertexID );
  }
  /**
   * Removes the vertex ID at the specified index, and shifts the
   * vertex ID array to remove the empty space.
   */
  void removeVertex( int index ) {
    vtx.erase( vtx.begin()+index );
  }
  /**
   * Returns a string representation of the given face. Used for
   * debugging purposes.
   */
  String toStringIDs( ) const {
    char buffer[80];
    sprintf(buffer, "(%d,%d,%d,%d)",vtx.at(0),vtx.at(1),vtx.at(2),vtx.at(3));
    return String(buffer);
  }
  /**
   * Returns the size of the face. The size is defined as the amount of
   * vertices that make up the face.
   */
  size_t size() const {
    return vtx.size();
  }
  /**
   * Sets the Material ID for the face.
   */
  void setMaterial( int _materialID ) {
    materialID = _materialID;
  }
  /**
   * Returns the Material ID for the face.
   */
  int getMaterial() const {
    return materialID;
  }
  /**
   * Sets wether the face should be sent to output.
   */
  void setOutput( bool _output ) {
    output = _output;
  }
  /**
   * Returns wether the face is intended for output.
   */
  bool outputable() const {
    return output;
  }
  /**
   * Returns whether the face has any texture coords defined.
   */
  bool hasTexCoords( ) const {
    return !tcd.empty();
  }
  /**
   * Returns whether the face is a MultiFace.
   */
  virtual bool isMultiFace() const {
    return false;
  }
  /**
   * Returns the index (position) in the face's vertex array of the
   * vertex specified by the passed ID. Used by MultiFace.
   */
  int getVertexIndex(int vertexID) const {
    for ( size_t i = 0; i < vtx.size(); i++ )
      if ( vtx.at(i) == vertexID )
	return i;
    return -1;
  }
  /**
   * Returns a copy of the texCoord indices array.
   */
  IntVector getTexCoords( ) const {
    return tcd;
  }
  /**
   * Sets the texCoord indices array to the passed vector.
   */
  void setTexCoords( IntVector _tcd ) {
    tcd = _tcd;
  }
  /**
   * Returns a copy of the vertex indices array.
   */
  IntVector getVertices( ) const {
    return vtx;
  }
  /**
   * Sets the vertex indices array to the passed vector.
   */
  void setVertices( IntVector _vtx ) {
    vtx = _vtx;
  }
};

/** Type definition for a Vector of Face pointers. */
typedef std::vector<Face*> FaceVector;
/** Type definition for a Vector of Face pointers iterator. */
typedef FaceVector::iterator FaceVectIter;

#endif /* __FACE3D_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

