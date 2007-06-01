#ifndef DATAENTRY_H_
#define DATAENTRY_H_

// every class the model stores must be derived from this:
class DataEntry {
public:
	virtual void getData(Model* m) = 0;
	virtual void updateData(Model* m) = 0;
	
	DataEntry() { 
	
	}
	virtual ~DataEntry() { 
	
	}
};

#endif /*DATAENTRY_H_*/
