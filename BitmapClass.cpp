#include "stdafx.h"
#include "BitmapClass.h"
#include "TerrainClass.h"

BitmapClass::BitmapClass()
{
}


BitmapClass::~BitmapClass()
{
}

bool BitmapClass::MakeBitmapFile(const char * fileName, int xSize, int zSize)
{
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * xSize * zSize;  //w is your image width, h is image height, both int

	img = (unsigned char *)malloc(3 * xSize * zSize);
	memset(img, 0, 3 * xSize*zSize);

	TERRAINVERTEX * pVerticies;
	TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0, 0, (void**)&pVerticies, 0);

	for (int i = 0; i < zSize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			unsigned char height = (unsigned char)floor(pVerticies[i * xSize + j].point.y);

			img[(j + i * xSize) * 3 + 2] = (unsigned char)(height);
			img[(j + i * xSize) * 3 + 1] = (unsigned char)(height);
			img[(j + i * xSize) * 3 + 0] = (unsigned char)(height);
		}
	}

	TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(xSize);
	bmpinfoheader[5] = (unsigned char)(xSize >> 8);
	bmpinfoheader[6] = (unsigned char)(xSize >> 16);
	bmpinfoheader[7] = (unsigned char)(xSize >> 24);
	bmpinfoheader[8] = (unsigned char)(zSize);
	bmpinfoheader[9] = (unsigned char)(zSize >> 8);
	bmpinfoheader[10] = (unsigned char)(zSize >> 16);
	bmpinfoheader[11] = (unsigned char)(zSize >> 24);

	fopen_s(&f,fileName, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i < zSize; i++)
	{
		fwrite(img + (xSize*(zSize - i - 1) * 3), 3, xSize, f);
		fwrite(bmppad, 1, (4 - (xSize * 3) % 4) % 4, f);
	}

	free(img);
	fclose(f);

	return true;
}

bool BitmapClass::MakeDataFile(const char * fileName, int xSize, int zSize)
{
	fstream file;

	file.open(fileName, ios_base::in | ios_base::out | ios_base::trunc);

	//float height

	file << xSize;
	file << " ";
	file << zSize;

	TERRAINVERTEX * pVerticies;
	TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0, 0, (void**)&pVerticies, 0);

	for (int i = 0; i < xSize*zSize; ++i)
	{
		file << " ";
		file << pVerticies[i].point.y;
	}

	TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();
	file.close();
	return true;
}

bool BitmapClass::LoadDataFile(const char * fileName)
{
	fstream file;

	file.open(fileName, ios_base::in);
	int xSize;
	int zSize;

	//if(file.is_open())
	//{
	//	file >> xSize;
	//	file >> zSize;

	//	while (!file.eof())
	//	{

	//	}
	//}

	file.close();
	return true;
}
