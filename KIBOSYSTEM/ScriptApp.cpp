#include "StdAfx.h"
#include "MainFrm.h"
#include "ScriptApp.h"

ScriptApp::ScriptApp(void)
{
}

ScriptApp::~ScriptApp(void)
{
}

//クラス実体の作成
ScriptApp* ScriptApp::CreateInstance()
{
	ScriptApp *pInstance = new ScriptApp;
	return pInstance;
}

//中心点表示
BOOL ScriptApp::CenterPoint(BOOL isVisible)
{
	//CMainFrame *pFrame = GetMainFrame();
	//if( pFrame != NULL ){
	//	GuiDlgBar &bar = pFrame->GetGuiBar();
	//	bar.GetCheck(IDC_CENTER)->SetCheck(isVisible);
	//	if( isVisible ){			
	//		bar.ShowCenter(); //中央ライン表示
	//	}else{
	//		bar.HideCenter(); //中央ライン表示			
	//	}
	//	return TRUE;
	//}
	return FALSE;
}

//Menu & Button Enable / Disenable
BOOL ScriptApp::EnableWindow(BOOL isEnable)
{
	//CMainFrame *pFrame = GetMainFrame();
	//if( pFrame != NULL ){
	//	GuiDlgBar &bar = pFrame->GetGuiBar();
	//	bar.EnableButton(isEnable);
	//	CMenu *pMenu = pFrame->GetMenu();
	//	if( pMenu != NULL ){
	//		int nMax = pMenu->GetMenuItemCount();
	//		int nState = (isEnable) ? MF_ENABLED : MF_DISABLED;
	//		for(int i=0;i<nMax;i++){				
	//			if( i != 2 ){ //2番の表示のみ使用可とする
	//				pFrame->GetMenu()->EnableMenuItem(i,MF_BYPOSITION | nState);//Menu変更
	//			}
	//		}
	//		pFrame->DrawMenuBar(); //メニュー再描画
	//	}
	//	return TRUE;
	//}
	return FALSE;
}