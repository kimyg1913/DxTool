
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MainFrm.h"
#include "MapView.h"
#include "MapToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_application = NULL;
}

CMainFrame::~CMainFrame()
{
	if (m_application)
	{
		m_application->Shutdown();
		delete m_application;
		m_application = nullptr;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

bool CMainFrame::CreateTerrain(int xN, int zN, int xS, int zS, bool isMapLoad, LPCWSTR str)
{
	if (!m_application->InitTerrain(xN, zN, xS, zS, isMapLoad, str))
		return false;

	return true;
}

void CMainFrame::ClearTerrain()
{
	m_application->ClearTerrain();
}

void CMainFrame::SetBrush(float radius, float strength)
{
	m_iBrushRadius = radius;
	m_iBrushStrength = strength;
}

void CMainFrame::GetBrush(float & radius, float & strength)
{
	radius = m_iBrushRadius;
	strength = m_iBrushStrength;
}

void CMainFrame::SetDrawMode(DRAWMODE mode)
{
	m_application->SetDrawMode(mode);
}

void CMainFrame::SetBRMODE(BRUSHOREROSION mode)
{
	m_application->SetBrushOrErosion(mode);
}

void CMainFrame::SetErosionBrush(float radius, float amount)
{
	m_application->SetRainRadius(radius);
	m_application->SetRainAmount(amount);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	CSize size1(DIRECT_WND_WIDTH, DIRECT_WND_HEIGHT);
	CSize size2(MAIN_WND_WIDTH - DIRECT_WND_WIDTH, DIRECT_WND_HEIGHT);

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	
	RECT rc;
	GetClientRect(&rc);

	int iSizeX = rc.right - rc.left;
	int iSizeY = rc.bottom - rc.top;

	int iMenuXSize = MENU_X_SIZE;

	m_wndSplitter.SetColumnInfo(0, iSizeX - iMenuXSize, 0);
	m_wndSplitter.SetColumnInfo(1, iMenuXSize, 0);

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMapToolView), size2, pContext) ||
		!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(MapView), size1, pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	
	m_pView = (MapView*)m_wndSplitter.GetPane(0, 0);
	m_pMenuView = (CMapToolView*)m_wndSplitter.GetPane(0, 1);
	
	m_pMenuView->SetWindowPos(NULL, DIRECT_WND_WIDTH, rc.top, size2.cx, size2.cy, SWP_NOREPOSITION);

	HWND tempHwnd[2];

	tempHwnd[0] = m_wndSplitter.GetPane(0, 0)->GetSafeHwnd();
	tempHwnd[1] = m_wndSplitter.GetPane(0, 1)->GetSafeHwnd();

	if (m_application)
	{
		m_application->Shutdown();
		delete m_application;
		m_application = nullptr;
	}

	m_application = new ApplicationClass;

	if (!m_application->Initialize(AfxGetInstanceHandle(), tempHwnd, DIRECT_WND_WIDTH, DIRECT_WND_HEIGHT))
	{
		m_application->Shutdown();
		delete m_application;
		m_application = nullptr;
		return false;
	}
	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.cx = MAIN_WND_WIDTH;
	cs.cy = MAIN_WND_HEIGHT;
	cs.style &= ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);	// Dialog size don't change.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
