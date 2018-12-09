
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
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView() noexcept
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

			//_wsplitpath_s(strPath.GetString(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

			CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
			pFrame->CreateTerrain(0,0,1,1,true,strPath.GetString());
		//	m_WorldTileList.AddString(strName);

			/*char	strNameMulti[MAX_PATH] = {};

			WideCharToMultiByte(CP_ACP, 0, strName, -1, strNameMulti, lstrlen(strName), 0, 0);

			CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();*/
			

			// 풀경로를 이용하여 이미지를 로드한다.
		/*	CTexture*	pTex = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strNameMulti,
				strPath.GetString());

			SAFE_RELEASE(pTex);*/
		}
	}
}
