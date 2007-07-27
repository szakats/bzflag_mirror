#ifndef UPDATEMESSAGE_H_
#define UPDATEMESSAGE_H_

/**
 * This is a simple message class that can be sent to objects in their update() method.
 */
 
class UpdateMessage {
	
public:

	// some common message types
	enum UpdateMessageType {
		SET_POSITION,
		SET_ROTATION,
		SET_SCALE,
		SET_SCALE_FACTOR
	};

	int type;
	void* data;
	
	UpdateMessage( int type, void* data) {
		this->type = type;
		this->data = data;
	}
};

#endif /*UPDATEMESSAGE_H_*/
