#include "../include/windows/ConsoleWindow.h"

Console* ConsoleWindow::console = NULL;
bool ConsoleWindow::initialized = false;

// constructor
ConsoleWindow::ConsoleWindow(int lineLimit) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench Messages") {
		
	this->end();
	
	console = new Console(10, 10, DEFAULT_WIDTH - 20, DEFAULT_HEIGHT - 20);
	this->add(console);
	
	initialized = true;
	
	console->setReadOnly(true);
}

// destructor
ConsoleWindow::~ConsoleWindow() {
	initialized = false;	
}

// outputs text to the console widget
void ConsoleWindow::output(const char* text, ...) {
	
	// borrowed from TextUtils.cxx
	va_list args;
    va_start(args, text);
    string result = TextUtils::vformat(text, args);
    va_end(args);

   
	if(!initialized) {
	    printf("%s", result.c_str());
	}
	else {
		console->add( result );
	}
}

