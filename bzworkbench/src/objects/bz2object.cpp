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

#include "objects/bz2object.h"

// default constructor
bz2object::bz2object(const char* name, const char* keys):
	Renderable(),
	DataEntry(name, keys)
{
	thisNode = NULL;
		
	transformations = vector< osg::ref_ptr<BZTransform> >();
	materials = vector< osg::ref_ptr< material > >();
	setSelected( false );
	setName( "(unknown bz2object)" );
	
	vector<float> data;
	data.push_back( 0.0 );
	data.push_back( 0.0 );
	data.push_back( 0.0 );
	
	vector<float> spinData_x, spinData_y, spinData_z;
	spinData_x.push_back(0.0);	spinData_x.push_back(1.0);	spinData_x.push_back(0.0);	spinData_x.push_back(0.0);
	spinData_y.push_back(0.0);	spinData_y.push_back(0.0);	spinData_y.push_back(1.0);	spinData_y.push_back(0.0);
	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(1.0);
	
	startShift = new BZTransform("shift", data);
	spin_x = new BZTransform("spin", spinData_x);
	spin_y = new BZTransform("spin", spinData_y);
	spin_z = new BZTransform("spin", spinData_z);
	endShift = new BZTransform("shift", data);
	/*
	addChild( spin_x.get() );
	spin_x->addChild( spin_y.get() );
	spin_y->addChild( spin_z.get() );
	spin_z->addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	*/
	addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	
	savedStateSet = NULL;
	
};

// constructor with data
bz2object::bz2object(const char* name, const char* keys, const char* data):
	Renderable(),
	DataEntry(name, keys, data)
{
	
	thisNode = NULL;
	
	transformations = vector< osg::ref_ptr<BZTransform> >();
	materials = vector< osg::ref_ptr< material > >();
	setSelected( false );
	
	vector<float> sdata;
	sdata.push_back( 0.0 );
	sdata.push_back( 0.0 );
	sdata.push_back( 0.0 );
	
	vector<float> spinData_x, spinData_y, spinData_z;
	spinData_x.push_back(0.0);	spinData_x.push_back(1.0);	spinData_x.push_back(0.0);	spinData_x.push_back(0.0);
	spinData_y.push_back(0.0);	spinData_y.push_back(0.0);	spinData_y.push_back(1.0);	spinData_y.push_back(0.0);
	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(1.0);
	
	startShift = new BZTransform("shift", sdata);
	spin_x = new BZTransform("spin", spinData_x);
	spin_y = new BZTransform("spin", spinData_y);
	spin_z = new BZTransform("spin", spinData_z);
	endShift = new BZTransform("shift", sdata);
	
	/*
	addChild( spin_x.get() );
	spin_x->addChild( spin_y.get() );
	spin_y->addChild( spin_z.get() );
	spin_z->addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	*/
	addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	
	savedStateSet = NULL;
	
	string d = string(data);
	update(d);
}

// constructor with node data
bz2object::bz2object(const char* name, const char* keys, osg::Node* node ):
	Renderable( node ),
	DataEntry(name, keys)
{
	
	thisNode = node;
	
	transformations = vector< osg::ref_ptr<BZTransform> >();
	materials = vector< osg::ref_ptr< material > >();
	setSelected( false );
	setName( "(unknown bz2object)" );
	
	vector<float> data;
	data.push_back( 0.0 );
	data.push_back( 0.0 );
	data.push_back( 0.0 );
	
	vector<float> spinData_x, spinData_y, spinData_z;
	spinData_x.push_back(0.0);	spinData_x.push_back(1.0);	spinData_x.push_back(0.0);	spinData_x.push_back(0.0);
	spinData_y.push_back(0.0);	spinData_y.push_back(0.0);	spinData_y.push_back(1.0);	spinData_y.push_back(0.0);
	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(1.0);
	
	startShift = new BZTransform("shift", data);
	spin_x = new BZTransform("spin", spinData_x);
	spin_y = new BZTransform("spin", spinData_y);
	spin_z = new BZTransform("spin", spinData_z);
	endShift = new BZTransform("shift", data);
	
	/*
	addChild( spin_x.get() );
	spin_x->addChild( spin_y.get() );
	spin_y->addChild( spin_z.get() );
	spin_z->addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	*/
	addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	endShift->addChild( node );
	
	savedStateSet = NULL;
}

