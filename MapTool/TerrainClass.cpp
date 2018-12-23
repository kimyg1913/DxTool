#include "stdafx.h"
#include "TerrainClass.h"


TerrainClass::TerrainClass(): m_pTerrainShader(nullptr),
	m_pTerrainVertex(nullptr), m_pTerrainIndex(nullptr),m_pVB(nullptr), m_pIB(nullptr)
	, m_pTexDiffuse(nullptr), m_pTexHeight(nullptr), m_heightMap(nullptr)
{
	m_vfScale = D3DXVECTOR3(5.f, 4.f, 5.f);
	m_vertexCount = 0;
	m_indexCount = 0;
	m_iCx = 0;
	m_iCz = 0;
}


TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(LPDIRECT3DDEVICE9 device, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str)
{
	m_iCx = xNumber;
	m_iCz = zNumber;
	m_vertexCount = (xNumber) * (zNumber);
	m_indexCount = m_vertexCount*2;
	//m_vfScale.x = xSize;
	//m_vfScale.y = xSize;
	//m_vfScale.z = zSize;


	//높이맵을 불러오는 거면 높이맵을 불러온다.
	if (isLoadMap)
	{
		if(!LoadHeightMap(device, str))
			return false;
	}

	if (!InitVertexSmallTexture(device))
	{
		return false;
	}

	if (FAILED(D3DXCreateTextureFromFile(device, L"Textures/snow.jpg", &m_pTexDiffuse)))
	{
		return false;
	}

	/*if (!InitVertex(device))
	{
		return false;
	}

	if (!InitIndex(device))
	{
		return false;
	}*/

	

	m_pTerrainShader = new TerrainShaderClass;

	if (!m_pTerrainShader->Initialize(device))
	{
		return false;
	}

	return true;
}

