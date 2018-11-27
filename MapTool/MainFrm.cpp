
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MainFrm.h"
#include "MapView.h"
#include "MapToolView.h"
#include "AplicationClass.h"

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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	
/*
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	int	screen_x_size = rect.Width();
	int screen_y_size = rect.Height();

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 300, 300, screen_x_size, screen_y_size, SWP_NOZORDER);
*/

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMapToolView), CSize(200, 800), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(MapView), CSize(1280, 800), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	m_pView = (MapView*)m_wndSplitter.GetPane(0, 1);
	m_pMenuView = (CMapToolView*)m_wndSplitter.GetPane(0, 0);
	
	HWND tempHwnd[2];

	tempHwnd[0] = m_wndSplitter.GetPane(0, 1)->GetSafeHwnd();

	if (m_application)
	{
		m_application->Shutdown();
		delete m_application;
		m_application = nullptr;
	}

	m_application = new ApplicationClass;
	m_application->Initialize(AfxGetInstanceHandle(), tempHwnd, 1280, 1024);
	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	cs.cx = 1280;
	cs.cy = 1024;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.


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

