#pragma once

#include "../LIB/Dom.h"

//KIBO�ɒǉ����郁�j���[

class ScadaMenu
{
private: //�����o
	CString m_strFilePath; //xml�t�@�C���p�X
public:
	ScadaMenu(CString &strFilePath);
	~ScadaMenu(void);

	CString& GetFilePath(){return m_strFilePath;}  //xml�t�@�C���p�X�擾
	BOOL LoadMenu(CMenu *pMenu,CString &strXPath); //�|�b�v�A�b�v���j���[�̓ǂݍ���

private:

	//SubMenu�擾
	CMenu* FindMenuItem(CMenu *pMenu, CString &strMenu);

};
