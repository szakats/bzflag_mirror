/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "dialogs/MasterConfigurationDialog.h"
#include "defines.h"
#include <iostream>

MasterConfigurationDialog::MasterConfigurationDialog(DataEntry* obj) :
	ConfigurationDialog(obj, "Master Configuration Dialog", WIDTH, HEIGHT) {
	
	this->end();
	
	this->moreDialog = NULL;
	
	// initialize the object
	this->object = dynamic_cast<bz2object*>( obj );
	if( !this->object )
		return;
	
	// get information from the object that can be put into the widgets
	string objectStr = object->get();
	
	// read position
	Point3D position = Point3D(object->getPos());
	
	// read rotation
	float rotation = (object->getRotation().z());
	
	// read size
	Point3D size = Point3D(object->getSize());
	
	// read transformations
	vector< osg::ref_ptr<BZTransform> > transforms = (object->getTransformations());
	
	// find out the supported transformations and determine their field format
	supportedTransformations = string("");
	transformationFormat = string("");
	
	if(object->isKey("shift")) {
		supportedTransformations += "<shift>";
		transformationFormat += "<shift:|along x|along y|along z|>";	
	}
	if(object->isKey("shear")) {
		supportedTransformations += "<shear>";
		transformationFormat += "<shear:|dx|dy|dz|>";
	}
	if(object->isKey("spin")) {
		supportedTransformations += "<spin>";
		transformationFormat += "<spin:|Angle|dx|dy|dz|>";
	}
	if(object->isKey("scale")) {
		supportedTransformations += "<scale>";
		transformationFormat += "<scale:|x multiplier|y multiplier|z multiplier|>";
	}	
	
	// initialize widgets
	
	// position
	if( object->isKey("position") )
		positionLabel = new QuickLabel("Position", 5, 15);
	else if( object->isKey("shift") )	// "shift" can emulate "position"
		positionLabel = new QuickLabel("Shift", 5, 15);
		
	positionXField = new Fl_Float_Input(5 + 100, 15, 100, DEFAULT_TEXTSIZE + 6, "X");
	positionYField = new Fl_Float_Input(5 + 100 + 120, 15, 100, DEFAULT_TEXTSIZE + 6, "Y");
	positionZField = new Fl_Float_Input(5 + 100 + 120 + 120, 15, 100, DEFAULT_TEXTSIZE + 6, "Z");
	
	if(!(object->isKey("position") || object->isKey("shift")) ) {
		positionXField->deactivate();
		positionYField->deactivate();
		positionZField->deactivate();	
	}
	else {
		positionXField->value(ftoa(position.x()).c_str());
		positionYField->value(ftoa(position.y()).c_str());
		positionZField->value(ftoa(position.z()).c_str());	
	}
	
	// rotation
	rotationLabel = new QuickLabel("Rotation", 5, 45);
	rotationField = new Fl_Float_Input(5 + 100, 45, 100, DEFAULT_TEXTSIZE + 6, "degrees");
	rotationField->align(FL_ALIGN_RIGHT);
	
	if(!object->isKey("rotation") || object->isKey("spin")) {
		rotationField->deactivate();	// objects supporting "spin" ignore "rotation"
		rotationLabel->deactivate();
	}
	else {
		rotationField->value(ftoa(rotation).c_str());	
	}
	
	// size
	if( object->isKey("size") )
		sizeLabel = new QuickLabel("Size", 5, 75);
	else if( object->isKey("scale") )
		sizeLabel = new QuickLabel("Scale", 5, 75);
		
	sizeXField = new Fl_Float_Input(5 + 100, 75, 100, DEFAULT_TEXTSIZE + 6, "dx");
	sizeYField = new Fl_Float_Input(5 + 100 + 120, 75, 100, DEFAULT_TEXTSIZE + 6, "dy");
	sizeZField = new Fl_Float_Input(5 + 100 + 120 + 120, 75, 100, DEFAULT_TEXTSIZE + 6, "dz");
	
	if(!(object->isKey("size") || object->isKey("scale")) ) {
		sizeXField->deactivate();
		sizeYField->deactivate();
		sizeZField->deactivate();	
	}
	else {
		sizeXField->value(ftoa(size.x()).c_str());
		sizeYField->value(ftoa(size.y()).c_str());
		sizeZField->value(ftoa(size.z()).c_str());	
	}
	
	// spin (BZW 2.0 objects only)
	spinLabel = new QuickLabel("Spin", 5, 105);
	spinXField = new Fl_Float_Input(5 + 100, 105, 100, DEFAULT_TEXTSIZE + 6, "rx");
	spinYField = new Fl_Float_Input(5 + 100 + 120, 105, 100, DEFAULT_TEXTSIZE + 6, "ry");
	spinZField = new Fl_Float_Input(5 + 100 + 120 + 120, 105, 100, DEFAULT_TEXTSIZE + 6, "rz");
	
	if( !object->isKey("spin") ) {
		spinXField->deactivate();
		spinYField->deactivate();
		spinZField->deactivate();
		spinLabel->deactivate();
	}
	else {
		spinXField->value(ftoa(object->getRotation().x()).c_str());
		spinYField->value(ftoa(object->getRotation().y()).c_str());
		spinZField->value(ftoa(object->getRotation().z()).c_str());	
	}
	
	// transformation scroll area
	transformationLabel = new QuickLabel("Transformations (order matters)", 5, 145);
	transformationScrollArea = new Fl_Scroll(5, 175, WIDTH - 10, 200);
	transformationScrollArea->end();
	transformationScrollArea->box(FL_UP_BOX);
	transformationScrollArea->type(Fl_Scroll::VERTICAL_ALWAYS);
	// add the transformations if they exist and are supported
	if(transformationFormat.length() > 0 && transforms.size() > 0) {
		
		// add the transforms
		for(vector< osg::ref_ptr<BZTransform> >::iterator i = transforms.end() - 1; i != transforms.begin() - 1; i--) {
			// next transform widget (the dimensions of which shall be calculated in the addTransformCallback_real method
			TransformWidget* nextTransform = new TransformWidget(0, 0, transformationScrollArea->w(), 3*DEFAULT_TEXTSIZE, this->transformationFormat.c_str(), false);
			
			nextTransform->setTransformationType( (*i)->getName().c_str() );
			nextTransform->setFields( (*i)->getData() );
			
			this->addTransformCallback_real(nextTransform);	
		}
	}
	
	// add transformation button
	addTransformationButton = new Fl_Button(15, 385, 90, DEFAULT_TEXTSIZE + 6, "Add");
	addTransformationButton->callback(addTransformCallback, this);
	addTransformationButton->when(FL_WHEN_RELEASE);
	if(transformationFormat.length() == 0)
		addTransformationButton->deactivate();	// don't add transformations if none are supported
	
	// remove transformation button
	removeTransformationButton = new Fl_Button(110, 385, 90, DEFAULT_TEXTSIZE + 6, "Remove");
	removeTransformationButton->callback(removeTransformCallback, this);
	removeTransformationButton->when(FL_WHEN_RELEASE);
	if(transformationFormat.length() == 0)
		removeTransformationButton->deactivate(); // don't remove transformations if none can be added
	
	// more button
	moreButton = new Fl_Button(WIDTH - 10 - 95 - 95 - 95, 385, 90, DEFAULT_TEXTSIZE + 6, "More...");
	moreButton->callback(moreCallback, this);
	moreButton->when(FL_WHEN_RELEASE);
	moreButton->deactivate();		// will be re-activated upon a successfull call to setAdditionalConfiguration()
	
	// edit button
	editTextButton = new Fl_Button(WIDTH - 10 - 95 - 95, 385, 90, DEFAULT_TEXTSIZE + 6, "Edit BZW");
	editTextButton->callback(editTextCallback, this);
	editTextButton->when(FL_WHEN_RELEASE);
	
	// advanced button
	advancedButton = new Fl_Button(WIDTH - 10 - 95, 385, 90, DEFAULT_TEXTSIZE + 6, "Advanced...");
	advancedButton->callback(advancedCallback, this);
	advancedButton->when(FL_WHEN_RELEASE);
	
	// add default OK/Cancel callbacks
	this->setOKEventHandler(OKButtonCallback, this);
	this->setCancelEventHandler(CancelButtonCallback, this);
	
	// add widgets
	this->add(positionLabel);
	this->add(positionXField);
	this->add(positionYField);
	this->add(positionZField);
	this->add(rotationLabel);
	this->add(rotationField);
	this->add(sizeLabel);
	this->add(sizeXField);
	this->add(sizeYField);
	this->add(sizeZField);
	this->add(spinLabel);
	this->add(spinXField);
	this->add(spinYField);
	this->add(spinZField);
	this->add(transformationLabel);
	this->add(transformationScrollArea);
	this->add(addTransformationButton);
	this->add(removeTransformationButton);
	this->add(moreButton);
	this->add(editTextButton);
	this->add(advancedButton);
	
}

