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

#include "dialogs/AdvancedOptionsDialog.h"

#include "objects/bz2object.h"

// main constructor
AdvancedOptionsDialog::AdvancedOptionsDialog( bz2object* _obj ) :
	ConfigurationDialog( _obj, "Advanced Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	
	obj = _obj;
	
	begin();
	
	materialLabel = new QuickLabel("Materials (order matters)", 5, 5);
	materialNew = new Fl_Button(DEFAULT_WIDTH - 180, 5, 55, DEFAULT_TEXTSIZE + 6, "New" );
	materialEdit = new Fl_Button(DEFAULT_WIDTH - 120, 5, 55, DEFAULT_TEXTSIZE + 6, "Edit" );
	materialDelete = new Fl_Button( DEFAULT_WIDTH - 60, 5, 55, DEFAULT_TEXTSIZE + 6, "Delete");
	materialList = new Fl_Scroll( 5, 30, DEFAULT_WIDTH - 10, 120 );
	materialList->end();
	materialList->box(FL_UP_BOX);
	materialList->type(Fl_Scroll::VERTICAL_ALWAYS);
	
	// add materials
	// first, query the model for them
	vector<string> materialRefs = vector<string>();
	materialRefs.push_back( MaterialWidget_NONE );
	
	map< string, material* > materialMap = Model::getMaterials();
	if( materialMap.size() > 0 ) {
		for( map< string, material* >::iterator i = materialMap.begin(); i != materialMap.end(); i++ ) {
			materialRefs.push_back( i->first );
		}
	}
	
	// second, add the widgets
	materialWidgets = vector< MaterialWidget* >();
	vector< material* > objMaterials = obj->getMaterials();
	if( objMaterials.size() > 0 ) {
		for( vector< material* >::iterator i = objMaterials.begin(); i != objMaterials.end(); i++ ) {
			MaterialWidget* mw = new MaterialWidget( materialList->x() + 5, 0, materialList->w() - 10, 2 * DEFAULT_TEXTSIZE, materialRefs );
			mw->setSelectedMaterial( (*i)->getName() );
			addMaterialCallback_real( materialList, mw );
		}
	}
	
	phydrvLabel = new QuickLabel("Physics Driver", 5, 190 );
	phydrvNew = new Fl_Button( DEFAULT_WIDTH - 180, 190, 55, DEFAULT_TEXTSIZE + 6, "New" );
	phydrvEdit = new Fl_Button( DEFAULT_WIDTH - 120, 190, 55, DEFAULT_TEXTSIZE + 6, "Edit" );
	phydrvDelete = new Fl_Button( DEFAULT_WIDTH - 60, 190, 55, DEFAULT_TEXTSIZE + 6, "Delete" );
	phydrvMenu = new Fl_Menu_Button( 5, 215, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "(coming soon)" );
	
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
	end();
}

// OK callback
void AdvancedOptionsDialog::OKCallback_real( Fl_Widget* w ) {
	
}

// Cancel Callback
void AdvancedOptionsDialog::CancelCallback_real( Fl_Widget* w ) {
	Fl::delete_widget( this );
}

// add material callback
void AdvancedOptionsDialog::addMaterialCallback_real( Fl_Widget* w, MaterialWidget* mw ) {
	// add an existing material widget if one is passed
	if( mw != NULL ) {
		int x = materialList->x() + 5;
		int y = materialList->y() + 5 + 2 * DEFAULT_TEXTSIZE * ( materialList->children() - 1 );
		mw->position( x, y );
		materialList->add( mw );
	}
	else {		// open up the material configuration dialog and get a new material
		
	}
}
