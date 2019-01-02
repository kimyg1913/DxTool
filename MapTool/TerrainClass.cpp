#include "stdafx.h"
#include "TerrainClass.h"
#include "MyMath.h"

TerrainClass::TerrainClass() : m_pTerrainShader(nullptr),
m_pTerrainVertex(nullptr), m_pTerrainIndex(nullptr), m_pVB(nullptr), m_pIB(nullptr)
, m_pTexDiffuse(nullptr), m_pTexHeight(nullptr), m_heightMap(nullptr), m_iHighestY(-1), m_iLowestY(1)
{
	m_vfScale = D3DXVECTOR3(1.f,1.f, 1.f);
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
	m_indexCount = ((xNumber-1) * (zNumber-1)) * 2;
	m_iXIndexCount = m_indexCount / (zNumber - 1);

	//높이맵을 불러오는 거면 높이맵을 불러온다.
	if (isLoadMap)
	{
		if(!LoadHeightMap(device, str))
			return false;
	}

	/*if (!InitVertexSmallTexture(device))
	{
		return false;
	}*/

	if (!InitVertex(device))
	{
		return false;
	}

	if (!InitIndex(device))
	{
		return false;
	}

	if (FAILED(D3DXCreateTextureFromFile(device, L"Textures/ground.tga", &m_pTexDiffuse)))
	{
		return false;
	}

	if (FAILED(D3DXCreateTextureFromFile(device, L"Textures/snow.jpg", &m_pTexDiffuse2)))
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

	m_pLight = new LightClass;

	m_pLight->SetPosition(-100.f, 100.f, -100.f);
	m_pLight->SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	//m_pLight->SetDirection(1.0f, 0.0f, 0.f);

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
	if (m_pTexDiffuse2)
	{
		m_pTexDiffuse2->Release();
		m_pTexDiffuse2 = nullptr;
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

	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = nullptr;
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
	float height;
	FILE * pFile = NULL;


	if (FAILED(D3DXCreateTextureFromFileEx(device, L"Textures/map128.bmp",
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexHeight)))
		return E_FAIL;

	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;

	m_pTexHeight->GetLevelDesc(0, &ddsd);	/// 텍스처의 정보
	m_iCx = ddsd.Width;				/// 텍스처의 가로크기
	m_iCz = ddsd.Height;				/// 텍스처의 세로크기
	

	m_heightMap = new HeightMapType[m_iCx * m_iCz];

	int k = 0;

	int index = 0;

	m_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

	for (int z = 0; z < m_iCz; ++z)
	{
		for (int x = 0; x < m_iCx; ++x)
		{
		
			height = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x000000ff));// / 10.0f;

			assert(index < m_iCx * m_iCz && index >= 0  && "error!!!!!!!!!!!!!!!!!!" );
			
			m_heightMap[index].x = (float)((x - m_iCx * 0.5f) * m_vfScale.x);
			m_heightMap[index].z = -(float)((z - m_iCz * 0.5f) * m_vfScale.z);
			m_heightMap[index].y = height;
			m_iHighestY = m_heightMap[index].y > m_iHighestY ? m_heightMap[index].y : m_iHighestY;
			m_iLowestY = m_heightMap[index].y < m_iLowestY ? m_heightMap[index].y : m_iLowestY;
		
			++index;
		}
	}

	m_pTexHeight->UnlockRect(0);

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

	if (!m_heightMap)
	{
		m_heightMap = new HeightMapType[m_iCx * m_iCz];

		int index = 0;

		for (int z = 0; z < m_iCz; z++)
		{
			for (int x = 0; x < m_iCx; x++)
			{
				m_heightMap[index].x = (float)((x - m_iCx * 0.5f) * m_vfScale.x);
				m_heightMap[index].z = -(float)((z - m_iCz * 0.5f) * m_vfScale.z);
				m_heightMap[index++].y = 0.f;
			}
		}

	}

	TERRAINVERTEX v;	
	pv = (TERRAINVERTEX *)pVertices;
	int offSet = 0;

	for (int z = 0; z < m_iCz; ++z)
	{
		for (int x = 0; x < m_iCx; ++x)
		{
			v.point.x = m_heightMap[z * m_iCx + x].x;
			v.point.z = m_heightMap[z * m_iCx + x].z;
			v.point.y = m_heightMap[z * m_iCx + x].y;
			
			v.texture.x = (float)x / (m_iCx-1);
			v.texture.y = (float)z / (m_iCz-1);
			*pv++ = v;
		}
	}

	SetPolygonIndexAndNormal();
	SetNormalVertex();

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
		
		int index = 0;

		for (int z = 0; z < m_iCz; z++)
		{
			for (int x = 0; x < m_iCx; x++)
				m_heightMap[index++].y = 0.f;
		}
		
	}

	m_vertexCount = (m_iCx-1) * (m_iCz-1) * 6;
	//m_indexCount = m_vertexCount / 3;
	m_indexCount = m_vertexCount / 3;

	m_iXIndexCount = 2 * (m_iCx - 1);

	for (int z = 0; z < m_iCz; z++)
	{
		for (int x = 0; x < m_iCx; x++)
		{
			index = (m_iCx * z) + x;

			// Set the X and Z coordinates.
			// Move the terrain depth into the positive range.  For example from (0, -256) to (256, 0).
			m_heightMap[index].x = (float)(((x - m_iCx * 0.5f) * m_vfScale.x)) * 2.0f;
			m_heightMap[index].z = -(float)((z - m_iCz * 0.5f) * m_vfScale.z) * 2.0f;
			//m_heightMap[index].y = rand() % 10000;
			// Scale the height.
			m_heightMap[index].y *= m_vfScale.y;
		}
	}

	if (FAILED(device->CreateVertexBuffer(m_vertexCount * sizeof(TERRAINVERTEX), 0,
		TERRAINFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return false;
	}

	if (FAILED(device->CreateIndexBuffer(m_indexCount * sizeof(MYINDEX), 0, D3DFMT_INDEX32,
		D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return false;
	}

	VOID* pVertices;

	TERRAINVERTEX * pv = new TERRAINVERTEX[m_vertexCount];
	m_pTerrainVertex = pv;

	VOID* pIndices;

	//MYINDEX * pi = new MYINDEX[m_indexCount];
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
	index = 0;

	for (size_t z = 0; z < m_iCz-1; ++z)
	{
		for (size_t x = 0; x < m_iCx-1; ++x)
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

	if (FAILED(device->CreateIndexBuffer(m_indexCount * sizeof(MYINDEX), 0, D3DFMT_INDEX32,
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
//	m_pTerrainIndex = pi;

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

void TerrainClass::CalculatePolygonNormal()
{
	int index1, index2, index3, index4;
	D3DXVECTOR3 v1, v2, v3, leftNor, rightNor;
	
	//m_vPolygonNormal.clear();
	if (m_vPolygonNormal.size() == 0)
	{
		m_vPolygonNormal.reserve(m_indexCount);

		TERRAINVERTEX * pVertices;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (int z = 0; z < m_iCz - 1; ++z)
		{
			for (int x = 0; x < m_iCx - 1; ++x)
			{
				index1 = z * m_iCx + x;
				index2 = z * m_iCx + x + 1;
				index3 = (z + 1) * m_iCx + x;
				index4 = (z + 1) * m_iCx + x + 1;

				v1.x = pVertices[index2].point.x - pVertices[index3].point.x; // 가운데 벡터
				v1.y = pVertices[index2].point.y - pVertices[index3].point.y;
				v1.z = pVertices[index2].point.z - pVertices[index3].point.z;

				//v2 = pVertices[index2].point - pVertices[index1].point; // 위 벡터
				v2.x = pVertices[index2].point.x - pVertices[index1].point.x;
				v2.y = pVertices[index2].point.y - pVertices[index1].point.y;
				v2.z = pVertices[index2].point.z - pVertices[index1].point.z;

				//	v3 = pVertices[index2].point - pVertices[index4].point; // 아래 벡터
				v3.x = pVertices[index2].point.x - pVertices[index4].point.x;
				v3.y = pVertices[index2].point.y - pVertices[index4].point.y;
				v3.z = pVertices[index2].point.z - pVertices[index4].point.z;


				D3DXVec3Cross(&leftNor, &v1, &v2);
				D3DXVec3Cross(&rightNor, &v3, &v1);

				float leftLength = sqrt(leftNor.x * leftNor.x + leftNor.y * leftNor.y + leftNor.z * leftNor.z);
				float rightLength = sqrt(rightNor.x * rightNor.x + rightNor.y * rightNor.y + rightNor.z * rightNor.z);

				leftNor = leftNor / leftLength;
				rightLength /= leftLength;

				m_vPolygonNormal.push_back(leftNor);
				m_vPolygonNormal.push_back(rightNor);
			}
		}
		m_pVB->Unlock();
	}


}

void TerrainClass::SetPolygonIndexAndNormal()
{
	int poly11, poly12, poly1, poly8, poly7, poly6;
	int iPolyXSize = m_indexCount / (m_iCz - 1);
	int sumVertexIndex;
	vector<int> vLocatePoly;

	m_vPolygonIndexPerVertex.reserve(m_iCx*m_iCz);

	for (int z = 0; z < m_iCz; ++z)
	{
		for (int x = 0; x < m_iCx; ++x)
		{
			sumVertexIndex = z * m_iCx + x;

			if (z > 0 && x > 0) // 11시방향 평면삼각형
			{
				poly11 = (2 * x - 1) + (z - 1) * iPolyXSize;
				vLocatePoly.push_back(poly11);
			}

			if (z > 0 && x < m_iCx - 1) //12, 1 시방향 평면 삼각형
			{
				poly12 = (2 * x) + (z - 1) * iPolyXSize;
				poly1 = (2 * x + 1) + (z - 1) * iPolyXSize;
				vLocatePoly.push_back(poly12);
				vLocatePoly.push_back(poly1);
			}

			if (z < m_iCz - 1 && x > 0) // 8,7시 방향 평면 삼각형
			{
				poly8 = (2 * (x - 1) + (z * iPolyXSize));
				poly7 = (2 * (x - 1) + 1 + (z * iPolyXSize));
				vLocatePoly.push_back(poly8);
				vLocatePoly.push_back(poly7);
			}

			if (z < m_iCz - 1 && x < m_iCx - 1) //6시 방향 평면삼각형
			{
				poly6 = ((2 * x) + (z * iPolyXSize));
				vLocatePoly.push_back(poly6);
			}

			m_vPolygonIndexPerVertex.push_back(vLocatePoly);
			vLocatePoly.clear();
		}
	}
}

bool TerrainClass::Picking(LPDIRECT3DDEVICE9 device, HWND hwnd, D3DXMATRIX * matWorld, D3DXMATRIX * matView, D3DXMATRIX * matProj)
{
	D3DVIEWPORT9 ViewPortInfo;
	D3DXVECTOR3 v;  //클릭한 점에 해당하는 직선
	D3DXVECTOR3 rayOrigin; //광선 위치
	D3DXVECTOR3 rayDir; //광선 방향

	D3DXMATRIX matWorldView;
	D3DXMATRIX m;

	float Strength = m_fBaseStrength;

	device->GetViewport(&ViewPortInfo);

	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);

	//printf("%d, %d \n", pt.x, pt.y);
	v.x = ((pt.x - ViewPortInfo.X) * 2.0f / ViewPortInfo.Width - 1.0f) / matProj->_11;
	v.y = -((pt.y - ViewPortInfo.Y) * 2.0f / ViewPortInfo.Height - 1.0f) / matProj->_22;
	v.z = 1.0f;
	
	matWorldView = *(matWorld) * *(matView);

	D3DXMatrixInverse(&m, NULL, &matWorldView);

	rayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	rayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	rayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	TERRAINVERTEX * pVertices;
	MYINDEX * pIndices;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (int i = 0; i < m_indexCount; ++i)
	{
		D3DXVECTOR3 v0 = D3DXVECTOR3(pVertices[pIndices[i].i_0].point);
		D3DXVECTOR3 v1 = D3DXVECTOR3(pVertices[pIndices[i].i_1].point);
		D3DXVECTOR3 v2 = D3DXVECTOR3(pVertices[pIndices[i].i_2].point);

		float d, u, v;

		bool isPicked = D3DXIntersectTri(&v0, &v1, &v2, &rayOrigin, &rayDir, &u, &v, &d);

		if (isPicked)
		{
			D3DXVECTOR3 tempVec = v0 + u * (v1 - v0) + v*(v2 - v0);

			m_vPickedIndex.clear();

			//아래위 삼각형
			if (i % 2 != 0)
				i -= 1;

			int leftTopIndex = pIndices[i].i_0;
			int rightTopIndex = pIndices[i].i_1;
			int leftBottomIndex = pIndices[i].i_2;
			int rightBottomIndex = pIndices[i + 1].i_2;

			bool isLeftTopAdd = true;
			bool isRightTopAdd = true;
			bool isleftBottomAdd = true;
			bool isRightBottomAdd = true;

			for (int j = 0; j < m_iBrushRadius; ++j)
			{
				int tmpleftTopIndex = leftTopIndex - m_iCx * j;
				int tmprightTopIndex = rightTopIndex - m_iCx * j;
				int tmpleftBottomIndex = leftBottomIndex + m_iCx * j;
				int tmprightBottomIndex = rightBottomIndex + m_iCx * j;

				bool isLeftTopVertexAdd = true;
				bool isRightTopVertexAdd = true;
				bool isleftBottomVertexAdd = true;
				bool isRightBottomVertexAdd = true;

				if (tmpleftTopIndex < 0)
					isLeftTopAdd = false;
				if (tmprightTopIndex < 0)
					isRightTopAdd = false;
				if (tmpleftBottomIndex > m_vertexCount - 1)
					isleftBottomAdd = false;
				if (tmprightBottomIndex > m_vertexCount - 1)
					isRightBottomAdd = false;

				for (int k = 0; k < m_iBrushRadius-j; ++k)
				{
					if ((tmpleftTopIndex - k) % m_iCx == (m_iCx-1))
						isLeftTopVertexAdd = false;
					if ((tmprightTopIndex + k) % m_iCx == (0))
						isRightTopVertexAdd = false;
					if ((tmpleftBottomIndex - k) % m_iCx == m_iCx - 1)
						isleftBottomVertexAdd = false;
					if ((tmprightBottomIndex + k) % m_iCx == (0))
						isRightBottomVertexAdd = false;

					if(isLeftTopVertexAdd && isLeftTopAdd)
						m_vPickedIndex.push_back(tmpleftTopIndex -k);
					
					if(isRightTopVertexAdd && isRightTopAdd)
						m_vPickedIndex.push_back(tmprightTopIndex +k);
					
					if(isleftBottomVertexAdd && isleftBottomAdd)
						m_vPickedIndex.push_back(tmpleftBottomIndex -k);

					if(isRightBottomVertexAdd && isRightBottomAdd)
						m_vPickedIndex.push_back(tmprightBottomIndex +k);
				}

			}

			ModifyYValue(tempVec);
			
			return true;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return false;
}

bool TerrainClass::Render(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetFVF(TERRAINFVF);
	device->SetTexture(0, m_pTexDiffuse2);
	//인덱스 세팅
	device->SetStreamSource(0, m_pVB, 0, sizeof(MyStruct::TERRAINVERTEX));
	// select the vertex buffer to display
	device->SetIndices(m_pIB);

	//텍스쳐 세팅
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pTerrainShader->RenderShader(device, world, view, proj, m_vertexCount, m_indexCount, m_pTexDiffuse,&m_pLight->GetDiffuseColor(),&m_pLight->GetPosition());

	return true;
}

void TerrainClass::SetBrush(int radius, float strength)
{
	m_iBrushRadius = radius;
	m_fStrength = strength;

	m_fBaseStrength = strength * 0.005;
}

void TerrainClass::FindVertex()
{

	/*TERRAINVERTEX * pVertices;

	m_pVB->Lock(0, 0, (void**)pVertices, 0);


	for (int i = 0; i < m_vPickedIndex.size(); ++i)
	{
		pVertices[m_vPickedIndex[i]].point.y += m_fStrength * 0.005f;
	}

	m_pVB->Unlock();*/
}

void TerrainClass::ModifyYValue(D3DXVECTOR3 center)
{
	TERRAINVERTEX * pVertices;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_vPickedIndex.size(); ++i)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(pVertices[m_vPickedIndex[i]].point);
		D3DXVECTOR3 lPos = (pos - center);

		float length = sqrt(lPos.x * lPos.x + lPos.z * lPos.z);

		float yValue = -(length * length) / (m_iBrushRadius * m_iBrushRadius) + 1;

		if(m_eDrawMode == DM_LIFT)
			pVertices[m_vPickedIndex[i]].point.y += yValue * m_fStrength * 0.1f;
		else if(m_eDrawMode == DM_FALL)
			pVertices[m_vPickedIndex[i]].point.y -= yValue * m_fStrength * 0.1f;
	}

	m_pVB->Unlock();

	ModifyPickedVertexNormal();
}

void TerrainClass::SetDrawMode(DRAWMODE mode)
{
	m_eDrawMode = mode;
}


void TerrainClass::SetNormalVertex()
{

//	else // 전체 노말 세팅
	{
		CalculatePolygonNormal();
		TERRAINVERTEX * pVertices;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);
		
		int size;
		
		for (int i = 0; i < m_vPolygonIndexPerVertex.size(); ++i)
		{
			D3DXVECTOR3 vSum = D3DXVECTOR3(0.f, 0.f, 0.f);
			size = m_vPolygonIndexPerVertex[i].size();

			for (int j = 0; j < size; ++j)
			{
				vSum += m_vPolygonNormal[m_vPolygonIndexPerVertex[i][j]];
			}

			vSum /= size;
			VectorNormalize(&vSum);
			//D3DXVECTOR3 n = vSum;
			//m_vPolygonNormal[i] = vSum;
			pVertices[i].normal.x = vSum.x;
			pVertices[i].normal.y = vSum.y;
			pVertices[i].normal.z = vSum.z;
		
		/*	
			int a = 10;*/
		}
		m_pVB->Unlock();
	}

}

//픽된 버텍스들의 노말만 변경하는 코드
void TerrainClass::ModifyPickedVertexNormal()
{
	set<int> changedPolygonIndex;

	//픽된 버텍스들의 인덱스정보로
	//기존에 버텍스들과 연결된 폴리곤들의 인덱스들을
	//set을 이용해 담는다. set을 이용하는 이유는
	//중복된 폴리곤인덱스가 여러번 나올수 있기때문에 사용했다.
	//픽된 버텍스들의 노멀을 결정하기 위해서는 주변 폴리곤들의 평균값을 구해야하므로
	//픽된 버텍스들과 연관된 주변 폴리곤들의 노멀맵도 변경해줘야한다.
	for (int i = 0; i < m_vPickedIndex.size(); ++i)
	{
		int size = m_vPolygonIndexPerVertex[m_vPickedIndex[i]].size();
		
		for (int j = 0; j < size; ++j)
		{
			changedPolygonIndex.insert(m_vPolygonIndexPerVertex[m_vPickedIndex[i]][j]);
		}
	}

	//
	set<int>::iterator iter; 

	TERRAINVERTEX * pVertices;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (iter = changedPolygonIndex.begin(); iter != changedPolygonIndex.end(); ++iter)
	{
		int polyIndex = *iter;
		int z = polyIndex / m_iXIndexCount;

		if (polyIndex % 2 == 0) //왼쪽 폴리곤이라면
		{
			int index1 = polyIndex / 2 + z;
			int index2 = polyIndex / 2 + z + 1;
			int index3 = ((polyIndex % m_iXIndexCount) / 2) + (z + 1) * m_iCx;

			D3DXVECTOR3 vUp = pVertices[index2].point - pVertices[index3].point;
			D3DXVECTOR3 vRight = pVertices[index2].point - pVertices[index1].point;
			
			D3DXVECTOR3 vCross;
			D3DXVec3Cross(&vCross, &vUp, &vRight);

			D3DXVec3Normalize(&m_vPolygonNormal[polyIndex], &vCross);
		}
		else //아래 폴리곤이라면
		{
			int index1 = polyIndex / 2 + (z + 1);
			int index2 = (((polyIndex - 1) % m_iXIndexCount) / 2) + ((z + 1) * m_iCx);
			int index3 = (((polyIndex - 1) % m_iXIndexCount) / 2) + ((z + 1) * m_iCx) + 1;

			D3DXVECTOR3 vUp = pVertices[index1].point - pVertices[index3].point;
			D3DXVECTOR3 vRightUp = pVertices[index1].point - pVertices[index2].point;
			D3DXVECTOR3 vCross;
			D3DXVec3Cross(&vCross, &vUp, &vRightUp);

			D3DXVec3Normalize(&m_vPolygonNormal[polyIndex], &vCross);
		}
	}

	for (int i = 0; i < m_vPickedIndex.size(); ++i)
	{
		int size = m_vPolygonIndexPerVertex[m_vPickedIndex[i]].size();
		int index = m_vPickedIndex[i];
		D3DXVECTOR3 sum = D3DXVECTOR3(0.f, 0.f, 0.f);

		for (int j = 0; j < size; ++j)
		{
			sum += m_vPolygonNormal[m_vPolygonIndexPerVertex[index][j]];
		}

		sum /= size;
		pVertices[index].normal = sum;
	}

	m_pVB->Unlock();

}
