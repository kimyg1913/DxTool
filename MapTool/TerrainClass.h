#pragma once

#include "Base.h"
#include "TerrainShaderClass.h"

using namespace MyStruct;

struct HeightMapType
{
	float x, y, z;
};

class TerrainClass
{
public:
	TerrainClass();
	~TerrainClass();

	bool Initialize(LPDIRECT3DDEVICE9 device, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str);
	void ShutDown();

	bool LoadHeightMap(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);

	bool Render(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj);

private:
	bool InitVertex(LPDIRECT3DDEVICE9 device);
	bool InitVertexSmallTexture(LPDIRECT3DDEVICE9 device);
	bool InitIndex(LPDIRECT3DDEVICE9 device);
	bool InitIndexSmallTexture(LPDIRECT3DDEVICE9 device);

private:
	int			m_iCx;	// 가로픽셀수
	int			m_iCz;	// 세로픽셀수
	int			m_vertexCount;
	int			m_indexCount;

	int			m_iHighestY;
	int			m_iLowestY;

	D3DXVECTOR3	m_vfScale; // x scale, y scale, z scale

	TerrainShaderClass * m_pTerrainShader;

	TERRAINVERTEX * m_pTerrainVertex;//높이맵의 정점배열
	MYINDEX		*	m_pTerrainIndex;
	HeightMapType* m_heightMap;
	queue<float> m_heightY;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexDiffuse;
	LPDIRECT3DTEXTURE9		m_pTexHeight;
};

