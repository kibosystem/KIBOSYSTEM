// MainFrm.cpp : CKIBOFrame クラスの実装
//

#include "stdafx.h"
#include "KIBO.h"
#include "enum.h"
#include "KIBOFrm.h"
#include "KIBODoc.h"
#include "KIBOView.h"
//#include "OutputView.h"
#include "CaptureBuffer.h"
#include "./LIB/FilePlus.h"
#include "./LIB/macro.h"
#include "./SCRIPT/UsbIO.h"
#include "./SCRIPT/App.h"
#include "./SCRIPT/ImageDevice.h"
#include "./SCript/SerialPort.h"


#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "kernel32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CKIBOFrame* s_pFrame = NULL; //KIBOFrame

//
inline CKIBOFrame* GetFrame()
{
	return s_pFrame;
}

//Main Window Check
BOOL IsWindowOK()
{ 
	CWnd *pWnd = AfxGetMainWnd();
	if( pWnd != NULL ){
		return ( pWnd->GetSafeHwnd() && pWnd->IsWindowVisible() );
	}
	return FALSE;
}

// CKIBOFrame

IMPLEMENT_DYNCREATE(CKIBOFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CKIBOFrame, CFrameWnd)
//	ON_COMMAND_RANGE(IDM_USER,IDM_USER_MAX,OnCommandFile)	

	//実験用コマンド
	//ON_COMMAND(IDM_HISTGRAM			,OnHistgram)		//ヒストグラム
	//ON_COMMAND(IDM_SAVE_VIEW			,OnSaveBitmap)		//表示画像の保存
	//ON_COMMAND(IDM_SAVE_CAMERA_IMAGE	,OnSaveCameraImage)	//カメラ画像の保存
	//ON_COMMAND(IDM_SAVE_OUTPUT		,OnSaveOutput)		//出力の保存
	//ON_COMMAND(ID_FILE_OPEN			,OnFileOpen)
	//ON_COMMAND(IDM_SCRIPT_TIMER		,OnScriptTimer)
	//ON_COMMAND(IDM_IMAGE_VALUE_DLG	,OnImageValueDlg)	//画像データ表示ダイアログ
	//ON_COMMAND(IDM_EDIT_COMMAND		,OnEditCommand)	

	//業務用コマンド
//	ON_MESSAGE(ID_LENSCHECK,OnLensCheck) //TEST

	//ON_MESSAGE(ID_LENS,OnLens) //TEST
	//ON_MESSAGE(ID_BACK,OnBack)//背景撮影

//	ON_MESSAGE(ID_ELEM_DIV	,OnElementDib) //要素分割
//	ON_MESSAGE(ID_LENGTH	,OnLength) //距離取得
//	ON_MESSAGE(ID_ELEM_DIFF,OnElemDiff) //要素内微分	

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CKIBOFrame コンストラクション/デストラクション

CKIBOFrame::CKIBOFrame()
{
	s_pFrame = this;
	m_pStatusBar = NULL;
}

CKIBOFrame::~CKIBOFrame()
{
}

//表示領域の高さを決定する
void CKIBOFrame::SetViewHeight(int nH)
{
	//初期サイズ決定
	if( m_wndSplitter.GetSafeHwnd() != NULL ){
		int cyCur,cyMin;
		m_wndSplitter.GetRowInfo(0,cyCur,cyMin); //行の情報を取得
		m_wndSplitter.SetRowInfo(0,nH,cyMin);
		m_wndSplitter.RecalcLayout();
	}	
}
//表示領域の幅を決定する
void CKIBOFrame::SetViewWidth(int nW)
{
	//初期サイズ決定
	if( m_wndSplitter.GetSafeHwnd() != NULL ){
		int cyCur,cyMin;
		m_wndSplitter.GetColumnInfo(0,cyCur,cyMin); //列の情報を取得
		m_wndSplitter.SetColumnInfo(0,nW,cyMin);
		m_wndSplitter.RecalcLayout();
	}
}