// constructor with node and string data
bz2object::bz2object( const char* name, const char* keys, const char* data, osg::Node* node ):
	Renderable( node ),
	DataEntry( name, keys, data )
{
	thisNode = node;
	
	transformations = vector< osg::ref_ptr<BZTransform> >();
	materials = vector< osg::ref_ptr< material > >();
	setSelected( false );
	
	vector<float> sdata;
	sdata.push_back( 0.0 );
	sdata.push_back( 0.0 );
	sdata.push_back( 0.0 );
	
	vector<float> spinData_x, spinData_y, spinData_z;
	spinData_x.push_back(0.0);	spinData_x.push_back(1.0);	spinData_x.push_back(0.0);	spinData_x.push_back(0.0);
	spinData_y.push_back(0.0);	spinData_y.push_back(0.0);	spinData_y.push_back(1.0);	spinData_y.push_back(0.0);
	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(1.0);
	
	startShift = new BZTransform("shift", sdata);
	spin_x = new BZTransform("spin", spinData_x);
	spin_y = new BZTransform("spin", spinData_y);
	spin_z = new BZTransform("spin", spinData_z);
	endShift = new BZTransform("shift", sdata);
	
	/*
	addChild( spin_x.get() );
	spin_x->addChild( spin_y.get() );
	spin_y->addChild( spin_z.get() );
	spin_z->addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	*/
	addChild( startShift.get() );
	startShift->addChild( endShift.get() );
	endShift->addChild( node );
	
	savedStateSet = NULL;
	
	string d = string(data);
	update(d);
}
	
// getter
string bz2object::get(void) 
{
  return toString();
}

