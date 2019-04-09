#include "stdafx.h"
#include "QuadTreeClass.h"
#include "FrustumClass.h"
#include "TerrainClass.h"
#include "Camera.h"
//#include "MyOperation.h"

QuadTreeClass::QuadTreeClass() : m_pParent(nullptr), m_fRadius(0.0f), m_bCulled(false)
{
	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = nullptr;     
		m_pNeighbor[i] = nullptr;
		m_CornerVertex[i] = 0;
	}
}

QuadTreeClass::QuadTreeClass(QuadTreeClass * pParent) : m_pParent(pParent), m_CenterVertex(0), m_fRadius(0.0f), m_bCulled(false)
{
	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = nullptr;
		m_pNeighbor[i] = nullptr;
		m_CornerVertex[i] = 0;
	}
}


QuadTreeClass::~QuadTreeClass()
{
	FrustumClass::DestroyInstance();
	DestoryChild();
}

void QuadTreeClass::SetCorner(int leftTopCorner, int rightTopCorner, int leftBottomCorner, int rightBottomCorner)
{
	m_CornerVertex[0] = leftTopCorner;
	m_CornerVertex[1] = rightTopCorner;
	m_CornerVertex[2] = leftBottomCorner;
	m_CornerVertex[3] = rightBottomCorner;
	m_CenterVertex = (m_CornerVertex[0] + m_CornerVertex[1] + m_CornerVertex[2] + m_CornerVertex[3]) / 4;
}

QuadTreeClass * QuadTreeClass::AddChild(int leftTopCorner, int rightTopCorner, int leftBottomCorner, int rightBottomCorner)
{
	QuadTreeClass * pChild;

	pChild = new QuadTreeClass(this);
	pChild->SetCorner(leftTopCorner, rightTopCorner, leftBottomCorner, rightBottomCorner);

	return pChild;
}

bool QuadTreeClass::SubDivide()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// ��ܺ� ���
	nTopEdgeCenter = (m_CornerVertex[LEFT_TOP] + m_CornerVertex[RIGHT_TOP]) / 2;
	// �ϴܺ� ��� 
	nBottomEdgeCenter = (m_CornerVertex[LEFT_BOTTOM] + m_CornerVertex[RIGHT_BOTTOM]) / 2;
	// ������ ���
	nLeftEdgeCenter = (m_CornerVertex[LEFT_TOP] + m_CornerVertex[LEFT_BOTTOM]) / 2;
	// ������ ���
	nRightEdgeCenter = (m_CornerVertex[RIGHT_TOP] + m_CornerVertex[RIGHT_BOTTOM]) / 2;
	// ���
	nCentralPoint = (m_CornerVertex[0] + m_CornerVertex[1] + m_CornerVertex[2] + m_CornerVertex[3]) /4;

	// ���̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
	if ((m_CornerVertex[RIGHT_TOP] - m_CornerVertex[LEFT_TOP]) <= 1)
	{
		return false;
	}

	m_pChild[0] = AddChild(m_CornerVertex[0], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[1] = AddChild(nTopEdgeCenter, m_CornerVertex[1], nCentralPoint, nRightEdgeCenter);
	m_pChild[2] = AddChild(nLeftEdgeCenter, nCentralPoint, m_CornerVertex[2], nBottomEdgeCenter);
	m_pChild[3] = AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_CornerVertex[3]);

	return true;
}


