

#ifndef _API_H_
#define _API_H_

#ifdef INSIDE_BZEDIT
	#define BE_API extern "C" __declspec( dllexport )
#else
	#define BE_API extern "C" __declspec( dllimport )
#endif


// all API stuff is implemented in DOC as that's who has the info;

// doc stuff
// redraw the world and all interfaces, call after you are all done
BE_API void UpdateWorld ( void );

// tells the app that the document changed, and can be saved
BE_API void SetDocDirty ( void );

// global world stuff
// returns number of objects in the world
BE_API int GetObjectCount ( void );

// add a new object of "szClass" type, i.e. BOX, PYRAMID, etc... Returns the new object's ID
BE_API int AddObject ( const char *szClass );

// take an existing object and copy it, Returns the new object's ID
BE_API int DupeObject ( int iID );

// remove an existing object.
BE_API void RemoveObject ( int iID );

// get the ID of the object currently selected, -1 = no object
BE_API int GetSelectedObject ( void );

// general object info
// all functions return true if they sucseeded

// get the class name of an object
BE_API bool GetObjectClass(  int iID, char *szClass );

// get the name of an object
BE_API bool GetObjectName ( int iID, char *szName );

// get the position, afPos is a float[3]
BE_API bool GetObjectPos ( int iID, float *afPos );

// get the rotation
BE_API bool GetObjectRot ( int iID, float *pfRot );

// get the scale, afScale is a float[3]
BE_API bool GetObjectScale ( int iID, float *afScale );

// set the objects name
BE_API bool SetObjectName ( int iID, char *szName );

// set the position, afPos is a float[3]
BE_API bool SetObjectPos ( int iID, float *afPos );

// set the rotation
BE_API bool SetObjectRot ( int iID, float fRot );

// set the scale, afScale is a float[3]
BE_API bool SetObjectScale ( int iID, float *afScale );

// apply any changes to an object and recalulate it's data
// call then when you are done modifying an object so it will display properly
BE_API bool ApplyObjectChanges ( int iID );

// specific object info;
// these functions are to set some of the extra data for some objects
// like link info, TP borders and base colors.

// TP boarders are Floats and use the 'infoF' functions, with the label 'Border'
// Base Colors are ints, and use the 'infoI' functions with the label 'Color'
// Links are Integers and use the 'infoI' fucntioins with the labels 'FromSide' and 'ToSide'

BE_API bool GetObjectInfoI ( int iID, char *szLabel, int *data);
BE_API bool GetObjectInfoF ( int iID, char *szLabel, float *data);
BE_API bool GetObjectInfoS ( int iID, char *szLabel, char *data);

BE_API bool SetObjectInfoI ( int iID, char *szLabel, int *data);
BE_API bool SetObjectInfoF ( int iID, char *szLabel, float *data);
BE_API bool SetObjectInfoS ( int iID, char *szLabel, char *data);

#endif//_API_H_