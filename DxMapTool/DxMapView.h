#pragma once


// DxMapView 보기

class DxMapView : public CListView
{
	DECLARE_DYNCREATE(DxMapView)

protected:
	DxMapView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~DxMapView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


