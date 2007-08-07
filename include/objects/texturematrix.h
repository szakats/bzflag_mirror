#ifndef TEXTUREMATRIX_H_
#define TEXTUREMATRIX_H_

#include "../DataEntry.h"
#include "../model/BZWParser.h"
#include "../render/TexCoord2D.h"

class texturematrix : public DataEntry {

public:
	
	// default constructor
	texturematrix();
	
	// constructor with data
	texturematrix(string& data);
	
	static DataEntry* init() { return new texturematrix(); }
	static DataEntry* init(string& data) { return new texturematrix(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	// getters
	string& getName() { return this->name; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	
private:

	string name;
	TexCoord2D texFreq, texScale, texShift, texCenter, texFixedScale, texFixedShift;
	float spin, fixedSpin;
	
};


#endif /*TEXTUREMATRIX_H_*/
