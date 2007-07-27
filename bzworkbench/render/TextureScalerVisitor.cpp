#include "../include/render/TextureScalerVisitor.h"

#define TEC( x ) (x != 0.0 ? (x > 1.0 ? 1.0 : x) : 0.0 )

// constructor
TextureScalerVisitor::TextureScalerVisitor( bz2object* obj, osg::Vec3 scaleFactor ) :
	osg::NodeVisitor( osg::NodeVisitor::NODE_VISITOR, osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
	
	this->obj = obj;
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
	
	if(this->obj == NULL)
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
		
		// get the primitive sets
		osg::Geometry::PrimitiveSetList primitives = geo->getPrimitiveSetList();
		
		// Here, we're iterating over all the faces in the mesh so we can compute one of the planar normals to each of the faces.
		// For each face's normal, the dot product between the scaleFactor and the normal will be computed, so as
		// to further determine whether or not to scale the texture coordinates.  For example,
		// if the normal and the scalefactor are perpendicular (i.e. the face is being stretched), then the texture
		// coordinates will fully scale.  If the vectors are parallel (i.e. the face faces in the direction of scaling)
		// then there is no need to modify the corresponding face's texture coordinates.
		
		// NOTE: The orientation of the face normal doesn't matter in the calculations; the angle between it and
		// the scale factor is what will determine the magnitude of scaling (in which case the normal can point either way).
		
		// NOTE: This Visitor is assumed to traverse the geometry nodes AFTER the geometry has been scaled
		// (i.e. scaleFactor has already been applied)
		
		if( primitives.size() > 0 ) {
			for( osg::Geometry::PrimitiveSetList::iterator i = primitives.begin(); i != primitives.end(); i++) {
				// our models use triangle strips or just triangles
				switch( (*i)->getMode() ) {
					
					// handle triangle strip meshes
					case osg::PrimitiveSet::TRIANGLE_STRIP : {
						// indexes to points in the mesh that form a face
						unsigned int i1 = (*i)->index(0);
						unsigned int i2 = (*i)->index(1);
						unsigned int i3;
						
						// working copies of points
						osg::Vec3 p1, p2, p3;
						
						// working vectors
						osg::Vec3 v1, v2, n;
						
						// working dot product
						double dp;
						
						// working angle value (in radians)
						double angle;
						
						// keep track of indices to which texture coordinates we've scaled (i.e. no repeats)
						// NOTE: tmp is a reflexive map, used in place of a vector for it's faster insertion and searching capabilities
						map<unsigned int, unsigned int> tmp = map<unsigned int, unsigned int>();
						tmp[i1] = i1;
						tmp[i2] = i2;
						
						// iterate through the rest to get the faces by indexing vertexes with the primitiveset elements
						for( unsigned int j = 2; j < (*i)->getNumIndices(); j++) {
							// assign the next index in the strip
							i3 = (*i)->index(j);
							
							// get the points
							p1 = (*vertexArray)[i1];
							p2 = (*vertexArray)[i2];
							p3 = (*vertexArray)[i3];
							
							// compute vectors from the face (the vectors will be along two of the edges)
							v1 = p2 - p1;
							v2 = p2 - p3;
							
							// compute one of the normals by finding the cross product between v1 and v2
							n = (v1 ^ v2);
							
							// compute the dot product between the normal and the scaleFactor
							dp = n * this->scaleFactor;
							
							// find the angle (in radians)
							// dp = |n| * |scaleFactor| * cos(angle)
							angle = acos( dp / ( n.length() * scaleFactor.length() ) );
							
							// no need to scale if the scaling is happening parallel to the face (i.e. angle is 0 or pi)
							if( fabs(angle) < 0.001 || fabs( fabs( angle ) - osg::PI ) < 0.001 )
								continue;
							
							// scale along X
							// only do this at the beginning for tri strips
							if( scaleFactor.x() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + 0.5 * TEC((*texCoords)[i1].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.x() * cos(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + 0.5 * TEC((*texCoords)[i2].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.x() * cos(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + 0.5 * TEC((*texCoords)[i3].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.x() * cos(angle)));
									tmp[i3] = i3;
								}
							}
							
							// scale along Y
							// only do this at the beginning for tri strips
							if( scaleFactor.y() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + 0.5 * TEC((*texCoords)[i1].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.y() * cos(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + 0.5 * TEC((*texCoords)[i2].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.y() * cos(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + 0.5 * TEC((*texCoords)[i3].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.y() * cos(angle)));
									tmp[i3] = i3;
								}
							}
							
							// scale along Z
							// only do this at the beginning for tri strips
							if( scaleFactor.z() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + TEC((*texCoords)[i1].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.z() * sin(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + TEC((*texCoords)[i2].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.z() * sin(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + TEC((*texCoords)[i3].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.z() * sin(angle)));
									tmp[i3] = i3;
								}
							}						  
							
							printf("(%d)   texcoord %f %f \n", i1, (*texCoords)[i1].x(), (*texCoords)[i1].y());
							printf("(%d)   texcoord %f %f \n", i2, (*texCoords)[i2].x(), (*texCoords)[i2].y());
							printf("(%d)   texcoord %f %f \n", i3, (*texCoords)[i3].x(), (*texCoords)[i3].y());
							printf("\n");
							
							// advance the previous two indexes
							i1 = i2;
							i2 = i3;
						}
						
						break;	// end tri-strip
					}
					
					// handle triangle primitives (similar to tristrips)
					case osg::PrimitiveSet::TRIANGLES : {
						// indexes to points in the mesh that form a face
						unsigned int i1;
						unsigned int i2;
						unsigned int i3;
						
						// working copies of points
						osg::Vec3 p1, p2, p3;
						
						// working vectors
						osg::Vec3 v1, v2, n;
						
						// working dot product
						double dp;
						
						// working angle value (in radians)
						double angle;
						
						// keep track of indices to which texture coordinates we've scaled (i.e. no repeats)
						// NOTE: tmp is a reflexive map, used in place of a vector for it's faster insertion and searching capabilities
						map<unsigned int, unsigned int> tmp = map<unsigned int, unsigned int>();
						tmp[i1] = i1;
						tmp[i2] = i2;
						
						printf("%d indices\n", (*i)->getNumIndices());
						
						// iterate through the rest to get the faces by indexing vertexes with the primitiveset elements
						for( unsigned int j = 0; j < (*i)->getNumIndices(); j+=3) {
							// assign the indexes
							i1 = (*i)->index(j);
							i2 = (*i)->index(j+1);
							i3 = (*i)->index(j+2);
							
							// get the points
							p1 = (*vertexArray)[i1];
							p2 = (*vertexArray)[i2];
							p3 = (*vertexArray)[i3];
							
							// compute vectors from the face (the vectors will be along two of the edges)
							v1 = p2 - p1;
							v2 = p2 - p3;
							
							// compute one of the normals by finding the cross product between v1 and v2
							n = (v1 ^ v2);
							
							// compute the dot product between the normal and the scaleFactor
							dp = n * this->scaleFactor;
							
							// find the angle (in radians)
							// dp = |n| * |scaleFactor| * cos(angle)
							angle = acos( dp / ( n.length() * scaleFactor.length() ) );
							
							// no need to scale if the scaling is happening parallel to the face (i.e. angle is 0 or pi)
							if( fabs(angle) < 0.001 || fabs( fabs( angle ) - osg::PI ) < 0.001 )
								continue;
							
							// scale along X
							// only do this at the beginning for tri strips
							if( scaleFactor.x() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + TEC((*texCoords)[i1].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.x() * cos(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + TEC((*texCoords)[i2].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.x() * cos(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + TEC((*texCoords)[i3].x())*(scaleFactor.x() * sin(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.x() * cos(angle)));
									tmp[i3] = i3;
								}
							}
							
							// scale along Y
							// only do this at the beginning for tri strips
							if( scaleFactor.y() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + TEC((*texCoords)[i1].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.y() * cos(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + TEC((*texCoords)[i2].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.y() * cos(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + TEC((*texCoords)[i3].x())*(scaleFactor.y() * sin(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.y() * cos(angle)));
									tmp[i3] = i3;
								}
							}
							
							// scale along Z
							// only do this at the beginning for tri strips
							if( scaleFactor.z() != 0 ) {	// catch this to prevent NaN's
								if( j == 2 ) {
									(*texCoords)[i1].set( (*texCoords)[i1].x() + TEC((*texCoords)[i1].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i1].y() + TEC((*texCoords)[i1].y())*(scaleFactor.z() * sin(angle)));
														  
									(*texCoords)[i2].set( (*texCoords)[i2].x() + TEC((*texCoords)[i2].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i2].y() + TEC((*texCoords)[i2].y())*(scaleFactor.z() * sin(angle)));
								}
								if( tmp.count(i3) == 0 ) {
									(*texCoords)[i3].set( (*texCoords)[i3].x() + TEC((*texCoords)[i3].x())*(scaleFactor.z() * cos(angle)),
														  (*texCoords)[i3].y() + TEC((*texCoords)[i3].y())*(scaleFactor.z() * sin(angle)));
									tmp[i3] = i3;
								}
							}						  
							
							printf("(%d)   texcoord %f %f \n", i1, (*texCoords)[i1].x(), (*texCoords)[i1].y());
							printf("(%d)   texcoord %f %f \n", i2, (*texCoords)[i2].x(), (*texCoords)[i2].y());
							printf("(%d)   texcoord %f %f \n", i3, (*texCoords)[i3].x(), (*texCoords)[i3].y());
							printf("\n");
							
						}
						
						break;	// end trangles
					}
		
				}
			}
		}
		
		// regenerate the OpenGL display list in the geometry so it reflects the changes to the texture coordinates
		geo->dirtyDisplayList();
		
		printf("scaled by %f %f %f\n", scaleFactor.x(), scaleFactor.y(), scaleFactor.z());
		
	}
}
