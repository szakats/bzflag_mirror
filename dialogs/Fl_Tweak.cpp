#include "../include/dialogs/Fl_Tweak.h"

// constructor function
Fl_Tweak::Fl_Tweak( bz2object* obj, TweakOp op ) : Fl_Dialog( "Configure", DEFAULT_WIDTH, DEFAULT_HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL ) {
	this->obj = obj;
	this->setOKEventHandler( Fl_Tweak::tweak_callback, this );
	
	this->op = op;
	
	// set up the dialog box
	
	// make the label
	switch( op ) {
		case TRANSLATE_X:
			this->fieldLabel = new QuickLabel( "Translate along X", 10, 10 );
			break;
		case TRANSLATE_Y:
			this->fieldLabel = new QuickLabel( "Translate along Y", 10, 10 );
			break;
		case TRANSLATE_Z:
			this->fieldLabel = new QuickLabel( "Translate along Z", 10, 10 );
			break;
		case ROTATE_X:
			this->fieldLabel = new QuickLabel( "Rotate around X", 10, 10 );
			break;
		case ROTATE_Y:
			this->fieldLabel = new QuickLabel( "Rotate around Y", 10, 10 );
			break;
		case ROTATE_Z:
			this->fieldLabel = new QuickLabel( "Rotate around Z", 10, 10 );
			break;
		case SCALE_X:
			this->fieldLabel = new QuickLabel( "Scale along X", 10, 10 );
			break;
		case SCALE_Y:
			this->fieldLabel = new QuickLabel( "Scale along Y", 10, 10 );
			break;
		case SCALE_Z:
			this->fieldLabel = new QuickLabel( "Scale along Z", 10, 10 );
			break;
		default:
			this->fieldLabel = new QuickLabel( "(unknown tweak operator)", 10, 10 );
	}
	
	// make the field
	this->inputField = new Fl_Float_Input(10, 16 + DEFAULT_TEXTSIZE, DEFAULT_WIDTH - 20, DEFAULT_TEXTSIZE + 6);
	
	this->add( this->fieldLabel );
	this->add( this->inputField );
}

// callback
// apply the desired tweak operation
void Fl_Tweak::tweak_callback_real( Fl_Widget* w ) {
	float value = atof( inputField->value() );
	
	switch( this->op ) {
		case TRANSLATE_X:
			this->obj->setPosition( this->obj->getPosition() + osg::Vec3( value, 0.0, 0.0 ) );
			break;	
		case TRANSLATE_Y:
			this->obj->setPosition( this->obj->getPosition() + osg::Vec3( 0.0, value, 0.0 ) );
			break;
		case TRANSLATE_Z:
			this->obj->setPosition( this->obj->getPosition() + osg::Vec3( 0.0, 0.0, value ) );
			break;
		case ROTATE_X:
			this->obj->setRotationX( this->obj->getRotation().x() + value );
			break;
		case ROTATE_Y:
			this->obj->setRotationY( this->obj->getRotation().y() + value );
			break;
		case ROTATE_Z:
			this->obj->setRotationZ( this->obj->getRotation().z() + value );
			break;
		case SCALE_X:
			this->obj->setScale( this->obj->getScale() + osg::Vec3( value, 0.0, 0.0 ) );
			break;
		case SCALE_Y:
			this->obj->setScale( this->obj->getScale() + osg::Vec3( 0.0, value, 0.0 ) );
			break;
		case SCALE_Z:
			this->obj->setScale( this->obj->getScale() + osg::Vec3( 0.0, 0.0, value ) );
			break;
		default:
			break;
	}
	
	// close the window
	Fl::delete_widget( this );
}
