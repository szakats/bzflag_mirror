#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "../model/Model.h"

#include "../dialogs/MainMenu.h"
#include "../dialogs/WorldOptionsDialog.h"


class MainWindow : public Fl_Window
{
public:

	static const int DEFAULT_WIDTH = 800;
	static const int DEFAULT_HEIGHT = 600;
	
	MainWindow();
	MainWindow(Model* model);
	virtual ~MainWindow();
	
	static void openDialog(const char* dialog);
	
private:
	
	// reference to the main menu
	MainMenu* mainMenu;
	
	// reference to the data model
	Model* model;
};

#endif /*MAINWINDOW_H_*/
