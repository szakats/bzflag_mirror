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

// this class is currently unused
#if 0
#include "dialogs/MainMenu.h"
#include "windows/MainWindow.h"

// builds the menu widgets
void MainMenu::buildMenu(void) {
	
	add("Add Object", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
		add("Add Object/Add box", FL_CTRL+'b', addBoxCallback, this);
		add("Add Object/Add pyramid", FL_CTRL+'p', addPyramidCallback, this);
		add("Add Object/Add mesh Box", FL_CTRL+FL_ALT+'b', addMeshboxCallback, this);
		add("Add Object/Add mesh pyramid", FL_CTRL+FL_ALT+'p', addMeshpyrCallback, this);
		add("Add Object/Add teleporter", FL_CTRL+'t', addTeleporterCallback, this);
		add("Add Object/Add sphere", 0, addSphereCallback, this);
		add("Add Object/Add arc", 0, addArcCallback, this);
		add("Add Object/Add mesh", FL_CTRL+'m', addMeshCallback, this);
		add("Add Object/Add tetrahedron", 0, addTetrahedronCallback, this);
		add("Add Object/Add torus", 0, addTorusCallback, this);
		add("Add Object/Add cone", 0, addConeCallback, this);
		add("Add Object/Add zone", 0, addZoneCallback, this);
		add("Add Object/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			add("Add Object/Add base/Add Purple Base", 0, addPurpleBaseCallback, this);
			add("Add Object/Add base/Add Red Base", 0, addRedBaseCallback, this);
			add("Add Object/Add base/Add Green Base", 0, addGreenBaseCallback, this);
			add("Add Object/Add base/Add Blue Base", 0, addBlueBaseCallback, this);
		add("Add Object/Import object...", 0, importObjectCallback, this);
	
	add("Cut", FL_CTRL+'x', cutCallback, this);
	add("Copy", FL_CTRL+'c', copyCallback, this);
	add("Paste", FL_CTRL+'v', pasteCallback, this);
	add("Duplicate", FL_CTRL+'d', cloneCallback, this);
	add("Delete", FL_Delete, deleteCallback, this);
	
	add("(Un)Group Selection", FL_CTRL+'g', groupCallback, this);
	add("(Un)Hide Selection", FL_CTRL+'h', hideCallback, this);
	add("Save Selection As...", 0, saveSelectionCallback, this, FL_MENU_DIVIDER);
	
	add("Configure World...", 0, configureWorldCallback, this);
	add("Configure Object...", FL_CTRL+'o', configureObjectCallback, this, FL_MENU_DIVIDER);
	
	add("Define World Weapon...", FL_CTRL+'w', worldWeaponCallback, this);
	add("Link Teleporters", 0, linkCallback, this);
	
}


// construct a menu at a given point
MainMenu::MainMenu(MainWindow* _parent, int x, int y, int width, int height) :
	Fl_Menu_Button(x, y, width, height) {
	
	buildMenu();
	
	type(Fl_Menu_Button::POPUP3);
	
	parent = _parent;
}

// main destructor
MainMenu::~MainMenu() { }

// callbacks

// add a box
void MainMenu::addBoxCallback_real(Fl_Widget* w) {
	// we're breaking the Law of Demeter here...
	
	// make a new box using the Model's object registry
	DataEntry* newBox = this->parent->getModel()->buildObject( "box" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newBox );
	
	if(!newObj)
		return;
		
	newObj->setName( SceneBuilder::makeUniqueName("box") );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	
	value(0);
}

// add a pyramid
void MainMenu::addPyramidCallback_real(Fl_Widget* w) {
	// make a new pyramid using the Model's object registry
	DataEntry* newPyr = parent->getModel()->_buildObject( "pyramid" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newPyr );
	
	if(!newObj)
		return;
	
	newObj->setName( SceneBuilder::makeUniqueName("pyramid") );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	value(0);
}

// add a meshbox
void MainMenu::addMeshboxCallback_real(Fl_Widget* w) {
	printf("added a meshbox\n");
	value(0);
}

// add a mesh pyramid
void MainMenu::addMeshpyrCallback_real(Fl_Widget* w) {
	printf("added a mesh pyramid\n");
	value(0);
}

// add a teleporter
void MainMenu::addTeleporterCallback_real(Fl_Widget* w) {
	// make a new box using the Model's object registry
	DataEntry* newTeleporter = parent->getModel()->_buildObject( "teleporter" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newTeleporter );
	
	if(!newObj)
		return;
	
	string name = SceneBuilder::makeUniqueName("teleporter");
	printf("teleporter name: %s\n", name.c_str());
	
	newObj->setName( name );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	
	value(0);
}

// add a sphere
void MainMenu::addSphereCallback_real(Fl_Widget* w) {
	printf("added a sphere\n");
	value(0);
}

// add an arc
void MainMenu::addArcCallback_real(Fl_Widget* w) {
	printf("added an arc\n");
	value(0);
}

// add a mesh
void MainMenu::addMeshCallback_real(Fl_Widget* w) {
	printf("added a mesh\n");
	value(0);
}

// add a tetrahedron
void MainMenu::addTetrahedronCallback_real(Fl_Widget* w) {
	printf("added a tetrahedron\n");
	value(0);
}

// add a torus
void MainMenu::addTorusCallback_real(Fl_Widget* w) {
	printf("added a torus\n");
	value(0);
}

// add a cone
void MainMenu::addConeCallback_real(Fl_Widget* w) {
	// make a new box using the Model's object registry
	DataEntry* newCone = parent->getModel()->_buildObject( "cone" );
	
	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newCone );
	
	if(!newObj)
		return;
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	
	value(0);
}

// add a zone
void MainMenu::addZoneCallback_real(Fl_Widget* w) {
	printf("added a zone\n");
	value(0);
}

// import an object
void MainMenu::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	value(0);
}

