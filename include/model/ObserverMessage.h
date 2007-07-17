#ifndef VIEWMESSAGE_H_
#define VIEWMESSAGE_H_

/**
 * This is a messaging class that the Model sends to the Observers.
 */
 
class ObserverMessage {

public:

	
	typedef enum ObserverMessageType {
		UPDATE_OBJECT = 1,	// update an object
		REMOVE_OBJECT,		// remove an object
		ADD_OBJECT,			// add an object
		UPDATE_WORLD,		// re-size the world
		UPDATE_WATERLEVEL	// alter the water level
	};
	
	ObserverMessageType type;
	void* data;
	
	ObserverMessage(ObserverMessageType type, void* data) { this->type = type; this->data = data; }
	
};

#endif /*VIEWMESSAGE_H_*/
