
#include "worldIO.h"
#include "utils.h"

std::string trimWhitespace ( std::string line )
{
	std::string temp = line.c_str()+firstVisible(line);

	int count = 0;
	std::string::reverse_iterator itr = temp.rbegin();
	while ( itr != temp.rend())
	{
		if ( isWhitespace(*itr) )
		{
			count++;
			itr++;
		}
		else
			itr = temp.rend();
	}
	temp.erase(temp.end()-count,temp.end());
	return temp;
}

//-------WorldParser----------
// class to fill out a world from a data file
WorldParser::WorldParser(World &_world) : world(_world)
{
}

bool WorldParser::read ( const std::string &filePath )
{
	COSFile	file(filePath.c_str());

	if (!file.Open("rb"))
		return false;

	std::string fileText;
	if (!file.GetFileText(fileText))
	{
		file.Close();
		return false;
	}

	std::string temp;

	// strip the shizzle if it's windows, and then tok that sucker up
	fileText = replace_all(fileText,std::string("\r"),std::string(""));
	lines = tokenize(fileText,std::string("\n"),0,false);

	bool inObject = false;

	size_t lineCount = lines.size();
	for ( size_t i = 0; i < lineCount; i++ )
	{
		if ( toNextLine(i) )
		{
			temp = lines[i];
			if ( isItem(i) )
			{
				std::string itemType;
				tolower(getItemType(i),itemType);

				std::vector<std::string> item = readItem(i);

				MapItem	*mapItem = readMapItem(itemType,item);

				if (mapItem)
					world.addItem(mapItem);
			}
			else if ( isDefine(i) )
			{
				GroupItem	*group = new GroupItem;
				group->setWorld(&world);

				group->addElement(parseDefineName(lines[i])); // the group name
			
				temp = lines[i];

				if (toNextLine(++i))
				{
					bool done = false;

					while( !done )
					{
						std::string itemType;
						tolower(getItemType(i),itemType);

						std::vector<std::string> item = readItem(i);

						MapItem	*mapItem = readMapItem(itemType,item);

						if (mapItem)
							group->addSubItem(mapItem);

						if ( i < lineCount )
						{
							if(!toNextLine(++i))
								done = true;
							else if(isEndDefine(i))
								done = true;
						}
						else
							done = true;	
					}

				}
				world.addItem(group);
			}
			else
				i++;
		}
	}
	return true;
}

std::string WorldParser::parseDefineName ( std::string &line )
{
	std::string name;

	std::vector<std::string> nubs = tokenize(trimWhitespace(line),std::string(" "),0,false);
	if ( nubs.size() < 2)
		return name;
	return nubs[1];
}

bool WorldParser::isEnd ( size_t i )
{
	return compare_nocase(trimWhitespace(lines[i]),"END") == 0;
}


bool WorldParser::isItem ( size_t i )
{
	if ( isEnd(i) || isDefine(i) )
		return false;
	return true;
}

std::string  WorldParser::getItemType (size_t i )
{
	std::vector<std::string> nubs = tokenize(trimWhitespace(lines[i]),std::string(" "));
	if ( !nubs.size() )
		return std::string("");

	return nubs[0];
}

bool WorldParser::isDefine ( size_t i )
{
	if ( i >= lines.size() )
		return false;

	std::vector<std::string> nubs = tokenize(trimWhitespace(lines[i]),std::string(" "));
	if ( !nubs.size() )
		return false;

	return compare_nocase(nubs[0],"DEFINE") == 0;
}

bool WorldParser::isEndDefine ( size_t i )
{
	if ( i >= lines.size() )
		return false;

	return compare_nocase(trimWhitespace(lines[i]),"ENDDEF") == 0;
}

bool WorldParser::toNextLine ( size_t &i )
{
	bool done = false;

	while ( !done )
	{
		if ( i >= lines.size() )
			return false;

		std::string line = lines[i];
		line = trimWhitespace(line);
		size_t lineSize = line.size();

		if (lineSize)
		{
			if ( (line.c_str()[0] != '#') )
				done = true;
			else
				i++;
		}
		else
			i++;
	}
	return i < lines.size();
}

std::vector<std::string> WorldParser::readItem ( size_t &i )
{
	std::vector<std::string> item;

	while ( i < lines.size() && !isEnd(i) )
	{
		std::string line = lines[i];

		line = trimWhitespace(line);

		if (line.size() && line.c_str()[0] != '#' )
			item.push_back(line);
		i++;
	}
	//if (i < lines.size())
	//	i++;

	return item;
}

