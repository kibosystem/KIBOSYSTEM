// MainFrm.cpp : CKIBOFrame �N���X�̎���
//

#include "stdafx.h"
#include "KIBO.h"
#include "enum.h"
#include "KIBOFrm.h"
#include "KIBODoc.h"
#include "KIBOView.h"
//#include "OutputView.h"
#include "CaptureBuffer.h"
#include "./LIB/FilePlus.h"
#include "./LIB/macro.h"
#include "./SCRIPT/UsbIO.h"
#include "./SCRIPT/App.h"
#include "./SCRIPT/ImageDevice.h"
#include "./SCript/SerialPort.h"


#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "kernel32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CKIBOFrame* s_pFrame = NULL; //KIBOFrame

//
inline CKIBOFrame* GetFrame()
{
	return s_pFrame;
}

//Main Window Check
BOOL IsWindowOK()
{ 
	CWnd *pWnd = AfxGetMainWnd();
	if( pWnd != NULL ){
		return ( pWnd->GetSafeHwnd() && pWnd->IsWindowVisible() );
	}
	return FALSE;
}

// CKIBOFrame

IMPLEMENT_DYNCREATE(CKIBOFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CKIBOFrame, CFrameWnd)
//	ON_COMMAND_RANGE(IDM_USER,IDM_USER_MAX,OnCommandFile)	

	//�����p�R�}���h
	//ON_COMMAND(IDM_HISTGRAM			,OnHistgram)		//�q�X�g�O����
	//ON_COMMAND(IDM_SAVE_VIEW			,OnSaveBitmap)		//�\���摜�̕ۑ�
	//ON_COMMAND(IDM_SAVE_CAMERA_IMAGE	,OnSaveCameraImage)	//�J�����摜�̕ۑ�
	//ON_COMMAND(IDM_SAVE_OUTPUT		,OnSaveOutput)		//�o�͂̕ۑ�
	//ON_COMMAND(ID_FILE_OPEN			,OnFileOpen)
	//ON_COMMAND(IDM_SCRIPT_TIMER		,OnScriptTimer)
	//ON_COMMAND(IDM_IMAGE_VALUE_DLG	,OnImageValueDlg)	//�摜�f�[�^�\���_�C�A���O
	//ON_COMMAND(IDM_EDIT_COMMAND		,OnEditCommand)	

	//�Ɩ��p�R�}���h
//	ON_MESSAGE(ID_LENSCHECK,OnLensCheck) //TEST

	//ON_MESSAGE(ID_LENS,OnLens) //TEST
	//ON_MESSAGE(ID_BACK,OnBack)//�w�i�B�e

//	ON_MESSAGE(ID_ELEM_DIV	,OnElementDib) //�v�f����
//	ON_MESSAGE(ID_LENGTH	,OnLength) //�����擾
//	ON_MESSAGE(ID_ELEM_DIFF,OnElemDiff) //�v�f������	

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CKIBOFrame �R���X�g���N�V����/�f�X�g���N�V����

CKIBOFrame::CKIBOFrame()
{
	s_pFrame = this;
	m_pStatusBar = NULL;
}

CKIBOFrame::~CKIBOFrame()
{
}

//�\���̈�̍��������肷��
void CKIBOFrame::SetViewHeight(int nH)
{
	//�����T�C�Y����
	if( m_wndSplitter.GetSafeHwnd() != NULL ){
		int cyCur,cyMin;
		m_wndSplitter.GetRowInfo(0,cyCur,cyMin); //�s�̏����擾
		m_wndSplitter.SetRowInfo(0,nH,cyMin);
		m_wndSplitter.RecalcLayout();
	}	
}
//�\���̈�̕������肷��
void CKIBOFrame::SetViewWidth(int nW)
{
	//�����T�C�Y����
	if( m_wndSplitter.GetSafeHwnd() != NULL ){
		int cyCur,cyMin;
		m_wndSplitter.GetColumnInfo(0,cyCur,cyMin); //��̏����擾
		m_wndSplitter.SetColumnInfo(0,nW,cyMin);
		m_wndSplitter.RecalcLayout();
	}
}

