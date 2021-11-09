#pragma once
#include "afxwin.h"

// RegistryDlg ダイアログ

class RegistryDlg : public CDialog
{
	DECLARE_DYNAMIC(RegistryDlg)

private:
	CListCtrl	m_ctrlList;
public:
	RegistryDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~RegistryDlg();

// ダイアログ データ
	enum { IDD = IDD_REGISTRY };

	
	void CollectItemInt(CStringArray &strItem);//レジストリ値項目一覧収集
	void CollectItemString(CStringArray &strItem);//レジストリ文字列項目一覧収集	
	int GetHitCell(int& nRow,int& nCol);//Click座標を取得
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	CEdit m_edit;
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