//コマンド一覧読み込み
void CKIBOFrame::ReadCommandMenu(CStringArray &aryMenuName)
{
	aryMenuName.RemoveAll(); //clear
	CString strPath = GetApp()->GetAppPath(); //アプリケーソン起動時のパス
	CFileFind find;
	BOOL isOK = find.FindFile(strPath + _T("\\Command\\*.js"));
	while ( isOK ){
		isOK = find.FindNextFile();
	   if(!find.IsDirectory() ){
			aryMenuName.Add( find.GetFilePath() ); //FilePathセット			
	   }
	}	
}

BOOL CKIBOFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ){
		return FALSE;
	}
	return TRUE;
}

//マスクリージョン 
//virtual 仮想関数
//必ずオーバーライドされています
CRgn& CKIBOFrame::GetMaskRgn()
{
	static CRgn rgnTmp; //テンポラリリージョン
	return rgnTmp;
}
//GUI入力値設定
BOOL CKIBOFrame::SetInputName(CString &strName,int nInput)
{
	return FALSE;
}
//Dialog BarにText出力
BOOL CKIBOFrame::OutputDlgBarText(LPCTSTR lpszText)
{
	return FALSE;
}
BOOL CKIBOFrame::DlgBarMsg(int nNo,LPCTSTR lpszText,COLORREF color/*=COLOR_WHITE*/)
{
	return FALSE;
}
// CKIBOFrame メッセージ ハンドラ

//表示画像の保存
void CKIBOFrame::OnSaveBitmap()
{
	GetView()->SaveBitmap( GetApp()->GetAppPath());
}

//カメラ画像の保存
void CKIBOFrame::OnSaveCameraImage()
{
	GetView()->SaveCameraImage(GetApp()->GetAppPath());
}

//出力の保存
void CKIBOFrame::OnSaveOutput()
{
	CFileDialog dlg(FALSE,_T("txt"),_T("*.txt"),NULL,_T("Text File (*.txt)|*.txt||"));
	if( dlg.DoModal() == IDOK ){
//		GetOutputView()->SaveTextFile( dlg.GetPathName() );
	}
}

//Script File Open
void CKIBOFrame::OnFileOpen()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	if( dlg.DoModal() == IDOK ){
		ScriptExec(dlg.GetPathName());
	}
}
//Script File実行
BOOL CKIBOFrame::ScriptExec(CString strFilePath)
{
	BOOL isOK = FALSE;
	m_mapIsScrptExec.SetAt(strFilePath,(void*)TRUE);
	::SetCurrentDirectory( GetApp()->GetAppPath() ); //カレントディレクトリ設定
	CString strCaption = App::GetInstance()->GetProfileString(L"Caption"); //キャプション指定文字列はありますか？
	if( strCaption.IsEmpty() ){ //キャプション指定文字列がない場合
		SetWindowText( strFilePath );//Caption変更
	}
	if( GetDoc()->ScriptFileOpen( strFilePath) ){ //Script File 実行
		//スクリプトファイル実行成功
		isOK = TRUE;
	}
	m_mapIsScrptExec.SetAt(strFilePath,(void*)FALSE);
	return isOK;
}

//スクリプト連続処理ファイル選択
void CKIBOFrame::OnScriptTimer()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	if( dlg.DoModal() == IDOK ){
		ScriptStart(dlg.GetPathName());
	}
}

//スクリプトファイル連続処理
void CKIBOFrame::ScriptStart(CString &strPath)
{
	m_strScriptPath = strPath; //連続処理用スクリプトファイルパス記憶
	SetTimer(TIMER_SCRIPT,10,NULL); //スクリプト連続処理用タイマー
}

//スクリプトファイル繰り返し処理
void CKIBOFrame::ScriptRepeat(CString &strPath,int nElapse)
{
	CFileFind find;
	if( find.FindFile(strPath) ){ //ファイルが存在する時だけ
		m_strScriptRepeat = strPath; //連続処理用スクリプトファイルパス記憶
		SetTimer(TIMER_REPEAT_SCRIPT,nElapse,NULL); //スクリプト連続処理用タイマー	
	}
}


