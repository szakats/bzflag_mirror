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

#include "objects/material.h"

// build the aliases for the textures
void material::buildAliases() { 
	textureAliases = map< string, string > ();
	
	textureAliases[ "std_ground" ] = "share/world/std_ground.png";
}

// default constructor
material::material() : 
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>"),
	osg::StateSet() {
	name = SceneBuilder::makeUniqueName("material");
	dynCol = NULL;
	textureMatrix = NULL;
	color = string("");
	textures = vector< osg::ref_ptr< osg::Texture2D > >();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = false;
	alphaThreshold = 1.0f;
	
	// allocate a material
	osg::Material* finalMaterial = new osg::Material();
	setAttribute( finalMaterial, osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );
	
	// deactivate texturing
	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF  | osg::StateAttribute::OVERRIDE );
	
	setAmbient( osg::Vec4( 1, 1, 1, 1) );
	setDiffuse( osg::Vec4( 1, 1, 1, 1) );
	setSpecular( osg::Vec4( 0, 0, 0, 1) );
	setEmissive( osg::Vec4( 1, 1, 1, 1) );
}

// constructor with data
material::material(string& data) :
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>", data.c_str()),
	osg::StateSet() {
	name = SceneBuilder::makeUniqueName("material");
	dynCol = NULL;
	textureMatrix = NULL;
	color = string("");
	textures = vector< osg::ref_ptr< osg::Texture2D > >();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = false;
	alphaThreshold = 1.0f;
	
	// allocate a material
	osg::Material* finalMaterial = new osg::Material();
	setAttribute( finalMaterial, osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );
	
	setAmbient( osg::Vec4( 1, 1, 1, 1) );
	setDiffuse( osg::Vec4( 1, 1, 1, 1) );
	setSpecular( osg::Vec4( 0, 0, 0, 1) );
	setEmissive( osg::Vec4( 0, 0, 0, 1) );
	
	// deactivate texturing
	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	
	
	update(data);
}

// getter
string material::get(void) { return toString(); }

