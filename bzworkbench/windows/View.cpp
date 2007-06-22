#include "../include/windows/View.h"

// view constructor
View::View(Model* m, int x, int y, int w, int h, const char *label) :
	RenderWindow(x,y,w,h) {
	
	// set OSG viewport
    this->getCamera()->setViewport(new osg::Viewport(0,0,w,h));
    
    // get the graphics context
    this->getCamera()->setGraphicsContext(this->getGraphicsWindow());
    
    // do single-threaded view
   this->setThreadingModel(osgViewer::Viewer::SingleThreaded);
   
   // set the model
   this->model = m;
}

// draw method (really simple)
void View::draw(void) {
	this->frame();	
}
