#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <string>
#include <vector>

#include "DataEntry.h"
#include "BZWParser.h"

using namespace std;

// a metadata entry that is used to describe a transformation such as spin, shear, shift, and scale
class Transform : public DataEntry {
	
	public:
	
		Transform() : DataEntry("", "") {
			name = string("");
			data = vector<float>();
		}
		
		Transform(string& data) : DataEntry("", "") {
			name = string("");
			this->data = vector<float>();
			this->update(data);
		}
		
		// getter
		string get(void) {
			return this->toString();	
		}
		
		// setter
		void update(string& newData) {
			// expect just one line
			this->name = BZWParser::key(newData.c_str());
			
			// break up the line
			vector<string> elements = BZWParser::getLineElements(newData.c_str());
			
			// erase the current data
			data.clear();
			
			// get the numbers (don't start with elements.begin(), because this is what name is)
			for(vector<string>::iterator i = elements.begin() + 1; i != elements.end(); i++) {
				data.push_back( atof( i->c_str() ) );
			}
		}
		
		// toString
		string toString(void) {
			string ret = string(name);
			for(vector<float>::iterator i = data.begin(); i != data.end(); i++) {
				ret += " " + string(ftoa(*i));
			}
			
			return ret + "\n";
		}
		
		// data getters
		string getName() { return name; }
		vector<float> getData() { return data; }
		
		// data setters
		void setName(string& s) { this->name = s; }
		void setData(vector<float>& data) { this->data = data; }
		
		// render
		int render(void) { return 0; }
	
	private:
		
		string name;
		vector<float> data;
		vector<string> transformationFormats;
};

#endif /*TRANSFORM_H_*/
