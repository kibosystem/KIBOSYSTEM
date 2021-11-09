
// KIBOSYSTEMView.cpp : CKIBOSYSTEMView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "KIBOSYSTEM.h"
#endif

#include "KIBOSYSTEMDoc.h"
#include "KIBOSYSTEMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKIBOSYSTEMView

IMPLEMENT_DYNCREATE(CKIBOSYSTEMView, CKIBOView)

BEGIN_MESSAGE_MAP(CKIBOSYSTEMView, CKIBOView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_RENDER, OnRender)	//レンダリング要求
END_MESSAGE_MAP()

// CKIBOSYSTEMView コンストラクション/デストラクション

CKIBOSYSTEMView::CKIBOSYSTEMView()
{
	// TODO: 構築コードをここに追加します。

}

CKIBOSYSTEMView::~CKIBOSYSTEMView()
{
}

BOOL CKIBOSYSTEMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CKIBOSYSTEMView 描画

void CKIBOSYSTEMView::OnDraw(CDC* pDC)
{
	CKIBOSYSTEMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CKIBOView::OnDraw(pDC);
}
//レンダリング要求
LRESULT CKIBOSYSTEMView::OnRender(WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_RENDER, WM_RENDER, PM_REMOVE)) { //他のメッセージを削除
		;
	}
	BOOL isDraw = (BOOL)wParam;
	return Render(isDraw);
}

// CKIBOSYSTEMView 印刷


void CKIBOSYSTEMView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKIBOSYSTEMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CKIBOSYSTEMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CKIBOSYSTEMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CKIBOSYSTEMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKIBOSYSTEMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKIBOSYSTEMView 診断

#ifdef _DEBUG
void CKIBOSYSTEMView::AssertValid() const
{
	CView::AssertValid();
}

void CKIBOSYSTEMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKIBOSYSTEMDoc* CKIBOSYSTEMView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKIBOSYSTEMDoc)));
	return (CKIBOSYSTEMDoc*)m_pDocument;
}
#endif //_DEBUG


// CKIBOSYSTEMView メッセージ ハンドラー


BOOL CKIBOSYSTEMView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CKIBOSYSTEMView::OnSize(UINT nType, int cx, int cy)
{
	CKIBOView::OnSize(nType, cx, cy);
	Render(FALSE);
}
