// KIBODoc.cpp : CKIBODoc クラスの実装
//

#include "stdafx.h"
#include "enum.h"
#include "KIBO.h"
#include "KIBODoc.h"
#include "./LIB/fileplus.h"
#include "./Script/ImageDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static TCHAR s_lpwszInclude[] = _T("#include");

// CKIBODoc

IMPLEMENT_DYNCREATE(CKIBODoc, CDocument)

BEGIN_MESSAGE_MAP(CKIBODoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKIBODoc, CDocument)
END_DISPATCH_MAP()

// メモ: VBA からタイプ セーフなバインドをサポートするために、IID_IKIBO のサポートを追加します。
//  この IID は、.IDL ファイルのディスパッチ インターフェイスへアタッチされる 
//  GUID と一致しなければなりません。

// {415E9623-5D0C-4C33-BA7A-4B83B246CA24}
static const IID IID_IKIBO =
{ 0x415E9623, 0x5D0C, 0x4C33, { 0xBA, 0x7A, 0x4B, 0x83, 0xB2, 0x46, 0xCA, 0x24 } };

BEGIN_INTERFACE_MAP(CKIBODoc, CDocument)
	INTERFACE_PART(CKIBODoc, IID_IKIBO, Dispatch)
END_INTERFACE_MAP()

static CKIBODoc* s_pCKIBODoc = NULL;
CKIBODoc* GetDoc(){return s_pCKIBODoc;}

// CKIBODoc コンストラクション/デストラクション

CKIBODoc::CKIBODoc()
{
	s_pCKIBODoc = this;

	EnableAutomation();
	AfxOleLockApp();
	m_Script.Prepare(); //コマンド実行
}

CKIBODoc::~CKIBODoc()
{	
	AfxOleUnlockApp();
}

BOOL CKIBODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()){
		return FALSE;
	}
	return TRUE;
}

void CKIBODoc::OnCloseDocument()
{
	m_Script.Close();//以前のコマンドを停止
	CDocument::OnCloseDocument();
}

//スクリプトは実行中ですか？
BOOL CKIBODoc::IsScriptRuning()
{ 
	return m_Script.m_isRuning;
}

//スクリプトの実行
void CKIBODoc::Execute()
{ 
	m_Script.Run( m_strScript );	
}

//スクリプト強制終了
void CKIBODoc::ScriptClose(){ 
	m_Script.Close(); 
}


//コマンド配列にスクリプトコマンド文字列を収集
BOOL CKIBODoc::CollectScriptCommand(CString& strFilePath,CStringArray &strAryCommand)
{
	strAryCommand.RemoveAll();

	CFileFind find;
	if( !find.FindFile(strFilePath) ){ //ファイルの存在確認
		return FALSE;
	}

	//スクリプトファイル読み込み
	FilePlus file;
	if( file.Open(strFilePath, FilePlus::modeUnicodeIn) ){
		BOOL isStartOK = FALSE;
		CString strLine;
		while( file.ReadString(strLine) ){
			if( strLine.Find(L"START:") >= 0){//スクリプト開始
				isStartOK = TRUE;
				break;
			}
		}
		if( !isStartOK ){ //スクリプト存在確認
			//::ErrorMsg(_T("START:タグを確認してください\n%s"),strFilePath);
			::ErrorMsg(_T("スクリプトファイル\n%s\nを交換してください。"),strFilePath);
		}
		m_strScript.Empty();
		//スクリプトファイル読み込み
		while( file.ReadString(strLine) ){			
			if(strLine.Find(L"END:") >= 0){ //スクリプト終了
				break;
			}
			CString strCommand;
			if( DeleteComment( strLine,strCommand ) ){ //コメント削除文字列のみ抽出
				strAryCommand.Add(strCommand);
			}
		}
		return TRUE;
	}	
	return FALSE;
}

//Includeファイル名将取得
CString CKIBODoc::GetIncludeFile(CString &strLine)
{
	int nStart = -1;	//ファイル名開始位置
	int nEnd = -1;		//ファイル名終了位置
	for(int i=0;i<strLine.GetLength();i++){
		TCHAR tc = strLine.GetAt(i); //1文字取得
		if( tc == '"' ){
			if( nStart < 0 ){ 
				nStart = i+1; //ファイル名開始位置取得
			}else{
				nEnd = i-1;
				break;
			}
		}
	}
	if( nStart > 0 && nEnd > 0 ){
		return strLine.Mid(nStart,nEnd-nStart+1);
	}
	return L"";
}

//Script File 実行
BOOL CKIBODoc::ScriptFileOpen(CString& strFilePath)
{
	if( m_strPrevFilePath != strFilePath ){ //以前読み込んだファイルと違う時だけ
		//▼m_strScriptの作成
		CStringArray strAryCommand; //コマンド配列
		if( !CollectScriptCommand(strFilePath,strAryCommand) ){ //コマンド配列にスクリプトコマンド文字列を収集
			::ErrorMsg(_T("スクリプトファイル%sの読み込みに失敗しました。"),strFilePath);
			return FALSE;
		}
		CString strLine;
		//include処理
		CStringArray strAryInclude; //Includeファイルコマンド
		for(int i=0;i<strAryCommand.GetSize();i++){
			strLine = strAryCommand[i];
			if( strLine.Find(s_lpwszInclude) >= 0){ //文字列差し替えの必要あり
				strFilePath = GetIncludeFile(strLine); //Includeファイル名将取得
				if( !CollectScriptCommand(strFilePath,strAryInclude) ){ //コマンド配列にスクリプトコマンド文字列を収集
					::ErrorMsg(_T("スクリプトファイル%sの読み込みに失敗しました。"),strFilePath);
					return FALSE;
				}
				//includeファイル読み込み成功
				strAryCommand.InsertAt(i+1,&strAryInclude); //コマンド文字列挿入
				i += (int)strAryInclude.GetSize();
			}
		}	
		//スクリプト文字列の作成
		for(int i=0;i<strAryCommand.GetSize();i++){
			//TRACE(_T("最終文字列:%s\n"),strAryCommand[i]);
			if( strAryCommand[i].Find(s_lpwszInclude) >= 0){ //二度目のincludeは削除
				continue; //コメント扱い
			}
			if( DeleteComment( strAryCommand[i],strLine ) ){ //コメント削除文字列のみ抽出
				m_strScript += strLine;
			}
		}
		m_strPrevFilePath = strFilePath; //以前のファイルパスを記憶
	}
	//スクリプト実行
	Execute(); //実行
	return TRUE;
}

//コメント削除文字列を渡す
BOOL CKIBODoc::DeleteComment( CString &strIn,CString &strOut )
{
	strIn = strIn.Trim(); //空白削除
	int nCmt = strIn.Find(L"//"); //コメント検索
	if( nCmt > 0 ){ //コメント部削除
		strIn = strIn.Left(nCmt);
	}else if( nCmt == 0 ){
		return FALSE; //コメント行
	}
	strOut = strIn; //コメント削除文字列作成
	return TRUE;
}
