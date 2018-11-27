
// DxMainView.cpp: CDxMainView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DxTool.h"
#endif

#include "DxToolDoc.h"
#include "DxMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDxMainView

IMPLEMENT_DYNCREATE(CDxMainView, CListView)

BEGIN_MESSAGE_MAP(CDxMainView, CListView)
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

// CDxMainView 생성/소멸

CDxMainView::CDxMainView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CDxMainView::~CDxMainView()
{
}

BOOL CDxMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CListView::PreCreateWindow(cs);
}

void CDxMainView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: GetListCtrl()을 호출하여 해당 list 컨트롤을 직접 액세스함으로써
	//  ListView를 항목으로 채울 수 있습니다.
}


// CDxMainView 진단

#ifdef _DEBUG
void CDxMainView::AssertValid() const
{
	CListView::AssertValid();
}

void CDxMainView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDxToolDoc* CDxMainView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDxToolDoc)));
	return (CDxToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CDxMainView 메시지 처리기
void CDxMainView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: 사용자가 창의 뷰 스타일을 변경했을 때 반응하는 코드를 추가합니다.
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);
}
