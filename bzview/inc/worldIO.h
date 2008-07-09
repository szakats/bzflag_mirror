#ifndef _WORLD_IO_H_
#define _WORLD_IO_H_

#include "world.h"
#include "OSFile.h"

class WorldParser
{
public:
	WorldParser(World &_world);

	bool read ( const std::string &filePath );

protected:
	World &world;

	std::vector<std::string> lines;

	bool isEnd ( size_t i );
	bool isItem ( size_t i );
	bool isDefine ( size_t i );
	bool isEndDefine ( size_t i );

	std::string getItemType ( size_t i );

	bool toNextLine ( size_t &i);

	std::vector<std::string> readItem ( size_t &i );

	MapItem* readMapItem ( std::string &classname, std::vector<std::string> &item );

	// object specific stuff
	MapItem* readBox ( std::vector<std::string> &item );
	MapItem* readWorld ( std::vector<std::string> &item );
	MapItem* readGroup ( std::vector<std::string> &item );
	MapItem* readPyramid ( std::vector<std::string> &item );

	std::string parseDefineName ( std::string &line );

	bool positionalField ( std::string &key, std::vector<std::string> &nubs, MapItem* item );
	bool affineTransformField ( std::string &key, std::vector<std::string> &nubs, MapItem* item );

};

class TextWorldConstructor
{
public:
	TextWorldConstructor(World &_world);

	bool write ( std::string filePath );

protected:

	void writeObject ( MapItem *item );

	// object specific stuff
	void writeClassicPosObject ( MapItem *item );
	void writeClassicPyramid ( MapItem *item );
	void writeWorld ( MapItem *item );
	void writeGroup ( MapItem *item );
	void writeInstance ( MapItem *item );

	void writeAffineTransforms ( AffineTransformationStack &transforms );
	void writeElements ( MapItem *item );

	void indent ( void );
	void unindent ( void );
	void writeIndents ( void );

	void writeStdPositionalFileds ( MapItem *item );

	World &world;

	std::string fileText;

	int indentLevel;
};


#endif //_WORLD_IO_H_