void TerrainClass::ShutDown()
{

	if (m_heightMap)
	{
		delete m_heightMap;
		m_heightMap = nullptr;
	}

	if (m_pTexDiffuse)
	{
		m_pTexDiffuse->Release();
		m_pTexDiffuse = nullptr;
	}

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

bool TerrainClass::LoadHeightMap(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{
	int count;
	int imageSize;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER  bitmapInfoHeader;
	char height;
	int index;
	FILE * pFile = NULL;

	fopen_s(&pFile, "Textures/map129.bmp", "rb");

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

	if (count != 1)
	{
		return false;
	}

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	if (count != 1)
	{
		return false;
	}

	m_iCx = bitmapInfoHeader.biWidth;
	m_iCz = bitmapInfoHeader.biHeight;

	m_heightMap = new HeightMapType[m_iCx * m_iCz];

	// 왜 이런식이 나올까  ???
	imageSize = m_iCz * ((m_iCx * 3) + 1);

	unsigned char * bitmapImage = new unsigned char[imageSize];

	fseek(pFile, bitmapFileHeader.bfOffBits, SEEK_SET);

	fread(bitmapImage, 1, imageSize, pFile);

	int k = 0;

	for (int z = 0; z < 129; ++z)
	{
		for (int x = 0; x < 129; ++x)
		{
			index = (129 * (129 - 1 - z)) + x;


			assert(k < imageSize && k >= 0 && "bitmapImage k error!!!!!!!!!!!!!!!!!!");

			height = bitmapImage[k];


			assert(index < m_iCx * m_iCz && index >= 0  && "error!!!!!!!!!!!!!!!!!!" );

			m_heightMap[index].y = (float(height));

			k += 3;
		}
		k++;
	}


	delete[] bitmapImage;

	return true;
}

bool TerrainClass::InitVertex(LPDIRECT3DDEVICE9 device)
{
	//m_pTerrainVertex = new TERRAINVERTEX[m_vertexCount];

	if (FAILED(device->CreateVertexBuffer(m_vertexCount * sizeof(TERRAINVERTEX), 0,
		TERRAINFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
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
	int offSet = 0;

	for (int z = 0; z < m_iCz; ++z)
	{
		for (int x = 0; x < m_iCx; ++x)
		{
			v.point.x = (float)((x - m_iCx / 2) * m_vfScale.x);
			v.point.z = -(float)((z - m_iCz / 2) * m_vfScale.z);
			v.point.y = 0.0f;

			v.texture.x = (float)x / (m_iCx-1);
			v.texture.y = (float)z / (m_iCz-1);
			*pv++ = v;
		}
	}

	m_pVB->Unlock();


	return true;
}

bool TerrainClass::InitVertexSmallTexture(LPDIRECT3DDEVICE9 device)
{
	//m_pTerrainVertex = new TERRAINVERTEX[m_vertexCount];
	
	int index, index1, index2, index3, index4;

	//m_heightMap = new HeightMapType[m_iCx * m_iCz];
	
	if (!m_heightMap)
	{
		m_heightMap = new HeightMapType[m_iCx * m_iCz];
	}

	m_vertexCount = (m_iCx-1) * (m_iCz-1) * 6;
	m_indexCount = m_vertexCount / 3;

	for (int z = 0; z < m_iCz; z++)
	{
		for (int x = 0; x < m_iCx; x++)
		{
			index = (m_iCx * z) + x;

			// Set the X and Z coordinates.
			// Move the terrain depth into the positive range.  For example from (0, -256) to (256, 0).
			m_heightMap[index].x = (float)(((x - m_iCx * 0.5f) * m_vfScale.x));
			m_heightMap[index].z = -(float)((z - m_iCz * 0.5f) * m_vfScale.z);
			//m_heightMap[index].y = rand() % 10000;
			// Scale the height.
			//m_heightMap[index].y /= m_vfScale.y;
		}
	}

	if (FAILED(device->CreateVertexBuffer(m_vertexCount * sizeof(TERRAINVERTEX), 0,
		TERRAINFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return false;
	}

	if (FAILED(device->CreateIndexBuffer(m_indexCount * sizeof(MYINDEX), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return false;
	}

	VOID* pVertices;

	TERRAINVERTEX * pv = new TERRAINVERTEX[m_vertexCount];
	m_pTerrainVertex = pv;

	VOID* pIndices;

	MYINDEX * pi = new MYINDEX[m_indexCount];
	m_pTerrainIndex = pi;

	if (FAILED(m_pVB->Lock(0, m_vertexCount * sizeof(TERRAINVERTEX), &pVertices, 0)))
	{
		return false;
	}

	if (FAILED(m_pIB->Lock(0, m_indexCount * sizeof(MYINDEX), &pIndices, 0)))
	{
		return false;
	}

	TERRAINVERTEX v;
	pv = (TERRAINVERTEX *)pVertices;

	MYINDEX i;
	pi = (MYINDEX*)pIndices;

	int offSet = 0;

	for (int z = 0; z < m_iCz-1; ++z)
	{
		for (int x = 0; x < m_iCx-1; ++x)
		{
			index1 = (m_iCx * z) + x;          // Upper left.
			index2 = (m_iCx * z) + (x + 1);      // Upper right.
			index3 = (m_iCx * (z + 1)) + x;      // Bottom left.
			index4 = (m_iCx * (z + 1)) + (x + 1);  // Bottom right.

			// left top
			v.point.x = m_heightMap[index1].x;
			v.point.z = m_heightMap[index1].z;
			v.point.y = m_heightMap[index1].y;
			v.texture.x = 0;
			v.texture.y = 0;
			pi->i_0 = offSet++;
			*pv++ = v;

			//right top
			v.point.x = m_heightMap[index2].x;
			v.point.z = m_heightMap[index2].z;
			v.point.y = m_heightMap[index2].y;
			v.texture.x = 1;
			v.texture.y = 0;
			pi->i_1 = offSet++;
			*pv++ = v;

			//left down
			v.point.x = m_heightMap[index3].x;
			v.point.z = m_heightMap[index3].z;
			v.point.y = m_heightMap[index3].y;
			v.texture.x = 0;
			v.texture.y = 1;
			pi->i_2 = offSet++;
			*pv++ = v;

			++pi;

			//left down
			v.point.x = m_heightMap[index3].x;
			v.point.z = m_heightMap[index3].z;
			v.point.y = m_heightMap[index3].y;
			v.texture.x = 0;
			v.texture.y = 1;
			pi->i_0 = offSet++;
			*pv++ = v;

			//right top
			v.point.x = m_heightMap[index2].x;
			v.point.z = m_heightMap[index2].z;
			v.point.y = m_heightMap[index2].y;
			v.texture.x = 1;
			v.texture.y = 0;
			pi->i_1 = offSet++;
			*pv++ = v;

			//right down
			v.point.x = m_heightMap[index4].x;
			v.point.z = m_heightMap[index4].z;
			v.point.y = m_heightMap[index4].y;
			v.texture.x = 1;
			v.texture.y = 1;
			pi->i_2 = offSet++;

			*pv++ = v;

			++pi;

		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

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

bool TerrainClass::InitIndexSmallTexture(LPDIRECT3DDEVICE9 device)
{
	if (FAILED(device->CreateIndexBuffer(m_indexCount * sizeof(MYINDEX), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return false;
	}

	VOID* pIndices;

	MYINDEX * pi = new MYINDEX[m_indexCount];
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
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetFVF(TERRAINFVF);
	device->SetTexture(0, m_pTexDiffuse);
	//인덱스 세팅
	device->SetStreamSource(0, m_pVB, 0, sizeof(MyStruct::TERRAINVERTEX));
	// select the vertex buffer to display
	device->SetIndices(m_pIB);

	//텍스쳐 세팅
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pTerrainShader->RenderShader(device, world, view, proj, m_vertexCount, m_indexCount, m_pTexDiffuse);

	return true;
}
