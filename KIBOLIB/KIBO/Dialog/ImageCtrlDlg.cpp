// ImageCtrlDlg.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "../LIB/globalmgr.h"
#include "../LIB/color.h"
#include "ImageCtrlDlg.h"


// ImageCtrlDlg �_�C�A���O

IMPLEMENT_DYNAMIC(ImageCtrlDlg, CDialog)

ImageCtrlDlg::ImageCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ImageCtrlDlg::IDD, pParent)
{

}

ImageCtrlDlg::~ImageCtrlDlg()
{
}

void ImageCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
}


BEGIN_MESSAGE_MAP(ImageCtrlDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


//������
BOOL ImageCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctrlList.InsertColumn(0,_T("����"),LVCFMT_RIGHT,2048); 
	m_ctrlList.InsertItem(0,L"");
	return TRUE;
}

// ImageCtrlDlg ���b�Z�[�W �n���h��

//�T�C�Y�ύX
void ImageCtrlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( cx > 0 && cy > 0 && IsWindow(m_ctrlList.GetSafeHwnd()) ){
		m_ctrlList.MoveWindow(0,0,cx,cy);
	}
}

//�I�[�i�[�h���[�`��
void ImageCtrlDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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
void ImageCtrlDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth  = 2048;
	lpMeasureItemStruct->itemHeight = 1536;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

