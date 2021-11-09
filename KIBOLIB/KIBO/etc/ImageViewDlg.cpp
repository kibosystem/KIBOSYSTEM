// ImageViewDlg.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include "ImageViewDlg.h"
#include "CameraEx.h"

// ImageViewDlg �_�C�A���O

IMPLEMENT_DYNAMIC(ImageViewDlg, CDialog)

ImageViewDlg::ImageViewDlg(CameraEx *pCamera,CWnd* pParent /*=NULL*/)
	: CDialog(ImageViewDlg::IDD, pParent)
{
	m_pCamera = pCamera;
}

ImageViewDlg::~ImageViewDlg()
{
}

//������
BOOL ImageViewDlg::OnInitDialog()
{
	if(m_pCamera != NULL){
		SetWindowText( m_pCamera->GetCameraName() );
	}
	return TRUE;
}

void ImageViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_V, m_scrollV);
	DDX_Control(pDX, IDC_SCROLLBAR_H, m_scrollH);
}

BEGIN_MESSAGE_MAP(ImageViewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// ImageViewDlg ���b�Z�[�W �n���h��

//�T�C�Y�ύX
void ImageViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( GetSafeHwnd() != NULL ){
		if( m_dib.GetWidth() != cx && m_dib.GetHeight() != cy ){ //�T�C�Y���ς���Ƃ�
			m_dib.Create(cx,cy);
		}
	}
}

//�w�i�`��
BOOL ImageViewDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void ImageViewDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

//�ʏ�`��
void ImageViewDlg::OnPaint()
{
	CPaintDC dc(this);
	m_dib.Draw(&dc,0,0,m_dib.GetWidth(),m_dib.GetHeight());
}

void ImageViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	 CameraEx *pCamera = m_pCamera;
	 if(pCamera != NULL){
		pCamera->Capture();//�J�����摜�擾
		m_dib.CopyDib(pCamera->GetDib(),m_dib.GetWidth(),m_dib.GetHeight());//�摜��w�i�摜�Ƃ��ē]��
		Invalidate(FALSE);		
	 }
}
