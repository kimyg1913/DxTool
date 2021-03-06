
// MapToolView.h: CMapToolView 클래스의 인터페이스
//

#pragma once

class CMapToolView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMapToolView() noexcept;
	DECLARE_DYNCREATE(CMapToolView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MAPTOOL_FORM };
#endif

// 특성입니다.
public:
	//CMapToolDoc* GetDocument() const;

// 작업입니다.
public:
	int m_iXNumber;
	int m_iZNumber;
	int m_iXSize;
	int m_iZSize;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCreateTerrain();
	afx_msg void OnBnClickedLoadhighmaptexture();
	CSliderCtrl m_RadiusSlider;
	CSliderCtrl m_strengthSlider;
	CEdit m_iRadius;
	CEdit m_iStrength;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CButton m_bLift;
	CButton m_bFall;
	afx_msg void OnBnClickedRadioLift();
	afx_msg void OnBnClickedRadioFall();
	CEdit m_XEdit;
	CEdit m_ZEdit;
	afx_msg void OnBnClickedClearTerrain();
	afx_msg void OnBnClickedBrushRadio();
	afx_msg void OnBnClickedErosionRadio();
	CButton m_BrushRadio;
	CButton m_ErosionRadio;
	CSliderCtrl m_ErosionRadius;
	CSliderCtrl m_erosionAmount;
	CEdit m_rainRadiusEdit;
	CEdit m_rainAmountEdit;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSavehighmap();
	afx_msg void OnBnClickedSavedatafile();
};

//#ifndef _DEBUG  // MapToolView.cpp의 디버그 버전
//inline CMapToolDoc* CMapToolView::GetDocument() const
//   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
//#endif

