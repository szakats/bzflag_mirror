#include "../include/Fl_Error.h"

Fl_Error::Fl_Error(const char* message) :
	Fl_Dialog("BZWorkbench Error", DEFAULT_WIDTH, 200, Fl_Dialog::Fl_OK) {
		
	this->errorImage = new Fl_XPM_Image("share/error.xpm");
	
	this->errorMessage = new Fl_Multiline_Output(60, 20, DEFAULT_WIDTH - 60 - 10, 200 - 30 - 10, "An error has occurred:");
	this->errorMessage->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
	this->errorMessage->value(message);
	
	this->errorLabel = new QuickLabel("", 5, 100 - 24);
	this->errorLabel->image(errorImage);
	
	this->add(errorLabel);
	this->add(errorMessage);
	
	this->setOKEventHandler(Fl_ErrorCallback, this);
	
	this->show();
}

Fl_Error::~Fl_Error() {
	if(this->errorImage)
		delete errorImage;
}

void Fl_Error::setText(const char* message) { this->errorMessage->value(message); }

// default callbacks
void Fl_Error::Fl_ErrorCallback_real(Fl_Widget* w) {
	Fl::delete_widget(this);
}
