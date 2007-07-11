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
WorldOptionsDialog::WorldOptionsDialog() :
	Fl_Dialog("World Options", WIDTH, HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	this->worldData 		= Model::getWorldData();
	this->optionsData 		= Model::getOptionsData();
	this->waterLevelData 	= Model::getWaterLevelData();
	
	// get the values
	string worldString = worldData->get();
	string optionsString = optionsData->get();
	string waterLevelString = waterLevelData->get();
	
	// parse the values
	string nameStr = BZWParser::getValuesByKey("name", "world", worldString.c_str())[0];
	float size = atof( BZWParser::getValuesByKey("size", "world", worldString.c_str())[0].c_str() );
	float flagHeight = atof( BZWParser::getValuesByKey("flagHeight", "world", worldString.c_str())[0].c_str() );
	bool noWalls = ( BZWParser::getValuesByKey("noWalls", "world", worldString.c_str()).size() == 0 ? false : true);
	
	string options = concat( BZWParser::getLines("options", BZWParser::getSectionsByHeader("options", optionsString.c_str())[0].c_str() ) );
	
	float waterHeight = atof( BZWParser::getValuesByKey("height", "waterLevel", waterLevelString.c_str())[0].c_str() );
	vector<string> _materials = BZWParser::getValuesByKey("material", "waterLevel", waterLevelString.c_str());
	
	string waterMaterial;
	if(_materials.size() == 0)
		waterMaterial = string("");
	else
		waterMaterial = _materials[0];
		
		
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
						 (waterMaterialString.length() != 0 ? "  material " + waterMaterialString : "# material") + "\n" +
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
