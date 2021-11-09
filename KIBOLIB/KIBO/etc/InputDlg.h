#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// 入力用 ダイアログ

class InputDlg : public CDialog
{
	DECLARE_DYNAMIC(InputDlg)
private:
	int m_nInput; //入力値
	int m_nMin;	//最小値
	int m_nMax;	//最大値
public:
	CSpinButtonCtrl m_spinInput;
	CEdit m_editInput;
public:
	InputDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~InputDlg();

// ダイアログ データ
	enum { IDD = IDD_INPUT };

	int  GetInput(){return m_nInput;}	//入力値取得
	void SetInput(int n){m_nInput=n;}	//入力初期値設定

	int GetMin(){return m_nMin;}//最小値
	int GetMax(){return m_nMax;}//最大値
	void SetRange(int nMin,int nMax){//レンジ設定
		m_nMin = nMin;	//最小値
		m_nMax = nMax;	//最大値	
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
