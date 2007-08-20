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

#include "../include/dialogs/MainMenu.h"
#include "../include/windows/MainWindow.h"

// builds the menu widgets
void MainMenu::buildMenu(void) {
	
	this->add("Add Object", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
		this->add("Add Object/Add box", FL_CTRL+'b', this->addBoxCallback, this);
		this->add("Add Object/Add pyramid", FL_CTRL+'p', this->addPyramidCallback, this);
		this->add("Add Object/Add mesh Box", FL_CTRL+FL_ALT+'b', this->addMeshboxCallback, this);
		this->add("Add Object/Add mesh pyramid", FL_CTRL+FL_ALT+'p', this->addMeshpyrCallback, this);
		this->add("Add Object/Add teleporter", FL_CTRL+'t', this->addTeleporterCallback, this);
		this->add("Add Object/Add sphere", 0, this->addSphereCallback, this);
		this->add("Add Object/Add arc", 0, this->addArcCallback, this);
		this->add("Add Object/Add mesh", FL_CTRL+'m', this->addMeshCallback, this);
		this->add("Add Object/Add tetrahedron", 0, this->addTetrahedronCallback, this);
		this->add("Add Object/Add torus", 0, this->addTorusCallback, this);
		this->add("Add Object/Add cone", 0, this->addConeCallback, this);
		this->add("Add Object/Add zone", 0, this->addZoneCallback, this);
		this->add("Add Object/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			this->add("Add Object/Add base/Add Purple Base", 0, this->addPurpleBaseCallback, this);
			this->add("Add Object/Add base/Add Red Base", 0, this->addRedBaseCallback, this);
			this->add("Add Object/Add base/Add Green Base", 0, this->addGreenBaseCallback, this);
			this->add("Add Object/Add base/Add Blue Base", 0, this->addBlueBaseCallback, this);
		this->add("Add Object/Import object...", 0, this->importObjectCallback, this);
	
	this->add("Cut", FL_CTRL+'x', this->cutCallback, this);
	this->add("Copy", FL_CTRL+'c', this->copyCallback, this);
	this->add("Paste", FL_CTRL+'v', this->pasteCallback, this);
	this->add("Duplicate", FL_CTRL+'d', this->cloneCallback, this);
	this->add("Delete", FL_Delete, this->deleteCallback, this);
	
	this->add("(Un)Group Selection", FL_CTRL+'g', this->groupCallback, this);
	this->add("(Un)Hide Selection", FL_CTRL+'h', this->hideCallback, this);
	this->add("Save Selection As...", 0, this->saveSelectionCallback, this, FL_MENU_DIVIDER);
	
	this->add("Configure World...", 0, this->configureWorldCallback, this);
	this->add("Configure Object...", FL_CTRL+'o', this->configureObjectCallback, this, FL_MENU_DIVIDER);
	
	this->add("Define World Weapon...", FL_CTRL+'w', this->worldWeaponCallback, this);
	this->add("Link Teleporters", 0, this->linkCallback, this);
	
}


// construct a menu at a given point
MainMenu::MainMenu(MainWindow* parent, int x, int y, int width, int height) :
	Fl_Menu_Button(x, y, width, height) {
	
	this->buildMenu();
	
	this->type(Fl_Menu_Button::POPUP3);
	
	this->parent = parent;
}

// main destructor
MainMenu::~MainMenu() { }

// callbacks

// add a box
void MainMenu::addBoxCallback_real(Fl_Widget* w) {
	// we're breaking the Law of Demeter here...
	
	// make a new box using the Model's object registry
	DataEntry* newBox = this->parent->getModel()->_buildObject( "box" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newBox );
	
	if(!newObj)
		return;
		
	newObj->setName( SceneBuilder::makeUniqueName("box") );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	
	this->value(0);
}

// add a pyramid
void MainMenu::addPyramidCallback_real(Fl_Widget* w) {
	// make a new pyramid using the Model's object registry
	DataEntry* newPyr = this->parent->getModel()->_buildObject( "pyramid" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newPyr );
	
	if(!newObj)
		return;
	
	newObj->setName( SceneBuilder::makeUniqueName("pyramid") );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	this->value(0);
}

// add a meshbox
void MainMenu::addMeshboxCallback_real(Fl_Widget* w) {
	printf("added a meshbox\n");
	this->value(0);
}

// add a mesh pyramid
void MainMenu::addMeshpyrCallback_real(Fl_Widget* w) {
	printf("added a mesh pyramid\n");
	this->value(0);
}

// add a teleporter
void MainMenu::addTeleporterCallback_real(Fl_Widget* w) {
	// make a new box using the Model's object registry
	DataEntry* newTeleporter = this->parent->getModel()->_buildObject( "teleporter" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newTeleporter );
	
	if(!newObj)
		return;
	
	string name = SceneBuilder::makeUniqueName("teleporter");
	printf("teleporter name: %s\n", name.c_str());
	
	newObj->setName( name );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	
	this->value(0);
}

// add a sphere
void MainMenu::addSphereCallback_real(Fl_Widget* w) {
	printf("added a sphere\n");
	this->value(0);
}

// add an arc
void MainMenu::addArcCallback_real(Fl_Widget* w) {
	printf("added an arc\n");
	this->value(0);
}

// add a mesh
void MainMenu::addMeshCallback_real(Fl_Widget* w) {
	printf("added a mesh\n");
	this->value(0);
}

// add a tetrahedron
void MainMenu::addTetrahedronCallback_real(Fl_Widget* w) {
	printf("added a tetrahedron\n");
	this->value(0);
}

// add a torus
void MainMenu::addTorusCallback_real(Fl_Widget* w) {
	printf("added a torus\n");
	this->value(0);
}

// add a cone
void MainMenu::addConeCallback_real(Fl_Widget* w) {
	// make a new box using the Model's object registry
	DataEntry* newCone = this->parent->getModel()->_buildObject( "cone" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newCone );
	
	if(!newObj)
		return;
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	
	this->value(0);
}

// add a zone
void MainMenu::addZoneCallback_real(Fl_Widget* w) {
	printf("added a zone\n");
	this->value(0);
}

// import an object
void MainMenu::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	this->value(0);
}

// add base 1
void MainMenu::addPurpleBaseCallback_real(Fl_Widget* w) {
	
	// get the objects and see that we don't have any other bases
	vector< bz2object* > objects = this->parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( *i );
			if(b != NULL && b->getTeam() == BASE_PURPLE)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = this->parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	// set the base
	newObj->setTeam( BASE_PURPLE );
	
	newObj->setName( "purple_base" );
	
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	this->value(0);
}

// add base 2
void MainMenu::addRedBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	vector< bz2object* > objects = this->parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( *i );
			if(b != NULL && b->getTeam() == BASE_RED)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = this->parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	newObj->setName( "red_base" );
	
	
	// set the base
	newObj->setTeam( BASE_RED );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	this->value(0);
}

