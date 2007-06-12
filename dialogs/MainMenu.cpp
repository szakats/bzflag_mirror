#include "../include/dialogs/MainMenu.h"
#include "../include/windows/MainWindow.h"

// builds the menu widgets
void MainMenu::buildMenu(void) {
	
	this->add("Add Object", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
		this->add("Add Object/Add box", FL_CTRL+'b', this->addBoxCallback, this);
		this->add("Add Object/Add pyramid", FL_CTRL+'p', this->addPyramidCallback, this);
		this->add("Add Object/Add mesh Box", FL_CTRL+FL_ALT+'b', this->addMeshboxCallback, this);
		this->add("Add Object/Add mesh pyramid", FL_CTRL+FL_ALT+'p', this->addMeshpyrCallback, this);
		this->add("Add Object/Add teleporter", FL_CTRL+'t', this->addTeleporterCallback, this);
		this->add("Add Object/Add sphere", 0, this->addSphereCallback, this);
		this->add("Add Object/Add arc", 0, this->addArcCallback, this);
		this->add("Add Object/Add mesh", FL_CTRL+'m', this->addMeshCallback, this);
		this->add("Add Object/Add tetrahedron", 0, this->addTetrahedronCallback, this);
		this->add("Add Object/Add torus", 0, this->addTorusCallback, this);
		this->add("Add Object/Add cone", 0, this->addConeCallback, this);
		this->add("Add Object/Add zone", 0, this->addZoneCallback, this);
		this->add("Add Object/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			this->add("Add Object/Add base/Add Purple Base", 0, this->addPurpleBaseCallback, this);
			this->add("Add Object/Add base/Add Red Base", 0, this->addRedBaseCallback, this);
			this->add("Add Object/Add base/Add Green Base", 0, this->addGreenBaseCallback, this);
			this->add("Add Object/Add base/Add Blue Base", 0, this->addBlueBaseCallback, this);
		this->add("Add Object/Import object...", 0, this->importObjectCallback, this);
	
	this->add("Cut", FL_CTRL+'x', this->cutCallback, this);
	this->add("Copy", FL_CTRL+'c', this->copyCallback, this);
	this->add("Paste", FL_CTRL+'v', this->pasteCallback, this);
	this->add("Duplicate", FL_CTRL+'d', this->cloneCallback, this);
	this->add("Delete", FL_Delete, this->deleteCallback, this);
	
	this->add("(Un)Group Selection", FL_CTRL+'g', this->groupCallback, this);
	this->add("(Un)Hide Selection", FL_CTRL+'h', this->hideCallback, this);
	this->add("Save Selection As...", 0, this->saveSelectionCallback, this, FL_MENU_DIVIDER);
	
	this->add("Configure World...", 0, this->configureWorldCallback, this);
	this->add("Configure Object...", FL_CTRL+'o', this->configureObjectCallback, this, FL_MENU_DIVIDER);
	
	this->add("Define World Weapon...", FL_CTRL+'w', this->worldWeaponCallback, this);
	this->add("Link Teleporters", 0, this->linkCallback, this);
	
}


// construct a menu at a given point
MainMenu::MainMenu(int x, int y, int width, int height) :
	Fl_Menu_Button(x, y, width, height) {
	
	this->buildMenu();
	
	this->type(Fl_Menu_Button::POPUP3);
}

// main destructor
MainMenu::~MainMenu() { }

// callbacks

// add a box
void MainMenu::addBoxCallback_real(Fl_Widget* w) {
	printf("added a box\n");
	this->value(0);
}

// add a pyramid
void MainMenu::addPyramidCallback_real(Fl_Widget* w) {
	printf("added a pyramid\n");
	this->value(0);
}

// add a meshbox
void MainMenu::addMeshboxCallback_real(Fl_Widget* w) {
	printf("added a meshbox\n");
	this->value(0);
}

// add a mesh pyramid
void MainMenu::addMeshpyrCallback_real(Fl_Widget* w) {
	printf("added a mesh pyramid\n");
	this->value(0);
}

// add a teleporter
void MainMenu::addTeleporterCallback_real(Fl_Widget* w) {
	printf("added a teleporter\n");
	this->value(0);
}

// add a sphere
void MainMenu::addSphereCallback_real(Fl_Widget* w) {
	printf("added a sphere\n");
	this->value(0);
}

// add an arc
void MainMenu::addArcCallback_real(Fl_Widget* w) {
	printf("added an arc\n");
	this->value(0);
}

// add a mesh
void MainMenu::addMeshCallback_real(Fl_Widget* w) {
	printf("added a mesh\n");
	this->value(0);
}

// add a tetrahedron
void MainMenu::addTetrahedronCallback_real(Fl_Widget* w) {
	printf("added a tetrahedron\n");
	this->value(0);
}

// add a torus
void MainMenu::addTorusCallback_real(Fl_Widget* w) {
	printf("added a torus\n");
	this->value(0);
}

// add a cone
void MainMenu::addConeCallback_real(Fl_Widget* w) {
	printf("added a cone\n");
	this->value(0);
}

// add a zone
void MainMenu::addZoneCallback_real(Fl_Widget* w) {
	printf("added a zone\n");
	this->value(0);
}

// import an object
void MainMenu::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	this->value(0);
}

// add base 1
void MainMenu::addPurpleBaseCallback_real(Fl_Widget* w) {
	printf("added purple base\n");
	this->value(0);
}

// add base 2
void MainMenu::addRedBaseCallback_real(Fl_Widget* w) {
	printf("added red base\n");
	this->value(0);
}

// add base 3
void MainMenu::addGreenBaseCallback_real(Fl_Widget* w) {
	printf("added green base\n");
	this->value(0);
}

// add base 4
void MainMenu::addBlueBaseCallback_real(Fl_Widget* w) {
	printf("added blue base\n");
	this->value(0);
}

// handle cut
void MainMenu::cutCallback_real(Fl_Widget* w) {
	printf("did a cut\n");
	this->value(0);
}

// handle copy
void MainMenu::copyCallback_real(Fl_Widget* w) {
	printf("did a copy\n");
	this->value(0);
}

// handle paste
void MainMenu::pasteCallback_real(Fl_Widget* w) {
	printf("did a paste\n");
	this->value(0);
}

// handle selection clone
void MainMenu::cloneCallback_real(Fl_Widget* w) {
	printf("did a clone\n");
	this->value(0);
}

// handle deletion
void MainMenu::deleteCallback_real(Fl_Widget* w) {
	printf("deletion!\n");
	this->value(0);
}

// handle (un)grouping
void MainMenu::groupCallback_real(Fl_Widget* w) {
	printf("(un)grouped\n");
	this->value(0);
}

// handle (un)hiding
void MainMenu::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	this->value(0);
}

// handle save selection
void MainMenu::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	this->value(0);
}

// handle configure world
void MainMenu::configureWorldCallback_real(Fl_Widget* w) {
	MainWindow::openDialog("WorldOptionsDialog");
	printf("configured world\n");
	this->value(0);
}

// handle world weapon creation
void MainMenu::worldWeaponCallback_real(Fl_Widget* w) {
	printf("world weapon created\n");
	this->value(0);
}

// handle object configuration
void MainMenu::configureObjectCallback_real(Fl_Widget* w) {
	printf("configured an object\n");	
	this->value(0);
}

// handle teleporter linking
void MainMenu::linkCallback_real(Fl_Widget* w) {
	printf("linkage\n");
	this->value(0);
}
