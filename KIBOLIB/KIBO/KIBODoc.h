// KIBODoc.h : CKIBODoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "./Script/ScriptCall.h"

class CKIBODoc : public CDocument
{
private: //�����o�ϐ�
	ScriptCall m_Script; //�X�N���v�g���s�N���X
	CString m_strScript; //�X�N���v�g������
	CString m_strPrevFilePath; //�ˑR�ǂݍ��񂾃t�@�C��

protected: // �V���A��������̂ݍ쐬���܂��B
	CKIBODoc();
	DECLARE_DYNCREATE(CKIBODoc)

// ����
public:
	
// ����
public:
	BOOL IsScriptRuning();	//�X�N���v�g�͎��s���ł����H
	void Execute();			//�X�N���v�g�̎��s
	void ScriptClose();		//�X�N���v�g�����I��
	BOOL ScriptFileOpen(CString& strFilePath);

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();

// ����
public:
	virtual ~CKIBODoc();

protected:
	//�R�}���h�z��ɃX�N���v�g�R�}���h����������W
	BOOL CollectScriptCommand(CString& strFilePath,CStringArray &strAryCommand);

	//Include�t�@�C�������擾
	CString GetIncludeFile(CString &strLine);

	//�R�����g�폜�������n��
	BOOL DeleteComment( CString &strIn,CString &strOut );

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()

	// �������ꂽ OLE �f�B�X�p�b�`���蓖�Ċ֐�

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual void OnCloseDocument();
};

extern CKIBODoc* GetDoc();
