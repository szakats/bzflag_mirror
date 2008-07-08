
#include "world.h"
#include "utils.h"


//-------MapItem----------
// base class for all map items

MapItem::MapItem()
{
	world = NULL;
	id = 0;
	drawInfo = NULL;
}

MapItem::~MapItem()
{
	if ( drawInfo )
		delete(drawInfo);
}

std::string MapItem::getClass ( void )
{
	return std::string("custom");
}

int MapItem::getID ( void )
{
	return id;
}

int MapItem::getElementCount ( void )
{
	return (int)elements.size();
}

std::string MapItem::getElement ( int elementID )
{
	if ( elementID < 0 || elementID >= getElementCount() )
		return std::string("");
	return elements[elementID];
}

void MapItem::clearElements ( void )
{
	elements.clear();
}

void MapItem::addElement ( std::string element )
{
	elements.push_back(element);
}

//-------World----------
// container class for all items in the world
// dosn't know how to load,save, or draw

World::World()
{
	lastID = 0;
}

World::~World()
{
	clearItems();
}

tvItList World::getItemList ( void )
{
	tvItList itemList;

	std::map<int,MapItem*>::iterator itr = items.begin();
	while ( itr != items.end() )
	{
		itemList.push_back(itr->second->getID());
		itr++;
	}
	return itemList;
}

int World::addItem ( MapItem *item )
{
	if (!item)
		return -1;

	item->setID(getNewID());
	items[item->getID()] = item;
	return item->getID();
}

bool World::removeItem ( int itemID )
{
	std::map<int,MapItem*>::iterator itr = items.find(itemID);
	
	if ( itr == items.end() )
		return false;
	
	items.erase(itr);
	return true;
}

MapItem* World::getItem ( int itemID )
{
	std::map<int,MapItem*>::iterator itr = items.find(itemID);

	if ( itr == items.end() )
		return NULL;

	return (itr->second);
}

void World::clearItems ( void )
{
	std::map<int,MapItem*>::iterator itr = items.begin();
	while ( itr != items.end() )
	{
		delete(itr->second);
		itr++;
	}
	items.clear();
	lastID = 0;
}

tvItList World::findItemsOfClass ( std::string className )
{
	tvItList itemList;

	std::map<int,MapItem*>::iterator itr = items.begin();
	while ( itr != items.end() )
	{
		if (itr->second->getClass() == className)
			itemList.push_back(itr->second->getID());
		itr++;
	}

	return itemList;
}

int World::findItemOfClassByName ( std::string className, std::string name )
{
	tvItList itemList = findItemsOfClass( className );

	for ( unsigned int i = 0; i < itemList.size(); i++ )
	{
		MapItem* item = getItem (itemList[i]);
		if (item)
		{
			if ( item->getName() == name )
				return item->getID();
		}
	}
	return -1;
}

// group item
// contains other sub items.

GroupItem::~GroupItem( void )
{
	for ( int i = 0; i < getSubItemCount(); i++ )
		delete(subItems[i]);

	subItems.clear();
}

std::string GroupItem::getClass ( void )
{
	return std::string("group");
}

std::string GroupItem::getName ( void )
{
	return getElement(0);
}

int GroupItem::getSubItemCount ( void )
{
	return (int)subItems.size();
}

MapItem* GroupItem::getSubItem ( int id )
{
	if ( id < 0 || id >= getSubItemCount() )
		return NULL;

	return subItems[id];
}

void GroupItem::clearSubItems( void )
{
	subItems.clear();
}

void GroupItem::addSubItem( MapItem *item )
{
	subItems.push_back(item);
}

// instance item

std::string InstanceItem::getClass ( void )
{
	return std::string("instance");
}

std::string InstanceItem::getName ( void )
{
	return name;
}
	


