#pragma once

#include "Base.h"
#include "TerrainShaderClass.h"
#include "ShadowShaderClass.h"
#include "LightClass.h"
#include "TemplateSingleton.h"

using namespace MyStruct;

struct HeightMapType
{
	float x, y, z;
};

class TerrainClass : public TemplateSingleton<TerrainClass>
{
public:
	TerrainClass();
	~TerrainClass();
	bool Initialize(LPDIRECT3DDEVICE9 device, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str, float fLODRatio);
	void ShutDown();

	bool LoadHeightMap(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);
	bool MakeDataFile(TCHAR * fileName);
	bool Picking(LPDIRECT3DDEVICE9 devce, HWND hwnd, D3DXMATRIX * matWorld, D3DXMATRIX * matView, D3DXMATRIX * matProj, D3DXVECTOR3 * vPicked, BRUSHOREROSION eBEMODE);

	bool Render(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj);
	void SetBrush(int radius, float strength);

	void ModifyYValue(D3DXVECTOR3 center);
	void SetDrawMode(DRAWMODE mode);
	void SetNormalVertex();

	void ModifyAllNormalVertex();
	bool IsCreated() { return m_isCreated; }
	int GetVertexNumber() { return m_iCz * m_iCx; }

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9  GetIndexBuffer() { return m_pIB; }

	int	GetRecentPickedIndex() { return m_iRecentIndex; }
	vector<int> & GetVectorPickedIndex() { return m_vPickedIndex; }

	LPDIRECT3DTEXTURE9 GetDepthTex();
	LPDIRECT3DTEXTURE9 GetShdowTex() { return m_pShadworShader->GetShaderTex(); }

private:
	bool InitVertex(LPDIRECT3DDEVICE9 device);
	bool InitIndex(LPDIRECT3DDEVICE9 device);


	void CalculatePolygonNormal();
	void SetPolygonIndexAndNormal();


	void ModifyPickedVertexNormal();

	bool LoadDataMap(LPCWSTR str);

private:
	bool		m_isCreated;
	int			m_iCx;	// 가로픽셀수
	int			m_iCz;	// 세로픽셀수
	int			m_vertexCount;
	int			m_indexCount;
	int			m_iXIndexCount; // 가로줄의 폴리곤 갯수

	int			m_iBrushRadius;
	float		m_fStrength;
	float		m_fBaseStrength;

	int			m_iHighestY;
	int			m_iLowestY;

	float		m_fLODRatio;

	D3DXVECTOR3 m_vRecentPicked;
	int			m_iRecentIndex;

	DRAWMODE	m_eDrawMode;

	vector<int> m_vPickedIndex;
	vector<D3DXVECTOR3> m_vPolygonNormal;
	vector<vector<int> > m_vPolygonIndexPerVertex;

	D3DXVECTOR3	m_vfScale; // x scale, y scale, z scale

	TerrainShaderClass * m_pTerrainShader;
	ShadowShaderClass * m_pShadworShader;

	TERRAINVERTEX * m_pTerrainVertex;//높이맵의 정점배열
	MYINDEX		*	m_pTerrainIndex;
	HeightMapType* m_heightMap;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DTEXTURE9		m_pTexDiffuse;
	LPDIRECT3DTEXTURE9		m_pTexDiffuse2;


	LPDIRECT3DTEXTURE9		m_pTexHeight;

	LightClass * m_pLight;

	class DepthShaderClass * m_pDepthShader;

	float * m_FileData;
};

