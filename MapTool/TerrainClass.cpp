#include "stdafx.h"
#include "TerrainClass.h"


TerrainClass::TerrainClass(): m_pTerrainShader(nullptr),
	m_pTerrainVertex(nullptr), m_pTerrainIndex(nullptr),m_pVB(nullptr), m_pIB(nullptr)
{
	m_vfScale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
}


TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(LPDIRECT3DDEVICE9 device, int iCx, int iCz)
{
	m_vertexCount = iCx * iCz;
	m_indexCount = (iCx - 1) * (iCz - 1) * 2;

	if (!InitVertex(device))
	{
		return false;
	}

	if (!InitIndex(device))
	{
		return false;
	}

	m_pTerrainShader = new TerrainShaderClass;

	if (!m_pTerrainShader->Initialize(device))
	{
		return false;
	}

	return true;
}

void TerrainClass::ShutDown()
{

	if (m_pTerrainVertex)
	{
		delete [] m_pTerrainVertex;
		m_pTerrainVertex = nullptr;
	}

	if (m_pTerrainIndex)
	{
		delete [] m_pTerrainIndex;
		m_pTerrainIndex = nullptr;
	}

	if (m_pTerrainShader)
	{
		m_pTerrainShader->ShutDown();
		delete m_pTerrainShader;
		m_pTerrainShader = nullptr;
	}

	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}

	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}

}

bool TerrainClass::InitVertex(LPDIRECT3DDEVICE9 device)
{
	m_iCx = 128;
	m_iCz = 128;

	//m_pTerrainVertex = new TERRAINVERTEX[m_vertexCount];

	if (FAILED(device->CreateVertexBuffer(m_vertexCount * sizeof(TERRAINVERTEX), 0,
		CUSTOMFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return false;
	}

	VOID* pVertices;

	TERRAINVERTEX * pv = new TERRAINVERTEX[m_vertexCount];;
	m_pTerrainVertex = pv;

	if (FAILED(m_pVB->Lock(0, m_vertexCount * sizeof(TERRAINVERTEX), &pVertices, 0)))
	{
		return false;
	}

	TERRAINVERTEX v;	
	pv = (TERRAINVERTEX *)pVertices;

	for (int z = 0; z < m_iCz; ++z)
	{
		for (int x = 0; x < m_iCx; ++x)
		{
			v.point.x = (float)((x - m_iCx / 2) * m_vfScale.x);
			v.point.z = -(float)((z - m_iCz / 2) * m_vfScale.z);
			v.point.y = 0.0f;
			v.color = D3DCOLOR_XRGB(0, 255, 0);
			//*m_pTerrainVertex++ = v;
			*pv++ = v;
		}
	}

	m_pVB->Unlock();


	return true;
}

bool TerrainClass::InitIndex(LPDIRECT3DDEVICE9 device)
{
	//m_pTerrainIndex = new MYINDEX[m_indexCount];

	if (FAILED(device->CreateIndexBuffer(m_indexCount * sizeof(MYINDEX), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return false;
	}

	VOID* pIndices;

	MYINDEX * pi =new MYINDEX[m_indexCount];
	m_pTerrainIndex = pi;
	
	if (FAILED(m_pIB->Lock(0, m_indexCount * sizeof(MYINDEX), &pIndices, 0)))
	{
		return false;
	}

	MYINDEX i;
	//MYINDEX* pi = (MYINDEX*)pIndices;
	pi = (MYINDEX*)pIndices;

	for (int z = 0; z < m_iCz - 1; ++z)
	{
		for (int x = 0; x < m_iCx - 1; ++x)
		{
			i.i_0 = z * m_iCx + x;
			i.i_1 = z * m_iCx + x + 1;
			i.i_2 = (z + 1) * m_iCx + x;
			*pi++ = i;

			i.i_0 = (z + 1) * m_iCx + x;
			i.i_1 = z * m_iCx + x + 1;
			i.i_2 = (z + 1) * m_iCx + x + 1;
			*pi++ = i;
		}
	}


	m_pIB->Unlock();

	return true;
}

bool TerrainClass::Render(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj)
{
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetFVF(CUSTOMFVF);
	device->SetStreamSource(0, m_pVB, 0, sizeof(MyStruct::TERRAINVERTEX));

	// select the vertex buffer to display
	device->SetIndices(m_pIB);

	m_pTerrainShader->RenderShader(device, world, view, proj, m_iCx*m_iCz, (m_iCx - 1)*(m_iCz - 1) * 2);

	return true;
}
