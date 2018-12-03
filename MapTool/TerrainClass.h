#pragma once

#include "Base.h"
#include "TerrainShaderClass.h"

using namespace MyStruct;

class TerrainClass
{
public:
	TerrainClass();
	~TerrainClass();

	bool Initialize(LPDIRECT3DDEVICE9 device, int iCx, int iCz);
	void ShutDown();

	bool Render(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj);

private:
	bool InitVertex(LPDIRECT3DDEVICE9 device);
	bool InitIndex(LPDIRECT3DDEVICE9 device);

private:
	int			m_iCx;	// 가로픽셀수
	int			m_iCz;	// 세로픽셀수
	int			m_vertexCount;
	int			m_indexCount;

	D3DXVECTOR3	m_vfScale; // x scale, y scale, z scale

	TerrainShaderClass * m_pTerrainShader;

	TERRAINVERTEX * m_pTerrainVertex;//높이맵의 정점배열
	MYINDEX		*	m_pTerrainIndex;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

};

