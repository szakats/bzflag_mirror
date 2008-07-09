#ifndef _WORLD_H_
#define _WORLD_H_

#include <map>
#include <vector>
#include <string>

typedef enum
{
	eShift,
	eShear,
	eSpin,
	eScale
}teAffineTransformType;

class AffineTransformation
{
public:
	teAffineTransformType type;
	float				  values[4];

	AffineTransformation()
	{
		type = eShift;
		values[0] = values[1] = values[2] = values[2] = 0;
	}
};

typedef std::vector<AffineTransformation> AffineTransformationStack;

typedef std::vector<int> tvItList;

typedef enum
{
	eMapItemHeader = 0,	// when you MUST be at the top
	eMapItemFirst,		// things that have to come before other thigns like world and vars
	eMapItemMaterials,	// Materials
	eMapItemDrivers,	// physics drivers
	eMapItemDefines,	// defines for groups
	eMapItemNormal,		// objects and group callouts
	eMapItemLinks,		// links into the object list
	eMapItemFooter		// stuff at the very end
}teMapItemPriority;

class World;

typedef std::vector<std::string> elementList;

class BaseDrawInfo
{
public:
	virtual ~BaseDrawInfo(){};
};

class MapItem
{
public:
	MapItem();
	virtual ~MapItem();

	virtual std::string getClass ( void );
	virtual std::string getName ( void ){return getElement(0);}
	
	int getID ( void );

	int getElementCount ( void );
	std::string getElement ( int id );

	void clearElements ( void );
	void addElement ( std::string element );

	void setID ( int _id ){id = _id;}

	virtual teMapItemPriority getPriority ( void ) {return eMapItemNormal;}

	void setWorld ( World* _world ) { world = _world;}

	AffineTransformationStack	transformStack;

	BaseDrawInfo	*drawInfo;

	int		id;
	std::string name;
	elementList	elements;

	World	*world;
};

class World
{
public:
	World();
	~World();

	tvItList getItemList ( void );

	int addItem ( MapItem *item );
	bool removeItem ( int itemID );

	MapItem* getItem ( int itemID );

	void clearItems ( void );

	tvItList findItemsOfClass ( std::string className );
	int findItemOfClassByName ( std::string className, std::string name );

	bool valid ( void ) {return items.size()>0;}
protected:

	std::map<int,MapItem*>	items;

	int lastID;

	int getNewID ( void ) { return ++lastID;}
};


// World item

class WorldItem : public MapItem
{
public:
	WorldItem () : MapItem() {size[0] = size[1] = size[2] = 0;}	
	virtual std::string getClass ( void ){return std::string("world");}

	float size[3];
};

// group item

class GroupItem : public MapItem
{
public:
	GroupItem() : MapItem(){};
	virtual ~GroupItem ( void );
	virtual std::string getClass ( void );
	virtual std::string getName ( void );

	int getSubItemCount ( void );
	MapItem* getSubItem ( int id );

	void clearSubItems( void );
	void addSubItem( MapItem *item );
	virtual teMapItemPriority getPriority ( void ) {return eMapItemDefines;}

protected:

	std::vector<MapItem*>	subItems;
};

// instance item

class InstanceItem : public MapItem
{
public:
	InstanceItem() : MapItem(){};
	virtual std::string getClass ( void );
	virtual std::string getName ( void );

	std::string define;
};

class BasePositional : public MapItem
{
public:
	float pos[3];
	float scale[3];
	float rot;

	bool driveThru;
	bool shootThru;

	virtual std::string getName ( void ){return name;}

	BasePositional()
	{
		pos[0] = pos[1] = pos[2] = 0;
		scale[0] = scale[1] = scale[2] = 0;
		rot = 0;
		driveThru = false;
		shootThru = false;
	}
};

// classic box item

class ClassicBoxItem : public BasePositional
{
public:
	virtual std::string getClass ( void ){return std::string("box");}
};

// classic pyramid item

class ClassicPyramidItem : public BasePositional
{
public:
	virtual std::string getClass ( void ){return std::string("pyramid");}

	bool flipedZ;

	ClassicPyramidItem() : BasePositional()
	{
		flipedZ = false;
	}
};

class CMaterialObject : public MapItem
{
public:
	virtual std::string getClass ( void ){return std::string("material");}
	virtual std::string getName ( void ){return name;}

	bool resetmat;
	
	float ambient[4];
	std::string ambientName;

	float diffuse[4];
	std::string diffuseName;

	float specular[4];
	std::string specularName;

	float emission[4];
	std::string emissionName;

	float shine;

	std::vector<std::string> textureStack;

	bool noTex;
	bool noTexAlpha;
	bool noTexColor;
	bool noRadar;
	bool noShadow;
	bool noSorting;
	bool noLighting;

	bool spheremap;

	std::string textureMatrix;
	std::string dynamicColor;

	float alphaThresh;
	bool groupAlpha;

	std::vector<int> occludedFaces;
};


#endif //_WORLD_H_
