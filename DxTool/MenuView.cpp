
// MenuView.cpp: CMenuView 클래스의 구현
//

#include "stdafx.h"
#include "DxTool.h"

#include "DxToolDoc.h"
#include "MenuView.h"
#include "CMenuDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CTreeView)

BEGIN_MESSAGE_MAP(CMenuView, CTreeView)
END_MESSAGE_MAP()


// CMenuView 생성/소멸

CMenuView::CMenuView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pMenuDlg = new CMenuDialog;
	m_pMenuDlg->Create(IDD_CMenuDialog,this);
	
	CRect r;
	ScreenToClient(r);
	
	m_pMenuDlg->SetWindowPos(NULL,r.left,r.top,r.Width(), r.Height(), SWP_SHOWWINDOW);
}

CMenuView::~CMenuView()
{
}

BOOL CMenuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.


	return CTreeView::PreCreateWindow(cs);
}

void CMenuView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: TreeView를 항목으로 채우려면
	//  GetTreeCtrl()을 호출하여 해당 tree 컨트롤을 직접 액세스하십시오.
}


// CMenuView 진단

#ifdef _DEBUG
void CMenuView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMenuView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CDxToolDoc* CMenuView::GetDocument() // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDxToolDoc)));
	return (CDxToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMenuView 메시지 처리기
