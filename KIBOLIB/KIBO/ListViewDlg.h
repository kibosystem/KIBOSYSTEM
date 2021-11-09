#pragma once
#include "afxcmn.h"

// ListViewDlg ダイアログ

class ListViewDlg : public CDialog
{
	DECLARE_DYNAMIC(ListViewDlg)

protected:
	CListCtrl m_ctrlList;
	CStringArray m_strAry;
public:
	ListViewDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~ListViewDlg();

// ダイアログ データ
	enum { IDD = IDD_LISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

////Listに文字画像を表示するダイアログ
//class CharPictureDlg : public ListViewDlg
//{
//	CharPictureDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
//	virtual ~CharPictureDlg();	
//};