MapItem* WorldParser::readMapItem ( std::string &classname, std::vector<std::string> &item )
{
	MapItem* mapItem = NULL;

	if ( classname == "box" )
		mapItem = readBox(item);
	else if ( classname == "pyramid" )
		mapItem = readPyramid(item);
	else if ( classname == "world" )
		mapItem = readWorld(item);
	else if ( classname == "group" )
		mapItem = readGroup(item);
	else
	{	
		// the default, just make a custom object
		mapItem = new MapItem;
		mapItem->setWorld(&world);
		for ( unsigned int j = 0; j < item.size(); j++ )
			mapItem->addElement(item[j]);
	}

	return mapItem;
}

bool WorldParser::positionalField ( std::string &key, std::vector<std::string> &nubs, MapItem* item )
{
	BasePositional *posItem = (BasePositional*)item;

	if ( key == "name" )
	{
		posItem->name = nubs[1];
		return true;
	}
	else if ( key == "pos" || key == "position" )
	{
		if ( nubs.size() > 1)
			posItem->pos[0] = (float)atof(nubs[1].c_str());
		if ( nubs.size() > 2)
			posItem->pos[1] = (float)atof(nubs[2].c_str());
		if ( nubs.size() > 3)
			posItem->pos[2] = (float)atof(nubs[3].c_str());
		return true;

	}
	else if ( key == "rot" || key == "rotation" )
	{
		if ( nubs.size() > 1)
			posItem->rot = (float)atof(nubs[1].c_str());
		return true;
	}
	else if ( key == "scale" || key == "size" )
	{
		if ( nubs.size() > 1)
			posItem->scale[0] = (float)atof(nubs[1].c_str());
		if ( nubs.size() > 2)
			posItem->scale[1] = (float)atof(nubs[2].c_str());
		if ( nubs.size() > 3)
			posItem->scale[2] = (float)atof(nubs[3].c_str());
		return true;
	}
	else if ( key == "drivethrough" )
	{
		posItem->driveThru = true;
		return true;
	}
	else if ( key == "shootthrough" )
	{
		posItem->shootThru = true;
		return true;
	}
	else if ( key == "passable" )
	{
		posItem->driveThru = posItem->shootThru = true;
		return true;
	}
	return false;
}

bool WorldParser::affineTransformField ( std::string &key, std::vector<std::string> &nubs, MapItem* item )
{
	if ( key == "shift")
	{
		AffineTransformation transform;
		transform.type = eShift;

		if (nubs.size() > 1)
			transform.values[0] = (float)atof(nubs[1].c_str());
		if (nubs.size() > 2)
			transform.values[1] = (float)atof(nubs[2].c_str());
		if (nubs.size() > 3)
			transform.values[2] = (float)atof(nubs[3].c_str());		

		item->transformStack.push_back(transform);
		return true;
	}
	else if ( key == "spin" )
	{
		AffineTransformation transform;
		transform.type = eSpin;

		if (nubs.size() > 1)
			transform.values[0] = (float)atof(nubs[1].c_str());
		if (nubs.size() > 2)
			transform.values[1] = (float)atof(nubs[2].c_str());
		if (nubs.size() > 3)
			transform.values[2] = (float)atof(nubs[3].c_str());		
		if (nubs.size() > 4)
			transform.values[3] = (float)atof(nubs[4].c_str());		

		item->transformStack.push_back(transform);
		return true;
	}
	else if ( key =="scale")
	{
		AffineTransformation transform;
		transform.type = eScale;
		transform.values[0] = 0;transform.values[1] = transform.values[2] = transform.values[3] = 1;

		if (nubs.size() > 1)
			transform.values[0] = (float)atof(nubs[1].c_str());
		if (nubs.size() > 2)
			transform.values[1] = (float)atof(nubs[2].c_str());
		if (nubs.size() > 3)
			transform.values[2] = (float)atof(nubs[3].c_str());		

		item->transformStack.push_back(transform);
		return true;
	}
	else if ( key =="shear")
	{
		AffineTransformation transform;
		transform.type = eShear;
		transform.values[0] = 0;transform.values[1] = transform.values[2] = transform.values[3] = 0;

		if (nubs.size() > 1)
			transform.values[0] = (float)atof(nubs[1].c_str());
		if (nubs.size() > 2)
			transform.values[1] = (float)atof(nubs[2].c_str());
		if (nubs.size() > 3)
			transform.values[2] = (float)atof(nubs[3].c_str());		

		item->transformStack.push_back(transform);
		return true;
	}

	return false;
}


