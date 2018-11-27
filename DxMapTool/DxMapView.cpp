// DxMapView.cpp: 구현 파일
//

#include "stdafx.h"
#include "DxMapTool.h"
#include "DxMapView.h"


// DxMapView

IMPLEMENT_DYNCREATE(DxMapView, CListView)

DxMapView::DxMapView()
{

}

DxMapView::~DxMapView()
{
}

BEGIN_MESSAGE_MAP(DxMapView, CListView)
END_MESSAGE_MAP()


// DxMapView 진단

#ifdef _DEBUG
void DxMapView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void DxMapView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// DxMapView 메시지 처리기
