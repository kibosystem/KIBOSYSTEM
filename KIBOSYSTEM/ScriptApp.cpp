#include "StdAfx.h"
#include "MainFrm.h"
#include "ScriptApp.h"

ScriptApp::ScriptApp(void)
{
}

ScriptApp::~ScriptApp(void)
{
}

//�N���X���̂̍쐬
ScriptApp* ScriptApp::CreateInstance()
{
	ScriptApp *pInstance = new ScriptApp;
	return pInstance;
}

//���S�_�\��
BOOL ScriptApp::CenterPoint(BOOL isVisible)
{
	//CMainFrame *pFrame = GetMainFrame();
	//if( pFrame != NULL ){
	//	GuiDlgBar &bar = pFrame->GetGuiBar();
	//	bar.GetCheck(IDC_CENTER)->SetCheck(isVisible);
	//	if( isVisible ){			
	//		bar.ShowCenter(); //�������C���\��
	//	}else{
	//		bar.HideCenter(); //�������C���\��			
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
	//			if( i != 2 ){ //2�Ԃ̕\���̂ݎg�p�Ƃ���
	//				pFrame->GetMenu()->EnableMenuItem(i,MF_BYPOSITION | nState);//Menu�ύX
	//			}
	//		}
	//		pFrame->DrawMenuBar(); //���j���[�ĕ`��
	//	}
	//	return TRUE;
	//}
	return FALSE;
}