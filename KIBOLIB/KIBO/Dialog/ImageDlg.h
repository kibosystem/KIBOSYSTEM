#pragma once
#include "../LIB/dib.h"

// ImageDlg ダイアログ

class ImageDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageDlg)
public:
	bool m_isSizing; //サイズ変更可能ですか？
	int  m_nAlpha;   //透明度
	CDIB m_dib; //描画ビットマップ
	CScrollBar m_scrollV;
	CScrollBar m_scrollH;
public:
	ImageDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~ImageDlg();

	virtual void SetBitmap(CDIB &dib){
		dib.Copy(m_dib);
	}
// ダイアログ データ
	enum { IDD = IDD_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