// add base 1
void MainMenu::addPurpleBaseCallback_real(Fl_Widget* w) {
	
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( i->get() );
			if(b != NULL && b->getTeam() == BASE_PURPLE)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	// set the base
	newObj->setTeam( BASE_PURPLE );
	
	newObj->setName( "purple_base" );
	
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	value(0);
}

// add base 2
void MainMenu::addRedBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( i->get() );
			if(b != NULL && b->getTeam() == BASE_RED)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	newObj->setName( "red_base" );
	
	
	// set the base
	newObj->setTeam( BASE_RED );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	value(0);
}

// add base 3
void MainMenu::addGreenBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( i->get() );
			if(b != NULL && b->getTeam() == BASE_GREEN)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
		
	// set the base
	newObj->setTeam( BASE_GREEN );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	value(0);
}

// add base 4
void MainMenu::addBlueBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = parent->getModel()->_getObjects();
	if( objects.size() > 0 ) {
		base* b;
		// find all bases
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			b = dynamic_cast< base* >( i->get() );
			if(b != NULL && b->getTeam() == BASE_BLUE)
				return;		// there already is a purple base; don't add a second!
		}
	}
	
	// make a new base using the Model's object registry
	DataEntry* newBase = parent->getModel()->_buildObject( "base" );
	
	// make it into a bz2object
	base* newObj = dynamic_cast< base* >( newBase );
	
	if(!newObj)
		return;
	
	newObj->setName( "blue_base" );
	
	// set the base
	newObj->setTeam( BASE_BLUE );
	
	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	value(0);
}

// handle cut
void MainMenu::cutCallback_real(Fl_Widget* w) {
	parent->getModel()->_cutSelection();
	value(0);
}

// handle copy
void MainMenu::copyCallback_real(Fl_Widget* w) {
	parent->getModel()->_copySelection();
	value(0);
}

// handle paste
void MainMenu::pasteCallback_real(Fl_Widget* w) {
	parent->getModel()->_pasteSelection();
	value(0);
}

// handle selection clone
void MainMenu::cloneCallback_real(Fl_Widget* w) {
	parent->getModel()->_copySelection();
	parent->getModel()->_pasteSelection();
	
	value(0);
}

// handle deletion
void MainMenu::deleteCallback_real(Fl_Widget* w) {
	parent->getModel()->_deleteSelection();
	value(0);
}

// handle (un)grouping
void MainMenu::groupCallback_real(Fl_Widget* w) {
	printf("(un)grouped\n");
	value(0);
}

// handle (un)hiding
void MainMenu::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	value(0);
}

// handle save selection
void MainMenu::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	value(0);
}

// handle configure world
void MainMenu::configureWorldCallback_real(Fl_Widget* w) {
	// configure the new world
	WorldOptionsDialog* wod = new WorldOptionsDialog( parent->getModel()->_getWorldData(),
													  parent->getModel()->_getOptionsData(),
													  parent->getModel()->_getWaterLevelData());
	wod->show();
	
	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }
	
	// reset the world
	ObserverMessage obs(ObserverMessage::UPDATE_WORLD, parent->getModel()->_getWorldData() );
	parent->getModel()->notifyObservers( &obs );
	value(0);
}

// handle world weapon creation
void MainMenu::worldWeaponCallback_real(Fl_Widget* w) {
	printf("world weapon created\n");
	value(0);
}

// handle object configuration
void MainMenu::configureObjectCallback_real(Fl_Widget* w) {
	printf("configured an object\n");	
	value(0);
}

// handle teleporter linking
void MainMenu::linkCallback_real(Fl_Widget* w) {
	// get all selected objects
	Model::objRefList selection = parent->getModel()->_getSelection(); 
	if( selection.size() <= 0 )
		return;
		
	// map of teleporter links to create
	map< teleporter*, teleporter* > teleporterMap;
	for( Model::objRefList::iterator i = selection.begin(); i != selection.end(); i++ ) {
		if( (*i)->getHeader() == "teleporter" ) {
			teleporter* t1 = dynamic_cast< teleporter* > (i->get());
			if( !t1 )
				continue;
			
			// get other teleporters
			if( selection.size() >= 2 ) {
				for( Model::objRefList::iterator j = i+1; j != selection.end(); j++ ) {
						
					if( (*j)->getHeader() == "teleporter" ) {
						teleporter* t2 = dynamic_cast< teleporter* >( j->get() );
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
			parent->getModel()->_linkTeleporters( i->first, i->second );
		}
	}
	
	value(0);
}

#endif
