// KiboFrame.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KiboFrame.h"
#include "KiboDoc.h"
#include "enum.h"
#include "./SCript/App.h"
#include "./LIB/WinAppPlus.h"
#include "./LIB/functions.h"


// KiboFrame

static KIBOFrame * s_pFrame = NULL; //KIBOFrame
									//
inline KIBOFrame * GetFrame()
{
	return s_pFrame;
}
//Main Window Check
BOOL IsWindowOK()
{
	CWnd *pWnd = AfxGetMainWnd();
	if (pWnd != NULL) {
		return (pWnd->GetSafeHwnd() && pWnd->IsWindowVisible());
	}
	return FALSE;
}


IMPLEMENT_DYNCREATE(KIBOFrame, CFrameWndEx)

KIBOFrame::KIBOFrame()
{
	s_pFrame = this;
}

KIBOFrame ::~KIBOFrame()
{
}

//Script File実行
BOOL KIBOFrame::ScriptExec(CString strFilePath)
{
	BOOL isOK = FALSE;
	m_mapIsScrptExec.SetAt(strFilePath, (void*)TRUE);
	::SetCurrentDirectory(GetApp()->GetAppPath()); //カレントディレクトリ設定
	CString strCaption = App::GetInstance()->GetProfileString(L"Caption"); //キャプション指定文字列はありますか？
	if (strCaption.IsEmpty()) { //キャプション指定文字列がない場合
		SetWindowText(strFilePath);//Caption変更
	}
	if (GetDoc()->ScriptFileOpen(strFilePath)) { //Script File 実行
												 //スクリプトファイル実行成功
		isOK = TRUE;
	}
	m_mapIsScrptExec.SetAt(strFilePath, (void*)FALSE);
	return isOK;
}

//スクリプト連続処理ファイル選択
void KIBOFrame::OnScriptTimer()
{
	CFileDialog dlg(TRUE, _T("js"), _T("*.js"), NULL, _T("Script File (*.js)|*.js||"));
	if (dlg.DoModal() == IDOK) {
		ScriptStart(dlg.GetPathName());
	}
}

//スクリプトファイル連続処理
void KIBOFrame::ScriptStart(CString &strPath)
{
	m_strScriptPath = strPath; //連続処理用スクリプトファイルパス記憶
	SetTimer(TIMER_SCRIPT, 10, NULL); //スクリプト連続処理用タイマー
}

//スクリプトファイル繰り返し処理
void KIBOFrame::ScriptRepeat(CString &strPath, int nElapse)
{
	CFileFind find;
	if (find.FindFile(strPath)) { //ファイルが存在する時だけ
		m_strScriptRepeat = strPath; //連続処理用スクリプトファイルパス記憶
		SetTimer(TIMER_REPEAT_SCRIPT, nElapse, NULL); //スクリプト連続処理用タイマー	
	}
}


BEGIN_MESSAGE_MAP(KIBOFrame, CFrameWndEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// KiboFrame メッセージ ハンドラー

BOOL KIBOFrame::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN) { //キーボードチェック
	//	int nVK = (int)pMsg->wParam; //キーコード取得
	//	CString strFilePath;
	//	if (m_mapKeyFilePath.Lookup(nVK, strFilePath)) { //Fileが指定されている場合は
	//		ScriptExec(strFilePath); //Script File 実行
	//	}
	//}
	App *pApp = App::GetInstance();

	if (pApp != NULL && TranslateAccelerator(this->GetSafeHwnd(), pApp->m_hAccel, pMsg)){
		return TRUE;
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}


//ドングルチェック関数
static BOOL CheckDongle()
{
	int nRet = Exec(SW_HIDE,L"KIBOSTART.exe", L"");
	return (nRet == 1104411044);
	return TRUE;
}


BOOL KIBOFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	BOOL isOK = CFrameWndEx::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
	if (isOK && !CheckDongle()) {
		::ErrorMsg(L"USB認証が出来ないため10分後に終了致します。");
		this->SetTimer(TIMER_FORCED_CLOSE,1000*60*10,NULL);
		this->SetTimer(TIMER_CLOSE,1000*60*10*2,NULL);//念のため
	}
	return TRUE;
}


void KIBOFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case TIMER_SCRIPT: //タイマースクリプトの場合
		KillTimer(TIMER_SCRIPT); //タイマーを複数呼ばないようにする
		ScriptExec(m_strScriptPath);//Script File 実行
		break;
	case TIMER_REPEAT_SCRIPT://繰り返し専用スクリプトの実行
		ScriptExec(m_strScriptRepeat); //繰り返しScript File 実行
		break;
	case TIMER_CLOSE: //タイマーによる終了待ちの場合
		PostMessage(WM_CLOSE);
		break;
#ifdef FORCED_CLOSE
	case TIMER_FORCED_CLOSE:
		::ErrorMsg(L"認証が出来ないため終了致します。");
		PostMessage(WM_CLOSE);
		break;
#endif
	}
	CFrameWndEx::OnTimer(nIDEvent);
}
