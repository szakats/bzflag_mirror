#include "../include/render/TextureScalerVisitor.h"

// constructor
TextureScalerVisitor::TextureScalerVisitor( Renderable* r, osg::Vec3 scaleFactor ) :
	osg::NodeVisitor( osg::NodeVisitor::NODE_VISITOR, osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
	
	this->renderable = r;
	this->scaleFactor = scaleFactor;
}
	
// apply method for nodes
void TextureScalerVisitor::apply( osg::Node& node ) {
	// see if this is a group
	osg::Group* group = dynamic_cast< osg::Group* >( &node );
	if( group ) {
		this->apply( *group );
		return;
	}
	
	// see if this is a geode
	osg::Geode* geode = dynamic_cast< osg::Geode* >( &node );
	if( geode ) {
		this->apply( *geode );
		return;
	}
	
	// otherwise, do nothing, since we can't get the texture coordinates
	return;
}

// apply method for groups
void TextureScalerVisitor::apply( osg::Group& group ) {
	// call the appropriate apply method for all children
	if( group.getNumChildren() > 0 ) {
		for( unsigned int i = 0; i < group.getNumChildren(); i++) {
			osg::Node* node = group.getChild( i );
			this->apply( *node );
		}
	}
}

// apply method for geodes (here's where the real work is done)
void TextureScalerVisitor::apply( osg::Geode& geode ) {
	// This code is kind of ugly, since ideally the rescaling of texture coordinates
	// (which is only done after scaling the object's geometry) should be left to the
	// object that was scaled, and not the NodeVisitor.
	// TODO: chunk up this code and put them into their corresponding object classes
	
	if(this->renderable == NULL)
		return;
		
	// get the drawables from this geode
	osg::Geode::DrawableList drawables = geode.getDrawableList();
	
	// iterate over the drawables
	for( osg::Geode::DrawableList::iterator i = drawables.begin(); i != drawables.end(); i++ ) {
		
		// dynamic_cast to geometry
		osg::Geometry* geo = (*i)->asGeometry();
		if(!geo) {
			printf("not a geometry\n");
			continue;
		}
			
		// get the geometric vertices
		osg::Vec3Array* vertexArray = dynamic_cast<osg::Vec3Array*>( geo->getVertexArray() );
		if(!vertexArray) {
			printf("not a Vec3Array\n");
			continue;
		}
		
		// get the texture coordinates (BZWB uses 2D textures, and it is assumed that each object has only one texture coordinate array)
		osg::Vec2Array* texCoords = dynamic_cast<osg::Vec2Array*> ( geo->getTexCoordArray(0) );
		if(!texCoords) {
			printf("not a Vec2Array\n");
			continue;
		}
		
		for( unsigned int i = 0; i < texCoords->size(); i++ ) {
			(*texCoords)[i].set( scaleFactor.x(), scaleFactor.y() );
		}
		
		geo->dirtyDisplayList();
	}
}
