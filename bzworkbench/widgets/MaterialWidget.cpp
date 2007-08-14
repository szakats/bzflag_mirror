#include "../include/widgets/MaterialWidget.h"

// copy constructor (does nothing )
MaterialWidget::MaterialWidget( const MaterialWidget& mat ) : Fl_Group( 0, 0, 0, 0 ) { this->end(); }

// main constructor
MaterialWidget::MaterialWidget( int x, int y, int width, int height, vector< string > materialChoices ) :
	Fl_Group( x, y, width, height ) {
		
	this->end();
	
	this->activeButton = new Fl_Check_Button(x, y, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6);
	this->materialMenu = new Fl_Menu_Button(x + 30, y, width - 50, DEFAULT_TEXTSIZE + 12);
	
	this->setMaterials( materialChoices );
	
	this->add( activeButton );
	this->add( materialMenu );
}

// set the current material
void MaterialWidget::setSelectedMaterial( const string& material ) {
	if( materials.size() > 0 ) {
		unsigned int i = 0;
		for( vector<string>::iterator itr = materials.begin(); itr != materials.end(); i++, itr++ ) {
			if( *itr == material ) {
				const Fl_Menu_Item* menu = materialMenu->menu();
				materialMenu->label( menu[ i ].label() );
				this->materialMenu->redraw();
				return;
			}
		}
	}
	else {
		materialMenu->copy_label( MaterialWidget_NONE );
	}
}

// set materials
void MaterialWidget::setMaterials( vector< string >& materialChoices ) {
	// set the new material array, making sure to add "(none)" as an option
	this->materials = materialChoices;
	materials.push_back( MaterialWidget_NONE );
	
	// erase the current menu
	materialMenu->menu(NULL);
	
	// rebuild the menu button options
	for( vector< string >::iterator i = materials.begin(); i != materials.end(); i++ ) {
		materialMenu->add( i->c_str(), 0, changeMaterialCallback, 0 );
	}
}

// handle a change in the menu (i.e. just relabel it)
void MaterialWidget::changeMaterialCallback_real( MaterialWidget* mw, Fl_Menu_Button* mb ) {
	const Fl_Menu_Item* menu = mb->menu();
	mb->label(menu[mb->value()].label());
	mb->redraw();
}
