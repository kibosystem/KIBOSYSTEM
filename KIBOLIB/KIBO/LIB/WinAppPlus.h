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
	bool					m_isEnd; //�I���t���O
	WSAData					m_wsaData;		// Windows �\�P�b�g���������
	CString					m_strAppPath;	//�A�v���P�[�V�����N�����̃p�X
	HANDLE					m_hMutex;		//���d�N���h�~Mutex
public:
	WinAppPlus();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~WinAppPlus();
	
	CString GetCurrentPath();//�J�����g�f�B���N�g���̐�΃p�X���擾
	CString& GetAppPath(){return m_strAppPath;} //�A�v���P�[�\���N�����̃p�X
	CString GetIPAddr();		//IP�A�h���X���擾���܂��B
	BOOL SetMacAddress(CStringArray& astrMacAddress);
	BOOL CheckDevice();//�r�f�I�J�[�h�̊g��k���̋@�\���`�F�b�N���܂��B
	BOOL CheckUSBMemory();//USB�������̃`�F�b�N�����܂��B	

	void SetEnd(bool b){m_isEnd=b;} //�I���t���O�ݒ�
	bool IsEnd(){return m_isEnd;} //�I�����܂����H
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ScriptBase *CreateInstanceScript(CString& strName){return NULL;} //Script�N���X���̍쐬
protected:	
	
protected:
	DECLARE_MESSAGE_MAP()
};


inline WinAppPlus* GetApp(){return (WinAppPlus*)::AfxGetApp();}