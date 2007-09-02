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
#include "commonControls.h"

void MenuBar::buildMenu(void) {
	
	add("File", 0, 0, 0, FL_SUBMENU);
		add("File/New...", FL_CTRL + 'n', new_world, this);
		add("File/Open...", FL_CTRL + 'o', open_world, this);
		add("File/Save", FL_CTRL + 's', save_world, this);
		add("File/Save As...", 0, save_world_as, this);
		add("File/Save Selection...", 0, save_selection, this, FL_MENU_DIVIDER);
		add("File/Exit", 0, exit_bzwb, this);
	
	add("Edit", 0, 0, 0, FL_SUBMENU);
		add("Edit/Undo", FL_CTRL + 'z', undo, this);
		add("Edit/Redo", FL_CTRL + 'y', redo, this, FL_MENU_DIVIDER);
		add("Edit/Cut", FL_CTRL + 'x', cut, this);
		add("Edit/Copy", FL_CTRL + 'c', copy, this);
		add("Edit/Paste", FL_CTRL + 'v', paste, this);
		add("Edit/Paste Saved Selection...", 0, paste_saved_selection, this);
		add("Edit/Duplicate", FL_CTRL + 'd', duplicate, this);
		add("Edit/Delete", FL_Delete, delete_callback, this, FL_MENU_DIVIDER);
		add("Edit/Select All", FL_CTRL + 'a', select_all, this);
		add("Edit/Unselect All", FL_CTRL + FL_ALT + 'a', unselect_all, this);
		
	add("Objects", 0, 0, 0, FL_SUBMENU);
		add("Objects/Add box", FL_CTRL+'b', addBoxCallback, this);
		add("Objects/Add pyramid", FL_CTRL+'p', addPyramidCallback, this);
		add("Objects/Add mesh Box", FL_CTRL+FL_ALT+'b', addMeshboxCallback, this);
		add("Objects/Add mesh pyramid", FL_CTRL+FL_ALT+'p', addMeshpyrCallback, this);
		add("Objects/Add teleporter", FL_CTRL+'t', addTeleporterCallback, this);
		add("Objects/Add sphere", 0, addSphereCallback, this);
		add("Objects/Add arc", 0, addArcCallback, this);
		add("Objects/Add mesh", FL_CTRL+'m', addMeshCallback, this);
		add("Objects/Add tetrahedron", 0, addTetrahedronCallback, this);
		add("Objects/Add torus", 0, addTorusCallback, this);
		add("Objects/Add cone", 0, addConeCallback, this);
		add("Objects/Add zone", 0, addZoneCallback, this);
		add("Objects/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			add("Objects/Add base/Add Purple Base", 0, addPurpleBaseCallback, this);
			add("Objects/Add base/Add Red Base", 0, addRedBaseCallback, this);
			add("Objects/Add base/Add Green Base", 0, addGreenBaseCallback, this);
			add("Objects/Add base/Add Blue Base", 0, addBlueBaseCallback, this);
			
	add("Scene", 0, 0, 0, FL_SUBMENU );
		add("Scene/Import object...", 0, importObjectCallback, this, FL_MENU_DIVIDER);
		
		add("Scene/(Un)Group Selection", FL_CTRL+'g', groupCallback, this);
		add("Scene/(Un)Hide Selection", FL_CTRL+'h', hideCallback, this, FL_MENU_DIVIDER);
		
		add("Scene/Configure World...", 0, configureWorldCallback, this);
		add("Scene/Configure Object...", FL_CTRL+'o', configureObjectCallback, this, FL_MENU_DIVIDER);
	
		add("Scene/Define World Weapon...", FL_CTRL+'w', worldWeaponCallback, this);
		add("Scene/Link Teleporters", 0, linkCallback, this);
}

// constructor
MenuBar::MenuBar( MainWindow* mw ) : Fl_Menu_Bar(0, 0, mw->w(), 30) {
	this->parent = mw;
	printf("MenuBar: parent mw addr: %x\n", parent);
	printf("MenuBar: parent mw model addr: %x\n", parent->getModel());
	this->buildMenu();	
}

void MenuBar::new_world_real( Fl_Widget* w ) {
	Model* model = this->parent->getModel();
	model->_newWorld();
	
	// configure the new world
	WorldOptionsDialog* wod = new WorldOptionsDialog( model->_getWorldData(),
													  model->_getOptionsData(),
													  model->_getWaterLevelData());
	wod->show();
	
	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }
	
	// reset the world
	ObserverMessage obs(ObserverMessage::UPDATE_WORLD, model->_getWorldData() );
	model->notifyObservers( &obs );
}

void MenuBar::open_world_real( Fl_Widget* w ) {

	string filename ;

	if (!callOpenFileDialog(filename,"*.bzw","share/","*.bzw","Open..."))
		return;

	/*Fl_File_Chooser* fc = new Fl_File_Chooser("share/", "*.bzw", Fl_File_Chooser::SINGLE, "Open..." );
	fc->value("*.bzw");
	fc->show();
	
	// wait for a value
	while( fc->shown() ) { Fl::wait(); }
	
	// get a value (the selected filename )
	if(fc->value() == NULL)
		return;
	
	string filename = fc->value(); */
	
	printf("file: %s\n", filename.c_str());
	
	// invoke BZWParser and load it
	vector<string> world = BZWParser::loadFile( filename.c_str() );
	
	// invoke the Model to build the world
	parent->getModel()->_build( world );
}

