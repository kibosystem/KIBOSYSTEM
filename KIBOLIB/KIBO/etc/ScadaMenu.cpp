#include "../StdAfx.h"
#include "ScadaMenu.h"

ScadaMenu::ScadaMenu(CString &strFilePath)
{
	m_strFilePath = strFilePath; //xmlファイルパスセット
}

ScadaMenu::~ScadaMenu(void)
{
}

//SubMenu取得
CMenu* ScadaMenu::FindMenuItem(CMenu *pMenu, CString &strMenu)
{
   ASSERT(pMenu);
   ASSERT(::IsMenu(pMenu->GetSafeHmenu()));

   int count = pMenu->GetMenuItemCount();
   for (int i = 0; i < count; i++){
      CString str;
	  if(pMenu->GetMenuString(i, str, MF_BYPOSITION) ){
		  if( str.Compare(strMenu) == 0){
			return pMenu->GetSubMenu(i);
		  }
	  }
   }
   return NULL;
}


//ポップアップメニューの読み込み
BOOL ScadaMenu::LoadMenu(CMenu *pMenu,CString &strXPath) 
{
	Dom dom(GetFilePath());

	CString strAttr = _T("name");
	CString strName = dom.GetAttributeText(strXPath,strAttr); //popup Menu 名称

	//MENU作成
	if( pMenu != NULL ){
		pMenu->InsertMenu(-1,MF_BYPOSITION|MF_STRING|MF_POPUP,0,strName);//Popup追加
		CMenu *pPopup = FindMenuItem(pMenu,strName); //メニュー位置特定
		if( pPopup != NULL ){
			int nCnt = dom.GetCount(strXPath);
			nCnt++;
		}
	}

	return TRUE;
}