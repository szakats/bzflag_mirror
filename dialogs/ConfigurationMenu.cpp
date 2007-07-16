#include "../include/dialogs/ConfigurationMenu.h"

// build the menu
void ConfigurationMenu::buildMenu(void) {
	
	this->add("Translate", 0, 0, 0, FL_SUBMENU);
		this->add("Translate/Along X", 0, this->translate_x, this);
		this->add("Translate/Along Y", 0, this->translate_y, this);
		this->add("Translate/Along Z", 0, this->translate_z, this);
	this->add("Rotate", 0, 0, 0, FL_SUBMENU);
		this->add("Rotate/Around X", 0, this->rotate_x, this);
		this->add("Rotate/Around Y", 0, this->rotate_y, this);
		this->add("Rotate/Around Z", 0, this->rotate_z, this);
	this->add("Scale", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
		this->add("Scale/Along X", 0, this->scale_x, this);
		this->add("Scale/Along Y", 0, this->scale_y, this);
		this->add("Scale/Along Z", 0, this->scale_z, this);
	
	this->add("Tweak...", 0, this->tweak, this);
}

// constructor...nothing atypical here
ConfigurationMenu::ConfigurationMenu( MainWindow* mw, int x, int y, int width, int height ) :
	Fl_Menu_Button( x, y, width, height ) {
		
	this->buildMenu();
	
	this->type(Fl_Menu_Button::POPUP3);
	
	this->parent = mw;
	
	this->obj = NULL;
}

// instantiated callbacks
void ConfigurationMenu::translate_x_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::translate_x_real() needs to be implemented");
}

void ConfigurationMenu::translate_y_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::translate_y_real() needs to be implemented");
}

void ConfigurationMenu::translate_z_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::translate_z_real() needs to be implemented");
}

void ConfigurationMenu::rotate_x_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::rotate_x_real() needs to be implemented");
}

void ConfigurationMenu::rotate_y_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::rotate_y_real() needs to be implemented");
}

void ConfigurationMenu::rotate_z_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::rotate_z_real() needs to be implemented");
}

void ConfigurationMenu::scale_x_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::scale_x_real() needs to be implemented");
}

void ConfigurationMenu::scale_y_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::scale_x_real() needs to be implemented");
}

void ConfigurationMenu::scale_z_real( Fl_Widget* w ) {
	parent->error("ConfigurationMenu::scale_x_real() needs to be implemented");
}

void ConfigurationMenu::tweak_real( Fl_Widget* w ) {
	MasterConfigurationDialog* mcd = new MasterConfigurationDialog( obj );
	mcd->show();
}
