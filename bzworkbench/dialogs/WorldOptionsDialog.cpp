#include "../include/dialogs/WorldOptionsDialog.h"

// helper method--concat strings from a vector of strings
string concat(vector<string> strings) {
	string ret = string("");
	for(vector<string>::iterator i = strings.begin(); i != strings.end(); i++) {
		ret += *i + " ";	
	}	
	return ret;
}

// constructor
// The buttons and fields are initialized and placed here.
WorldOptionsDialog::WorldOptionsDialog( world* theWorld, options* theOptions, waterLevel* theWaterLevel ) :
	Fl_Dialog("World Options", WIDTH, HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	this->worldData 		= theWorld;
	this->optionsData 		= theOptions;
	this->waterLevelData 	= theWaterLevel;
	
	// parse the values
	string nameStr = worldData->getName();
	float size = worldData->getSize();
	float flagHeight = worldData->getFlagHeight();
	bool noWalls = !worldData->hasWalls();
	
	string options = optionsData->getOptionsString();
	
	float waterHeight = waterLevelData->getHeight();
	string waterMaterial = waterLevelData->getMaterial();		
		
	// initialize widgets
	worldNameLabel = new QuickLabel("Name:", 5, 5);
	worldNameField = new Fl_Input(100, 5, 200, DEFAULT_TEXTSIZE + 6);
	worldNameField->value(nameStr.c_str());
	
	worldSizeLabel = new QuickLabel("Size:", 5, 35);
	worldSizeField = new Fl_Counter(120, 35, 120, DEFAULT_TEXTSIZE + 6);
	worldSizeField->value(size);
	
	flagHeightLabel = new QuickLabel("Flag height:", 5, 65);
	flagHeightField = new Fl_Counter(120, 65, 120, DEFAULT_TEXTSIZE + 6);
	flagHeightField->type(FL_NORMAL_COUNTER);
	flagHeightField->value(flagHeight);
	
	waterCheckButton = new Fl_Check_Button(5, 95, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6, "Water level");
	waterCheckButton->value( waterHeight >= 0.0f ? true : false );
	waterLevelField = new Fl_Counter(120, 95, 120, DEFAULT_TEXTSIZE + 6);
	waterLevelField->value(waterHeight);
	
	worldOptionsLabel = new QuickLabel("Options:", 5, 125);
	worldOptionsField = new Fl_Input(100, 125, 200, DEFAULT_TEXTSIZE + 6);
	worldOptionsField->value(options.c_str());
	
	waterTextureLabel = new QuickLabel("Water Material:", 5, 155);
	waterTextureField = new Fl_Input(120, 155, 180, DEFAULT_TEXTSIZE + 6);
	waterTextureField->value(waterMaterial.c_str());
	
	noWallsCheckButton = new Fl_Check_Button(5, 185, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6, "No Walls");
	noWallsCheckButton->value(noWalls);
	
	// add widgets
	this->add(worldNameLabel);
	this->add(worldNameField);
	
	this->add(worldSizeLabel);
	this->add(worldSizeField);
	
	this->add(flagHeightLabel);
	this->add(flagHeightField);
	
	this->add(waterCheckButton);
	this->add(waterLevelField);
	
	this->add(worldOptionsLabel);
	this->add(worldOptionsField);
	
	this->add(waterTextureLabel);
	this->add(waterTextureField);
	
	this->add(noWallsCheckButton);
	
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
	
	string waterMaterialString = string(this->waterTextureField->value());
	
	float size = this->worldSizeField->value();
	
	bool noWalls = (noWallsCheckButton->value() != 0);
	
	worldData->setName( worldName.c_str() );
	worldData->setSize( size );
	worldData->setWalls( !noWalls );
	worldData->setFlagHeight(flagHeight);
	
	optionsData->setOptionsString(optionsString);
	
	waterLevelData->setHeight(waterLevel);
	waterLevelData->setMaterial(waterMaterialString);
	
	Fl::delete_widget(this);
}	

// close the window when the Cancel button is pressed
void WorldOptionsDialog::CancelButtonCallback_real(Fl_Widget* w) {

	Fl::delete_widget(this);
}
