#pragma once


// ChaReaderBar ダイアログ

class ChaReaderBar : public CDialogBar
{
	DECLARE_DYNAMIC(ChaReaderBar)
private:

public:
	ChaReaderBar();   // 標準コンストラクタ
	virtual ~ChaReaderBar();

	void Init();//初期化	

// ダイアログ データ
	enum { IDD = IDD_CHAREADER_BAR };

private: //内部関数	
	int GetInt(int nID){return GetDlgItemInt(nID);}
	CSpinButtonCtrl* GetSpin(int nID){return (CSpinButtonCtrl*)GetDlgItem(nID);}
	CButton* GetRadio(int nID){return (CButton*)GetDlgItem(nID);}
	CComboBox* GetCB(int nID){return (CComboBox*)GetDlgItem(nID);}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop ();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnBnClickedBlack();
	afx_msg void OnBnClickedPink();
	afx_msg void OnBnClickedHistgram();
	afx_msg void OnBnClickedInput();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedClibration();
	afx_msg void OnEnChangeEditTh();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnCbnSelchangeComboMove();
	afx_msg void OnCbnSelchangeComboSkip();
	afx_msg void OnCbnSelchangeComboSpaceTop();
	afx_msg void OnCbnSelchangeComboFont();
	afx_msg void OnCbnSelchangeComboSpaceLeft();
	afx_msg void OnCbnSelchangeComboSpaceLeft2();
	afx_msg void OnCbnSelchangeComboEnd();
	afx_msg void OnBnClickedCheckSelrect();
};