int QuadTreeClass::GenerateTriangleIndex(int nTriangles, float fLODRatio)
{
	// �ø��� ����� �׳� ����
	if (m_bCulled)
	{
		m_bCulled = false;
		return nTriangles;
	}

	// ���� ��尡 ��µǾ�� �ϴ°�?
	if (IsVisible(fLODRatio))
	{
		MYINDEX * pIndices;
		TerrainClass::GetInstance()->GetIndexBuffer()->Lock(0, 0, (void**)&pIndices, 0);

		if (m_CornerVertex[RIGHT_TOP] - m_CornerVertex[LEFT_TOP] <= 1)
		{
			pIndices[nTriangles].i_0 = m_CornerVertex[0];
			pIndices[nTriangles].i_1 = m_CornerVertex[1];
			pIndices[nTriangles].i_2 = m_CornerVertex[2];
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CornerVertex[2];
			pIndices[nTriangles].i_1 = m_CornerVertex[1];
			pIndices[nTriangles].i_2 = m_CornerVertex[3];
			nTriangles++;
			TerrainClass::GetInstance()->GetIndexBuffer()->Unlock();
			return nTriangles;
		}

		bool	b[4] = { false,};
		// ��� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_UP])
			b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(fLODRatio);
		// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_DN])
			b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_LT])
			b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_RT])
			b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(fLODRatio);

		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			pIndices[nTriangles].i_0 = m_CornerVertex[0];
			pIndices[nTriangles].i_1 = m_CornerVertex[1];
			pIndices[nTriangles].i_2 = m_CornerVertex[2];
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CornerVertex[2];
			pIndices[nTriangles].i_1 = m_CornerVertex[1];
			pIndices[nTriangles].i_2 = m_CornerVertex[3];
			nTriangles++;
			TerrainClass::GetInstance()->GetIndexBuffer()->Unlock();
			return nTriangles;
		}

		int		n;

		if (!b[EDGE_UP]) // ��� �κк����� �ʿ��Ѱ�?
		{
			n = (m_CornerVertex[LEFT_TOP] + m_CornerVertex[RIGHT_TOP]) / 2;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[LEFT_TOP];
			pIndices[nTriangles].i_2 = n;
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = n;
			pIndices[nTriangles].i_2 = m_CornerVertex[RIGHT_TOP];
			nTriangles++;
		}
		else	// ��� �κк����� �ʿ���� ���
		{
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[LEFT_TOP];
			pIndices[nTriangles].i_2 = m_CornerVertex[RIGHT_TOP];
			nTriangles++;
		}

		if (!b[EDGE_DN]) // �ϴ� �κк����� �ʿ��Ѱ�?
		{
			n = (m_CornerVertex[LEFT_BOTTOM] + m_CornerVertex[RIGHT_BOTTOM]) / 2;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[RIGHT_BOTTOM];
			pIndices[nTriangles].i_2 = n;
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = n;
			pIndices[nTriangles].i_2 = m_CornerVertex[LEFT_BOTTOM];
			nTriangles++;
		}
		else	// �ϴ� �κк����� �ʿ���� ���
		{
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[RIGHT_BOTTOM];
			pIndices[nTriangles].i_2 = m_CornerVertex[LEFT_BOTTOM];
			nTriangles++;
		}

		if (!b[EDGE_LT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (m_CornerVertex[LEFT_TOP] + m_CornerVertex[LEFT_BOTTOM]) / 2;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[LEFT_BOTTOM];
			pIndices[nTriangles].i_2 = n;
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = n;
			pIndices[nTriangles].i_2 = m_CornerVertex[LEFT_TOP];
			nTriangles++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[LEFT_BOTTOM];
			pIndices[nTriangles].i_2 = m_CornerVertex[LEFT_TOP];
			nTriangles++;
		}

		if (!b[EDGE_RT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (m_CornerVertex[RIGHT_TOP] + m_CornerVertex[RIGHT_BOTTOM]) / 2;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[RIGHT_TOP];
			pIndices[nTriangles].i_2 = n;
			nTriangles++;
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = n;
			pIndices[nTriangles].i_2 = m_CornerVertex[RIGHT_BOTTOM];
			nTriangles++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			pIndices[nTriangles].i_0 = m_CenterVertex;
			pIndices[nTriangles].i_1 = m_CornerVertex[RIGHT_TOP];
			pIndices[nTriangles].i_2 = m_CornerVertex[RIGHT_BOTTOM];
			nTriangles++;
		}

		TerrainClass::GetInstance()->GetIndexBuffer()->Unlock();
		return nTriangles;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	// �ڽ� ���� �˻�
	if (m_pChild[0])
		nTriangles = m_pChild[0]->GenerateTriangleIndex(nTriangles, fLODRatio);

	if (m_pChild[1])
		nTriangles = m_pChild[1]->GenerateTriangleIndex(nTriangles, fLODRatio);

	if (m_pChild[2])
		nTriangles = m_pChild[2]->GenerateTriangleIndex(nTriangles, fLODRatio);

	if (m_pChild[3])
		nTriangles = m_pChild[3]->GenerateTriangleIndex(nTriangles, fLODRatio);

	return nTriangles;
}


bool QuadTreeClass::Init(int xSize, int ySize)
{
	if(xSize % 2 == 0)
		return false;
	if (ySize % 2 == 0)
		return false;

	m_CornerVertex[0] = 0;
	m_CornerVertex[1] = xSize - 1;
	m_CornerVertex[2] = xSize * (ySize - 1);
	m_CornerVertex[3] = xSize * ySize - 1;
	m_CenterVertex = (m_CornerVertex[0] + m_CornerVertex[1] + m_CornerVertex[2] + m_CornerVertex[3]) /4;

	m_fRadius = 0.0f;
	m_bCulled = false;
	m_pParent = nullptr;

	Build();
}


void QuadTreeClass::Build()
{
	BuildQuadTree();
	BuildNeighborNode(this, m_CornerVertex[RIGHT_TOP]+1);
}

void QuadTreeClass::DestoryChild()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i])
		{
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		}
	}
}