MapItem* WorldParser::readBox ( std::vector<std::string> &item )
{
	ClassicBoxItem	*box = new ClassicBoxItem;
	box->setWorld(&world);

	for ( unsigned int j = 0; j < item.size(); j++ )
	{
		std::vector<std::string> nubs = tokenize(item[j],std::string(" "));
		if ( nubs.size() )
		{
			std::string keyword;
			tolower(nubs[0],keyword);

			if (!positionalField(keyword,nubs,box))
				box->addElement(item[j]);
		}
	}
	return box;
}

MapItem* WorldParser::readPyramid ( std::vector<std::string> &item )
{
	ClassicPyramidItem	*pry = new ClassicPyramidItem;
	pry->setWorld(&world);

	for ( unsigned int j = 0; j < item.size(); j++ )
	{
		std::vector<std::string> nubs = tokenize(item[j],std::string(" "));
		if ( nubs.size() )
		{
			std::string keyword;
			tolower(nubs[0],keyword);

			if (!positionalField(keyword,nubs,pry))
			{
				if (keyword == "flipz")
					pry->flipedZ = true;
				else
					pry->addElement(item[j]);
			}
		}
	}
	return pry;
}

MapItem* WorldParser::readGroup ( std::vector<std::string> &item )
{
	InstanceItem	*instance = new InstanceItem;
	instance->setWorld(&world);

	for ( unsigned int j = 0; j < item.size(); j++ )
	{
		std::vector<std::string> nubs = tokenize(item[j],std::string(" "));
		if ( nubs.size() )
		{
			std::string keyword;
			tolower(nubs[0],keyword);

			if ( keyword == "group" )
			{
				instance->define = nubs[1].c_str();
			}
			else if ( keyword == "name" )
				instance->name = nubs[1];
			else if ( !affineTransformField(keyword,nubs,instance) )		
				instance->addElement(item[j]);
		}
	}
	return instance;
}

MapItem* WorldParser::readWorld ( std::vector<std::string> &item )
{
	WorldItem	*worldItem = new WorldItem;
	worldItem->setWorld(&world);

	for ( unsigned int j = 0; j < item.size(); j++ )
	{
		std::vector<std::string> nubs = tokenize(item[j],std::string(" "));
		if ( nubs.size() )
		{
			std::string keyword;
			tolower(nubs[0],keyword);

			if ( keyword == "size" || keyword == "size" )
			{
				if ( nubs.size() > 1)
					worldItem->size[0] = (float)atof(nubs[1].c_str());
				if ( nubs.size() > 2)
					worldItem->size[1] = (float)atof(nubs[2].c_str());
				if ( nubs.size() > 3)
					worldItem->size[2] = (float)atof(nubs[3].c_str());
			}
			else
				worldItem->addElement(item[j]);
		}
	}
	return worldItem;
}

// TextWorldConstructor
// saves a world to disk

TextWorldConstructor::TextWorldConstructor(World &_world) : world(_world)
{
	indentLevel = 0;
}

bool TextWorldConstructor::write ( std::string filePath )
{
	indentLevel = 0;

	fileText = "#world file auto generated by mapedit\n\n";

	tvItList itemList = world.getItemList();

	for ( int p = 0; p <= (int)eMapItemFooter; p++)
	{
		for ( unsigned int i = 0; i < itemList.size(); i++ )
		{
			MapItem	*item = world.getItem(itemList[i]);
			if (item)
			{
				if ( item->getPriority() == (teMapItemPriority)p )
				{
					writeObject(item);
					fileText += "\n";
				}
			}
		}
	}

	if ( fileText.size() )	// if we have soemthing to write then do so
	{
		COSFile	file(filePath.c_str());

		if (!file.Open("wt"))
			return false;

		file.Write(fileText.c_str());
		file.Close();
	}
	return true;
}

void TextWorldConstructor::writeObject ( MapItem *item )
{
	if (!item)
		return;

	if (item->getClass() == "box")
		writeClassicPosObject(item);
	else if (item->getClass() == "pyramid")
		writeClassicPyramid(item);
	else if (item->getClass() == "world")
		writeWorld(item);
	else if (item->getClass() == "group")
		writeGroup(item);
	else if (item->getClass() == "instance")
		writeInstance(item);
	else
	{
		if (!item->getElementCount())
			return;

		// we asume the first element is it's class and name
		writeIndents();
		fileText += item->getElement(0);
		fileText += "\n";

		indent();
		for ( int i = 1; i < item->getElementCount(); i++ )
		{
			writeIndents();
			fileText += item->getElement(i);
			fileText += "\n";
		}
		unindent();
		writeIndents();
		fileText += "end\n";
	}
}

