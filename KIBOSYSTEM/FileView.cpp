
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
// CWorkspaceBar ���b�Z�[�W �n���h���[

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �r���[�̍쐬:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("�t�@�C�� �r���[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// �r���[�̃C���[�W�̓ǂݍ���:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ���b�N����Ă��܂�*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ���ׂẴR�}���h���A�e�t���[���o�R�ł͂Ȃ����̃R���g���[���o�R�œn����܂�:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// �ÓI�c���[ �r���[ �f�[�^ (�_�~�[ �R�[�h) ����͂��܂�
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
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("KIBOSYSTEM �t�@�C��"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_wndFileView.InsertItem(g_lpszIniFileName, 2, 2, hRoot);
	m_wndFileView.InsertItem(_T("Init.js"), 2, 2, hRoot);
	//�R�}���h�t�H���_�̒��̃t�@�C���ꗗ��\��
	HTREEITEM hCmd = m_wndFileView.InsertItem(_T("COMMAND"), 0, 0, hRoot);

	CStringArray aryCommandPath;
	ReadCommandMenu(aryCommandPath); //�R�}���h�ꗗ�ǂݍ���
	for (int i = 0; i<aryCommandPath.GetSize(); i++) {
		CString strFileName = ::GetFileName(aryCommandPath[i]); //�R�}���h�t�@�C���^�C�g��
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
		// �N���b�N���ꂽ���ڂ̑I��:
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
	AfxMessageBox(_T("�v���p�e�B..."));

}

void CFileView::OnFileOpen()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnFileOpenWith()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnDummyCompile()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditCut()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditCopy()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditClear()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ���b�N����܂���*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�r�b�g�}�b�v��ǂݍ��߂܂���ł���: %x\n"), uiBmpId);
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
		HTREEITEM hItem = m_wndFileView.GetSelectedItem(); //�I�����ꂽ�A�C�e���擾

		if (hItem != NULL) {
			if (m_wndFileView.ItemHasChildren(hItem)){
				return FALSE;
			}
			CString strItem = m_wndFileView.GetItemText(hItem);
			HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
			CString strParent = m_wndFileView.GetItemText(hParent);
			CString strPath;			
			if (strParent.MakeUpper() == L"COMMAND") { //COMMAND �I��
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
