#include "../include/Model.h"

Model::Model()
{
	this->worldOptionsData = new WorldOptionsData("Brave New World", "-s -n -i -ff", 0.0, 0.0);
}

Model::~Model()
{
	if(this->worldOptionsData)
		delete worldOptionsData;
}
