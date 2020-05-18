#pragma once

#include "uniqueObj.h"

static class uniqueObjDB
{
public:
	unsigned int lasdID;

	uniqueObjDB();
	~uniqueObjDB();

	static void addObj(uniqueObj obj);
};

