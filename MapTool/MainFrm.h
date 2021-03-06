
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "AplicationClass.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:
	ApplicationClass * m_application;
// 작업입니다.
public:

	bool CreateTerrain(int xN, int zN, int xS, int zS, bool isMapLoad = false, LPCWSTR str = nullptr);
	void ClearTerrain();
	void SetBrush(float radius, float strength);
	void GetBrush(float & radius, float & strength);
	void SetDrawMode(DRAWMODE mode);
	void SetBRMODE(BRUSHOREROSION mode);
	void SetErosionBrush(float radius, float amount);
// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	class MapView*	m_pView;
	class CMapToolView* m_pMenuView;

	float m_iBrushRadius;
	float m_iBrushStrength;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


