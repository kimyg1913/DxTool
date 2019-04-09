#include "Base.h"
#include "TemplateSingleton.h"

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

class QuadTreeClass : public TemplateSingleton<QuadTreeClass>
{
	enum ENUM_CORNER
	{
		LEFT_TOP,
		RIGHT_TOP,
		LEFT_BOTTOM,
		RIGHT_BOTTOM
	};

	enum ENUM_FRUSTUM_STATE
	{
		FRUSTUM_OUT,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_COMPLETELY_IN,
	};

	enum
	{
		EDGE_UP,
		EDGE_DN,
		EDGE_LT,
		EDGE_RT
	};

public:
	QuadTreeClass();
	QuadTreeClass(QuadTreeClass * pParent);
	virtual ~QuadTreeClass();

	bool Init(int xSize, int ySize);
	int GenerateIndex(float fLODRatio);

private:
	void Build();
	QuadTreeClass * AddChild(int leftTopCorner, int rightTopCorner, int leftBottomCorner, int rightBottomCorner);
	bool SubDivide();
	void SetCorner(int leftTopCorner, int rightTopCorner, int leftBottomCorner, int rightBottomCorner);
	void DestoryChild();

	int IsInFrustum();
	void FrustumCull();
	int GenerateTriangleIndex(int nTriangles, float fLodRatio);
	bool IsVisible(float fLODRatio);
	float GetDistance(D3DXVECTOR3 * pv1, D3DXVECTOR3 * pv2);
	int GetLODLevel(float fLODRatio);

	//LOD±Õ¿­ ¹æÁö
	void BuildNeighborNode(QuadTreeClass * pQuad, int cx);
	bool BuildQuadTree();
	QuadTreeClass * FindNode(int _0, int _1, int _2, int _3);
	int GetNodeIndex(int ed, int cx, int & _0, int & _1, int & _2, int & _3);
	void AllInFrustum();
	bool IsInRect(RECT* rc, POINT pt);

private:
	int m_CenterVertex;
	int m_CornerVertex[4];
	QuadTreeClass * m_pChild[4];
	QuadTreeClass * m_pParent;
	QuadTreeClass * m_pNeighbor[4];
	float m_fRadius;
	bool m_bCulled;

};