/**
 * 
 * Callback Implementation
 * 
 */
 
/*
 * Load data back into the object we are configuring and quit
 */
void MasterConfigurationDialog::OKButtonCallback_real(Fl_Widget* w) {
	
	// get the data
	osg::Vec3 position = osg::Vec3( atof(positionXField->value()), atof(positionYField->value()), atof(positionZField->value()) );
	osg::Vec3 size = osg::Vec3( atof(sizeXField->value()), atof(sizeYField->value()), atof(sizeZField->value()) );
	osg::Vec3 spinVals = osg::Vec3( atof(spinXField->value()), atof(spinYField->value()), atof(spinZField->value()) );
	
	float zRotation = atof( rotationField->value() );
	osg::Vec3 rotation = osg::Vec3( 0.0, 0.0, zRotation );
	
	// get the transformations
	vector< osg::ref_ptr< BZTransform > > transforms = vector< osg::ref_ptr< BZTransform > >();
	
	if(transformations.size() != 0) {
		for(vector<TransformWidget*>::iterator i = transformations.end() - 1; i != transformations.begin() - 1; i--) {
			if( (*i)->active() ) {
				string widgetData = (*i)->toString();
				transforms.push_back( new BZTransform( widgetData ) );
			}
		}
	}
	
	// call updates
	UpdateMessage positionUpdate( UpdateMessage::SET_POSITION, &position );
	UpdateMessage sizeUpdate( UpdateMessage::SET_SCALE, &size );
	UpdateMessage rotationUpdate( UpdateMessage::SET_ROTATION, &rotation );
	UpdateMessage transformUpdate( UpdateMessage::SET_TRANSFORMATIONS, &transforms );
	
	if( object->isKey("position") || object->isKey("shift") )
		object->update( positionUpdate );
		
	if( object->isKey("size") || object->isKey("scale") )
		object->update( sizeUpdate );
		
	if( object->isKey("rotation") && !object->isKey("spin"))
		object->update( rotationUpdate );
		
	if( object->isKey("spin") ) {
		object->setRotation( spinVals );
	}
	object->update( transformUpdate );
	
	printf("data: \n|%s|\n", object->toString().c_str());
	
	Fl::delete_widget(this);	
}

