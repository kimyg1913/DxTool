// MapView.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapView.h"


// MapView

IMPLEMENT_DYNCREATE(MapView, CView)

MapView::MapView()
{

}

MapView::~MapView()
{
}

BEGIN_MESSAGE_MAP(MapView, CView)
END_MESSAGE_MAP()


// MapView 그리기

void MapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// MapView 진단

#ifdef _DEBUG
void MapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// MapView 메시지 처리기
