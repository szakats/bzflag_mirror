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

#include "render/GeometryExtractorVisitor.h"

// constructor
GeometryExtractorVisitor::GeometryExtractorVisitor( osg::Node* node ) :
	osg::NodeVisitor( osg::NodeVisitor::NODE_VISITOR, osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
	
	geometries = vector< osg::Geometry* >();
	
	// tell the Renderable to accept this visitor
	if( node != NULL )
		node->accept( *this );
}

// apply method for nodes
void GeometryExtractorVisitor::apply( osg::Node& node ) {
	// see if this is a group
	osg::Group* group = dynamic_cast< osg::Group* >( &node );
	if( group ) {
		// if so, call the apply method for groups
		apply( *group );
		return;
	}
	
	// see if this is a geode
	osg::Geode* geode = dynamic_cast< osg::Geode* >( &node );
	if( geode ) {
		// if so, call the apply method for geodes
		apply( *geode );
		return;
	}
	
	// otherwise, do nothing, since this node has no geometry data
	return;
}

// apply method for groups
void GeometryExtractorVisitor::apply( osg::Group& group ) {
	
	// call the appropriate apply method for all children
	if( group.getNumChildren() > 0 ) {
		for( unsigned int i = 0; i < group.getNumChildren(); i++) {
			osg::Node* node = group.getChild( i );
			apply( *node );
		}
	}
}

// apply method for geodes
void GeometryExtractorVisitor::apply( osg::Geode& geode ) {
	// get the drawables from this geode
	osg::Geode::DrawableList drawables = geode.getDrawableList();
	
	// if there are no drawables, then return
	if( drawables.size() <= 0 )
		return;
	
	// iterate over the drawables
	for( osg::Geode::DrawableList::iterator i = drawables.begin(); i != drawables.end(); i++ ) {
		
		// dynamic_cast to geometry
		osg::Geometry* geo = (*i)->asGeometry();
		if(!geo) {
			continue;
		}
		
		// if the cast worked, save it to the cache
		geometries.push_back( geo );
	}
}
