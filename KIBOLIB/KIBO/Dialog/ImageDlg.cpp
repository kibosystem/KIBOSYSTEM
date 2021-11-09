// ImageDlg.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "ImageDlg.h"


// ImageDlg �_�C�A���O

IMPLEMENT_DYNAMIC(ImageDlg, CDialog)

ImageDlg::ImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ImageDlg::IDD, pParent)
{
	m_isSizing = false; //�T�C�Y�ύX�\�ł����H
	m_nAlpha = 128;		//�����x
}

ImageDlg::~ImageDlg()
{
}

void ImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_V, m_scrollV);
	DDX_Control(pDX, IDC_SCROLLBAR_H, m_scrollH);
}

//������
BOOL ImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_scrollV.ShowWindow(SW_HIDE);
	m_scrollH.ShowWindow(SW_HIDE);
	if( m_isSizing ){
		ModifyStyle(WS_SIZEBOX,0);//�g��k������
	}
	if( m_nAlpha > 0 ){
		ModifyStyleEx(0,WS_EX_LAYERED);//����������
		SetLayeredWindowAttributes( RGB(0,0,0), m_nAlpha, LWA_ALPHA);//�����x�ݒ�
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(ImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ImageDlg ���b�Z�[�W �n���h��

//�`��
void ImageDlg::OnPaint()
{
	CRect rect;
	GetClientRect(&rect);
	CPaintDC dc(this); // device context for painting
	m_dib.StretchDraw(&dc,rect.Width(),rect.Height());
}

//�w�i�`��
BOOL ImageDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void ImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
//	m_dib.Create(cx,cy);
}

