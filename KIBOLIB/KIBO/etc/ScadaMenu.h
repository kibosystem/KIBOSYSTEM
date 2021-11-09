#pragma once

#include "../LIB/Dom.h"

//KIBOに追加するメニュー

class ScadaMenu
{
private: //メンバ
	CString m_strFilePath; //xmlファイルパス
public:
	ScadaMenu(CString &strFilePath);
	~ScadaMenu(void);

	CString& GetFilePath(){return m_strFilePath;}  //xmlファイルパス取得
	BOOL LoadMenu(CMenu *pMenu,CString &strXPath); //ポップアップメニューの読み込み

private:

	//SubMenu取得
	CMenu* FindMenuItem(CMenu *pMenu, CString &strMenu);

};
