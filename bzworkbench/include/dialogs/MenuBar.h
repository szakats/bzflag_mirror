#ifndef MENUBAR_H_
#define MENUBAR_H_

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Bar.H>

#include <FL/Fl_File_Chooser.H>

#include "../model/BZWParser.h"
#include "../model/Model.h"

class MainWindow;

/**
 * This is BZWB's menu bar class
 */
 
class MenuBar : public Fl_Menu_Bar {

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
	void paste_real( Fl_Widget* w );
	void paste_saved_selection_real( Fl_Widget* w );
	void select_all_real( Fl_Widget* w );
	void unselect_all_real( Fl_Widget* w );
	
	// reference to the MainWindow parent
	MainWindow* parent;
	
	// build the menu
	void buildMenu(void);
	
};

#include "../windows/MainWindow.h"

#endif /*MENUBAR_H_*/
