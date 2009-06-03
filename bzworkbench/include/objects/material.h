/* BZWorkbench
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

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "DataEntry.h"
#include "render/RGBA.h"
#include "model/BZWParser.h"
#include "model/Model.h"

#include <string>
#include <vector>

#include <osg/Material>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/TexEnv>

class dynamicColor;
class texturematrix;

#define IS_VALID_COLOR( c ) (c.x() >= 0.0 && c.y() >= 0.0 && c.z() >= 0.0 && c.w() >= 0.0)

class material : public DataEntry, public osg::StateSet {
	
	friend class SceneBuilder;		// allow SceneBuilder to access protected/private methods

public:
	// default constructor
	material();
	
	// constructor with data
	material(string& data);
	
	static DataEntry* init() { return new material(); }
	static DataEntry* init(string& data) { return new material(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// binary getters and setters
	string getName() { return name; }
	dynamicColor* getDynamicColor() { return dynCol; }
	
	const osg::Vec4& getAmbient() { return getCurrentMaterial()->getAmbient( osg::Material::FRONT ); }
	const osg::Vec4& getDiffuse() { return getCurrentMaterial()->getDiffuse(osg::Material::FRONT); }
	const osg::Vec4& getSpecular() { return getCurrentMaterial()->getSpecular(osg::Material::FRONT); }
	const osg::Vec4& getEmissive() { return getCurrentMaterial()->getEmission(osg::Material::FRONT); }
	float getShininess() { return getCurrentMaterial()->getShininess(osg::Material::FRONT); }
	float getAlphaThreshold() { return alphaThreshold; }

	bool getNoTextures() { return noTextures; }
	bool getNoShadows() { return noShadow; }
	bool getNoCulling() { return noCulling; }
	bool getNoSorting() { return noSorting; }
	bool getNoRadar() { return noRadar; }
	bool getNoLighting() { return noLighting; }
	bool getNoShaders() { return noShaders; }
	bool getGroupAlpha() { return groupAlpha; }
	bool getOccluder() { return occluder; }
	
	void setName( const string& _name ) { this->name = _name; }
	void setDynamicColor( dynamicColor* _dynCol ) { dynCol = _dynCol; }

	void setAmbient( const osg::Vec4& value ) { getCurrentMaterial()->setAmbient( osg::Material::FRONT, value ); }
	void setDiffuse( const osg::Vec4& value ) { getCurrentMaterial()->setDiffuse( osg::Material::FRONT, value ); }
	void setSpecular( const osg::Vec4& value ) { getCurrentMaterial()->setSpecular( osg::Material::FRONT, value ); }
	void setEmissive( const osg::Vec4& value ) { getCurrentMaterial()->setEmission( osg::Material::FRONT, value ); }
	// setters with RGBA values
	void setAmbient( const RGBA& value ) { setAmbient( osg::Vec4( value.x(), value.y(), value.z(), value.w() ) ); }
	void setDiffuse( const RGBA& value ) { setDiffuse( osg::Vec4( value.x(), value.y(), value.z(), value.w() ) ); }
	void setSpecular( const RGBA& value ) { setSpecular( osg::Vec4( value.x(), value.y(), value.z(), value.w() ) ); }
	void setEmissive( const RGBA& value ) { setEmissive( osg::Vec4( value.x(), value.y(), value.z(), value.w() ) ); }
	void setShininess( float value ) { getCurrentMaterial()->setShininess( osg::Material::FRONT, value );  }
	void setAlphaThreshold( float value ) { alphaThreshold = value; }

	void setNoTextures( bool value ) { noTextures = value; }
	void setNoShadows( bool value ) { noShadow = value; }
	void setNoCulling( bool value ) { noCulling = value; }
	void setNoSorting( bool value ) { noSorting = value; }
	void setNoRadar( bool value ) { noRadar = value; }
	void setNoLighting( bool value ) { noLighting = value; }
	void setNoShaders( bool value ) { noShaders = value; }
	void setGroupAlpha( bool value ) { groupAlpha = value; }
	void setOccluder( bool value ) { occluder = value; }

	// the following set()'s operate on the last added texture
	void addTexture(const std::string&);
    void setTexture(const std::string&);
    void setTextureMatrix( texturematrix* texmat );
	void setCombineMode( osg::TexEnv::Mode value );
    void setNoTexAlpha( bool value );
    void setNoTexColor( bool value);
    void setSphereMap( bool value );
	void clearTextures() { textures.clear(); } // remove all textures

	int getTextureCount();
    const std::string& getTexture( int num ) { return textures[num].name; }
    texturematrix* getTextureMatrix( int num ) { return textures[num].matrix; }
	osg::TexEnv::Mode getCombineMode( int num ) { return textures[num].combineMode; }
    bool getNoTexAlpha( int num ) { return textures[num].noAlpha; }
    bool getNoTexColor( int num ) { return textures[num].noColor; }
	bool getUseSphereMap( int num ) { return textures[num].sphereMap; }

	void reset();
	
	// use this to compute the osg stateset to apply
	// this entails merging parts of other materials
	static material* computeFinalMaterial( vector< material* >& materialList );
	
	// get the current material
	osg::Material* getCurrentMaterial();
	
	// get the current texture
	osg::Texture2D* getCurrentTexture();
	
private:
	std::string name;
	std::string color;

	std::vector< std::string > shaders;

	std::vector< material* > materials;

	dynamicColor* dynCol;
	bool noTextures;
	bool noShadow;
	bool noCulling;
	bool noSorting;
	bool noRadar;
	bool noLighting;
	bool noShaders;
	bool groupAlpha;
	bool occluder;
	float alphaThreshold;

	struct TextureInfo {
      std::string name;
      texturematrix* matrix;
	  osg::TexEnv::Mode combineMode;
      bool noAlpha;
      bool noColor;
      bool sphereMap;
    };
	std::vector< TextureInfo > textures;	// the various textures from "texture" and "addTexture"
	
	// compute the final OSG texture
	void computeFinalTexture();
	
	// list of texture aliases (input: texture alias; output: BZWB texture file)
	map< string, string > textureAliases;
	
	// compute the final OSG material
	void computeFinalMaterial();
	
	// build the alias map
	void buildAliases();
};

#endif /*MATERIAL_H_*/
