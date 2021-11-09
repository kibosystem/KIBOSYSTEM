
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "KIBOSYSTEM.h"
#include "KIBOSYSTEM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ビューの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// ビューのイメージの読み込み:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 静的ツリー ビュー データ (ダミー コード) を入力します
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("KIBOSYSTEM ファイル"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_wndFileView.InsertItem(g_lpszIniFileName, 2, 2, hRoot);
	m_wndFileView.InsertItem(_T("Init.js"), 2, 2, hRoot);
	//コマンドフォルダの中のファイル一覧を表示
	HTREEITEM hCmd = m_wndFileView.InsertItem(_T("COMMAND"), 0, 0, hRoot);

	CStringArray aryCommandPath;
	ReadCommandMenu(aryCommandPath); //コマンド一覧読み込み
	for (int i = 0; i<aryCommandPath.GetSize(); i++) {
		CString strFileName = ::GetFileName(aryCommandPath[i]); //コマンドファイルタイトル
		m_wndFileView.InsertItem(strFileName, 2, 2, hCmd);
	}
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hCmd, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// クリックされた項目の選択:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("プロパティ..."));

}

void CFileView::OnFileOpen()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnFileOpenWith()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnDummyCompile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCopy()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditClear()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("ビットマップを読み込めませんでした: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




BOOL CFileView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR pNMHDR = (LPNMHDR)lParam;
	if (pNMHDR->code == NM_DBLCLK) {
		HTREEITEM hItem = m_wndFileView.GetSelectedItem(); //選択されたアイテム取得

		if (hItem != NULL) {
			if (m_wndFileView.ItemHasChildren(hItem)){
				return FALSE;
			}
			CString strItem = m_wndFileView.GetItemText(hItem);
			HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
			CString strParent = m_wndFileView.GetItemText(hParent);
			CString strPath;			
			if (strParent.MakeUpper() == L"COMMAND") { //COMMAND 選択
				strPath.Format(L"%s/Command/%s", GetApp()->GetAppPath(), strItem.GetBuffer());
			}
			else {
				strPath.Format(L"%s/%s", GetApp()->GetAppPath(), strItem.GetBuffer());
			}

			::Exec(SW_NORMAL,L"notepad.exe", strPath.GetBuffer());
		}		
	}
	return CDockablePane::OnNotify(wParam, lParam, pResult);
}