/*
 * Just quit
 */
void MasterConfigurationDialog::CancelButtonCallback_real(Fl_Widget* w) {
	Fl::delete_widget(this);	
}

/*
 * Add a transformation to the list
 */
void MasterConfigurationDialog::addTransformCallback_real(Fl_Widget* widgetToAdd) {
	
	int x = transformationScrollArea->x() + 5;
	int y = transformationScrollArea->y() + 3*DEFAULT_TEXTSIZE*(transformations.size()+1);
	
	int width = transformationScrollArea->w();
	int height = 3*DEFAULT_TEXTSIZE;
	
	// the passed Fl_Widget* should be the next transformation to add
	TransformWidget* newTransform = dynamic_cast< TransformWidget* >(widgetToAdd);
	
	if( widgetToAdd == this || !newTransform ) {
		// if the dynamic_cast failed, then just make the default
		newTransform = new TransformWidget(x, y, width, height, this->transformationFormat.c_str(), false);
	}
	else {
		newTransform->position( x, y );
	}
	
	transformationScrollArea->add(newTransform);
	transformationScrollArea->redraw();
	newTransform->redraw();
	
	transformations.push_back( newTransform );
}

/*
 * Remove a transformation from the list
 */
void MasterConfigurationDialog::removeTransformCallback_real(Fl_Widget* w) {
	
	if(transformations.size() == 0)
		return;
		
	// new list of transformations
	vector<TransformWidget*> newTransformList = vector<TransformWidget*>();
	
	// remove the marked transformations, and add the unmarked to newTransformList
	for(unsigned int i = 0; i < transformations.size(); i++) {
		if(transformations[i]->isActive()) {
			transformationScrollArea->remove(transformations[i]);	
		}
		else {
			newTransformList.push_back(transformations[i]);	
		}
	}
	
	// copy over newTransformList to transformations
	transformations.clear();
	transformations.assign(newTransformList.begin(), newTransformList.end());	
	
	// shift everything up
	int y = transformationScrollArea->y() + 2*DEFAULT_TEXTSIZE;
	for(vector<TransformWidget*>::iterator i = transformations.begin(); i != transformations.end(); i++) {
		(*i)->position((*i)->x(), y);
		y += 3*DEFAULT_TEXTSIZE;
	}
	
	// refresh
	transformationScrollArea->redraw();
}

/*
 * Bring up object-specific dialog box
 */
void MasterConfigurationDialog::moreCallback_real(Fl_Widget* w) {
	if( this->moreDialog )
		this->moreDialog->show();
}

/*
 * Bring up the Advanced... dialog
 */
void MasterConfigurationDialog::advancedCallback_real(Fl_Widget* w) {
	AdvancedOptionsDialog* aod = new AdvancedOptionsDialog( this->object );
	aod->show();
}

/*
 * Open/close the text edit widget
 */
void MasterConfigurationDialog::editTextCallback_real(Fl_Widget* w) {
	
}