void MenuBar::save_world_real( Fl_Widget* w ) {
	// do a "save as..." if the world is yet unnamed
	if( parent->getWorldName().size() <= 0 ) {
		save_world_as_real( w );
		return;
	}
	
	// save the world
	string path = parent->getWorldName();
	
	// do a world save
	do_world_save( path.c_str() );
}

void MenuBar::save_world_as_real( Fl_Widget* w ) {
	
	string filename;

	if (!callSaveFileDialog(filename,parent->getWorldName().c_str(),"share/","*.bzw","Open..."))
		return;
	
	// set the world name
	parent->setWorldName( filename.c_str() );
	
	// save the world
	string path = parent->getWorldName();
	
	// do the world save
	do_world_save( path.c_str() ); 
}

// save the world
void MenuBar::do_world_save( const char* filename ) {
	
	ofstream fileOutput( filename );
	
	// if we can't open a new file (access permissions, etc), then bail
	if(!fileOutput.is_open()) {
		parent->error( TextUtils::format("Could not open %s for writing\n", filename).c_str() );
		return;	
	}
	
	string text = parent->getModel()->toString();
	
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
	parent->getModel()->_cutSelection();
}

void MenuBar::copy_real( Fl_Widget* w ) {
	parent->getModel()->_copySelection();
}

void MenuBar::paste_real( Fl_Widget* w ) {
	parent->getModel()->_pasteSelection();
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
	printf("adding a box\n");
	printf("model addr: %x\n", this->parent->getModel());
	// make a new box using the Model's object registry
	DataEntry* newBox = this->parent->getModel()->_buildObject( "box" );
	
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
void MenuBar::addPyramidCallback_real(Fl_Widget* w) {
	// make a new pyramid using the Model's object registry
	DataEntry* newPyr = this->parent->getModel()->_buildObject( "pyramid" );
	
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
void MenuBar::addMeshboxCallback_real(Fl_Widget* w) {
	printf("added a meshbox\n");
	value(0);
}

// add a mesh pyramid
void MenuBar::addMeshpyrCallback_real(Fl_Widget* w) {
	printf("added a mesh pyramid\n");
	value(0);
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
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	
	value(0);
}

// add a sphere
void MenuBar::addSphereCallback_real(Fl_Widget* w) {
	printf("added a sphere\n");
	value(0);
}

// add an arc
void MenuBar::addArcCallback_real(Fl_Widget* w) {
	printf("added an arc\n");
	value(0);
}

// add a mesh
void MenuBar::addMeshCallback_real(Fl_Widget* w) {
	printf("added a mesh\n");
	value(0);
}

// add a tetrahedron
void MenuBar::addTetrahedronCallback_real(Fl_Widget* w) {
	printf("added a tetrahedron\n");
	value(0);
}

// add a torus
void MenuBar::addTorusCallback_real(Fl_Widget* w) {
	printf("added a torus\n");
	value(0);
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
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );
	
	// open up a MasterConfigurationDialog and configure it
	parent->configure( newObj );
	
	
	value(0);
}

// add a zone
void MenuBar::addZoneCallback_real(Fl_Widget* w) {
	printf("added a zone\n");
	value(0);
}

// import an object
void MenuBar::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	value(0);
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

// handle selection clone
void MenuBar::duplicate_real(Fl_Widget* w) {
	parent->getModel()->_copySelection();
	parent->getModel()->_pasteSelection();
	
	value(0);
}

// handle deletion
void MenuBar::delete_real(Fl_Widget* w) {
	parent->getModel()->_deleteSelection();
	value(0);
}

// handle (un)grouping
void MenuBar::groupCallback_real(Fl_Widget* w) {
	// get the selection
	Model::objRefList objects = this->parent->getModel()->_getSelection();
	
	if( objects.size() < 0 )
		return;
		
	// only do an un-group if the only object selected is a group
	if( objects.size() == 1 && objects[0]->getHeader() == "group" )
		parent->getModel()->_ungroupObjects( dynamic_cast< group* > (objects[0].get()) );
	else {
		parent->getModel()->_groupObjects( objects );
	}
	
	value(0);
}

// handle (un)hiding
void MenuBar::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	value(0);
}

// handle save selection
void MenuBar::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	value(0);
}

// handle configure world
void MenuBar::configureWorldCallback_real(Fl_Widget* w) {
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
void MenuBar::worldWeaponCallback_real(Fl_Widget* w) {
	printf("world weapon created\n");
	value(0);
}

// handle object configuration
void MenuBar::configureObjectCallback_real(Fl_Widget* w) {
	printf("configured an object\n");	
	value(0);
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
			parent->getModel()->_linkTeleporters( i->first, i->second );
		}
	}
	
	value(0);
}
