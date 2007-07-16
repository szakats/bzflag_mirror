#include "../include/dialogs/MenuBar.h"

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
		this->add("Edit/Paste Saved Selection...", 0, this->paste_saved_selection, this, FL_MENU_DIVIDER);
		this->add("Edit/Select All", FL_CTRL + 'a', this->select_all, this);
		this->add("Edit/Unselect All", FL_CTRL + FL_ALT + 'a', this->unselect_all, this);
}

// constructor
MenuBar::MenuBar( MainWindow* mw ) : Fl_Menu_Bar(0, 0, mw->w(), 30) {
	this->parent = mw;
	
	this->buildMenu();	
}

void MenuBar::new_world_real( Fl_Widget* w ) {
	
}

void MenuBar::open_world_real( Fl_Widget* w ) {
	
}

void MenuBar::save_world_real( Fl_Widget* w ) {
	
}

void MenuBar::save_world_as_real( Fl_Widget* w ) {
	
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
