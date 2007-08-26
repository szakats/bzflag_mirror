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

#include "dialogs/MenuBar.h"
#include "model/Model.h"

void MenuBar::buildMenu(void) {
	
	this->add("File", 0, 0, 0, FL_SUBMENU);
		this->add("File/New...", FL_CTRL + 'n', this->new_world, this);
		this->add("File/Open...", FL_CTRL + 'o', this->open_world, this);
		this->add("File/Save", FL_CTRL + 's', this->save_world, this);
		this->add("File/Save As...", 0, this->save_world_as, this);
		this->add("File/Save Selection...", 0, this->save_selection, this, FL_MENU_DIVIDER);
		this->add("File/Exit", 0, this->exit_bzwb, this);
	
	this->add("Edit", 0, 0, 0, FL_SUBMENU);
		this->add("Edit/Undo", FL_CTRL + 'z', this->undo, this);
		this->add("Edit/Redo", FL_CTRL + 'y', this->redo, this, FL_MENU_DIVIDER);
		this->add("Edit/Cut", FL_CTRL + 'x', this->cut, this);
		this->add("Edit/Copy", FL_CTRL + 'c', this->copy, this);
		this->add("Edit/Paste", FL_CTRL + 'v', this->paste, this);
		this->add("Edit/Paste Saved Selection...", 0, this->paste_saved_selection, this);
		this->add("Edit/Duplicate", FL_CTRL + 'd', this->duplicate, this);
		this->add("Edit/Delete", FL_Delete, this->delete_callback, this, FL_MENU_DIVIDER);
		this->add("Edit/Select All", FL_CTRL + 'a', this->select_all, this);
		this->add("Edit/Unselect All", FL_CTRL + FL_ALT + 'a', this->unselect_all, this);
		
	this->add("Objects", 0, 0, 0, FL_SUBMENU);
		this->add("Objects/Add box", FL_CTRL+'b', this->addBoxCallback, this);
		this->add("Objects/Add pyramid", FL_CTRL+'p', this->addPyramidCallback, this);
		this->add("Objects/Add mesh Box", FL_CTRL+FL_ALT+'b', this->addMeshboxCallback, this);
		this->add("Objects/Add mesh pyramid", FL_CTRL+FL_ALT+'p', this->addMeshpyrCallback, this);
		this->add("Objects/Add teleporter", FL_CTRL+'t', this->addTeleporterCallback, this);
		this->add("Objects/Add sphere", 0, this->addSphereCallback, this);
		this->add("Objects/Add arc", 0, this->addArcCallback, this);
		this->add("Objects/Add mesh", FL_CTRL+'m', this->addMeshCallback, this);
		this->add("Objects/Add tetrahedron", 0, this->addTetrahedronCallback, this);
		this->add("Objects/Add torus", 0, this->addTorusCallback, this);
		this->add("Objects/Add cone", 0, this->addConeCallback, this);
		this->add("Objects/Add zone", 0, this->addZoneCallback, this);
		this->add("Objects/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			this->add("Objects/Add base/Add Purple Base", 0, this->addPurpleBaseCallback, this);
			this->add("Objects/Add base/Add Red Base", 0, this->addRedBaseCallback, this);
			this->add("Objects/Add base/Add Green Base", 0, this->addGreenBaseCallback, this);
			this->add("Objects/Add base/Add Blue Base", 0, this->addBlueBaseCallback, this);
			
	this->add("Scene", 0, 0, 0, FL_SUBMENU );
		this->add("Scene/Import object...", 0, this->importObjectCallback, this, FL_MENU_DIVIDER);
		
		this->add("Scene/(Un)Group Selection", FL_CTRL+'g', this->groupCallback, this);
		this->add("Scene/(Un)Hide Selection", FL_CTRL+'h', this->hideCallback, this, FL_MENU_DIVIDER);
		
		this->add("Scene/Configure World...", 0, this->configureWorldCallback, this);
		this->add("Scene/Configure Object...", FL_CTRL+'o', this->configureObjectCallback, this, FL_MENU_DIVIDER);
	
		this->add("Scene/Define World Weapon...", FL_CTRL+'w', this->worldWeaponCallback, this);
		this->add("Scene/Link Teleporters", 0, this->linkCallback, this);
}

// constructor
MenuBar::MenuBar( MainWindow* mw ) : Fl_Menu_Bar(0, 0, mw->w(), 30) {
	this->parent = mw;
	
	this->buildMenu();	
}

void MenuBar::new_world_real( Fl_Widget* w ) {
	this->parent->getModel()->_newWorld();
	
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
}

void MenuBar::open_world_real( Fl_Widget* w ) {
	Fl_File_Chooser* fc = new Fl_File_Chooser("share/", "*.bzw", Fl_File_Chooser::SINGLE, "Open..." );
	fc->value("*.bzw");
	fc->show();
	
	// wait for a value
	while( fc->shown() ) { Fl::wait(); }
	
	// get a value (the selected filename )
	if(fc->value() == NULL)
		return;
	
	string filename = fc->value();
	
	printf("file: %s\n", filename.c_str());
	
	// invoke BZWParser and load it
	vector<string> world = BZWParser::loadFile( filename.c_str() );
	
	// invoke the Model to build the world
	this->parent->getModel()->_build( world );
}

void MenuBar::save_world_real( Fl_Widget* w ) {
	// do a "save as..." if the world is yet unnamed
	if( this->parent->getWorldName().size() <= 0 ) {
		this->save_world_as_real( w );
		return;
	}
	
	// save the world
	string path = this->parent->getWorldName();
	
	// do a world save
	this->do_world_save( path.c_str() );
}

void MenuBar::save_world_as_real( Fl_Widget* w ) {
	Fl_File_Chooser* fc = new Fl_File_Chooser("share/", "*.bzw", Fl_File_Chooser::CREATE, "Save As..." );
	fc->show();
	
	// wait for a value
	while( fc->shown() ) { Fl::wait(); }
	
	if(fc->value() == NULL)
		return;
	
	// get a value (the file path)
	string filename = fc->value();
	
	// set the world name
	this->parent->setWorldName( filename.c_str() );
	
	// save the world
	string path = this->parent->getWorldName();
	
	// do the world save
	this->do_world_save( path.c_str() ); 
}

// save the world
void MenuBar::do_world_save( const char* filename ) {
	
	ofstream fileOutput( filename );
	
	// if we can't open a new file (access permissions, etc), then bail
	if(!fileOutput.is_open()) {
		this->parent->error( TextUtils::format("Could not open %s for writing\n", filename).c_str() );
		return;	
	}
	
	string text = this->parent->getModel()->toString();
	
	fileOutput.write( text.c_str(), text.size() );
	fileOutput.close();
	
}

void MenuBar::save_selection_real( Fl_Widget* w ) {
	
}

void MenuBar::exit_bzwb_real( Fl_Widget* w ) {
	
}

void MenuBar::undo_real( Fl_Widget* w ) {
	
}

void MenuBar::redo_real( Fl_Widget* w ) {
	
}

void MenuBar::cut_real( Fl_Widget* w ) {
	this->parent->getModel()->_cutSelection();
}

void MenuBar::copy_real( Fl_Widget* w ) {
	this->parent->getModel()->_copySelection();
}

void MenuBar::paste_real( Fl_Widget* w ) {
	this->parent->getModel()->_pasteSelection();
}

void MenuBar::paste_saved_selection_real( Fl_Widget* w ) {
	
}

void MenuBar::select_all_real( Fl_Widget* w ) {
	
}

void MenuBar::unselect_all_real( Fl_Widget* w ) {
	
}

// add a box
void MenuBar::addBoxCallback_real(Fl_Widget* w) {
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
void MenuBar::addPyramidCallback_real(Fl_Widget* w) {
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
void MenuBar::addMeshboxCallback_real(Fl_Widget* w) {
	printf("added a meshbox\n");
	this->value(0);
}

// add a mesh pyramid
void MenuBar::addMeshpyrCallback_real(Fl_Widget* w) {
	printf("added a mesh pyramid\n");
	this->value(0);
}

// add a teleporter
void MenuBar::addTeleporterCallback_real(Fl_Widget* w) {
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
void MenuBar::addSphereCallback_real(Fl_Widget* w) {
	printf("added a sphere\n");
	this->value(0);
}

// add an arc
void MenuBar::addArcCallback_real(Fl_Widget* w) {
	printf("added an arc\n");
	this->value(0);
}

// add a mesh
void MenuBar::addMeshCallback_real(Fl_Widget* w) {
	printf("added a mesh\n");
	this->value(0);
}

// add a tetrahedron
void MenuBar::addTetrahedronCallback_real(Fl_Widget* w) {
	printf("added a tetrahedron\n");
	this->value(0);
}

// add a torus
void MenuBar::addTorusCallback_real(Fl_Widget* w) {
	printf("added a torus\n");
	this->value(0);
}

// add a cone
void MenuBar::addConeCallback_real(Fl_Widget* w) {
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
void MenuBar::addZoneCallback_real(Fl_Widget* w) {
	printf("added a zone\n");
	this->value(0);
}

// import an object
void MenuBar::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	this->value(0);
}

// add base 1
void MenuBar::addPurpleBaseCallback_real(Fl_Widget* w) {
	
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = this->parent->getModel()->_getObjects();
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
void MenuBar::addRedBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = this->parent->getModel()->_getObjects();
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
void MenuBar::addGreenBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = this->parent->getModel()->_getObjects();
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
void MenuBar::addBlueBaseCallback_real(Fl_Widget* w) {
	// get the objects and see that we don't have any other bases
	Model::objRefList objects = this->parent->getModel()->_getObjects();
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

// handle selection clone
void MenuBar::duplicate_real(Fl_Widget* w) {
	this->parent->getModel()->_copySelection();
	this->parent->getModel()->_pasteSelection();
	
	this->value(0);
}

// handle deletion
void MenuBar::delete_real(Fl_Widget* w) {
	this->parent->getModel()->_deleteSelection();
	this->value(0);
}

// handle (un)grouping
void MenuBar::groupCallback_real(Fl_Widget* w) {
	// get the selection
	Model::objRefList objects = this->parent->getModel()->_getSelection();
	
	if( objects.size() < 0 )
		return;
		
	// only do an un-group if the only object selected is a group
	if( objects.size() == 1 && objects[0]->getHeader() == "group" )
		this->parent->getModel()->_ungroupObjects( dynamic_cast< group* > (objects[0].get()) );
	else {
		this->parent->getModel()->_groupObjects( objects );
	}
	
	this->value(0);
}

// handle (un)hiding
void MenuBar::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	this->value(0);
}

// handle save selection
void MenuBar::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	this->value(0);
}

// handle configure world
void MenuBar::configureWorldCallback_real(Fl_Widget* w) {
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
void MenuBar::worldWeaponCallback_real(Fl_Widget* w) {
	printf("world weapon created\n");
	this->value(0);
}

// handle object configuration
void MenuBar::configureObjectCallback_real(Fl_Widget* w) {
	printf("configured an object\n");	
	this->value(0);
}

// handle teleporter linking
void MenuBar::linkCallback_real(Fl_Widget* w) {
	// get all selected objects
	Model::objRefList selection = this->parent->getModel()->_getSelection(); 
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
			this->parent->getModel()->_linkTeleporters( i->first, i->second );
		}
	}
	
	this->value(0);
}