// setter
int bz2object::update(string& data)
{
	// find the first occurence of the section in the data
	const char* header = getHeader().c_str();

	// get the lines
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(BZW_NOT_FOUND == lines[0]) {
		printf("bz2object::update(): Error! Could not find object data!\n");
		return 0;
	}
		
	if(!hasOnlyOne(lines, header)) {
		printf("bz2object: improper information\n");
		return 0;
	}
	
	// just go with the first box definition we find (only one should be passed anyway)
	const char* objData = lines[0].c_str();
	
	// data
	vector<string> names, positions, rotations, sizes, physicsDrivers, matrefs;
	
	// get the name (break if there are more than one)
	if(isKey("name")) {
		names = BZWParser::getValuesByKey("name", header, objData);
		if(names.size() > 1) {
			printf("%s::update():  Error! Defined \"name\" %d times!\n", header, (int)names.size());
			return 0;	
		}
	}
	
	// get the position
	if(isKey("position")) {
		positions = BZWParser::getValuesByKey("position", header, objData);
		
		// just go with the first position (only one should be defined)
		if(positions.size() > 1) {
			printf("%s::update(): Error! Defined \"position\" %d times!\n", header, (int)positions.size());
			return 0;
		}
		if(positions.size() == 0) {
			positions.push_back(Point3D(0.0f, 0.0f, 0.0f).toString());
		}
			
	}
	
	// get the rotation
	if(isKey("rotation")) {
		rotations = BZWParser::getValuesByKey("rotation", header, objData);
		
		// just go with the first rotation
		if(rotations.size() > 1) {
			printf("%s::update(): Error! Defined \"rotation\" %d times!\n", header, (int)rotations.size());
			return 0;
		}
		if(rotations.size() == 0) {
			string rot = "0.0";
			rotations.push_back(rot);	
		}
			
	}
	
	if(isKey("size")) {
		// get the size
		sizes = BZWParser::getValuesByKey("size", header, objData);
		
		// just go with the first size (only one should be defined)
		if(sizes.size() > 1) {
			printf("%s::update(): Error! Defined \"size\" %d times!\n", header, (int)sizes.size());
			return 0;
		}
		if(sizes.size() == 0) {
			sizes.push_back(Point3D(10.0f, 10.0f, 10.0f).toString());	
		}
		
	}
	
	// get the physics driver
	if(isKey("phydrv")) {
		physicsDrivers = BZWParser::getValuesByKey("phydrv", header, objData);
		if(physicsDrivers.size() > 1) {
			printf("%s::update(): Error! Defined \"phydrv\" %d times!\n", header, (int)physicsDrivers.size());
			return 0;
		}
	}
	
	// get materials
	if(isKey("matref")) {
		matrefs = BZWParser::getValuesByKey("matref", header, objData);
	}
	
	// load in the simpler data
	if(isKey("name") && names.size() > 0)
		this->setName( names[0] );
	if(isKey("position") && positions.size() > 0)
		this->setPosition( Point3D( positions[0].c_str() ) );
	if(isKey("rotation") && rotations.size() > 0)
		Renderable::setRotationZ( atof( rotations[0].c_str() ) );
	if(isKey("size") && sizes.size() > 0)
		this->setSize( Point3D( sizes[0].c_str() ) );
	
	// query the model for a physics driver
	if(isKey("phydrv") && physicsDrivers.size() > 0) {
		physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", physicsDrivers[0] );
		this->physicsDriver = phys;
	}
	
	if(isKey("matref") && matrefs.size() > 0) {
		// erase previous materials
		this->materials.clear();
		// use the model to resolve the references into material pointers
		for( vector<string>::iterator i = matrefs.begin(); i != matrefs.end(); i++ ) {
			material* mat = (material*)Model::command( MODEL_GET, "material", *i );
			if( mat )
				this->materials.push_back( mat );
		}
		
		// assign the material
		this->setStateSet( material::computeFinalMaterial(materials) );
	}
	
	
	// find out which transformations are valid
	vector<string> transformKeys = vector<string>();
	if(isKey("shear"))
		transformKeys.push_back(string("shear"));
	if(isKey("shift"))
		transformKeys.push_back(string("shift"));
	if(isKey("scale"))
		transformKeys.push_back(string("scale"));
	if(isKey("spin"))
		transformKeys.push_back(string("spin"));
		
	// get the transformation occurences
	if(transformKeys.size() > 0) {
		
		vector<string> transforms = BZWParser::getLinesByKeys(transformKeys, header, data.c_str());
		if( transforms.size() > 0 ) {
			/**
			 * Note: BZWB does transformations differently from BZFlag.  In BZFlag, geometric data is handled
			 * in the raw, meaning that all transformations are relative the the world origin.  In BZWB, all 
			 * transformations are relative to the object's local origin.  Therefore, to make sure all transformations
			 * made to the object in BZWB are reflected EXACTLY in BZFlag, we must first shift the geometric data to
			 * the center of the world BEFORE applying the other transformations.  After they have been applied,
			 * then we shift it back.  Therefore, if you look at the output of BZWB, you'll notice that the block
			 * of transformations is preceeded by "shift -x -y -z" and followed by "shift x y z", where the point
			 * (x,y,z) is the global position of the object in question
			 */
			 
			// update the encapsulating shifts
			
			// update the first shift
			vector<float> startShiftData;
			startShiftData.push_back( -this->getPosition().x() );
			startShiftData.push_back( -this->getPosition().y() );
			startShiftData.push_back( -this->getPosition().z() );
			
			this->startShift->setData( startShiftData );
			
			// update the last shift
			vector<float> endShiftData;
			endShiftData.push_back( this->getPosition().x() );
			endShiftData.push_back( this->getPosition().y() );
			endShiftData.push_back( this->getPosition().z() );
			
			this->endShift->setData( endShiftData );
			
			// update rotations (find any occurrences of an angular rotation about cartesian axes )
			
			// handle x
			for( vector<string>::iterator i = transforms.begin(); i != transforms.end(); i++ ) {
				if( BZWParser::key( i->c_str() ) == "spin" ) {
					BZTransform t = BZTransform( *i );
					if( t.getData()[1] == 1.0f && t.getData()[2] == 0.0f && t.getData()[3] == 0.0f ) {
						// this must be the rotation around x
						this->spin_x->setData( t.getData() );
						this->setRotationX( t.getData()[0] );	// change the spin appearance as well
						transforms.erase(i);		// don't duplicate it later on
						break;
					}
				}
			}
			
			// handle y
			for( vector<string>::iterator i = transforms.begin(); i != transforms.end(); i++ ) {
				if( BZWParser::key( i->c_str() ) == "spin" ) {
					BZTransform t = BZTransform( *i );
					if( t.getData()[1] == 0.0f && t.getData()[2] == 1.0f && t.getData()[3] == 0.0f ) {
						// this must be the rotation around y
						this->spin_y->setData( t.getData() );
						this->setRotationY( t.getData()[0] );	// change the spin appearance as well
						transforms.erase(i);		// don't duplicate it later on
						break;
					}
				}
			}
			
			// handle z
			for( vector<string>::iterator i = transforms.begin(); i != transforms.end(); i++ ) {
				if( BZWParser::key( i->c_str() ) == "spin" ) {
					BZTransform t = BZTransform( *i );
					if( t.getData()[1] == 0.0f && t.getData()[2] == 0.0f && t.getData()[3] == 1.0f ) {
						// this must be the rotation around z
						this->spin_z->setData( t.getData() );
						this->setRotationZ( t.getData()[0] );	// change the spin appearance as well
						transforms.erase(i);		// don't duplicate it later on
						break;
					}
				}
			}
			
			
			// read in the remaining transformations
			vector< osg::ref_ptr< BZTransform > > newTransformations = vector< osg::ref_ptr< BZTransform > >();
			
			for( vector<string>::iterator i = transforms.begin(); i != transforms.end(); i++ ) {
				this->transformations.push_back( new BZTransform( *i ) );
			}
			
			// update the transformation stack
			this->recomputeTransformations( &newTransformations );
		}
		
	}
		
	return DataEntry::update(data);
}

