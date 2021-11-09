#pragma once
#include "afxwin.h"
#include "../ImageCtrl.h"
#include "../TextCtrl.h"

// HistgramDlg ダイアログ

class HistgramDlg : public CDialog
{
	DECLARE_DYNAMIC(HistgramDlg)
private:
	int m_nMaxPixels; //濃度値最大画素数
	ImageCtrl m_stHistGram;
	TextCtrl m_stKido; 
public:
	int m_nGray[MAXBYTE+1];
public:
	HistgramDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~HistgramDlg();
	
	void Init();//初期化
// ダイアログ データ
	enum {
		ID_TIMER = 100
	};
private:
	void DrawHistGram(CDC &dc);
	void ChangeHistGram(int cx, int cy);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnShowInfo(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};
