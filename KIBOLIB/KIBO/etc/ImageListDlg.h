#pragma once

#include <afxcmn.h>
#include "../LIB/Dib.h"

//画像をリストビューで表示し、その値を表示します。
// ImageListDlg ダイアログ

#define DATA_SIZE_COL 100 //列最大
#define DATA_SIZE_ROW 100 //行最大

class ImageListDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageListDlg)
private:
	CListCtrl m_ctrList;
public:
	CDIB m_dib; //データ画像

public:
	ImageListDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~ImageListDlg();

	//表示データの書き換え
	void ResetBitmapData();

// ダイアログ データ
	enum { IDD = IDD_IMAGE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