void TextWorldConstructor::writeStdPositionalFileds ( MapItem *item )
{
	BasePositional	*object = (BasePositional*)item;

	writeIndents();
	fileText += format("position %f %f %f\n",object->pos[0],object->pos[1],object->pos[2]);

	writeIndents();
	fileText += format("size %f %f %f\n",object->scale[0],object->scale[1],object->scale[2]);

	writeIndents();
	fileText += format("rotation %f\n",object->rot);

	if ( object->driveThru )
	{
		writeIndents();
		fileText += "drivethrough\n";
	}

	if ( object->shootThru )
	{
		writeIndents();
		fileText += "shootthrough\n";
	}

	writeElements(item);
}


void TextWorldConstructor::writeClassicPosObject ( MapItem *item )
{
	BasePositional	*object = (BasePositional*)item;

	writeIndents();
	fileText += object->getClass()+ "\n";
	indent();

	writeStdPositionalFileds(item);

	unindent();
	writeIndents();
	fileText += "end\n";
}

void TextWorldConstructor::writeClassicPyramid ( MapItem *item )
{
	ClassicPyramidItem	*pry = (ClassicPyramidItem*)item;

	writeIndents();
	fileText += pry->getClass()+ "\n";
	indent();

	writeStdPositionalFileds(pry);

	if (pry->flipedZ)
	{
		writeIndents();
		fileText += "flipz\n";
	}

	unindent();
	writeIndents();
	fileText += "end\n";
}


void TextWorldConstructor::writeWorld ( MapItem *item )
{
	WorldItem	*worldItem = (WorldItem*)item;

	writeIndents();
	fileText += "world\n";
	indent();

	writeIndents();
	fileText += format("size %f\n",worldItem->size[0]);

	unindent();
	writeIndents();
	fileText += "end\n";
}
void TextWorldConstructor::writeInstance ( MapItem *item )
{
	InstanceItem	*instanceItem = (InstanceItem*)item;

	writeIndents();
	fileText += "group ";
	fileText += instanceItem->define;
	fileText += "\n";
	indent();

	writeAffineTransforms(instanceItem->transformStack);
	writeElements(item);

	unindent();
	writeIndents();
	fileText += "end\n";
}

void TextWorldConstructor::writeGroup ( MapItem *item )
{
	GroupItem	*groupItem = (GroupItem*)item;

	writeIndents();
	fileText += "define ";
	fileText += groupItem->getName();
	fileText += "\n";
	indent();
	
	writeAffineTransforms(groupItem->transformStack);
	writeElements(item);

	for ( int i = 0; i < groupItem->getSubItemCount(); i++)
		writeObject(groupItem->getSubItem(i));

	unindent();
	writeIndents();
	fileText += "end\n";
}

void TextWorldConstructor::writeAffineTransforms ( AffineTransformationStack &transforms )
{
	for ( unsigned int i = 0; i < transforms.size(); i++ )
	{
		writeIndents();
		switch(transforms[i].type) 
		{
		default:
		case eShift:
			fileText += format("shift %f %f %f\n",transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
			break;
		case eShear:
			fileText += format("shear %f %f %f\n",transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
			break;
		case eSpin:
			fileText += format("spin %f %f %f %f\n",transforms[i].values[0],transforms[i].values[1],transforms[i].values[2],transforms[i].values[3]);
			break;
		case eScale:
			fileText += format("scale %f %f %f\n",transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
			break;
		}
	}
}

void TextWorldConstructor::writeElements ( MapItem *item )
{
	for ( int i = 0; i < item->getElementCount(); i++ )
	{
		writeIndents();
		fileText += item->getElement(i) + "\n";
	}
}

void TextWorldConstructor::indent ( void )
{
	indentLevel++;
}

void TextWorldConstructor::unindent ( void )
{
	indentLevel--;
	if (indentLevel < 0)
		indentLevel = 0;
}

void TextWorldConstructor::writeIndents ( void )
{
	for ( int i = 0; i < indentLevel; i++)
		fileText += "\t";
}
	


