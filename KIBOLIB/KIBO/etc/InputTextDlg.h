#pragma once

// InputTextDlg ダイアログ

class InputTextDlg : public CDialog
{
	DECLARE_DYNAMIC(InputTextDlg)
private:
	CString m_strInput; //入力文字列
	CString m_strCaption; //Windowタイトル
public:
	InputTextDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~InputTextDlg();

// ダイアログ データ
	enum { IDD = IDD_INPUTTEXTDLG };
	CString& GetInputString(){return m_strInput;} //入力文字列取得
	void SetInputStringArray(CStringArray &sAry); //複数行文字列入力
	void SetCaption(CString s){m_strCaption = s;} //Windowタイトル設定
	CString& GetCaption(){return m_strCaption;} //Windowタイトル取得
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