int QuadTreeClass::IsInFrustum()
{
	bool	b[4];
	bool	bInSphere;

	TERRAINVERTEX * pVertices;

	TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0, 0, (void**)&pVertices, 0);

	// ��豸�ȿ� �ִ°�?
	bInSphere = FrustumClass::GetInstance()->IsInSphere(pVertices[m_CenterVertex].point, m_fRadius);

	if (!bInSphere) 
	{ 
		TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();
		return FRUSTUM_OUT;	// ��豸 �ȿ� ������ �������� �������� �׽�Ʈ ����
	}
	// ����Ʈ���� 4���� ��� �������� �׽�Ʈ
	b[0] = FrustumClass::GetInstance()->IsIn(pVertices[m_CornerVertex[0]].point);
	b[1] = FrustumClass::GetInstance()->IsIn(pVertices[m_CornerVertex[1]].point);
	b[2] = FrustumClass::GetInstance()->IsIn(pVertices[m_CornerVertex[2]].point);
	b[3] = FrustumClass::GetInstance()->IsIn(pVertices[m_CornerVertex[3]].point);

	TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();

	// 4����� �������� �ȿ� ����
	if (b[0] && b[1] && b[2] && b[3]) 
		return FRUSTUM_COMPLETELY_IN;

	// �Ϻκ��� �������ҿ� �ִ� ���
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTreeClass::FrustumCull()
{
	int ret;

	m_bCulled = false;
	ret = IsInFrustum();

	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:	// �������ҿ� ��������, ������� �˻� �ʿ����
		AllInFrustum();
		return;
	case FRUSTUM_PARTIALLY_IN:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
		m_bCulled = false;
		break;
	case FRUSTUM_OUT:				// �������ҿ��� �������, ������� �˻� �ʿ����
		m_bCulled = true;
		return;
	}
	
	if (m_pChild[0]) 
		m_pChild[0]->FrustumCull();
	
	if (m_pChild[1]) 
		m_pChild[1]->FrustumCull();
	
	if (m_pChild[2]) 
		m_pChild[2]->FrustumCull();
	
	if (m_pChild[3]) 
		m_pChild[3]->FrustumCull();
}