// setter
int material::update(string& data) {
	
	const char* header = getHeader().c_str();
	
	// get the section
	vector<string> chunks = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(chunks[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(chunks, "material"))
		return 0;
	
	const char* tmp = chunks[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, tmp);
	if(!hasOnlyOne(names, "name"))
		return 0;
	
	// get resetmats if any
	vector<string> resetmats = BZWParser::getLinesByKey("resetmat", header, tmp);
	
	string matData = tmp;
	
	if( resetmats.size() > 0 ) {
		// find the location of the last resetmat
		string::size_type i = matData.rfind("resetmat", matData.length() - 1);
		
		// eliminate all texture data before it
		matData = "material\n" + matData.substr(i);
		
		printf("material data is \n|%s|\n", tmp );
	}
	
	const char* materialData = matData.c_str();
	
	// get the dynamic color
	vector<string> dyncols = BZWParser::getValuesByKey("dyncol", header, materialData);
	if(dyncols.size() > 1) {
		printf("material::update(): Error! Defined \"dyncol\" %d times!\n", (int)dyncols.size());
		return 0;
	}
		
	// get the texture matrix
	vector<string> texmats = BZWParser::getValuesByKey("texmat", header, materialData);
	if(texmats.size() > 1) {
		printf("material::update(): Error! Defined \"texmat\" %d times!\n", (int)texmats.size());
		return 0;
	}
		
	// get the diffuse colors
	vector<string> colors = BZWParser::getValuesByKey("color", header, materialData);
	vector<string> diffs = BZWParser::getValuesByKey("diffuse", header, materialData);
	if((colors.size() > 1 && diffs.size() == 0) || 
	   (colors.size() == 0 && diffs.size() > 1)) {
	   printf("material::update():  Error! Could not parse \"color\" or \"diffuse\" properly!\n");
	   return 0;
	}
	   
	vector<string> diffuses = (colors.size() == 0 ? diffs : colors);
	
	// get the ambient colors
	vector<string> ambients = BZWParser::getValuesByKey("ambient", header, materialData);
	
	// get the emissive colors
	vector<string> emissives = BZWParser::getValuesByKey("emission", header, materialData);
	
	// get the specular colors
	vector<string> speculars = BZWParser::getValuesByKey("specular", header, materialData);
	
	// get the textures
	vector<string> textureKeys;
	textureKeys.push_back("addtexture");
	textureKeys.push_back("texture");
	vector<string> texs = BZWParser::getValuesByKeys(textureKeys, header, materialData);
	
	// get notextures
	vector<string> notextures = BZWParser::getValuesByKey("notexture", header, materialData);
	
	// get notexcolor
	vector<string> notexcolors = BZWParser::getValuesByKey("notexcolor", header, materialData);
	
	// get spheremap
	vector<string> spheremaps = BZWParser::getValuesByKey("spheremap", header, materialData);
	
	// get noshadow
	vector<string> noshadows = BZWParser::getValuesByKey("noshadow", header, materialData);
	
	// get noculling
	vector<string> nocullings = BZWParser::getValuesByKey("noculling", header, materialData);
	
	// get nolighting
	vector<string> nolightings = BZWParser::getValuesByKey("nolighting", header, materialData);
	
	// get nosorting
	vector<string> nosortings = BZWParser::getValuesByKey("nosort", header, materialData);
	
	// get noradar
	vector<string> noradars = BZWParser::getValuesByKey("noradar", header, materialData);
	
	// get notexalpha
	vector<string> notexalphas = BZWParser::getValuesByKey("notexalpha", header, materialData);
	
	// get groupalpha
	vector<string> groupalphas = BZWParser::getValuesByKey("groupalpha", header, materialData);
	
	// get occluder
	vector<string> occluders = BZWParser::getValuesByKey("occluder", header, materialData);
	
	// get shininess
	vector<string> shininesses = BZWParser::getValuesByKey("shininess", header, materialData);
	if(shininesses.size() > 1) {
		printf("material::update(): Error! Defined \"shininess\" %d times!\n", (int)shininesses.size());
		return 0;
	}
		
	// get alpha threshold
	vector<string> alphathresholds = BZWParser::getValuesByKey("alphathresh", header, materialData);
	if(alphathresholds.size() > 1) {
		printf("material::update(): Error! Defined \"alphathresh\" %d times!\n", (int)alphathresholds.size());
		return 0;
	}
		
	// get other material refrences
	vector<string> matrefs = BZWParser::getValuesByKey("matref", header, materialData);
		
	// load the retrieved data into the class
	if(!DataEntry::update(data))
		return 0;
	name = names[0];
	dynCol = (dyncols.size() != 0 ? (dynamicColor*)Model::command( MODEL_GET, "dynamicColor", dyncols[0] ) : NULL);
	textureMatrix = (texmats.size() != 0 ? (texturematrix*)Model::command( MODEL_GET, "texturematrix", texmats[0]) : NULL);
	(emissives.size() != 0 ? setEmissive(RGBA( emissives[emissives.size() - 1].c_str() )) : setEmissive(RGBA(-1, -1, -1, -1)));
	(speculars.size() != 0 ? setSpecular(RGBA( speculars[speculars.size() - 1].c_str() )) : setSpecular(RGBA(-1, -1, -1, -1)));
	(ambients.size() != 0 ? setAmbient(RGBA( ambients[ambients.size() - 1].c_str() )) : setAmbient(RGBA(-1, -1, -1, -1)));
	(diffuses.size() != 0 ? setDiffuse(RGBA( diffuses[diffuses.size() - 1].c_str() )) : setDiffuse(RGBA(-1, -1, -1, -1)));
	
	textures.clear();
	materials.clear();
	// get the materials from the model
	for( vector<string>::iterator i = matrefs.begin(); i != matrefs.end(); i++) {
		material* mat = (material*)Model::command( MODEL_GET, "material", *i );
		if( mat != NULL )
			materials.push_back( mat );
	}
	// compute the final material
	computeFinalMaterial();
	
	// get the textures
	textures.clear();
	if( texs.size() > 0 ) {
		for( vector<string>::iterator i = texs.begin(); i != texs.end(); i++ ) {
			// filename should be the texture name + .png
			string filename = (*i);
			if( i->find(".png", filename.size() - 4) == string::npos ) {
				filename = (*i) + ".png";
			}
			osg::Texture2D* tex = SceneBuilder::buildTexture2D( filename.c_str() );
			if( tex )
				textures.push_back( tex );
		}
	}
	
	// compute the final texture
	computeFinalTexture();
	
	noTextures = (notextures.size() == 0 ? false : true);
	noTexColor = (notexcolors.size() == 0 ? false : true);
	spheremap = (spheremaps.size() == 0 ? false : true);
	noShadow = (noshadows.size() == 0 ? false : true);
	noCulling = (nocullings.size() == 0 ? false : true);
	noSorting = (nosortings.size() == 0 ? false : true);
	noRadar = (noradars.size() == 0 ? false : true);
	noTexAlpha = (notexalphas.size() == 0 ? false : true);
	groupAlpha = (groupalphas.size() == 0 ? false : true);
	occluder = (occluders.size() == 0 ? false : true);
	noLighting = (nolightings.size() == 0 ? false : true);
	(shininesses.size() > 0 ? setShininess(atof( shininesses[0].c_str() )) : setShininess(0.0) );
	alphaThreshold = (alphathresholds.size() > 0 ? atof( alphathresholds[0].c_str() ) : 1.0f);
	
	return 1;
}

// tostring
string material::toString(void) {
	string texString = string("");
	if( textures.size() > 0 ) {
		for(vector< osg::ref_ptr<osg::Texture2D> >::iterator i = textures.begin(); i != textures.end(); i++) {
			string filename = (*i)->getImage()->getFileName();
			string::size_type suffixIndex = filename.find(".png");
			if( suffixIndex != string::npos )
				filename = filename.substr(0, suffixIndex );
				
			texString += "  addtexture " + filename + "\n";	
		}
	}
	
	string matString = string("");
	if( materials.size() > 0 ) {
		for(vector< osg::ref_ptr< material > >::iterator i = materials.begin(); i != materials.end(); i++) {
			matString += "  matref " + (*i)->getName() + "\n";	
		}
	}
	// colors
	string ambientString, diffuseString, specularString, emissiveString;
	
	RGBA ambientColor = RGBA( getAmbient().x(), getAmbient().y(), getAmbient().z(), getAmbient().w() );
	RGBA diffuseColor = RGBA( getDiffuse().x(), getDiffuse().y(), getDiffuse().z(), getDiffuse().w() );
	RGBA specularColor = RGBA( getSpecular().x(), getSpecular().y(), getSpecular().z(), getSpecular().w() );
	RGBA emissiveColor = RGBA( getEmissive().x(), getEmissive().y(), getEmissive().z(), getEmissive().w() );
	
	if( IS_VALID_COLOR( ambientColor ) )
		ambientString = "  ambient " + ambientColor.toString();
	else
		ambientString = "";
		
	if( IS_VALID_COLOR( diffuseColor ) )
		diffuseString = "  diffuse " + diffuseColor.toString();
	else
		diffuseString = "";
		
	if( IS_VALID_COLOR( specularColor ) )
		specularString = "  specular " + specularColor.toString();
	else
		specularString = "";
		
	if( IS_VALID_COLOR( emissiveColor ) )
		emissiveString = "  emission " + emissiveColor.toString();
	else	
		emissiveString = "";
	
	return string("material\n") +
				  (name.length() == 0 ? string("# name") : "  name " + name) + "\n" +
				  (dynCol != NULL ? string("  dyncol ") + dynCol->getName() : string("  dyncol -1")) + "\n" +
				  (textureMatrix != NULL ? string("  texmat ") + textureMatrix->getName() : string("  texmat -1")) + "\n" +
				  (noTextures == true ? "  notextures\n" : "") +
				  (noTexColor == true ? "  notexcolor\n" : "") +
				  (noTexAlpha == true ? "  notexalpha\n" : "") +
				  (spheremap == true ? "  spheremap\n" : "") +
				  (noShadow == true ? "  noshadow\n" : "") +
				  (noCulling == true ? "  noculling\n" : "") +
				  (noSorting == true ? "  nosort\n" : "") +
				  (noLighting == true ? "  nolighting\n" : "") +
				  (noRadar == true ? "  noradar\n" : "") +
				  (groupAlpha == true ? "  groupalpha\n" : "") +
				  (occluder == true ? "  occluder\n" : "") +
				  ambientString +
				  (color.length() == 0 ? diffuseString : string("  color ") + color + "\n" ) +
				  specularString +
				  emissiveString +
				  "  shininess " + string(ftoa(getShininess())) + "\n" +
				  "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" +
				  texString +
				  matString +
				  getUnusedText() + 
				  "end\n";
}

// compute the final material from a list of materials
material* material::computeFinalMaterial( vector< material* >& materialList ) { 
	osg::Vec4 ambient = osg::Vec4( 0, 0, 0, 0),
			  diffuse = osg::Vec4( 0, 0, 0, 0),
			  specular = osg::Vec4( 0, 0, 0, 0),
			  emissive = osg::Vec4( 0, 0, 0, 0);
	float shiny = 0.0;
	
	osg::Texture2D* tex = NULL;
	
	if( materialList.size() > 0 ) {
		for( vector< material* >::iterator i = materialList.begin(); i != materialList.end(); i++ ) {
			
			(*i)->computeFinalMaterial();
			(*i)->computeFinalTexture();
			
			// get OSG's material from the material class
			osg::Material* mat = dynamic_cast< osg::Material* >(((*i)->getAttribute( osg::StateAttribute::MATERIAL ) ));
			
			// if it's valid, get the colors
			// NOTE: BZFlag pays attention only to the LAST occurence of a color
			if( mat ) {
				if(IS_VALID_COLOR( mat->getAmbient( osg::Material::FRONT ) ) ) {
					ambient = mat->getAmbient( osg::Material::FRONT );
				}
					
				if(IS_VALID_COLOR( mat->getDiffuse( osg::Material::FRONT ) ) ) {
					diffuse = mat->getDiffuse( osg::Material::FRONT );
				}	
					
				if(IS_VALID_COLOR( mat->getSpecular( osg::Material::FRONT ) ) ) {
					specular = mat->getSpecular( osg::Material::FRONT );
				}
				
				if(IS_VALID_COLOR( mat->getEmission( osg::Material::FRONT ) ) ) {
					emissive = mat->getSpecular( osg::Material::FRONT );
				}
				
				shiny = mat->getShininess( osg::Material::FRONT );
			}
			
			// get the texture
			osg::Texture2D* texture = dynamic_cast< osg::Texture2D* >( (*i)->getTextureAttribute( 0, osg::StateAttribute::TEXTURE ) );
			
			// see if the texture is valid
			// NOTE: BZFlag pays attention only to the FIRST texture declared
			if( !tex && texture ) {
				tex = texture;
				printf("  got texture %s\n", texture->getImage()->getFileName().c_str() );
			}
		}
	}
	
	// build the material
	material* mat = new material();
	
	mat->setAmbient( ambient );
	mat->setDiffuse( diffuse );
	mat->setSpecular( specular );
	mat->setEmissive( emissive );
	mat->setShininess( shiny );
	
	if( tex != NULL ) {
		mat->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
		mat->setTextureAttribute( 0, tex, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
	else {
		// deactivate texturing if there was none
		mat->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	}
	return mat;
}

// compute the final texture
// simple: BZFlag only pays attention to the first texture declared
void material::computeFinalTexture() { 
	
	if( textures.size() > 0 ) {
		osg::Texture2D* finalTexture = textures[ 0 ].get();
		setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
		setTextureAttribute( 0, finalTexture );
	}
	else {
		setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	}
}

// compute the final osg material
void material::computeFinalMaterial() {
	
	osg::Vec4 ambient = osg::Vec4( 0, 0, 0, 0),
			  diffuse = osg::Vec4( 0, 0, 0, 0),
			  specular = osg::Vec4( 0, 0, 0, 0),
			  emissive = osg::Vec4( 0, 0, 0, 0);
			  
	float shiny = 0.0;
	
	if( materials.size() > 0 ) {
		
		for( vector< osg::ref_ptr< material > >::iterator i = materials.begin(); i != materials.end(); i++ ) {
			
			material* mat = i->get();
			
			// NOTE: BZFlag pays attention only to the LAST occurence of a color
			if( mat ) {
				if(IS_VALID_COLOR( mat->getAmbient() ) ) {
					ambient = mat->getAmbient();
				}
					
				if(IS_VALID_COLOR( mat->getDiffuse() ) ) {
					diffuse = mat->getDiffuse();
				}	
					
				if(IS_VALID_COLOR( mat->getSpecular() ) ) {
					specular = mat->getSpecular();
				}
				
				if(IS_VALID_COLOR( mat->getEmissive() ) ) {
					emissive = mat->getSpecular();
				}
				
				shiny = mat->getShininess();
			}
		}
		
		osg::Material* finalMaterial = new osg::Material();
			
		finalMaterial->setAmbient( osg::Material::FRONT, ambient );
		finalMaterial->setDiffuse( osg::Material::FRONT, diffuse );
		finalMaterial->setSpecular( osg::Material::FRONT, specular );
		finalMaterial->setEmission( osg::Material::FRONT, emissive );
		finalMaterial->setShininess( osg::Material::FRONT, shiny );
		
		setAttribute( finalMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	}
	
}

// get the current material
osg::Material* material::getCurrentMaterial() {
	return (osg::Material*)getAttribute( osg::StateAttribute::MATERIAL );
}

// get the current texture
osg::Texture2D* material::getCurrentTexture() {
	return (osg::Texture2D*)getTextureAttribute( 0, osg::StateAttribute::TEXTURE );
}
