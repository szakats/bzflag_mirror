#ifndef MESHBOX_H_
#define MESHBOX_H_

#include "bz2object.h"

#include <string>
#include <vector>

using namespace std;

class meshbox : public bz2object {

public:
	// construct an empty box
	meshbox();
	
	// construct a box from data
	meshbox(string& data);
	
	static DataEntry* init() { return new meshbox(); }
	static DataEntry* init(string& data) { return new meshbox(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// render
	int render(void);

private:
	vector<string> topMaterials, outsideMaterials;
};

#endif /*MESHBOX_H_*/
