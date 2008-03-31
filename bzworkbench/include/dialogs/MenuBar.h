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

#ifndef MENUBAR_H_
#define MENUBAR_H_

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Bar.H>

#include <FL/Fl_File_Chooser.H>

#include <FL/filename.H>

#include <iostream>
#include <fstream>

#include "../model/BZWParser.h"
#include "../model/Model.h"

#include "../dialogs/Fl_Error.h"

class MainWindow;

using namespace std;
/**
 * This is BZWB's menu bar class
 */
 
class MenuBar : public Fl_Menu_Bar {
	
	friend class MainWindow;		// let MainWindow use MenuBar's private callbacks for object creation, etc

public:
	// constructor and destructor
	MenuBar( MainWindow* parent );
	~MenuBar() { }
	
	// static callbacks
	static void new_world( Fl_Widget* w, void* data ) {
		MenuBar* mb = (MenuBar*)(data);
		mb->new_world_real( w );	
	}
	
	static void open_world( Fl_Widget* w, void* data ) {
		MenuBar* mb = (MenuBar*)(data);
		mb->open_world_real( w );	
	}
	
	static void save_world( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->save_world_real( w );
	}
	
	static void save_world_as( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->save_world_as_real( w );
	}
	
	static void save_selection( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->save_selection_real( w );
	}
	
	static void exit_bzwb( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->exit_bzwb_real( w );
	}
	
	static void undo( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->undo_real( w );
	}
	
	static void redo( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->redo_real( w );
	}
	
	static void cut( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->cut_real( w );
	}
	
	static void copy( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->copy_real( w );
	}
	
	static void paste( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->paste_real( w );
	}
	
	static void duplicate( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->duplicate_real( w );
	}
	
	static void delete_callback( Fl_Widget* w, void* data ) {
		MenuBar* mb = (MenuBar*)(data);
		mb->delete_real( w );
	}
	
	static void paste_saved_selection( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->paste_saved_selection_real( w );
	}
	
	static void select_all( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->select_all_real( w );
	}
	
	static void unselect_all( Fl_Widget* w, void* data) {
		MenuBar* mb = (MenuBar*)(data);
		mb->unselect_all_real( w );
	}
	
	static void addBoxCallback(Fl_Widget* w, void* data) { 
		MenuBar* m = (MenuBar*)data;
		m->addBoxCallback_real(w);
	}
	
	static void addPyramidCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addPyramidCallback_real(w);
	}
	
	static void addMeshboxCallback(Fl_Widget* w, void* data) { 
		MenuBar* m = (MenuBar*)data;
		m->addMeshboxCallback_real(w);
	}
	
	static void addMeshpyrCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addMeshpyrCallback_real(w);
	}
	
	static void addTeleporterCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addTeleporterCallback_real(w);
	}
	
	static void addSphereCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addSphereCallback_real(w);
	}
	
	static void addArcCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addArcCallback_real(w);
	}
	
	static void addMeshCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addMeshCallback_real(w);
	}
	
	static void addTetrahedronCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addTetrahedronCallback_real(w);	
	}
	
	static void addTorusCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addTorusCallback_real(w);
	}
	
	static void addConeCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addConeCallback_real(w);
	}
	
	static void addZoneCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addZoneCallback_real(w);
	}
	
	static void importObjectCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->importObjectCallback_real(w);
	}
	
	static void addPurpleBaseCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addPurpleBaseCallback_real(w);	
	}
	
	static void addRedBaseCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addRedBaseCallback_real(w);	
	}
	
	static void addGreenBaseCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addGreenBaseCallback_real(w);	
	}
	
	static void addBlueBaseCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->addBlueBaseCallback_real(w);	
	}
	
	static void groupCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->groupCallback_real(w);	
	}
	
	static void hideCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->hideCallback_real(w);	
	}
	
	static void saveSelectionCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->saveSelectionCallback_real(w);	
	}
	
	static void configureWorldCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->configureWorldCallback_real(w);	
	}
	
	static void worldWeaponCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->worldWeaponCallback_real(w);	
	}
	
	static void configureObjectCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->configureObjectCallback_real(w);	
	}
	
	static void linkCallback(Fl_Widget* w, void* data) {
		MenuBar* m = (MenuBar*)data;
		m->linkCallback_real(w);
	}
	
	// do a world save
	void do_world_save( const char* filename );
	
private:

	// instantiated callbacks
	void new_world_real( Fl_Widget* w );
	void open_world_real( Fl_Widget* w );
	void save_world_real( Fl_Widget* w );
	void save_world_as_real( Fl_Widget* w );
	void save_selection_real( Fl_Widget* w );
	void exit_bzwb_real( Fl_Widget* w );
	
	void undo_real(Fl_Widget* w );
	void redo_real( Fl_Widget* w );
	void cut_real( Fl_Widget* w );
	void copy_real( Fl_Widget* w );
	void duplicate_real( Fl_Widget* w );
	void delete_real( Fl_Widget* w );
	void paste_real( Fl_Widget* w );
	void paste_saved_selection_real( Fl_Widget* w );
	void select_all_real( Fl_Widget* w );
	void unselect_all_real( Fl_Widget* w );
	
	void addBoxCallback_real(Fl_Widget* w);
	void addPyramidCallback_real(Fl_Widget* w);
	void addMeshboxCallback_real(Fl_Widget* w);
	void addMeshpyrCallback_real(Fl_Widget* w);
	void addTeleporterCallback_real(Fl_Widget* w);
	void addSphereCallback_real(Fl_Widget* w);
	void addArcCallback_real(Fl_Widget* w);
	void addMeshCallback_real(Fl_Widget* w);
	void addTetrahedronCallback_real(Fl_Widget* w);
	void addTorusCallback_real(Fl_Widget* w);
	void addConeCallback_real(Fl_Widget* w);
	void addZoneCallback_real(Fl_Widget* w);
	void importObjectCallback_real(Fl_Widget* w);
	void addPurpleBaseCallback_real(Fl_Widget* w);
	void addRedBaseCallback_real(Fl_Widget* w);
	void addGreenBaseCallback_real(Fl_Widget* w);
	void addBlueBaseCallback_real(Fl_Widget* w);
	
	void groupCallback_real(Fl_Widget* w);
	void hideCallback_real(Fl_Widget* w);
	void saveSelectionCallback_real(Fl_Widget* w);
	void configureWorldCallback_real(Fl_Widget* w);
	void worldWeaponCallback_real(Fl_Widget* w);
	void configureObjectCallback_real(Fl_Widget* w);
	void linkCallback_real(Fl_Widget* w);
	
	// reference to the MainWindow parent
	MainWindow* parent;
	
	// build the menu
	void buildMenu(void);
	
};

#include "../windows/MainWindow.h"

#endif /*MENUBAR_H_*/
