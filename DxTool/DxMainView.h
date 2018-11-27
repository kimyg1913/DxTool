
// DxMainView.h: CDxMainView 클래스의 인터페이스
//

#pragma once


class CDxMainView : public CListView
{
protected: // serialization에서만 만들어집니다.
	CDxMainView() noexcept;
	DECLARE_DYNCREATE(CDxMainView)

// 특성입니다.
public:
	CDxToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CDxMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DxMainView.cpp의 디버그 버전
inline CDxToolDoc* CDxMainView::GetDocument() const
   { return reinterpret_cast<CDxToolDoc*>(m_pDocument); }
#endif

