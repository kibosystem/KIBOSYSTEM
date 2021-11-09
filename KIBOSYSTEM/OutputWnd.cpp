
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, OutputWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OutputWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �^�u�t���E�B���h�E�̍쐬:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("�^�u�t���o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// �o�̓y�C���̍쐬:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutput1.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutput2.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutput3.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// �ꗗ�E�B���h�E���^�u�Ɋ��蓖�Ă܂�:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutput1, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutput2, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutput3, strTabName, (UINT)2);


	//m_wndOutput1.AddString(_T("�o�͂P�f�[�^�������ɕ\������܂��B"));
	//m_wndOutput2.AddString(_T("�o�͂Q�f�[�^�������ɕ\������܂��B"));
	//m_wndOutput3.AddString(_T("�o�͂R�f�[�^�������ɕ\������܂��B"));

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	OutputWnd::OnSize(nType, cx, cy);

	// �^�u �R���g���[���́A�N���C�A���g�̈�S�̂��J�o�[����K�v������܂�:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::OutputText(int nNo, LPWSTR lpsz, ...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	OutputText(nNo, strFormat);
}
void COutputWnd::OutputText(int nNo, CString &strFormat)
{
	if (nNo == 1) {
		m_wndOutput1.AddString(strFormat);
	}
	else if (nNo == 2) {
		m_wndOutput2.AddString(strFormat);
	}
	else if (nNo == 3) {
		m_wndOutput3.AddString(strFormat);
	}
	else {
		m_wndOutput1.AddString(_T("Error:�o�͔ԍ����قȂ�܂��B"));
	}	
}
void COutputWnd::OutputText(LPWSTR lpsz, ...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);

	OutputText(strFormat);
}
void COutputWnd::OutputText(CString &strFormat)
{
	OutputText(1, strFormat);
}
void COutputWnd::UpdateFonts()
{
	m_wndOutput1.SetFont(&afxGlobalData.fontRegular);
	m_wndOutput2.SetFont(&afxGlobalData.fontRegular);
	m_wndOutput3.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::Clear()
{
	m_wndOutput1.ResetContent();
	m_wndOutput2.ResetContent();
	m_wndOutput3.ResetContent();
}
/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ���b�Z�[�W �n���h���[

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
