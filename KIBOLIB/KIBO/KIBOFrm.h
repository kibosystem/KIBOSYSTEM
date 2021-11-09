// MainFrm.h : CKIBOFrame �N���X�̃C���^�[�t�F�C�X
//
#pragma once
#pragma comment(lib,"winmm.lib")

#include <mmsystem.h>
#include "COMMANDID.h"
#include "./LIB/typedef.h"

#define IDM_USER	 10000 //�R�}���h���j���[
#define IDM_USER_MAX 10100 //�ő�R�}���h�~���[�ԍ�
#define VIEW_HEIGHT		830		//�J�����摜�\���̈捂�������l

class CKIBOFrame : public CFrameWnd
{
protected: //�����o
	CString m_strScriptPath;	//���s���̃X�N���v�g�t�@�C���p�X(�؂�ւ��\�j
	CString m_strScriptRepeat;  //�J��Ԃ����̃X�N���v�g�t�@�C���p�X(�؂�ւ��s�j
	CSortStringArray m_aryCommandPath;
	CMapStringToPtr m_mapIsScrptExec;//�X�N���v�g�t�@�C���͎��s���ł����H
	CSplitterWnd m_wndSplitter; //���������̕����E�B���h�E
public:
	CMap<int,int,CString,CString&> m_mapKeyFilePath; //Virtual Key �t�@�C��
	CStatusBar *m_pStatusBar;

// ����
public:
	CSplitterWnd* GetSplitterPtr(){return &m_wndSplitter;}	
	void SetStatusBarPtr(CStatusBar *p){m_pStatusBar=p;} //�X�e�[�^�X�o�[�A�h���X�L�^
	CStatusBar* GetStatusBarPtr(){return m_pStatusBar;}	//�X�e�[�^�X�o�[�A�h���X�擾

protected: // �V���A��������̂ݍ쐬���܂��B
	CKIBOFrame();
	DECLARE_DYNCREATE(CKIBOFrame)

// ����
public:
	void SetViewHeight(int nH);//�\���̈�̍��������肷��		
	void SetViewWidth(int nW);//�\���̈�̕������肷��

	BOOL ScriptExec(CString strFilePath); //�w��X�N���v�g�t�@�C�����s
	void ScriptStart(CString &strPath);	//�X�N���v�g�t�@�C���A������	
	void ScriptRepeat(CString &strPath, int nElapse);//�X�N���v�g�t�@�C���J��Ԃ�����

	BOOL CreateImageViewDlg();			//�摜�\���_�C�A���O�̍쐬
	void SetStatusBarText(int nIndex,LPCWSTR lpsz,...);
	void SetStatusBarText(CStringArray &strAry);

protected:
	void ReadCommandMenu(CStringArray &aryMenuName);//�R�}���h�ꗗ�ǂݍ���

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual CRgn& GetMaskRgn(); //�}�X�N���[�W�����擾
	virtual BOOL SetInputName(CString &strName,int nValue);
	virtual BOOL OutputDlgBarText(LPCTSTR lpszText);//Dialog Bar��Text�o��
	virtual BOOL DlgBarMsg(int nNo,LPCTSTR lpszText,COLORREF color=RGB(255,255,255)); //Message Color�w��
// ����
public:
	virtual ~CKIBOFrame();

private:
	BOOL IsScriptExec();//���s���̃X�N���v�g�͑��݂��܂����H
// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:	
	void OnCommandFile(UINT unID);//�R�}���h�t�@�C�����s

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	//afx_msg void OnHistgram();

	afx_msg void OnCameraInitEx();	//�J�����̏�����
	afx_msg void OnSaveBitmap();
	afx_msg void OnSaveCameraImage();
	afx_msg void OnBmpLabeling();	
	afx_msg void OnFileOpen();//Script File Open
	afx_msg void OnScriptTimer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCamraSecond();

	//	afx_msg void OnSaveData();
	afx_msg void OnSaveOutput();
	afx_msg void OnClose();
	afx_msg void OnEditCommand();
};

extern CKIBOFrame* GetFrame(); 
extern BOOL IsWindowOK();
