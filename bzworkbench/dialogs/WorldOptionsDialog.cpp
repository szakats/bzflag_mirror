#include "../include/WorldOptionsDialog.h"

// constructor
// The buttons and fields are initialized and placed here.
WorldOptionsDialog::WorldOptionsDialog() :
	Fl_Dialog("World Options (incomplete)", this->WIDTH, this->HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	this->worldData 		= (world*)Model::query("world");
	this->optionsData 		= (options*)Model::query("options");
	this->waterLevelData 	= (waterLevel*)Model::query("waterLevel");
	
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

WorldOptionsDialog::~WorldOptionsDialog() { 
	
}

// set all variables from the widget values when the OK button is pressed
void WorldOptionsDialog::OKButtonCallback_real(Fl_Widget* w) {
	unsigned char hasWater = waterCheckButton->value();
	
	float waterLevel;
	if(hasWater == 0)
		waterLevel = -1.0f;
	else
		waterLevel = waterLevelField->value();
		
	float flagHeight = this->flagHeightField->value();
	
	string worldName = string(this->worldNameField->value());
	
	string optionsString = string(this->worldOptionsField->value());
	
	char* waterMaterialName = NULL;
	
	float size = 400.0f;
	
	bool noWalls = false;
	
	string sizeString = string(ftoa(size));
	
	string flagHeightString = string(ftoa(flagHeight));
	
	string waterLevelString = string(ftoa(waterLevel));
	
	string worldStr = string("world\n") +
						 "  name " + worldName + "\n" +
						 "  size " + sizeString + "\n" +
						 "  flagHeight " + flagHeightString + "\n" +
							(noWalls == true ? "  noWalls\n" : "# noWalls\n") +
						 "end\n";
						  
	string optionsStr = string("options\n") +
						 "  " + optionsString + "\n" +
						 "end\n";
						  
	string waterLevelStr = string("waterLevel\n") +
						 "  name defaultWaterLevel\n" +
						 "  height " + waterLevelString + "\n" +
						 (waterMaterialName == NULL ? "# materials " : "  materials ") + (waterMaterialName == NULL ? " " : waterMaterialName) + "\n" +
						 "end\n";
				
	// printf("%s\n", data.c_str());
	this->worldData->update(worldStr);
	this->optionsData->update(optionsStr);
	this->waterLevelData->update(waterLevelStr);
	
	printf(this->worldData->toString().c_str());
	printf(this->optionsData->toString().c_str());
	printf(this->waterLevelData->toString().c_str());
	
	Fl::delete_widget(this);
}	

// close the window when the Cancel button is pressed
void WorldOptionsDialog::CancelButtonCallback_real(Fl_Widget* w) {

	Fl::delete_widget(this);
}
