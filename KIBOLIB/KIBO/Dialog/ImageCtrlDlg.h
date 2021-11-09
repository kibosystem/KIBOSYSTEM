#pragma once

#include "../LIB/dib.h"

// ImageCtrlDlg ダイアログ

class ImageCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageCtrlDlg)
private:
	CDIB *m_pDib; //表示ビットマップポインタ
	CListCtrl m_ctrlList;
public:
	ImageCtrlDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~ImageCtrlDlg();

// ダイアログ データ
	enum { IDD = IDD_IMAGECTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);	
};
