// ListViewDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KIBO.h"
#include "./LIB/globalmgr.h"
#include "./LIB/color.h"
#include "ListViewDlg.h"


// ListViewDlg �_�C�A���O

IMPLEMENT_DYNAMIC(ListViewDlg, CDialog)

ListViewDlg::ListViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ListViewDlg::IDD, pParent)
{

}

ListViewDlg::~ListViewDlg()
{
}

void ListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
}


BEGIN_MESSAGE_MAP(ListViewDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// ListViewDlg ���b�Z�[�W �n���h��

//�T�C�Y�ύX
void ListViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	
}

//������
BOOL ListViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctrlList.InsertColumn(0,_T("����"),LVCFMT_RIGHT,2048); 
	m_ctrlList.InsertItem(0,L""); //���b�Z�[�W�\��
	m_ctrlList.ShowScrollBar(SB_BOTH);
	return TRUE; 
}

//�I�[�i�[�h���[�`��
void ListViewDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;
	LOGBRUSH lb; //�u���V
	lb.lbColor = COLOR_GREEN; //�F�w��
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID;
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
	CBrush *pBrOld  = pDC->SelectObject(pBrush);
	pDC->Rectangle(rcItem);
	//��n��
	pDC->SelectObject(pBrOld);	
}

//�ݒ�
void ListViewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth  = 2048;
	lpMeasureItemStruct->itemHeight = 1536;
	//CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

