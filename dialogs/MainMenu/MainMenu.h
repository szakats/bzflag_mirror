#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <stdlib.h>
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu_Button.H>

#include "../../defines.h"

class MainMenu : public Fl_Menu_Button
{
public:
	
	// constructor and destructor
	MainMenu(int x, int y, int width, int height);
	virtual ~MainMenu();
	
	// callbacks
	static void addBoxCallback(Fl_Widget* w, void* data) { 
		MainMenu* m = (MainMenu*)data;
		m->addBoxCallback_real(w);
	}
	
	static void addPyramidCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addPyramidCallback_real(w);
	}
	
	static void addTeleporterCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addTeleporterCallback_real(w);
	}
	
	static void addSphereCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addSphereCallback_real(w);
	}
	
	static void addArcCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addArcCallback_real(w);
	}
	
	static void addMeshCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addMeshCallback_real(w);
	}
	
	static void addTetrahedronCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addTetrahedronCallback_real(w);	
	}
	
	static void addTorusCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addTorusCallback_real(w);
	}
	
	static void addConeCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addConeCallback_real(w);
	}
	
	static void addZoneCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addZoneCallback_real(w);
	}
	
	static void importObjectCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->importObjectCallback_real(w);
	}
	
	static void addPurpleBaseCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addPurpleBaseCallback_real(w);	
	}
	
	static void addRedBaseCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addRedBaseCallback_real(w);	
	}
	
	static void addGreenBaseCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addGreenBaseCallback_real(w);	
	}
	
	static void addBlueBaseCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->addBlueBaseCallback_real(w);	
	}
	
	static void cutCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->cutCallback_real(w);	
	}
	
	static void copyCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->copyCallback_real(w);
	}
	
	static void pasteCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->pasteCallback_real(w);	
	}
	
	static void cloneCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->cloneCallback_real(w);	
	}
	
	static void deleteCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->deleteCallback_real(w);
	}
	
	static void groupCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->groupCallback_real(w);	
	}
	
	static void hideCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->hideCallback_real(w);	
	}
	
	static void saveSelectionCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->saveSelectionCallback_real(w);	
	}
	
	static void configureWorldCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->configureWorldCallback_real(w);	
	}
	
	static void worldWeaponCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->worldWeaponCallback_real(w);	
	}
	
	static void configureObjectCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->configureObjectCallback_real(w);	
	}
	
	static void linkCallback(Fl_Widget* w, void* data) {
		MainMenu* m = (MainMenu*)data;
		m->linkCallback_real(w);
	}
	
	
private:

	// a constructor helper
	void buildMenu(void);
	
	// real callbacks
	void addBoxCallback_real(Fl_Widget* w);
	void addPyramidCallback_real(Fl_Widget* w);
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
	void cutCallback_real(Fl_Widget* w);
	void copyCallback_real(Fl_Widget* w);
	void pasteCallback_real(Fl_Widget* w);
	void cloneCallback_real(Fl_Widget* w);
	void deleteCallback_real(Fl_Widget* w);
	void groupCallback_real(Fl_Widget* w);
	void hideCallback_real(Fl_Widget* w);
	void saveSelectionCallback_real(Fl_Widget* w);
	void configureWorldCallback_real(Fl_Widget* w);
	void worldWeaponCallback_real(Fl_Widget* w);
	void configureObjectCallback_real(Fl_Widget* w);
	void linkCallback_real(Fl_Widget* w);
};

#endif /*MAINMENU_H_*/
