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

	bool Initialize(LPDIRECT3DDEVICE9 device, int xNumber, int zNumber, int xSize, int zSize);
	void ShutDown();

	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	void ShutdownHeightMap();

	bool Render(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj);

private:
	bool InitVertex(LPDIRECT3DDEVICE9 device);
	bool InitIndex(LPDIRECT3DDEVICE9 device);

private:
	int			m_iCx;	// �����ȼ���
	int			m_iCz;	// �����ȼ���
	int			m_vertexCount;
	int			m_indexCount;

	D3DXVECTOR3	m_vfScale; // x scale, y scale, z scale

	TerrainShaderClass * m_pTerrainShader;

	TERRAINVERTEX * m_pTerrainVertex;//���̸��� �����迭
	MYINDEX		*	m_pTerrainIndex;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexDiffuse;

	HeightMapType* m_heightMap;
};

