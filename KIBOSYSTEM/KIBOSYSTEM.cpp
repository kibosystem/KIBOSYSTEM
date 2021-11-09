
// KIBOSYSTEM.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "KIBOSYSTEM.h"
#include "MainFrm.h"
#include "ScriptApp.h"
#include "KIBOSYSTEMDoc.h"
#include "KIBOSYSTEMView.h"
#include "ColorBitmap.h"
#include "BitmapCamera.h"
#include "DirectXCamera.h"
#include "Mp4Camera.h"
#include "ICube.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKIBOSYSTEMApp

BEGIN_MESSAGE_MAP(CKIBOSYSTEMApp, WinAppPlus)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
END_MESSAGE_MAP()


// CKIBOSYSTEMApp コンストラクション

CKIBOSYSTEMApp::CKIBOSYSTEMApp()
{
	m_bHiColorIcons = TRUE;

	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("KIBOSYSTEM.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CKIBOSYSTEMApp オブジェクトです。

CKIBOSYSTEMApp theApp;


// CKIBOSYSTEMApp 初期化

BOOL CKIBOSYSTEMApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	WinAppPlus::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(0);  // 標準の INI ファイルのオプションをロードします (MRU を含む)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKIBOSYSTEMDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CKIBOSYSTEMView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//初期化処理
	CString strPath;
	strPath.Format(_T("%s\\Init.js"), GetAppPath());
	if (!GetDoc()->ScriptFileOpen(strPath)) { //Script File 実行
		::ErrorMsg(_T("初期化処理が正しく行われませんでした。\n 初期化ファイルを交換して再起動してください。"));
	}
	return TRUE;
}

int CKIBOSYSTEMApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return WinAppPlus::ExitInstance();
}

// CKIBOSYSTEMApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CKIBOSYSTEMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CKIBOSYSTEMApp のカスタマイズされた読み込みメソッドと保存メソッド

void CKIBOSYSTEMApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CKIBOSYSTEMApp::LoadCustomState()
{
}

void CKIBOSYSTEMApp::SaveCustomState()
{
}

// CKIBOSYSTEMApp メッセージ ハンドラー


//Scriptクラス実体作成
ScriptBase* CKIBOSYSTEMApp::CreateInstanceScript(CString& strName)
{
	if (strName == L"App") {
		return ScriptApp::CreateInstance();
	}
	else if (strName == L"ImageDevice") {
		CString strPath;
		strPath.Format(L"%s\\%s", theApp.GetAppPath(), g_lpszIniFileName);
		int nType = ::GetPrivateProfileInt(L"CAMERA", L"TYPE", 2, strPath);

		if (nType == CAMERA_TYPE_DIRECTX) {
			return DXCamera::CreateInstance();
		}
		else if (nType == CAMERA_TYPE_BITMAP) {
			return BitmapCamera::CreateInstance();
		}
		//else if (nType == CAMERA_TYPE_MP4) {
		//	return Mp4Camera::CreateInstance();
		//}
		else if (nType == CAMERA_TYPE_ICUBE) {
#ifdef USE_ICUBE
			return ICube::CreateInstance();
#else
			return NULL;
#endif
		}				
	}
	else if (strName == L"ColorBitmap") {
		return ColorBitmap::CreateInstance();
	}
	return NULL;
}


