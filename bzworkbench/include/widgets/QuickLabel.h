#ifndef QUICKLABEL_H_
#define QUICKLABEL_H_

#include <stdlib.h>
#include <memory.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

// A quick'n'dirty label widget (easier than making an Fl_box, clearing its box, and giving it a label
// all the freakin' time).
class QuickLabel : public Fl_Box
{
public:
	
	// constructor
	QuickLabel(const char* labelText, int x, int y);
	
	// destructor
	virtual ~QuickLabel();
};

#endif /*QUICKLABEL_H_*/
