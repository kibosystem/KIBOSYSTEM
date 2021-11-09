#pragma once


// KIBOFrame  フレーム

class KIBOFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(KIBOFrame)
	// 属性
public:
	CString m_strScriptPath;	//実行中のスクリプトファイルパス(切り替え可能）
	CString m_strScriptRepeat;  //繰り返し中のスクリプトファイルパス(切り替え不可）
	CMap<int, int, CString, CString&> m_mapKeyFilePath; //Virtual Key ファイル
	CMapStringToPtr m_mapIsScrptExec;//スクリプトファイルは実行中ですか？
									 // 操作
public:
	BOOL ScriptExec(CString strFilePath); //指定スクリプトファイル実行
	void ScriptStart(CString &strPath);	//スクリプトファイル連続処理	
	void ScriptRepeat(CString &strPath, int nElapse);//スクリプトファイル繰り返し処理
	void OnScriptTimer();	
	virtual void SetStatusBarText(LPCTSTR lpszText) {}

protected:
	KIBOFrame();           // 動的生成で使用される protected コンストラクター
	virtual ~KIBOFrame();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


extern KIBOFrame * GetFrame();
extern BOOL IsWindowOK(); //Main Window Check