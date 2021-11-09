// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once
#include <afxtempl.h>
#include "afxwinappex.h"
#include "../Script/ScriptBase.h"
#pragma comment(lib,"version.lib")

// WinAppPlus

class WinAppPlus : public CWinAppEx
{
	DECLARE_DYNCREATE(WinAppPlus)
protected:
	bool					m_isEnd; //終了フラグ
	WSAData					m_wsaData;		// Windows ソケット初期化情報
	CString					m_strAppPath;	//アプリケーション起動時のパス
	HANDLE					m_hMutex;		//多重起動防止Mutex
public:
	WinAppPlus();           // 動的生成で使用される protected コンストラクタ
	virtual ~WinAppPlus();
	
	CString GetCurrentPath();//カレントディレクトリの絶対パスを取得
	CString& GetAppPath(){return m_strAppPath;} //アプリケーソン起動時のパス
	CString GetIPAddr();		//IPアドレスを取得します。
	BOOL SetMacAddress(CStringArray& astrMacAddress);
	BOOL CheckDevice();//ビデオカードの拡大縮小の機能をチェックします。
	BOOL CheckUSBMemory();//USBメモリのチェックをします。	

	void SetEnd(bool b){m_isEnd=b;} //終了フラグ設定
	bool IsEnd(){return m_isEnd;} //終了しますか？
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ScriptBase *CreateInstanceScript(CString& strName){return NULL;} //Scriptクラス実体作成
protected:	
	
protected:
	DECLARE_MESSAGE_MAP()
};


inline WinAppPlus* GetApp(){return (WinAppPlus*)::AfxGetApp();}