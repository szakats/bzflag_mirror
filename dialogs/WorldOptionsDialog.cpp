#include "../include/WorldOptionsDialog.h"

// constructor
// The buttons and fields are initialized and placed here.
WorldOptionsDialog::WorldOptionsDialog() :
	Fl_Dialog("World Options", WorldOptionsDialog::WIDTH, WorldOptionsDialog::HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	this->worldName = NULL;
	this->optionString = NULL;
	this->flagHeight = 1.0;
	this->waterLevel = -1.0;
	
	this->updateData();
	
	// initialize widgets
	worldNameLabel = new QuickLabel("Name:", 5, 5);
	worldNameField = new Fl_Input(100, 5, 200, DEFAULT_TEXTSIZE + 6);
	
	flagHeightLabel = new QuickLabel("Flag height:", 5, 45);
	flagHeightField = new Fl_Counter(120, 45, 120, DEFAULT_TEXTSIZE + 6);
	flagHeightField->type(FL_NORMAL_COUNTER);
	
	waterCheckButton = new Fl_Check_Button(5, 85, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6, "Water level");
	waterLevelField = new Fl_Counter(120, 85, 120, DEFAULT_TEXTSIZE + 6);
	
	worldOptionsLabel = new QuickLabel("Options:", 5, 125);
	worldOptionsField = new Fl_Input(100, 125, 200, DEFAULT_TEXTSIZE + 6);
	
	// add widgets
	this->add(worldNameLabel);
	this->add(worldNameField);
	
	this->add(flagHeightLabel);
	this->add(flagHeightField);
	
	this->add(waterCheckButton);
	this->add(waterLevelField);
	
	this->add(worldOptionsLabel);
	this->add(worldOptionsField);
	
	// set event handlers
	this->setOKEventHandler(OKButtonCallback, this);
	this->setCancelEventHandler(CancelButtonCallback, this);
}

WorldOptionsDialog::~WorldOptionsDialog() { }

// set all variables from the widget values when the OK button is pressed
void WorldOptionsDialog::OKButtonCallback_real(Fl_Widget* w) {
	unsigned char hasWater = waterCheckButton->value();
	
	if(hasWater == 0)
		this->waterLevel = -1.0f;
	else
		this->waterLevel = (float)waterLevelField->value();
		
	this->flagHeight = (float)this->flagHeightField->value();
	
	this->worldName = (char*)this->worldNameField->value();
	
	this->optionString = (char*)this->worldOptionsField->value();
	
	this->updateData();
	
	Fl::delete_widget(this);
}	

// close the window when the Cancel button is pressed
void WorldOptionsDialog::CancelButtonCallback_real(Fl_Widget* w) {

	Fl::delete_widget(this);
}
