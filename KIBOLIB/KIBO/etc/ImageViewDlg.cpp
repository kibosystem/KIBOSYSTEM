// ImageViewDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "ImageViewDlg.h"
#include "CameraEx.h"

// ImageViewDlg ダイアログ

IMPLEMENT_DYNAMIC(ImageViewDlg, CDialog)

ImageViewDlg::ImageViewDlg(CameraEx *pCamera,CWnd* pParent /*=NULL*/)
	: CDialog(ImageViewDlg::IDD, pParent)
{
	m_pCamera = pCamera;
}

ImageViewDlg::~ImageViewDlg()
{
}

//初期化
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

// ImageViewDlg メッセージ ハンドラ

//サイズ変更
void ImageViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( GetSafeHwnd() != NULL ){
		if( m_dib.GetWidth() != cx && m_dib.GetHeight() != cy ){ //サイズが変わっとき
			m_dib.Create(cx,cy);
		}
	}
}

//背景描画
BOOL ImageViewDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void ImageViewDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

//通常描画
void ImageViewDlg::OnPaint()
{
	CPaintDC dc(this);
	m_dib.Draw(&dc,0,0,m_dib.GetWidth(),m_dib.GetHeight());
}

void ImageViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	 CameraEx *pCamera = m_pCamera;
	 if(pCamera != NULL){
		pCamera->Capture();//カメラ画像取得
		m_dib.CopyDib(pCamera->GetDib(),m_dib.GetWidth(),m_dib.GetHeight());//画像を背景画像として転送
		Invalidate(FALSE);		
	 }
}
