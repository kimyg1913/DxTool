#pragma once

#include "Base.h"
#include "TerrainShaderClass.h"
#include "LightClass.h"

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
	bool Picking(LPDIRECT3DDEVICE9 devce, HWND hwnd, D3DXMATRIX * matWorld, D3DXMATRIX * matView, D3DXMATRIX * matProj);

	bool Render(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj);
	void SetBrush(int radius, float strength);
	void FindVertex();
	void ModifyYValue(D3DXVECTOR3 center);
	void SetDrawMode(DRAWMODE mode);

private:
	bool InitVertex(LPDIRECT3DDEVICE9 device);
	bool InitVertexSmallTexture(LPDIRECT3DDEVICE9 device);
	bool InitIndex(LPDIRECT3DDEVICE9 device);
	bool InitIndexSmallTexture(LPDIRECT3DDEVICE9 device);

	void CalculatePolygonNormal();
	void SetPolygonIndexAndNormal();
	void SetNormalVertex();

	void ModifyPickedVertexNormal();

private:
	int			m_iCx;	// �����ȼ���
	int			m_iCz;	// �����ȼ���
	int			m_vertexCount;
	int			m_indexCount;
	int			m_iXIndexCount; // �������� ������ ����

	int		m_iBrushRadius;
	float		m_fStrength;
	float		m_fBaseStrength;

	int			m_iHighestY;
	int			m_iLowestY;

	DRAWMODE	m_eDrawMode;

	vector<int> m_vPickedIndex;
	vector<float> m_vPickedLength;
	vector<D3DXVECTOR3> m_vPolygonNormal;
	vector<vector<int>> m_vPolygonIndexPerVertex;

	D3DXVECTOR3	m_vfScale; // x scale, y scale, z scale

	TerrainShaderClass * m_pTerrainShader;

	TERRAINVERTEX * m_pTerrainVertex;//���̸��� �����迭
	MYINDEX		*	m_pTerrainIndex;
	HeightMapType* m_heightMap;
	queue<float> m_heightY;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexDiffuse;
	LPDIRECT3DTEXTURE9		m_pTexDiffuse2;

	LPDIRECT3DTEXTURE9		m_pTexHeight;

	LightClass * m_pLight;
};