//�R�}���h�ꗗ�ǂݍ���
void CKIBOFrame::ReadCommandMenu(CStringArray &aryMenuName)
{
	aryMenuName.RemoveAll(); //clear
	CString strPath = GetApp()->GetAppPath(); //�A�v���P�[�\���N�����̃p�X
	CFileFind find;
	BOOL isOK = find.FindFile(strPath + _T("\\Command\\*.js"));
	while ( isOK ){
		isOK = find.FindNextFile();
	   if(!find.IsDirectory() ){
			aryMenuName.Add( find.GetFilePath() ); //FilePath�Z�b�g			
	   }
	}	
}

BOOL CKIBOFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ){
		return FALSE;
	}
	return TRUE;
}

//�}�X�N���[�W���� 
//virtual ���z�֐�
//�K���I�[�o�[���C�h����Ă��܂�
CRgn& CKIBOFrame::GetMaskRgn()
{
	static CRgn rgnTmp; //�e���|�������[�W����
	return rgnTmp;
}
//GUI���͒l�ݒ�
BOOL CKIBOFrame::SetInputName(CString &strName,int nInput)
{
	return FALSE;
}
//Dialog Bar��Text�o��
BOOL CKIBOFrame::OutputDlgBarText(LPCTSTR lpszText)
{
	return FALSE;
}
BOOL CKIBOFrame::DlgBarMsg(int nNo,LPCTSTR lpszText,COLORREF color/*=COLOR_WHITE*/)
{
	return FALSE;
}
// CKIBOFrame ���b�Z�[�W �n���h��

//�\���摜�̕ۑ�
void CKIBOFrame::OnSaveBitmap()
{
	GetView()->SaveBitmap( GetApp()->GetAppPath());
}

//�J�����摜�̕ۑ�
void CKIBOFrame::OnSaveCameraImage()
{
	GetView()->SaveCameraImage(GetApp()->GetAppPath());
}

//�o�͂̕ۑ�
void CKIBOFrame::OnSaveOutput()
{
	CFileDialog dlg(FALSE,_T("txt"),_T("*.txt"),NULL,_T("Text File (*.txt)|*.txt||"));
	if( dlg.DoModal() == IDOK ){
//		GetOutputView()->SaveTextFile( dlg.GetPathName() );
	}
}

//Script File Open
void CKIBOFrame::OnFileOpen()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	if( dlg.DoModal() == IDOK ){
		ScriptExec(dlg.GetPathName());
	}
}
//Script File���s
BOOL CKIBOFrame::ScriptExec(CString strFilePath)
{
	BOOL isOK = FALSE;
	m_mapIsScrptExec.SetAt(strFilePath,(void*)TRUE);
	::SetCurrentDirectory( GetApp()->GetAppPath() ); //�J�����g�f�B���N�g���ݒ�
	CString strCaption = App::GetInstance()->GetProfileString(L"Caption"); //�L���v�V�����w�蕶����͂���܂����H
	if( strCaption.IsEmpty() ){ //�L���v�V�����w�蕶���񂪂Ȃ��ꍇ
		SetWindowText( strFilePath );//Caption�ύX
	}
	if( GetDoc()->ScriptFileOpen( strFilePath) ){ //Script File ���s
		//�X�N���v�g�t�@�C�����s����
		isOK = TRUE;
	}
	m_mapIsScrptExec.SetAt(strFilePath,(void*)FALSE);
	return isOK;
}

//�X�N���v�g�A�������t�@�C���I��
void CKIBOFrame::OnScriptTimer()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	if( dlg.DoModal() == IDOK ){
		ScriptStart(dlg.GetPathName());
	}
}

//�X�N���v�g�t�@�C���A������
void CKIBOFrame::ScriptStart(CString &strPath)
{
	m_strScriptPath = strPath; //�A�������p�X�N���v�g�t�@�C���p�X�L��
	SetTimer(TIMER_SCRIPT,10,NULL); //�X�N���v�g�A�������p�^�C�}�[
}

//�X�N���v�g�t�@�C���J��Ԃ�����
void CKIBOFrame::ScriptRepeat(CString &strPath,int nElapse)
{
	CFileFind find;
	if( find.FindFile(strPath) ){ //�t�@�C�������݂��鎞����
		m_strScriptRepeat = strPath; //�A�������p�X�N���v�g�t�@�C���p�X�L��
		SetTimer(TIMER_REPEAT_SCRIPT,nElapse,NULL); //�X�N���v�g�A�������p�^�C�}�[	
	}
}


//�R�}���h�t�@�C�����s
void CKIBOFrame::OnCommandFile(UINT unID)
{
	int nIndex = (int)unID - IDM_USER;
	if( 0 <= nIndex && nIndex < m_aryCommandPath.GetSize() ){
		CString strPath = m_aryCommandPath[nIndex]; //Script File Path�擾		
		ScriptExec(strPath);
	}
}

