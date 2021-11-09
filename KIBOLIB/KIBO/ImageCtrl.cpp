// ImageCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KIBO.h"
#include "ImageCtrl.h"


// ImageCtrl

IMPLEMENT_DYNAMIC(ImageCtrl, CStatic)

ImageCtrl::ImageCtrl()
{

}

ImageCtrl::~ImageCtrl()
{
}

BEGIN_MESSAGE_MAP(ImageCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// ImageCtrl メッセージ ハンドラ

void ImageCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_dib.Draw(&dc,0,0,m_dib.GetWidth(),m_dib.GetHeight(),0,0);
}

void ImageCtrl::OnSize(UINT nType, int cx, int cy)
{
	if( cx > 0 ){
		m_dib.Create(cx,cy);
	}
}

void ImageCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();
	if(pParent != NULL){
		pParent->SendMessage(WM_POSITION,point.x,point.y);
	}
}
