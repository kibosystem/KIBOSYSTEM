
// KIBOSYSTEM.h : KIBOSYSTEM �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CKIBOSYSTEMApp:
// ���̃N���X�̎����ɂ��ẮAKIBOSYSTEM.cpp ���Q�Ƃ��Ă��������B
//

class CKIBOSYSTEMApp : public WinAppPlus
{
public:
	CKIBOSYSTEMApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual ScriptBase *CreateInstanceScript(CString& strName); //Script�N���X���̍쐬

};

extern CKIBOSYSTEMApp theApp;
