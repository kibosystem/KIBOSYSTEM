#include "../StdAfx.h"
#include "ScadaMenu.h"

ScadaMenu::ScadaMenu(CString &strFilePath)
{
	m_strFilePath = strFilePath; //xml�t�@�C���p�X�Z�b�g
}

ScadaMenu::~ScadaMenu(void)
{
}

//SubMenu�擾
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


//�|�b�v�A�b�v���j���[�̓ǂݍ���
BOOL ScadaMenu::LoadMenu(CMenu *pMenu,CString &strXPath) 
{
	Dom dom(GetFilePath());

	CString strAttr = _T("name");
	CString strName = dom.GetAttributeText(strXPath,strAttr); //popup Menu ����

	//MENU�쐬
	if( pMenu != NULL ){
		pMenu->InsertMenu(-1,MF_BYPOSITION|MF_STRING|MF_POPUP,0,strName);//Popup�ǉ�
		CMenu *pPopup = FindMenuItem(pMenu,strName); //���j���[�ʒu����
		if( pPopup != NULL ){
			int nCnt = dom.GetCount(strXPath);
			nCnt++;
		}
	}

	return TRUE;
}