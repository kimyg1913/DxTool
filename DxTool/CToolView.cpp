// CToolView.cpp: 구현 파일
//

#include "stdafx.h"
#include "DxTool.h"
#include "CToolView.h"


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

CToolView::CToolView()
{

}

CToolView::~CToolView()
{
}


BEGIN_MESSAGE_MAP(CToolView, CScrollView)
END_MESSAGE_MAP()


// CToolView 그리기

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CToolView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolView 메시지 처리기
