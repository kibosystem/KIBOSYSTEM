
// KIBOSYSTEM.h : KIBOSYSTEM アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CKIBOSYSTEMApp:
// このクラスの実装については、KIBOSYSTEM.cpp を参照してください。
//

class CKIBOSYSTEMApp : public WinAppPlus
{
public:
	CKIBOSYSTEMApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual ScriptBase *CreateInstanceScript(CString& strName); //Scriptクラス実体作成

};

extern CKIBOSYSTEMApp theApp;