// toString
string bz2object::toString(void)
{
	return getHeader() + "\n" + BZWLines( this ) + "end\n";
}

// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
string bz2object::BZWLines( bz2object* obj )
{
	string ret = string("");
	
	// add name key/value to the string if supported
	if(obj->isKey("name") && obj->getName().length() > 0)
		ret += "  name " + obj->getName() + "\n";
	
	// add position key/value to the string if supported
	if(obj->isKey("position"))
		ret += "  position " + Point3D( obj->getPosition() ).toString();
		
	// add rotation key/value to the string if supported
	if(obj->isKey("rotation") && !obj->isKey("spin"))
		ret += "  rotation " + string( ftoa(obj->getRotation().z()) ) + "\n";
		
	// add size key/value to the string if supported
	if(obj->isKey("size"))
		ret += "  size " + Point3D( obj->getSize() ).toString();
	
	// add a scale if size isn't supported (this is the case with groups, for example)
	if( !obj->isKey("size") && obj->isKey("scale") )
		ret += "  scale " + Point3D( obj->getSize() ).toString();
	
	// add the initial transformation
	if( obj->isKey("shift") )
		ret += "  " + obj->startShift->toString();
	
	// add all transformations to the string if they are supported
	for(vector< osg::ref_ptr<BZTransform> >::iterator i = obj->transformations.begin(); i != obj->transformations.end(); i++) {
		if(obj->isKey((*i)->getHeader().c_str()) && (*i)->isApplied())
			ret += "  " + (*i)->toString();
	}
	
	// add the Euler rotation values as spin keys
	if( obj->isKey("spin") ) {
		ret += "  " + obj->spin_x->toString();
		ret += "  " + obj->spin_y->toString();
		ret += "  " + obj->spin_z->toString();
	}
		
	// add the final transformation
	if( obj->isKey("shift") )
		ret += "  " + obj->endShift->toString();
	
	// if position isn't supported, then add another shift to emulate it (i.e. with groups)
	if( !obj->isKey("position") && obj->isKey("shift") )
		ret += "  shift " + Point3D( obj->getPos() ).toString();
	
	// add phydrv key/value to the string if supported and if defined
	if(obj->isKey("phydrv") && obj->physicsDriver != NULL)
		ret += "  phydrv " + obj->physicsDriver->getName() + "\n";
	
	// add all matref key/value pairs to the string if supported and defined
	if(obj->isKey("matref") && obj->materials.size() != 0) {
		for(vector<osg::ref_ptr<material> >::iterator i = obj->materials.begin(); i != obj->materials.end(); i++) {
			ret += "  matref " + (*i)->getName() + "\n";
		}	
	}
	
	// add unused text
	ret += obj->getUnusedText();
	
	return ret;
}

