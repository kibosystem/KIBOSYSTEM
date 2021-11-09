// KIBODoc.h : CKIBODoc クラスのインターフェイス
//


#pragma once

#include "./Script/ScriptCall.h"

class CKIBODoc : public CDocument
{
private: //メンバ変数
	ScriptCall m_Script; //スクリプト実行クラス
	CString m_strScript; //スクリプト文字列
	CString m_strPrevFilePath; //依然読み込んだファイル

protected: // シリアル化からのみ作成します。
	CKIBODoc();
	DECLARE_DYNCREATE(CKIBODoc)

// 属性
public:
	
// 操作
public:
	BOOL IsScriptRuning();	//スクリプトは実行中ですか？
	void Execute();			//スクリプトの実行
	void ScriptClose();		//スクリプト強制終了
	BOOL ScriptFileOpen(CString& strFilePath);

// オーバーライド
public:
	virtual BOOL OnNewDocument();

// 実装
public:
	virtual ~CKIBODoc();

protected:
	//コマンド配列にスクリプトコマンド文字列を収集
	BOOL CollectScriptCommand(CString& strFilePath,CStringArray &strAryCommand);

	//Includeファイル名将取得
	CString GetIncludeFile(CString &strLine);

	//コメント削除文字列を渡す
	BOOL DeleteComment( CString &strIn,CString &strOut );

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

	// 生成された OLE ディスパッチ割り当て関数

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual void OnCloseDocument();
};

extern CKIBODoc* GetDoc();