// add base 3
void MainMenu::addGreenBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	vector< bz2object* > objects = this->parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( *i );
			if(b != NULL && b->getTeam() == BASE_GREEN)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = this->parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	// set the base
	newObj->setTeam( BASE_GREEN );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	this->value(0);
}

// add base 4
void MainMenu::addBlueBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	vector< bz2object* > objects = this->parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( *i );
			if(b != NULL && b->getTeam() == BASE_BLUE)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = this->parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
	
	newObj->setName( "blue_base" );
	
	// set the base
	newObj->setTeam( BASE_BLUE );
	
	// add the object to the model
	this->parent->getModel()->_unselectAll();
	this->parent->getModel()->_addObject( newObj );
	this->parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	this->parent->configure( newObj );
	
	this->value(0);
}

// handle cut
void MainMenu::cutCallback_real(Fl_Widget* w) {
	this->parent->getModel()->_cutSelection();
	this->value(0);
}

// handle copy
void MainMenu::copyCallback_real(Fl_Widget* w) {
	this->parent->getModel()->_copySelection();
	this->value(0);
}

// handle paste
void MainMenu::pasteCallback_real(Fl_Widget* w) {
	this->parent->getModel()->_pasteSelection();
	this->value(0);
}

// handle selection clone
void MainMenu::cloneCallback_real(Fl_Widget* w) {
	this->parent->getModel()->_copySelection();
	this->parent->getModel()->_pasteSelection();
	
	this->value(0);
}

// handle deletion
void MainMenu::deleteCallback_real(Fl_Widget* w) {
	this->parent->getModel()->_deleteSelection();
	this->value(0);
}

// handle (un)grouping
void MainMenu::groupCallback_real(Fl_Widget* w) {
	printf("(un)grouped\n");
	this->value(0);
}

// handle (un)hiding
void MainMenu::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	this->value(0);
}

// handle save selection
void MainMenu::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	this->value(0);
}

// handle configure world
void MainMenu::configureWorldCallback_real(Fl_Widget* w) {
	// configure the new world
	WorldOptionsDialog* wod = new WorldOptionsDialog( this->parent->getModel()->_getWorldData(),
													  this->parent->getModel()->_getOptionsData(),
													  this->parent->getModel()->_getWaterLevelData());
	wod->show();
	
	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }
	
	// reset the world
	ObserverMessage obs(ObserverMessage::UPDATE_WORLD, this->parent->getModel()->_getWorldData() );
	this->parent->getModel()->notifyObservers( &obs );
	this->value(0);
}

// handle world weapon creation
void MainMenu::worldWeaponCallback_real(Fl_Widget* w) {
	printf("world weapon created\n");
	this->value(0);
}

// handle object configuration
void MainMenu::configureObjectCallback_real(Fl_Widget* w) {
	printf("configured an object\n");	
	this->value(0);
}

// handle teleporter linking
void MainMenu::linkCallback_real(Fl_Widget* w) {
	// get all selected objects
	vector< bz2object* > selection = this->parent->getModel()->_getSelection(); 
	if( selection.size() <= 0 )
		return;
		
	// map of teleporter links to create
	map< teleporter*, teleporter* > teleporterMap;
	for( vector< bz2object *>::iterator i = selection.begin(); i != selection.end(); i++ ) {
		if( (*i)->getHeader() == "teleporter" ) {
			teleporter* t1 = dynamic_cast< teleporter* > (*i);
			if( !t1 )
				continue;
			
			// get other teleporters
			if( selection.size() >= 2 ) {
				for( vector< bz2object* >::iterator j = i+1; j != selection.end(); j++ ) {
						
					if( (*j)->getHeader() == "teleporter" ) {
						teleporter* t2 = dynamic_cast< teleporter* >( *j );
						if( !t2 )
							continue;
							
						teleporterMap[ t1 ] = t2;
					}
				}
			}
		}
	}
	
	// make the links
	if( teleporterMap.size() > 0 ) {
		for( map< teleporter*, teleporter* >::iterator i = teleporterMap.begin(); i != teleporterMap.end(); i++ ) {
			this->parent->getModel()->_linkTeleporters( i->first, i->second );
		}
	}
	
	this->value(0);
}
