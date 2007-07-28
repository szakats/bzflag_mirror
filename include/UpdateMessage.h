#ifndef UPDATEMESSAGE_H_
#define UPDATEMESSAGE_H_

/**
 * This is a simple message class that can be sent to objects in their update() method.
 * 
 * NOTE: the intended use of this messaging class is to inform an object that it was changed externally.
 * It's the job of the update() method to interpret the message and carry out any additional changes
 * specific to the object itself.
 */
 
class UpdateMessage {
	
public:

	// some common message types
	enum UpdateMessageType {
		SET_POSITION,				// i.e. use this to indicate a new position
		SET_POSITION_FACTOR,		// i.e. use this to indicate a translation
		SET_ROTATION,				// i.e. use this to indicate a new rotation
		SET_ROTATION_FACTOR,		// i.e. use this to indicate a change in rotation
		SET_SCALE,					// i.e. use this to indicate a new scale
		SET_SCALE_FACTOR			// i.e. use this to indicate a change in scale
	};

	int type;
	void* data;
	
	UpdateMessage( int type, void* data) {
		this->type = type;
		this->data = data;
	}
};

#endif /*UPDATEMESSAGE_H_*/
