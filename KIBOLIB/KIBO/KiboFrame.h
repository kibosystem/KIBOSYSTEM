#pragma once


// KIBOFrame  �t���[��

class KIBOFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(KIBOFrame)
	// ����
public:
	CString m_strScriptPath;	//���s���̃X�N���v�g�t�@�C���p�X(�؂�ւ��\�j
	CString m_strScriptRepeat;  //�J��Ԃ����̃X�N���v�g�t�@�C���p�X(�؂�ւ��s�j
	CMap<int, int, CString, CString&> m_mapKeyFilePath; //Virtual Key �t�@�C��
	CMapStringToPtr m_mapIsScrptExec;//�X�N���v�g�t�@�C���͎��s���ł����H
									 // ����
public:
	BOOL ScriptExec(CString strFilePath); //�w��X�N���v�g�t�@�C�����s
	void ScriptStart(CString &strPath);	//�X�N���v�g�t�@�C���A������	
	void ScriptRepeat(CString &strPath, int nElapse);//�X�N���v�g�t�@�C���J��Ԃ�����
	void OnScriptTimer();	
	virtual void SetStatusBarText(LPCTSTR lpszText) {}

protected:
	KIBOFrame();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~KIBOFrame();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


extern KIBOFrame * GetFrame();
extern BOOL IsWindowOK(); //Main Window Check