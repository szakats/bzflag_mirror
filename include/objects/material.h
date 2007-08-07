#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../DataEntry.h"
#include "../render/RGBA.h"
#include "../model/BZWParser.h"
#include "../model/Model.h"

#include <string>
#include <vector>

#include <osg/Material>
#include <osg/Texture2D>

using namespace std;

class dynamicColor;
class texturematrix;

class material : public DataEntry, public osg::Material {

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
	texturematrix* getTextureMatrix() { return textureMatrix; }
	vector< osg::ref_ptr< material > >& getMaterials() { return materials; }
	vector< osg::ref_ptr< osg::Texture2D > >& getTextures() { return textures; }
	const osg::Vec4& getAmbient() { return this->getAmbient( ); }
	const osg::Vec4& getDiffuse() { return this->getDiffuse( ); }
	const osg::Vec4& getSpecular() { return this->getSpecular( ); }
	const osg::Vec4& getEmissive() { return this->getEmissive( ); }
	float getShininess() { return this->getShininess( ); }
	float getAlphaThreshold() { return alphaThreshold; }
	bool usesTextures() { return !noTextures; }
	bool usesTexColor() { return !noTexColor; }
	bool usesSphereMap() { return spheremap; }
	bool usesShadows() { return !noShadow; }
	bool usesCulling() { return !noCulling; }
	bool usesRadar() { return !noRadar; }
	bool usesGroupAlpha() { return groupAlpha; }
	bool usesOccluder() { return occluder; }
	bool usesResetMat() { return resetmat; }
	
	void setName( const string& name ) { this->name = name; }
	void setDynamicColor( dynamicColor* dynCol ) { this->dynCol = dynCol; }
	void setTextureMatrix( texturematrix* texmat ) { this->textureMatrix = texmat; }
	void setMaterials( const vector< osg::ref_ptr< material > >& materials ) { this->materials = materials; }
	void setTextures( const vector< osg::ref_ptr< osg::Texture2D > >& textures ) { this->textures = textures; }
	void setAmbient( const osg::Vec4& rgba ) { this->setAmbient( rgba ); }
	void setDiffuse( const osg::Vec4& rgba ) { this->setDiffuse(  rgba ); }
	void setSpecular( const osg::Vec4& rgba ) { this->setSpecular( rgba ); }
	void setEmissive( const osg::Vec4& rgba ) { this->setEmissive(  rgba ); }
	// setters with RGBA values
	void setAmbient( const RGBA& rgba ) { this->setAmbient( osg::Vec4( rgba.x(), rgba.y(), rgba.z(), rgba.w() ) ); }
	void setDiffuse( const RGBA& rgba ) { this->setDiffuse( osg::Vec4( rgba.x(), rgba.y(), rgba.z(), rgba.w() ) ); }
	void setSpecular( const RGBA& rgba ) { this->setSpecular( osg::Vec4( rgba.x(), rgba.y(), rgba.z(), rgba.w() ) ); }
	void setEmissive( const RGBA& rgba ) { this->setEmissive( osg::Vec4( rgba.x(), rgba.y(), rgba.z(), rgba.w() ) ); }
	
	void setShininess( float shininess ) { this->setShininess(  shininess );  }
	void setAlphaThreshold( float alphaThreshold ) { this->alphaThreshold = alphaThreshold; }
	void useTextures( bool value ) { this->noTextures = !value; }
	void useTexColor( bool value ) { this->noTexColor = !value; }
	void useSphereMap( bool value ) { this->spheremap = value; }
	void useShadows( bool value ) { this->noShadow = !value; }
	void useCulling( bool value ) { this->noCulling = !value; }
	void useRadar( bool value ) { this->noRadar = !value; }
	void useGroupAlpha( bool value ) { this->groupAlpha = value; }
	void useOccluder( bool value ) { this->occluder = value; }
	void useResetMat( bool value ) { this->resetmat = value; }
	
private:
	string name, color;
	dynamicColor* dynCol;
	texturematrix* textureMatrix;
	
	vector< osg::ref_ptr< material > > materials;
	vector< osg::ref_ptr< osg::Texture2D > > textures;
	
	bool noTextures, noTexColor, noTexAlpha, spheremap, noShadow, noCulling, noSorting, noRadar, noLighting, groupAlpha, occluder, resetmat;
	float alphaThreshold;
	
};

#endif /*MATERIAL_H_*/