//コマンドファイル実行
void CKIBOFrame::OnCommandFile(UINT unID)
{
	int nIndex = (int)unID - IDM_USER;
	if( 0 <= nIndex && nIndex < m_aryCommandPath.GetSize() ){
		CString strPath = m_aryCommandPath[nIndex]; //Script File Path取得		
		ScriptExec(strPath);
	}
}

BOOL CKIBOFrame::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){ //キーボードチェック
		int nVK = (int)pMsg->wParam; //キーコード取得
		CString strFilePath;
		if( m_mapKeyFilePath.Lookup(nVK,strFilePath) ){ //Fileが指定されている場合は
			ScriptExec(strFilePath); //Script File 実行
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

//編集
void CKIBOFrame::OnEditCommand()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	dlg.m_ofn.lpstrInitialDir = GetApp()->GetAppPath() + "\\Command";
	if( dlg.DoModal() == IDOK ){
		ShellExecute(NULL,TEXT("edit"),dlg.GetPathName(),NULL,NULL,SW_SHOW);
	}
	::SetCurrentDirectory(GetApp()->GetAppPath());
}

//実行中のスクリプトは存在しますか？
BOOL CKIBOFrame::IsScriptExec()
{
	CString key;
	void *pVal; //アドレス化した値
	for(POSITION pos = m_mapIsScrptExec.GetStartPosition(); pos != NULL;){
		m_mapIsScrptExec.GetNextAssoc(pos, key,pVal);
		if( pVal != NULL ){ //実行中のスクリプトは存在する
			return TRUE;
		}
	}
	//実行中のスクリプトは存在しません
	return FALSE;
}

//タイマー処理
void CKIBOFrame::OnTimer(UINT_PTR nIDEvent)
{	
	switch(nIDEvent){
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
	CFrameWnd::OnTimer(nIDEvent);	
}

//終了時のスクリプト実行中確認
void CKIBOFrame::OnClose()
{
	CKIBOView *pView = GetView();
	CKIBODoc  *pDoc  = GetDoc();

	GetApp()->SetEnd(TRUE); //アプリケーション終了フラグ

	//タイマー終了
	KillTimer(TIMER_SCRIPT);//スクリプト連続処理用タイマー
	KillTimer(TIMER_REPEAT_SCRIPT); //繰り返しスクリプト終了

	if( pDoc->IsScriptRuning() || IsScriptExec() ){ //スクリプト実行中
		SetTimer(TIMER_CLOSE,100,NULL); //終了を後回しにする		

		CString key;
		void *pVal; //アドレス化した値
		for(POSITION pos = m_mapIsScrptExec.GetStartPosition(); pos != NULL;){
			m_mapIsScrptExec.GetNextAssoc(pos, key,pVal);		
			m_mapIsScrptExec.SetAt(key,FALSE); //前スクリプト終了
		}
		return ; //いったん抜ける
	}
	CFrameWnd::OnClose();
}

//ドングルチェック関数
static BOOL CheckDongle()
{
	//WinAppPlus *pApp = GetApp();
	//int nRun = pApp->GetProfileInt(L"RUN",L"int",0);
	//CString strRun = pApp->GetProfileString(L"RUN",L"string");
	//if( strRun != L"START" || nRun != 110 ){ //終了タイマー
	//	return FALSE;
	//}
	//pApp->WriteProfileInt(L"RUN",L"int",0);
	//pApp->WriteProfileString(L"RUN",L"string",L"END");
	//return TRUE;

	//USBドングルチェック

	int nRet = Exec(SW_HIDE,L"KIBOSTART.exe",L"");
	return (nRet==11044);
}

BOOL CKIBOFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	BOOL isOK = CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
	if( isOK && !CheckDongle() ){
		//::exit(0);
		//////////////////////////TEST//////////////////////////////////////
		//::ErrorMsg(L"USB認証が出来ないため10分後に終了致します。");
		//this->SetTimer(TIMER_FORCED_CLOSE,1000*60*10,NULL);
		//this->SetTimer(TIMER_CLOSE,1000*60*10*2,NULL);//念のため
		//////////////////////////TEST//////////////////////////////////////
	}
	return isOK;
}
