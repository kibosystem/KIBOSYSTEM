// MainFrm.h : CKIBOFrame クラスのインターフェイス
//
#pragma once
#pragma comment(lib,"winmm.lib")

#include <mmsystem.h>
#include "COMMANDID.h"
#include "./LIB/typedef.h"

#define IDM_USER	 10000 //コマンドメニュー
#define IDM_USER_MAX 10100 //最大コマンドミュー番号
#define VIEW_HEIGHT		830		//カメラ画像表示領域高さ初期値

class CKIBOFrame : public CFrameWnd
{
protected: //メンバ
	CString m_strScriptPath;	//実行中のスクリプトファイルパス(切り替え可能）
	CString m_strScriptRepeat;  //繰り返し中のスクリプトファイルパス(切り替え不可）
	CSortStringArray m_aryCommandPath;
	CMapStringToPtr m_mapIsScrptExec;//スクリプトファイルは実行中ですか？
	CSplitterWnd m_wndSplitter; //水平方向の分割ウィンドウ
public:
	CMap<int,int,CString,CString&> m_mapKeyFilePath; //Virtual Key ファイル
	CStatusBar *m_pStatusBar;

// 属性
public:
	CSplitterWnd* GetSplitterPtr(){return &m_wndSplitter;}	
	void SetStatusBarPtr(CStatusBar *p){m_pStatusBar=p;} //ステータスバーアドレス記録
	CStatusBar* GetStatusBarPtr(){return m_pStatusBar;}	//ステータスバーアドレス取得

protected: // シリアル化からのみ作成します。
	CKIBOFrame();
	DECLARE_DYNCREATE(CKIBOFrame)

// 操作
public:
	void SetViewHeight(int nH);//表示領域の高さを決定する		
	void SetViewWidth(int nW);//表示領域の幅を決定する

	BOOL ScriptExec(CString strFilePath); //指定スクリプトファイル実行
	void ScriptStart(CString &strPath);	//スクリプトファイル連続処理	
	void ScriptRepeat(CString &strPath, int nElapse);//スクリプトファイル繰り返し処理

	BOOL CreateImageViewDlg();			//画像表示ダイアログの作成
	void SetStatusBarText(int nIndex,LPCWSTR lpsz,...);
	void SetStatusBarText(CStringArray &strAry);

protected:
	void ReadCommandMenu(CStringArray &aryMenuName);//コマンド一覧読み込み

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual CRgn& GetMaskRgn(); //マスクリージョン取得
	virtual BOOL SetInputName(CString &strName,int nValue);
	virtual BOOL OutputDlgBarText(LPCTSTR lpszText);//Dialog BarにText出力
	virtual BOOL DlgBarMsg(int nNo,LPCTSTR lpszText,COLORREF color=RGB(255,255,255)); //Message Color指定
// 実装
public:
	virtual ~CKIBOFrame();

private:
	BOOL IsScriptExec();//実行中のスクリプトは存在しますか？
// 生成された、メッセージ割り当て関数
protected:	
	void OnCommandFile(UINT unID);//コマンドファイル実行

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	//afx_msg void OnHistgram();

	afx_msg void OnCameraInitEx();	//カメラの初期化
	afx_msg void OnSaveBitmap();
	afx_msg void OnSaveCameraImage();
	afx_msg void OnBmpLabeling();	
	afx_msg void OnFileOpen();//Script File Open
	afx_msg void OnScriptTimer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCamraSecond();

	//	afx_msg void OnSaveData();
	afx_msg void OnSaveOutput();
	afx_msg void OnClose();
	afx_msg void OnEditCommand();
};

extern CKIBOFrame* GetFrame(); 
extern BOOL IsWindowOK();
