
// MapToolView.cpp: CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MainFrm.h"
#include "MapToolDoc.h"
#include "MapToolView.h"
#include "Base.h"
#include "TerrainClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CFormView)

BEGIN_MESSAGE_MAP(CMapToolView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_CREATE_TERRAIN, &CMapToolView::OnBnClickedCreateTerrain)
	ON_BN_CLICKED(IDC_LoadHighMapTexture, &CMapToolView::OnBnClickedLoadhighmaptexture)
	ON_BN_CLICKED(IDC_CLEAR_TERRAIN, &CMapToolView::OnBnClickedClearTerrain)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO_LIFT, &CMapToolView::OnBnClickedRadioLift)
	ON_BN_CLICKED(IDC_RADIO_FALL, &CMapToolView::OnBnClickedRadioFall)
	ON_BN_CLICKED(IDC_BRUSH_RADIO, &CMapToolView::OnBnClickedBrushRadio)
	ON_BN_CLICKED(IDC_EROSION_RADIO, &CMapToolView::OnBnClickedErosionRadio)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SaveDataFile, &CMapToolView::OnBnClickedSavedatafile)
END_MESSAGE_MAP()

// CMapToolView 생성/소멸



CMapToolView::CMapToolView()
noexcept
	: CFormView(IDD_MAPTOOL_FORM),
	m_iXNumber(0), m_iZNumber(0), m_iXSize(0), m_iZSize(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
}

void CMapToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_NUMBER, m_iXNumber);
	DDX_Text(pDX, IDC_Z_NUMBER, m_iZNumber);
	DDX_Text(pDX, IDC_X_SIZE, m_iXSize);
	DDX_Text(pDX, IDC_Z_SIZE, m_iZSize);
	DDX_Control(pDX, IDC_SLIDER_RADIUS, m_RadiusSlider);
	DDX_Control(pDX, IDC_SLIDER_STRENGTH, m_strengthSlider);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_iRadius);
	DDX_Control(pDX, IDC_EDIT_STRENGTH, m_iStrength);
	DDX_Control(pDX, IDC_RADIO_LIFT, m_bLift);
	DDX_Control(pDX, IDC_RADIO_FALL, m_bFall);
	DDX_Control(pDX, IDC_X_NUMBER, m_XEdit);
	DDX_Control(pDX, IDC_Z_NUMBER, m_ZEdit);
	DDX_Control(pDX, IDC_BRUSH_RADIO, m_BrushRadio);
	DDX_Control(pDX, IDC_EROSION_RADIO, m_ErosionRadio);
	DDX_Control(pDX, IDC_SLIDER1, m_ErosionRadius);
	DDX_Control(pDX, IDC_SLIDER2, m_erosionAmount);
	DDX_Control(pDX, IDC_RAIN_RADIUS_EDIT, m_rainRadiusEdit);
	DDX_Control(pDX, IDC_RAIN_AMOUNT_EDIT, m_rainAmountEdit);
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMapToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_RadiusSlider.SetRange(1, 50);
	m_strengthSlider.SetRange(0, 10);

	m_RadiusSlider.SetPos(1);
	m_strengthSlider.SetPos(1);

	m_ErosionRadius.SetRange(0, 50);
	m_erosionAmount.SetRange(0, 100);

	m_ErosionRadius.SetPos(5);
	m_erosionAmount.SetPos(50);

	int basic = m_RadiusSlider.GetPos();
	int strBasic = m_strengthSlider.GetPos();

	CString radStr;
	radStr.Format(L"%d", basic);

	CString stStr;
	stStr.Format(L"%d", strBasic);

	m_iRadius.SetWindowTextW(radStr);
	m_iStrength.SetWindowTextW(stStr);

	float erosionRadius = m_ErosionRadius.GetPos();
	float erosionAmount = m_erosionAmount.GetPos();

	CString eradStr;
	eradStr.Format(L"%f", erosionRadius);

	CString eAmountStr;
	eAmountStr.Format(L"%f", erosionAmount);

	m_rainRadiusEdit.SetWindowTextW(eradStr);
	m_rainAmountEdit.SetWindowTextW(eAmountStr);

	m_bLift.SetCheck(1);
	m_bFall.SetCheck(0);

	m_BrushRadio.SetCheck(1);
	m_ErosionRadio.SetCheck(0);

	int n = 129;

	CString xStr;
	CString yStr;

	xStr.Format(L"%d", n);
	yStr.Format(L"%d", n);

	m_XEdit.SetWindowTextW(xStr);
	m_ZEdit.SetWindowTextW(yStr);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->SetBrush(basic, strBasic);
	pFrame->SetDrawMode(DRAWMODE::DM_LIFT);
	pFrame->SetBRMODE(BE_BRUSH);
}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMapToolView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

//CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
//	return (CMapToolDoc*)m_pDocument;
//}
#endif //_DEBUG


// CMapToolView 메시지 처리기

void CMapToolView::OnBnClickedCreateTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->CreateTerrain(m_iXNumber, m_iZNumber, m_iXSize, m_iZSize);

	pFrame->SetErosionBrush(m_ErosionRadius.GetPos(), m_erosionAmount.GetPos());
}


void CMapToolView::OnBnClickedLoadhighmaptexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR	strFilter[] = L"AllFiles(*.*)|*.*||";
	CFileDialog	dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			CString	strPath = dlg.GetNextPathName(pos);
			TCHAR	strName[MAX_PATH] = {};

			_wsplitpath_s(strPath.GetString(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

			CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
			
			pFrame->CreateTerrain(0,0,1,1,true, strPath);
		}
	}
}



void CMapToolView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (pScrollBar != NULL && pScrollBar->m_hWnd == m_RadiusSlider.m_hWnd)
	{
		CString str;

		int fRadius = m_RadiusSlider.GetPos();

		str.Format(L"%d", fRadius);

		m_iRadius.SetWindowTextW(str);
	}

	if (pScrollBar != NULL && pScrollBar->m_hWnd == m_strengthSlider.m_hWnd)
	{
		CString str;

		int fStrength = m_strengthSlider.GetPos();

		str.Format(L"%d", fStrength);

		m_iStrength.SetWindowTextW(str);
	}

	if (pScrollBar != NULL && pScrollBar->m_hWnd == m_ErosionRadius.m_hWnd)
	{
		CString str;

		float fRadius = m_ErosionRadius.GetPos();

		str.Format(L"%f", fRadius);

		m_rainRadiusEdit.SetWindowTextW(str);
	}

	if (pScrollBar != NULL && pScrollBar->m_hWnd == m_erosionAmount.m_hWnd)
	{
		CString str;

		float fAmount = m_erosionAmount.GetPos();

		str.Format(L"%f", fAmount);

		m_rainAmountEdit.SetWindowTextW(str);
	}

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	if (m_BrushRadio.GetCheck())
		pFrame->SetBrush(m_RadiusSlider.GetPos(), m_strengthSlider.GetPos());
	else
		pFrame->SetErosionBrush(m_ErosionRadius.GetPos(), m_erosionAmount.GetPos());


	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMapToolView::OnBnClickedRadioLift()
{
	m_bLift.SetCheck(1);
	m_bFall.SetCheck(0);
	m_BrushRadio.SetCheck(1);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->SetDrawMode(DRAWMODE::DM_LIFT);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapToolView::OnBnClickedRadioFall()
{
	m_bLift.SetCheck(0);
	m_bFall.SetCheck(1);
	m_BrushRadio.SetCheck(1);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->SetDrawMode(DRAWMODE::DM_FALL);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapToolView::OnBnClickedClearTerrain()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->ClearTerrain();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapToolView::OnBnClickedBrushRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_BrushRadio.SetCheck(1);
	m_ErosionRadio.SetCheck(0);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->SetBRMODE(BRUSHOREROSION::BE_BRUSH);
}


void CMapToolView::OnBnClickedErosionRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_BrushRadio.SetCheck(0);
	m_ErosionRadio.SetCheck(1);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->SetBRMODE(BRUSHOREROSION::BE_EROSION);

	pFrame->SetErosionBrush(m_ErosionRadius.GetPos(), m_erosionAmount.GetPos());
}


int CMapToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//std::thread t(std::bind(CMapToolView::terrainThread, this));
	//t.join();
	//CWinThread *p1 = NULL;
	//AfxBeginThread(terrainThread, (CMainFrame *)AfxGetMainWnd());

	return 0;
}

void CMapToolView::OnBnClickedSavedatafile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString m_strPath;

	// CFile file;

	CFileException ex;

	CFileDialog dlg(FALSE, _T("*.dat"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("DATA Files(*.dat)|*.dat|"), NULL);

	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();

		if (m_strPath.Right(4) != ".dat")

		{

			m_strPath += ".dat";

		}
		// 에디트 박스에 있는 것을 저장한다. 

		int length = m_strPath.GetLength();
		TCHAR * fileName;

		fileName = m_strPath.GetBuffer(m_strPath.GetLength());

		if (TerrainClass::GetInstance()->IsCreated())
		{
			TerrainClass::GetInstance()->MakeDataFile(fileName);
		}

	}
}
