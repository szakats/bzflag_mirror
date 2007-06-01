#ifndef MODEL_H_
#define MODEL_H_

class Model
{
public:
	Model();
	virtual ~Model();
	
	// every class the model stores must be derived from this:
	class DataEntry {
	public:
		virtual void getData(Model* m);
		virtual void updateData(Model* m);
		
		DataEntry() { }
		virtual ~DataEntry() { }
	};

	class WorldOptionsData : public Model::DataEntry {
		public:
			WorldOptionsData(char* worldName, char* optionsString, float waterLevel, float flagHeight) {
				update(worldName, optionsString, waterLevel, flagHeight);	
			}
			
			// send the data
			void get(char* worldName, char* optionsString, float& waterLevel, float& flagHeight) {
				worldName = this->worldName;
				optionsString = this->optionsString;
				waterLevel = this->waterLevel;
				flagHeight = this->flagHeight;	
			}
			
			// receive the data
			void update(char* worldName, char* optionsString, float waterLevel, float flagHeight) {
				this->worldName = worldName;
				this->optionsString = optionsString;
				this->waterLevel = waterLevel;
				this->flagHeight = flagHeight;	
			}
			
			// methods to implement from DataEntry:
			void getData(Model* h) {
					
			}
			
			void updateData(Model* h) {
				
			}
			
		private:
			char* worldName;
			char* optionsString;
			float waterLevel;
			float flagHeight;
	};
	
private:
// world options
	WorldOptionsData* worldOptionsData;
	
};

#endif /*MODEL_H_*/