int QuadTreeClass::GenerateIndex(float fLODRatio)
{
	FrustumClass::GetInstance()->MakeVectorAndPlane();
	FrustumCull();
	return GenerateTriangleIndex(0, fLODRatio);
}

bool QuadTreeClass::IsVisible(float fLODRatio)
{
	return ((m_CornerVertex[1] - m_CornerVertex[0]) <= GetLODLevel(fLODRatio));
}

float QuadTreeClass::GetDistance(D3DXVECTOR3 * pv1, D3DXVECTOR3 * pv2)
{
	  return D3DXVec3Length(&(*pv2 - *pv1));
}

int QuadTreeClass::GetLODLevel(float fLODRatio)
{
	TERRAINVERTEX * pVerticies;
	TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0,0,(void**)&pVerticies,0);

	D3DXVECTOR3 pos = *FrustumClass::GetInstance()->GetCameraPos();

	float d = GetDistance(&pVerticies[m_CenterVertex].point, &pos);

//	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

	TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();

	return std::max<int>((int)d*fLODRatio, 1);
}

void QuadTreeClass::BuildNeighborNode(QuadTreeClass * pQuad, int cx)
{
	int				n;
	int				_0, _1, _2, _3;

	for (int i = 0; i < 4; i++)
	{
		_0 = m_CornerVertex[0];
		_1 = m_CornerVertex[1];
		_2 = m_CornerVertex[2];
		_3 = m_CornerVertex[3];
		// �̿������ 4�� �ڳʰ��� ��´�.
	
		n = GetNodeIndex(i, cx, _0, _1, _2, _3);
		// �ڳʰ����� �̿������ �����͸� ���´�.
		
		if (n >= 0)
		{
			m_pNeighbor[i] = pQuad->FindNode(_0, _1, _2, _3);
		}
			
	}

	// �ڽĳ��� ���ȣ��
	if (m_pChild[0])
	{
		m_pChild[0]->BuildNeighborNode(pQuad, cx);
		m_pChild[1]->BuildNeighborNode(pQuad, cx);
		m_pChild[2]->BuildNeighborNode(pQuad, cx);
		m_pChild[3]->BuildNeighborNode(pQuad, cx);
	}
}

bool QuadTreeClass::BuildQuadTree()
{
	if (SubDivide())
	{
		TERRAINVERTEX * pVerticies;
		TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0, 0, (void**)&pVerticies, 0);

		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = pVerticies[m_CornerVertex[LEFT_TOP]].point
			- pVerticies[m_CornerVertex[RIGHT_BOTTOM]].point;
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		m_fRadius = D3DXVec3Length(&v) * 0.5f;
		m_pChild[LEFT_TOP]->BuildQuadTree();
		m_pChild[RIGHT_TOP]->BuildQuadTree();
		m_pChild[LEFT_BOTTOM]->BuildQuadTree();
		m_pChild[RIGHT_BOTTOM]->BuildQuadTree();

		TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();
	}

	return true;
}

