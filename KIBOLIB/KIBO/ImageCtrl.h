#pragma once
#include "./LIB/dib.h"

#define WM_POSITION (WM_APP + 1) //コントロールから座標を親へ通知

// ImageCtrl

class ImageCtrl : public CStatic
{
	DECLARE_DYNAMIC(ImageCtrl)
public:
	CDIB m_dib;
public:
	ImageCtrl();
	virtual ~ImageCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
