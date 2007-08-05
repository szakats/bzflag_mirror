#include "../include/dialogs/MasterConfigurationDialog.h"
#include "../include/defines.h"
#include <iostream>

MasterConfigurationDialog::MasterConfigurationDialog(DataEntry* obj) :
	ConfigurationDialog(obj, "Master Configuration Dialog", WIDTH, HEIGHT) {
	
	this->moreDialog = NULL;
	
	this->startShift = NULL;
	this->endShift = NULL;
	
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
	positionLabel = new QuickLabel("Position", 5, 15);
	positionXField = new Fl_Float_Input(5 + 100, 15, 100, DEFAULT_TEXTSIZE + 6, "X");
	positionYField = new Fl_Float_Input(5 + 100 + 120, 15, 100, DEFAULT_TEXTSIZE + 6, "Y");
	positionZField = new Fl_Float_Input(5 + 100 + 120 + 120, 15, 100, DEFAULT_TEXTSIZE + 6, "Z");
	
	if(!object->isKey("position")) {
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
	
	if(!object->isKey("rotation")) {
		rotationField->deactivate();	
	}
	else {
		rotationField->value(ftoa(rotation).c_str());	
	}
	
	// size
	sizeLabel = new QuickLabel("Size", 5, 75);
	sizeXField = new Fl_Float_Input(5 + 100, 75, 100, DEFAULT_TEXTSIZE + 6, "dx");
	sizeYField = new Fl_Float_Input(5 + 100 + 120, 75, 100, DEFAULT_TEXTSIZE + 6, "dy");
	sizeZField = new Fl_Float_Input(5 + 100 + 120 + 120, 75, 100, DEFAULT_TEXTSIZE + 6, "dz");
	
	if(!object->isKey("size")) {
		sizeXField->deactivate();
		sizeYField->deactivate();
		sizeZField->deactivate();	
	}
	else {
		sizeXField->value(ftoa(size.x()).c_str());
		sizeYField->value(ftoa(size.y()).c_str());
		sizeZField->value(ftoa(size.z()).c_str());	
	}
	
	// transformation scroll area
	transformationLabel = new QuickLabel("Transformations (order matters)", 5, 115);
	transformationScrollArea = new Fl_Scroll(5, 145, WIDTH - 10, 200);
	transformationScrollArea->end();
	transformationScrollArea->box(FL_UP_BOX);
	transformationScrollArea->type(Fl_Scroll::VERTICAL_ALWAYS);
	// add the transformations if they exist and are supported
	if(transformationFormat.length() > 0 && transforms.size() > 0) {
		// check and see if the first transformation is the necessary "shift -x -y -z" transformation.
		// if so, then deactivate it to prevent people from using it (i.e. removing it).
		// see bz2object.cpp for details on why this is.
		vector< osg::ref_ptr<BZTransform> >::iterator itr = transforms.begin();
		vector<float> data = (*itr)->getData();
		
		// see if this is indeed the ubiquitous shift transform
		if( transforms.size() >= 2 &&		// there should be two transformations, then: this, and the last shift
			(*itr)->getName() == "shift" &&  data[0] == -object->getPos().x() &&
											 data[1] == -object->getPos().y() &&
											 data[2] == -object->getPos().z() ) {
			
			// add the transformation to the list	 	
			TransformWidget* firstShift = new TransformWidget(0, 0, transformationScrollArea->w(), 3*DEFAULT_TEXTSIZE, this->transformationFormat.c_str(), false );
			firstShift->setTransformationType( (*itr)->getName().c_str() );
			firstShift->setFields( data );
			
			this->addTransformCallback_real( firstShift );
			firstShift->deactivate();
			
			// save the reference
			this->startShift = firstShift;
		}
		
		// add the rest of the transforms
		for(vector< osg::ref_ptr<BZTransform> >::iterator i = transforms.end() - 2; i != itr; i--) {
			// next transform widget (the dimensions of which shall be calculated in the addTransformCallback_real method
			TransformWidget* nextTransform = new TransformWidget(0, 0, transformationScrollArea->w(), 3*DEFAULT_TEXTSIZE, this->transformationFormat.c_str(), false);
			
			nextTransform->setTransformationType( (*i)->getName().c_str() );
			nextTransform->setFields( (*i)->getData() );
			
			this->addTransformCallback_real(nextTransform);
			
			
		}
		
		// see if the last transformation added was indeed the ending shift
		// and if so, deactivate it (see why in bz2object.cpp)
		itr = transforms.end() - 1;
		data = (*itr)->getData();
		
		// see if this is indeed the ubiquitous shift transform
		if( transforms.size() >= 2 &&		// there should be two transformations, then: the first shift and this one
			(*itr)->getName() == "shift" &&  data[0] == object->getPos().x() &&
											 data[1] == object->getPos().y() &&
											 data[2] == object->getPos().z() ) {
			
			TransformWidget* lastShift = new TransformWidget(transformationScrollArea->x() + 5, transformationScrollArea->y() + 3*DEFAULT_TEXTSIZE*(transforms.size()), transformationScrollArea->w(), 3*DEFAULT_TEXTSIZE, this->transformationFormat.c_str(), false );
			lastShift->setTransformationType( (*itr)->getName().c_str() );
			lastShift->setFields( data );
			
			transformations.push_back( lastShift );
			transformationScrollArea->add( lastShift );
			
			// deactivate it
			lastShift->deactivate();	
			
			transformationScrollArea->redraw();
			
			// save the reference
			this->endShift = lastShift;					 	
		}
		else {
			TransformWidget* nextTransform = new TransformWidget(0, 0, transformationScrollArea->w(), 3*DEFAULT_TEXTSIZE, this->transformationFormat.c_str(), false);
			
			nextTransform->setTransformationType( (*itr)->getName().c_str() );
			nextTransform->setFields( (*itr)->getData() );
			
			this->addTransformCallback_real(nextTransform);
		}
		
	}
	
	// add transformation button
	addTransformationButton = new Fl_Button(15, 355, 90, DEFAULT_TEXTSIZE + 6, "Add");
	addTransformationButton->callback(addTransformCallback, this);
	addTransformationButton->when(FL_WHEN_RELEASE);
	if(transformationFormat.length() == 0)
		addTransformationButton->deactivate();	// don't add transformations if none are supported
	
	// remove transformation button
	removeTransformationButton = new Fl_Button(110, 355, 90, DEFAULT_TEXTSIZE + 6, "Remove");
	removeTransformationButton->callback(removeTransformCallback, this);
	removeTransformationButton->when(FL_WHEN_RELEASE);
	if(transformationFormat.length() == 0)
		removeTransformationButton->deactivate(); // don't remove transformations if none can be added
	
	// more button
	moreButton = new Fl_Button(WIDTH - 10 - 95 - 95 - 95, 355, 90, DEFAULT_TEXTSIZE + 6, "More...");
	moreButton->callback(moreCallback, this);
	moreButton->when(FL_WHEN_RELEASE);
	moreButton->deactivate();		// will be re-activated upon a successfull call to setAdditionalConfiguration()
	
	// edit button
	editTextButton = new Fl_Button(WIDTH - 10 - 95 - 95, 355, 90, DEFAULT_TEXTSIZE + 6, "Edit BZW");
	editTextButton->callback(editTextCallback, this);
	editTextButton->when(FL_WHEN_RELEASE);
	
	// advanced button
	advancedButton = new Fl_Button(WIDTH - 10 - 95, 355, 90, DEFAULT_TEXTSIZE + 6, "Advanced...");
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
	Point3D position = Point3D( string(positionXField->value()) + " " + positionYField->value() + " " + positionZField->value() + "\n" );
	Point3D size = Point3D( string(sizeXField->value()) + " " + sizeYField->value() + " " + sizeZField->value() + "\n" );
	
	// get the position, rotation, and size fields and string-ify them
	string positionString = string("position ") + string(positionXField->value()) + " " + positionYField->value() + " " + positionZField->value() + "\n";
	string rotationString = string("rotation ") + rotationField->value() + "\n";
	string sizeString = string("size ") + string(sizeXField->value()) + " " + sizeYField->value() + " " + sizeZField->value() + "\n";
	
	// get the transformations and string-ify them
	string transformationString("");
	
	if(transformations.size() != 0) {
		for(vector<TransformWidget*>::iterator i = transformations.end() - 1; i != transformations.begin() - 1; i--) {
			if( (*i)->active() )
				transformationString += (*i)->toString();
		}
	}
	
	// update the object
	string objectString = object->getHeader() + "\n" +
						  "  " + positionString +
						  "  " + rotationString +
						  "  " + sizeString +
						  "  " + transformationString + "\n" +
						  "end\n";
	
	object->update( objectString );
	
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
	int y;
	
	// pick the value for y
	if( this->startShift != NULL )	{	// we have the ubiquitous footer shift ==> y should be above it it
		// move the widget down
		y = startShift->y() + 3*DEFAULT_TEXTSIZE*(transformations.size());	
	}
	else	// we have a transformation before this one ==> y should be just underneath it
		y = transformationScrollArea->y() + 3*DEFAULT_TEXTSIZE*(transformations.size()+1);
	
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
	
	// if we have the inactive end shift, then insert the transformation just before it
	if( this->endShift != NULL ) {
		transformations.insert( transformations.end() - 1, newTransform );
		endShift->position( endShift->x(), endShift->y() * 3*DEFAULT_TEXTSIZE );
	}
	else
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
	
}

/*
 * Open/close the text edit widget
 */
void MasterConfigurationDialog::editTextCallback_real(Fl_Widget* w) {
	
}