QuadTreeClass * QuadTreeClass::FindNode(int _0, int _1, int _2, int _3)
{
	QuadTreeClass *	p = nullptr;
	// ��ġ�ϴ� ����� ��尪�� ����
	if ((m_CornerVertex[0] == _0) && (m_CornerVertex[1] == _1) && (m_CornerVertex[2] == _2) && (m_CornerVertex[3] == _3))
		return this;

	// �ڽ� ��尡 �ְ�?
	if (m_pChild[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) / 4;


		TERRAINVERTEX * pVertices;

		TerrainClass::GetInstance()->GetVertexBuffer()->Lock(0, 0, (void**)&pVertices, 0);

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pVertices[n].point.x;
		pt.y = (int)pVertices[n].point.z;

		TerrainClass::GetInstance()->GetVertexBuffer()->Unlock();

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect(&rc,(int)pVertices[m_pChild[0]->m_CornerVertex[LEFT_TOP]].point.x,
					(int)pVertices[m_pChild[0]->m_CornerVertex[LEFT_TOP]].point.z,
					(int)pVertices[m_pChild[0]->m_CornerVertex[RIGHT_BOTTOM]].point.x,
					(int)pVertices[m_pChild[0]->m_CornerVertex[RIGHT_BOTTOM]].point.z);
		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
		if (IsInRect(&rc, pt))
			return m_pChild[0]->FindNode(_0, _1, _2, _3);

		SetRect(&rc,(int)pVertices[m_pChild[1]->m_CornerVertex[LEFT_TOP]].point.x,
					(int)pVertices[m_pChild[1]->m_CornerVertex[LEFT_TOP]].point.z,
					(int)pVertices[m_pChild[1]->m_CornerVertex[RIGHT_BOTTOM]].point.x,
					(int)pVertices[m_pChild[1]->m_CornerVertex[RIGHT_BOTTOM]].point.z);
		
		if (IsInRect(&rc, pt))
			return m_pChild[1]->FindNode(_0, _1, _2, _3);

		SetRect(&rc,(int)pVertices[m_pChild[2]->m_CornerVertex[LEFT_TOP]].point.x,
					(int)pVertices[m_pChild[2]->m_CornerVertex[LEFT_TOP]].point.z,
					(int)pVertices[m_pChild[2]->m_CornerVertex[RIGHT_BOTTOM]].point.x,
					(int)pVertices[m_pChild[2]->m_CornerVertex[RIGHT_BOTTOM]].point.z);
		
		if (IsInRect(&rc, pt))
			return m_pChild[2]->FindNode(_0, _1, _2, _3);

		SetRect(&rc,(int)pVertices[m_pChild[3]->m_CornerVertex[LEFT_TOP]].point.x,
					(int)pVertices[m_pChild[3]->m_CornerVertex[LEFT_TOP]].point.z,
					(int)pVertices[m_pChild[3]->m_CornerVertex[RIGHT_BOTTOM]].point.x,
					(int)pVertices[m_pChild[3]->m_CornerVertex[RIGHT_BOTTOM]].point.z);
	
		if (IsInRect(&rc, pt))
			return m_pChild[3]->FindNode(_0, _1, _2, _3);

	}

	return nullptr;
}

int QuadTreeClass::GetNodeIndex(int ed, int cx, int & _0, int & _1, int & _2, int & _3)
{
	int		n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a;	// ���� ����� �¿�����

	switch (ed)
	{
	case EDGE_UP:	// ���� ���� �̿������ �ε���
		_0 = _a - cx * gap;
		_1 = _b - cx * gap;
		_2 = _a;
		_3 = _b;
		break;
	case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
		_0 = _c;
		_1 = _d;
		_2 = _c + cx * gap;
		_3 = _d + cx * gap;
		break;
	case EDGE_LT:	// ���� ���� �̿������ �ε���
		_0 = _a - gap;
		_1 = _a;
		_2 = _c - gap;
		_3 = _c;
		break;
	case EDGE_RT:	// ���� ���� �̿������ �ε���
		_0 = _b;
		_1 = _b + gap;
		_2 = _d;
		_3 = _d + gap;
		if ((_0 / cx) != (_a / cx)) 
			return -1;
		if ((_1 / cx) != (_b / cx)) 
			return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;	// ��� �ε���

	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) 
		return -1;

	return n;
}

void QuadTreeClass::AllInFrustum()
{
	m_bCulled = false;

	if (!m_pChild[0]) 
		return;

	m_pChild[0]->AllInFrustum();
	m_pChild[1]->AllInFrustum();
	m_pChild[2]->AllInFrustum();
	m_pChild[3]->AllInFrustum();
}

bool QuadTreeClass::IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) && (rc->bottom <= pt.y) && (pt.y <= rc->top))
		return true;

	return false;
}