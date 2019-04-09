#pragma once
#include "TemplateSingleton.h"

class BitmapClass : public TemplateSingleton<BitmapClass>
{
public:
	BitmapClass();
	~BitmapClass();
	bool MakeBitmapFile(const char * fileName, int xSize, int zSize);
	bool MakeDataFile(const char * fileName, int xSize, int zSize);
	bool LoadDataFile(const char * fileName);
};

