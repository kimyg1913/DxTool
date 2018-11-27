
// DxMapToolView.cpp: CDxMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DxMapTool.h"
#endif

#include "DxMapToolDoc.h"
#include "DxMapToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDxMapToolView

IMPLEMENT_DYNCREATE(CDxMapToolView, CFormView)

BEGIN_MESSAGE_MAP(CDxMapToolView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CDxMapToolView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDxMapToolView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDxMapToolView::OnBnClickedButton3)
END_MESSAGE_MAP()

// CDxMapToolView 생성/소멸

CDxMapToolView::CDxMapToolView() noexcept
	: CFormView(IDD_DXMAPTOOL_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CDxMapToolView::~CDxMapToolView()
{
}

void CDxMapToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDxMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CDxMapToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CDxMapToolView 인쇄

BOOL CDxMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CDxMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CDxMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CDxMapToolView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CDxMapToolView 진단

#ifdef _DEBUG
void CDxMapToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDxMapToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
//
//CDxMapToolDoc* CDxMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDxMapToolDoc)));
//	return (CDxMapToolDoc*)m_pDocument;
//}
#endif //_DEBUG


// CDxMapToolView 메시지 처리기


void CDxMapToolView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"1");
}


void CDxMapToolView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"2");
}


void CDxMapToolView::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"3");
}