// event handler
int bz2object::update( UpdateMessage& message )
{
	switch( message.type ) {
		case UpdateMessage::SET_TRANSFORMATIONS: {		// update the transformation stack
			if( !( isKey("spin") || isKey("shift") || isKey("shear") || isKey("scale") ) )
				break;
				
			vector< osg::ref_ptr< BZTransform > >* newTransformations = message.getAsTransformationStack();
			recomputeTransformations( newTransformations );
			break;
		}
		
		case UpdateMessage::SET_MATERIALS: {		// update the material stack
			if( !isKey("matref") )
				break;
				
			vector< osg::ref_ptr< material > >* materialList = message.getAsMaterialList();
			if( materialList != NULL ) {
				materials = *materialList;
				refreshMaterial();
			}
			break;
		}
		
		case UpdateMessage::ADD_MATERIAL:	{ 		// push a material
			if( !isKey("matref") )
				break;
				
			addMaterial( message.getAsMaterial() );
			break;
		}
		
		case UpdateMessage::REMOVE_MATERIAL: {			// remove a material
			if( !isKey("matref") )
				break;
			
			removeMaterial( message.getAsMaterial() );
			break;
		}
		
		case UpdateMessage::UPDATE_MATERIAL: {		// update a material in the material list, or add it if it doesn't exist
			if( !isKey("matref") )
				break;
			
			material* mat = message.getAsMaterial();
			
			// search for the material in the list
			if( materials.size() > 0 ) {
				unsigned int i = 0;
				for( vector< osg::ref_ptr< material > >::iterator itr = materials.begin(); itr != materials.end(); itr++, i++) {
					if( itr->get() == mat ) {
						printf(" updating material...\n");
						materials[i] = mat;
						break;
					}
				}
				
				refreshMaterial();
			}
			else {
				materials.push_back( mat );
				printf(" adding material...\n" );
				refreshMaterial();
			}
			
			break;
		}
		default:
			return 0;		// not handled
	}
	
	return 1;
}

// update the transformation stack with new ones
void bz2object::recomputeTransformations( vector< osg::ref_ptr< BZTransform > >* newTransformations )
{
	
	// see if we have start/end/ shifts and spins
	if( this->startShift.get() == NULL || 
	    this->endShift.get() == NULL || 
	    this->spin_x.get() == NULL || 
	    this->spin_y.get() == NULL || 
	    this->spin_z.get() == NULL)
			return;			// something seriously wrong here
	
	// clear all current transformations
	if( this->transformations.size() > 0 ) {
		this->startShift->removeChild( transformations[0].get() );
		this->transformations[ this->transformations.size() - 1 ]->removeChild( endShift.get() );
		this->transformations.clear();
	} else {
	  // if there are no current transformations, then try to remove the endShift from the startShift
	  this->startShift->removeChild( this->endShift.get() );
	}
	
	this->transformations = *newTransformations;	// copy the array over
	
	// add the new transformations
	if( this->transformations.size() > 0 ) {
		// add each transformation to the next
		startShift->addChild( transformations[0].get() );
		
		for( unsigned int i = 1; i < transformations.size(); i++ ) {
			transformations[i-1]->addChild( transformations[i].get() );
		}
		
		transformations[ transformations.size() - 1 ]->addChild( endShift.get() );
	} else {
		startShift->addChild( endShift.get() );	// connect the start and end shifts if there are no transformations in between
	}
}

// add a material to the object
void bz2object::addMaterial( material* mat ) {
	if( mat != NULL ) {
		materials.push_back( mat );
		refreshMaterial();
	}
}

// insert a material
void bz2object::insertMaterial( unsigned int index, material* mat )
{
	if( index > materials.size() - 1 )
		return;
		
	if( materials.size() == 0 ) {
		materials.push_back( mat );
	} else {
		vector< osg::ref_ptr< material > >::iterator itr = materials.begin();
		for( unsigned int i = 0; i < materials.size(); i++, itr++ ) {
			if( i == index ) {
				materials.insert( itr, mat );
				break;
			}
		}
	}
	
	refreshMaterial();
}

// remove a material
void bz2object::removeMaterial( material* mat )
{
	if( materials.size() == 0 || mat == NULL )
		return;
	
	for( vector< osg::ref_ptr< material > >::iterator i = materials.begin(); i != materials.end(); i++ ) {
		if( i->get() == mat ) {
			materials.erase( i );
			break;
		}
	}
	
	refreshMaterial();
}