BOOL CKIBOFrame::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){ //�L�[�{�[�h�`�F�b�N
		int nVK = (int)pMsg->wParam; //�L�[�R�[�h�擾
		CString strFilePath;
		if( m_mapKeyFilePath.Lookup(nVK,strFilePath) ){ //File���w�肳��Ă���ꍇ��
			ScriptExec(strFilePath); //Script File ���s
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

//�ҏW
void CKIBOFrame::OnEditCommand()
{
	CFileDialog dlg(TRUE,_T("js"),_T("*.js"),NULL,_T("Script File (*.js)|*.js||"));
	dlg.m_ofn.lpstrInitialDir = GetApp()->GetAppPath() + "\\Command";
	if( dlg.DoModal() == IDOK ){
		ShellExecute(NULL,TEXT("edit"),dlg.GetPathName(),NULL,NULL,SW_SHOW);
	}
	::SetCurrentDirectory(GetApp()->GetAppPath());
}

//���s���̃X�N���v�g�͑��݂��܂����H
BOOL CKIBOFrame::IsScriptExec()
{
	CString key;
	void *pVal; //�A�h���X�������l
	for(POSITION pos = m_mapIsScrptExec.GetStartPosition(); pos != NULL;){
		m_mapIsScrptExec.GetNextAssoc(pos, key,pVal);
		if( pVal != NULL ){ //���s���̃X�N���v�g�͑��݂���
			return TRUE;
		}
	}
	//���s���̃X�N���v�g�͑��݂��܂���
	return FALSE;
}

//�^�C�}�[����
void CKIBOFrame::OnTimer(UINT_PTR nIDEvent)
{	
	switch(nIDEvent){
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
	CFrameWnd::OnTimer(nIDEvent);	
}

//�I�����̃X�N���v�g���s���m�F
void CKIBOFrame::OnClose()
{
	CKIBOView *pView = GetView();
	CKIBODoc  *pDoc  = GetDoc();

	GetApp()->SetEnd(TRUE); //�A�v���P�[�V�����I���t���O

	//�^�C�}�[�I��
	KillTimer(TIMER_SCRIPT);//�X�N���v�g�A�������p�^�C�}�[
	KillTimer(TIMER_REPEAT_SCRIPT); //�J��Ԃ��X�N���v�g�I��

	if( pDoc->IsScriptRuning() || IsScriptExec() ){ //�X�N���v�g���s��
		SetTimer(TIMER_CLOSE,100,NULL); //�I������񂵂ɂ���		

		CString key;
		void *pVal; //�A�h���X�������l
		for(POSITION pos = m_mapIsScrptExec.GetStartPosition(); pos != NULL;){
			m_mapIsScrptExec.GetNextAssoc(pos, key,pVal);		
			m_mapIsScrptExec.SetAt(key,FALSE); //�O�X�N���v�g�I��
		}
		return ; //�������񔲂���
	}
	CFrameWnd::OnClose();
}

//�h���O���`�F�b�N�֐�
static BOOL CheckDongle()
{
	//WinAppPlus *pApp = GetApp();
	//int nRun = pApp->GetProfileInt(L"RUN",L"int",0);
	//CString strRun = pApp->GetProfileString(L"RUN",L"string");
	//if( strRun != L"START" || nRun != 110 ){ //�I���^�C�}�[
	//	return FALSE;
	//}
	//pApp->WriteProfileInt(L"RUN",L"int",0);
	//pApp->WriteProfileString(L"RUN",L"string",L"END");
	//return TRUE;

	//USB�h���O���`�F�b�N

	int nRet = Exec(SW_HIDE,L"KIBOSTART.exe",L"");
	return (nRet==11044);
}

BOOL CKIBOFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	BOOL isOK = CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
	if( isOK && !CheckDongle() ){
		//::exit(0);
		//////////////////////////TEST//////////////////////////////////////
		//::ErrorMsg(L"USB�F�؂��o���Ȃ�����10����ɏI���v���܂��B");
		//this->SetTimer(TIMER_FORCED_CLOSE,1000*60*10,NULL);
		//this->SetTimer(TIMER_CLOSE,1000*60*10*2,NULL);//�O�̂���
		//////////////////////////TEST//////////////////////////////////////
	}
	return isOK;
}
