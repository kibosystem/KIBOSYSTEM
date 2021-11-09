#pragma once
#include "../resource.h"
#include "../LIB/DIB.h"
#include "afxwin.h"
class CameraEx;

// ImageViewDlg 画像表示ダイアログ

class ImageViewDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageViewDlg)
private:
	CameraEx *m_pCamera;	//カメラ
	CDIB m_dib;				//表示画像
public:
	ImageViewDlg(CameraEx *pCamera,CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~ImageViewDlg();

// ダイアログ データ
	enum { IDD = IDD_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_scrollV;
	CScrollBar m_scrollH;
};