// remove a material by index
void bz2object::removeMaterial( unsigned int index )
{
	if( index >= materials.size() || index < 0 ) 
		return;
	
	vector< osg::ref_ptr< material > >::iterator itr = materials.begin();
	for( unsigned int i = 0; i < materials.size(); i++, itr++ ) {
		if( i == index ) {
			materials.erase( itr );
			break;
		}
	}
	
	refreshMaterial();
}

// recompute the material
void bz2object::refreshMaterial()
{
	material* mat = material::computeFinalMaterial( materials );
	SceneBuilder::assignBZMaterial( mat, this );
}


// add a transformation 
void bz2object::addTransformation( BZTransform* t ) {
	if( t != NULL ) {
		
		if( this->transformations.size() > 0 ) {
			// if we actually have othet transformations, we'll need to insert the transformation between
			// endShift and the last transformation
			t->addChild( this->endShift.get() );
			this->transformations[ this->transformations.size() - 1 ]->removeChild( this->endShift.get() );
			this->transformations.push_back( t );
		}
		else {
			// if there are no transformations, then insert this one betweens startShift and endShift
			t->addChild( this->endShift.get() );
			this->startShift->removeChild( this->endShift.get() );
			this->startShift->addChild( t );
			this->transformations.push_back( t );	
		}
	}	
}

// insert a transformation
void bz2object::insertTransformation( unsigned int index, BZTransform* t ) {
	if( t == NULL )
		return;		// don't deal with NULL transformations
		
	// don't deal with out-of-bounds indexes when there are already transformations defined.
	if( this->transformations.size() >= index )
		return;
		
	// if there are no transformations, then insert the transformation between startShift and endShift
	if( this->transformations.size() == 0 ) {
		t->addChild( this->endShift.get() );
		this->startShift->removeChild( this->endShift.get() );
		this->startShift->addChild( t );
		this->transformations.push_back( t );	
	}
	// otherwise...
	else {
		// if the index is 0, then insert it between startShift and the start of the transformation list
		if( index == 0 ) {
			osg::ref_ptr< BZTransform > t_ref = osg::ref_ptr< BZTransform >( t );
			// remove the head of the list from startShift
			this->startShift->removeChild( this->transformations[0].get() );
			// insert the transformation to the head of the list
			this->transformations.insert( this->transformations.begin(), t );
			// add the transformation as a child of startShift
			this->startShift->addChild( t_ref.get() );
		}
		// if the index is the last one...
		else if( index == transformations.size()-1 ) {
			// just invoke the addTransformation() method, since it appends transformations
			this->addTransformation( t );
		}
		// otherwise, the index is somewhere in between, so we need to advance an iterator over to that spot
		// and insert it
		else {
			// count over to the point of insertion
			vector< osg::ref_ptr< BZTransform > >::iterator itr = this->transformations.begin();
			unsigned int i = 0;
			// increment the iterator
			for(; i < index; i++, ++itr );
			// now insert the transformation
			this->transformations[i]->removeChild( this->transformations[i+1].get() );
			this->transformations.insert( itr, t );
			// now, transformations[i+1] is the transformation we just added
			this->transformations[i]->addChild( this->transformations[i+1].get() );
			this->transformations[i+1]->addChild( this->transformations[i+2].get() );
		}
	}
}

// remove a transformation by pointer (NOTE: removes only the 1st occurence)
void bz2object::removeTransformation( BZTransform* t ) {
	if( this->transformations.size() == 0 )
		return;
	
	// iterate over the vector and pull the first occurence
	for( vector< osg::ref_ptr< BZTransform > >::iterator i = this->transformations.begin(); i != this->transformations.end(); ++i ) {
		if( i->get() == t ) {
			this->transformations.erase( i );
			return;
		}
	}
	
}

// remove a transformation by index
void bz2object::removeTransformation( unsigned int index ) {
	if( this->transformations.size() == 0 )
		return;
	
	// iterate over the vector to find the index
	unsigned int i = 0;
	vector< osg::ref_ptr< BZTransform > >::iterator itr = this->transformations.begin();
	for(; i < index; i++, ++itr );
	
	// remove the transformation at itr
	this->transformations.erase( itr );
}
