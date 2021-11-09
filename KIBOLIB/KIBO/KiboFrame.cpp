// KiboFrame.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KiboFrame.h"
#include "KiboDoc.h"
#include "enum.h"
#include "./SCript/App.h"
#include "./LIB/WinAppPlus.h"
#include "./LIB/functions.h"


// KiboFrame

static KIBOFrame * s_pFrame = NULL; //KIBOFrame
									//
inline KIBOFrame * GetFrame()
{
	return s_pFrame;
}
//Main Window Check
BOOL IsWindowOK()
{
	CWnd *pWnd = AfxGetMainWnd();
	if (pWnd != NULL) {
		return (pWnd->GetSafeHwnd() && pWnd->IsWindowVisible());
	}
	return FALSE;
}


IMPLEMENT_DYNCREATE(KIBOFrame, CFrameWndEx)

KIBOFrame::KIBOFrame()
{
	s_pFrame = this;
}

KIBOFrame ::~KIBOFrame()
{
}

//Script File���s
BOOL KIBOFrame::ScriptExec(CString strFilePath)
{
	BOOL isOK = FALSE;
	m_mapIsScrptExec.SetAt(strFilePath, (void*)TRUE);
	::SetCurrentDirectory(GetApp()->GetAppPath()); //�J�����g�f�B���N�g���ݒ�
	CString strCaption = App::GetInstance()->GetProfileString(L"Caption"); //�L���v�V�����w�蕶����͂���܂����H
	if (strCaption.IsEmpty()) { //�L���v�V�����w�蕶���񂪂Ȃ��ꍇ
		SetWindowText(strFilePath);//Caption�ύX
	}
	if (GetDoc()->ScriptFileOpen(strFilePath)) { //Script File ���s
												 //�X�N���v�g�t�@�C�����s����
		isOK = TRUE;
	}
	m_mapIsScrptExec.SetAt(strFilePath, (void*)FALSE);
	return isOK;
}

//�X�N���v�g�A�������t�@�C���I��
void KIBOFrame::OnScriptTimer()
{
	CFileDialog dlg(TRUE, _T("js"), _T("*.js"), NULL, _T("Script File (*.js)|*.js||"));
	if (dlg.DoModal() == IDOK) {
		ScriptStart(dlg.GetPathName());
	}
}

//�X�N���v�g�t�@�C���A������
void KIBOFrame::ScriptStart(CString &strPath)
{
	m_strScriptPath = strPath; //�A�������p�X�N���v�g�t�@�C���p�X�L��
	SetTimer(TIMER_SCRIPT, 10, NULL); //�X�N���v�g�A�������p�^�C�}�[
}

//�X�N���v�g�t�@�C���J��Ԃ�����
void KIBOFrame::ScriptRepeat(CString &strPath, int nElapse)
{
	CFileFind find;
	if (find.FindFile(strPath)) { //�t�@�C�������݂��鎞����
		m_strScriptRepeat = strPath; //�A�������p�X�N���v�g�t�@�C���p�X�L��
		SetTimer(TIMER_REPEAT_SCRIPT, nElapse, NULL); //�X�N���v�g�A�������p�^�C�}�[	
	}
}


BEGIN_MESSAGE_MAP(KIBOFrame, CFrameWndEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// KiboFrame ���b�Z�[�W �n���h���[

BOOL KIBOFrame::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN) { //�L�[�{�[�h�`�F�b�N
	//	int nVK = (int)pMsg->wParam; //�L�[�R�[�h�擾
	//	CString strFilePath;
	//	if (m_mapKeyFilePath.Lookup(nVK, strFilePath)) { //File���w�肳��Ă���ꍇ��
	//		ScriptExec(strFilePath); //Script File ���s
	//	}
	//}
	App *pApp = App::GetInstance();

	if (pApp != NULL && TranslateAccelerator(this->GetSafeHwnd(), pApp->m_hAccel, pMsg)){
		return TRUE;
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}


//�h���O���`�F�b�N�֐�
static BOOL CheckDongle()
{
	int nRet = Exec(SW_HIDE,L"KIBOSTART.exe", L"");
	return (nRet == 1104411044);
	return TRUE;
}


BOOL KIBOFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	BOOL isOK = CFrameWndEx::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
	if (isOK && !CheckDongle()) {
		::ErrorMsg(L"USB�F�؂��o���Ȃ�����10����ɏI���v���܂��B");
		this->SetTimer(TIMER_FORCED_CLOSE,1000*60*10,NULL);
		this->SetTimer(TIMER_CLOSE,1000*60*10*2,NULL);//�O�̂���
	}
	return TRUE;
}


void KIBOFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case TIMER_SCRIPT: //�^�C�}�[�X�N���v�g�̏ꍇ
		KillTimer(TIMER_SCRIPT); //�^�C�}�[�𕡐��Ă΂Ȃ��悤�ɂ���
		ScriptExec(m_strScriptPath);//Script File ���s
		break;
	case TIMER_REPEAT_SCRIPT://�J��Ԃ���p�X�N���v�g�̎��s
		ScriptExec(m_strScriptRepeat); //�J��Ԃ�Script File ���s
		break;
	case TIMER_CLOSE: //�^�C�}�[�ɂ��I���҂��̏ꍇ
		PostMessage(WM_CLOSE);
		break;
#ifdef FORCED_CLOSE
	case TIMER_FORCED_CLOSE:
		::ErrorMsg(L"�F�؂��o���Ȃ����ߏI���v���܂��B");
		PostMessage(WM_CLOSE);
		break;
#endif
	}
	CFrameWndEx::OnTimer(nIDEvent);
}
