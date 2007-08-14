#ifndef MATERIALWIDGET_H_
#define MATERIALWIDGET_H_

#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Check_Button.H>

#include "../defines.h"

#include <vector>
#include <string>

using namespace std;

#define MaterialWidget_NONE "(none)"

/**
 * This is a widget that controls the selection of a material
 */
 
class MaterialWidget : public Fl_Group {

public:

	// needed for Fl_Group compatibility
	MaterialWidget( const MaterialWidget& m );
	
	// main constructor
	MaterialWidget( int x, int y, int width, int height, vector< string > materialChoices );
	
	// destructor
	virtual ~MaterialWidget() { }
	
	// callbacks
	static void getMaterialCallback( Fl_Widget* w, void* data ) {
		MaterialWidget* mw = (MaterialWidget*)data;
		mw->getMaterialCallback_real( w );
	}
	
	static void changeMaterialCallback( Fl_Widget* w, void* data ) {
		MaterialWidget* mw = (MaterialWidget*)data;
		Fl_Menu_Button* button = (Fl_Menu_Button*)w;
		mw->changeMaterialCallback_real( mw, button );
	}
	
	// getters
	vector< string > getMaterials() { return this->materials; }
	string getSelectedMaterial() { return string(this->materialMenu->label()); }
	bool isActive() { return activeButton->value() == 0 ? false : true; }
	
	// setters
	void setMaterials( vector< string>& newMaterials );
	void setSelectedMaterial( const string& material );
	void setActive( bool value ) { activeButton->value( value ? 1 : 0 ); }
	
private:
	
	// check box
	Fl_Check_Button* activeButton;
	
	// menu button of references
	Fl_Menu_Button* materialMenu;
	
	// the list of material choices
	vector< string > materials;
	
	// real callbacks
	void getMaterialCallback_real( Fl_Widget* w );
	void changeMaterialCallback_real( MaterialWidget* w, Fl_Menu_Button* button );
};

#endif /*MATERIALWIDGET_H_*/
