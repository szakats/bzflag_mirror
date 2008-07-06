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
 * @file MultiFace.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a Face class that can be made up from many faces.
 */

#ifndef __MULTIFACE_H__
#define __MULTIFACE_H__

#include "globals.h"
#include "Output.h"
#include "Face.h"
#include "Mesh.h"

/** 
 * @class MultiFace
 * @brief Class representing a composite mesh face.
 *
 * MultiFace holds a composite face made of many Faces. The faces can then 
 * be extracted from the MultiFace allowing complex shape handling.
 */
class MultiFace : public Face {
  /**
   * Vector of component face pointers. Managed and deleted by the class.
   */
  FaceVector* comps;
  /**
   * As MultiFace has many complex operations, it needs a pointer to the 
   * underlying mesh, to access vertex data.
   */
  Mesh* mesh;
public:
  /**
   * Standard constructor, takes the underlying mesh as an argument.
   * Initializes the underlying component face vector.
   */
  MultiFace( Mesh* _mesh ) : Face( ), mesh( _mesh ) {
    comps = new FaceVector; 
  }
  /**
   * Detaches a face from the MultiFace. The id here is the internal index 
   * in the component vector. Returns a list of Mesh face ID's of the faces
   * that are left after the detaching.
   */
  IntVector* detachFace( int id );
  /**
   * Adds a new component to the MultiFace. The passed face must already
   * exist in the underlying mesh. Returns the internal component ID of the
   * added face.
   */
  int addFace( Face* f );
  /**
   * Checks whether this face is a MultiFace or not. Returns true.
   */
  virtual bool isMultiFace() const { 
    return true; 
  }
  /**
   * Returns the count of stored component faces.
   */
  int componentCount() { 
    return comps->size(); 
  }
  /**
   * Standard destructor, disposes of the component vector.
   */
  virtual ~MultiFace() { delete comps; }
private:
  /**
   * Updates the Z-coordinate of all stored faces. Called right before 
   * a detach is performed.
   */
  void updateFaces( double z );
  void storeFaces();
  void refineFace( Face* f, Face* target );
  bool vertexInside( int vid );
  bool vertexNearestIntersect( const Vector2Dd A, const Vector2Dd B, 
    Vector2Dd &P, int &index, Face* face );
  int pickRemovalIndex( Face *f, IntVector* visited );
  Face* getOtherFaceWithVertex( Face* f, int vid );
  bool isLeftOfVectors( int x, int a, int b, int c );
};


#endif /* __MULTIFACE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
