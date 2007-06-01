#include "../include/WorldOptionsDialog.h"

// constructor
// The buttons and fields are initialized and placed here.
WorldOptionsDialog::WorldOptionsDialog() :
	Fl_Dialog("World Options", this->WIDTH, this->HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	this->data = new WorldOptionsData();
	
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
	
	float waterLevel;
	if(hasWater == 0)
		waterLevel = -1.0f;
	else
		waterLevel = waterLevelField->value();
		
	float flagHeight = this->flagHeightField->value();
	
	char* worldName = (char*)this->worldNameField->value();
	
	char* optionsString = (char*)this->worldOptionsField->value();
	
	char* waterMaterialName = NULL;
	
	float size = 400.0f;
	
	bool noWalls = false;
	
	string sizeString = string(ftoa(size));
	
	string data = string("world\n") +
						 "  name " + worldName + "\n" +
						 "  size " + sizeString + "\n";/* +
						 "  flagHeight" + ftoa(flagHeight) + "\n" +
							(noWalls == true ? "noWalls\n" : "# noWalls\n") +
						 "end\n\n" +
						  
						 "options\n" +
						 "  " + optionsString + "\n" +
						 "end\n\n" +
						  
						 "waterLevel\n" +
						 "  name defaultWaterLevel\n" +
						 "  height " + ftoa(waterLevel) + "\n" +
						 "  materials " + waterMaterialName + "\n" +
						 "end\n";*/
						 
	printf("%s\n", data.c_str());
	
	Fl::delete_widget(this);
}	

// close the window when the Cancel button is pressed
void WorldOptionsDialog::CancelButtonCallback_real(Fl_Widget* w) {

	Fl::delete_widget(this);
}
