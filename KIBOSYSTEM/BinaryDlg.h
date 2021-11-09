#pragma once


// BinaryDlg ダイアログ

class BinaryDlg : public InputDlg
{
	DECLARE_DYNAMIC(BinaryDlg)
public:
	CDIB m_dibSrc; //2値化画像元データ
public:
	BinaryDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~BinaryDlg();

// ダイアログ データ
	enum { IDD = IDD_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeEdit();
public:
	virtual BOOL OnInitDialog();
};
