#include "../stdafx.h"
#include "Windows.h"
#include "App.h"
#include "../enum.h"
#include "../KIBOView.h"
#include "../KIBOFrame.h"
#include "../OutputWnd.h"
#include "../LIB/functions.h"
#include "../KIBODoc.h"
#include "../LIB/FolderDialog.h"
#include "../LIB/Object.h"
#include "../LIB/FilePlus.h"
#include <mmsystem.h>	// timeGetTime()
#pragma comment(lib, "winmm.lib")

extern CStringW ConvertUTF8ToUnicode(const CStringA& utf8);

static App *s_pInstance = NULL; //�B��̃N���X����
App* App::GetInstance()//���̎擾
{
	return s_pInstance;
}
App::App(void)
{
	s_pInstance = this;
	m_hWnd = ::AfxGetMainWnd()->GetSafeHwnd();
	m_crText = COLOR_BLACK; //�����F
	ZeroMemory(&m_lf,sizeof(LOGFONT)); //�t�H���g�f�[�^
	wcscpy_s(m_lf.lfFaceName,LF_FACESIZE,_T("�l�r ����"));		//Defalut�t�H���g
	m_lf.lfHeight = 15; //Default Font Size
	m_rect.SetRectEmpty();		//�ėp�̈惊�Z�b�g
	m_strSectionInt = L"INT";		//���W�X�g���l�Z�N�V����
	m_strSectionString = L"STRING";	//���W�X�g��������Z�N�V����

	m_hAccel = ::LoadAccelerators(::AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));//���ǉ�
	m_ulCount = ::GetTickCount64();//���Ԍv���J�E���^�[
}

App::~App(void)
{
}

static CommandTable s_table[] =
{
	 {App::FUNC_WAIT		,1, L"Wait"}		//�w�莞�Ԓ�~
	,{App::FUNC_CLICK_WAIT	,0, L"ClickWait"}	//�}�E�X�N���b�N�����܂Œ�~
	,{App::FUNC_KEY_WAIT	,1, L"KeyboardWait"}//�L�[�{�[�h���b�Z�[�W�҂�
	,{App::FUNC_COUNTSTART	,0, L"CountStart"} //���Ԍv���J�n		
	,{App::FUNC_COUNTEND	,0, L"CountEnd"} //���Ԍv���I��
	,{App::FUNC_STARTSCRIPTTIMER,1,L"StartScriptTimer"}	//�X�N���v�g�^�C�}�[��~
	,{App::FUNC_STOPSCRIPTTIMER,0,L"StopScriptTimer"}	//�X�N���v�g�^�C�}�[��~
	,{App::FUNC_SET_REPEAT_SCRIPT,2,L"SetRepeatScript"}	//�J��Ԃ��X�N���v�g�t�@�C���ݒ�
	,{App::FUNC_SETWINDOWTEXT,1,L"SetWindowText"}  //���C��Window Caption�ύX
	,{App::FUNC_DRAWRECT	, 9,L"DrawRect"}	//��`�`��
	,{App::FUNC_DRAWLINE	, 8,L"DrawLine"}	//�w�胉�C���`��
	,{App::FUNC_PAINTMASK	, 4,L"PaintMask"}	//�}�X�N�̈�`��
	,{App::FUNC_RENDER		, 0,L"Render"}		//�����_�����O
	,{App::FUNC_REMOVEALL	, 0,L"RemoveAll"}	//�S�폜
	,{App::FUNC_REMOVE		, 1,L"Remove"}		//�w��I�u�W�F�N�g�폜
	,{App::FUNC_REMOVEEXCEPT, 1,L"RemoveExcept"}//�w��I�u�W�F�N�g�ȊO�폜
	,{App::FUNC_SETTEXTCOLOR, 3,L"SetTextColor"}//�����F�̐ݒ�
	,{App::FUNC_SETFONTNAME	, 1,L"SetFontName"}	//Font���̐ݒ�
	,{App::FUNC_SETFONTSIZE	, 1,L"SetFontSize"}	//FontSize�ݒ�
	,{App::FUNC_DRAWTEXT	, 4,L"DrawText"}	//������`��
	,{App::FUNC_DRAWCHAR	, 4,L"DrawChar"}	//�����`��
	,{App::FUNC_SETVISIBLE  , 2,L"SetVisible"}	//�\��ON�EOFF�؂�ւ�
	,{App::FUNC_SETTEXTRECT , 7,L"SetTextRect"}	//������\���̈�w��
	,{App::FUNC_BINARYIMAGE	, 1,L"BinaryImage"}	//�\���摜2�l��
	,{App::FUNC_SETKEYFILE	, 2,L"SetKeyFile"}	//�R�}���h�t�@�C���̃L�[���蓖��
	,{App::FUNC_GETBITMAP	,3,L"GetBitmap"}	//�w��t�H���_���r�b�g�}�b�v�̎擾
	,{App::FUNC_GETBITMAPNUM,2,L"GetBitmapNum"}//�w��t�H���_���r�b�g�}�b�v���̎擾
	,{App::FUNC_TEXTOUTPUT	,-1,L"TextOutput"}	//������o��
	,{App::FUNC_MESSAGE		,-1,L"Message"}		//���b�Z�[�W�\��
	,{App::FUNC_SETSTATUSBARTEXT,1,L"SetStatusBarText"}	//�X�e�[�^�X�o�[�����Z�b�g
	,{App::FUNC_OUTPUTCLEAR	, 0,L"OutputClear"}	//������o�̓N���A
	,{App::FUNC_SETVIEWRATE , 2,L"SetViewRate"}	//�\���{���ݒ�
	,{App::FUNC_GETVIEWRATECX,0,L"GetViewRateCx"}//�\���{���擾 x���� 
	,{App::FUNC_GETVIEWRATECY,0,L"GetViewRateCy"}//�\���{���擾 y����
	,{App::FUNC_SHOWWINDOW	, 1,L"ShowWindow"}  //���C��Window�\����ԕύX
	,{App::FUNC_MOVEWINDOW	, 4,L"MoveWindow"}  //���C��Window�\���T�C�Y�ύX
	,{App::FUNC_VIEWHEIGHT  , 1,L"ViewHeight"}	//���ʏo�͍����ύX
	,{App::FUNC_VIEWWIDTH   , 1,L"ViewWidth"}	//���ʏo�͕��ύX
	,{App::FUNC_SETTEXT	, 2,L"SetText"}	//�ėp������ݒ�
	,{App::FUNC_GETTEXT	, 1,L"GetText"}	//�ėp������擾
	,{App::FUNC_SETNUM,2,L"SetNum"}	//�ėp���l�ݒ�
	,{App::FUNC_GETNUM,1,L"GetNum"}	//�ėp���l�擾
	,{App::FUNC_SETDBL,2,L"SetDbl"}	//�ėp�����ݒ�
	,{App::FUNC_GETDBL,1,L"GetDbl"}	//�ėp�����擾
	,{App::FUNC_CREATE_FOLDER,1,L"CreateFolder"}//�w��t�H���_�쐬	
	,{App::FUNC_SENDMESSAGE	 ,-1,L"SendMessage"}//�R�}���h�Ăяo��
	,{App::FUNC_POSTMESSAGE	 ,-1,L"PostMessage"}//�R�}���h�Ăяo��
	,{App::FUNC_SETRECT		 , 4,L"SetRect"}	//�̈�ݒ�
	,{App::FUNC_GETTIME		 , 0,L"GetTime"}	//�}���e�B���f�A�^�C�}�[�擾
	,{App::FUNC_GETVALUE	 , 1,L"GetValue"}	//�w��l�擾
	,{App::FUNC_SETLINECOLOR , 3,L"SetLineColor"}//���F�ݒ�
	,{App::FUNC_GETPRIVATEPROFILEINT,4,L"GetPrivateProfileInt"}//.ini�t�@�C���l�擾
	,{App::FUNC_WRITEPRIVATEPROFILEINT,4,L"WritePrivateProfileInt"}//.ini�t�@�C���l��������
	,{App::FUNC_GETPRIVATEPROFILEDBL,4,L"GetPrivateProfileDbl"}//.ini�t�@�C���l�擾
	,{App::FUNC_WRITEPRIVATEPROFILEDBL,4,L"WritePrivateProfileDbl"}//.ini�t�@�C���l��������
	,{App::FUNC_GETPRIVATEPROFILESTRING,4,L"GetPrivateProfileString"}//.ini�t�@�C��������擾
	,{App::FUNC_WRITEPRIVATEPROFILESTRING,4,L"WritePrivateProfileString"}//.ini�t�@�C�������񏑂�����
	,{App::FUNC_GETPROFILEINT, 1,L"GetProfileInt"}//���W�X�g���l�擾
	,{App::FUNC_WRITEPROFILEINT, 2,L"WriteProfileInt"}//���W�X�g���l�ݒ�
	,{App::FUNC_GETPROFILEDBL, 1,L"GetProfileDbl"}//���W�X�g���l�����擾
	,{App::FUNC_WRITEPROFILEDBL, 2,L"WriteProfileDbl"}//���W�X�g���l�����ݒ�
	,{App::FUNC_GETPROFILESTRING,1,L"GetProfileString"}//���W�X�g���l�擾
	,{App::FUNC_WRITEPROFILESTRING,2,L"WriteProfileString"}//���W�X�g���l�ݒ�
	,{App::FUNC_SETINPUTNAME,2,L"SetInuptName"}//GUI���͒l�ύX
	,{App::FUNC_CENTERPOINT,1,L"CenterPoint"} //���S�_�\���E��\��
	,{App::FUNC_ENABLEWINDOW,1,L"EnableWindow"} //Menu���{�^�� Enable/DisEnable
	,{App::FUNC_GETPIXELSIZE,1,L"GetPixelSize"} //1��f������̑傫�����擾
	,{App::FUNC_SELECTFOLDER ,0,L"SelectFolder"}		//�t�H���_�I��
	,{App::FUNC_SELECTBITMAP ,0,L"SelectBitmap"}		//�r�b�g�}�b�v�t�@�C���I��
	,{App::FUNC_EXEC,1,L"Exec" }		//EXE���s
	,{App::FUNC_READTEXT,1,L"ReadText" }	//������S�s�ǂݍ���
	,{App::FUNC_READSTRING,2,L"ReadString"}	//������P�s�ǂݍ���
	,{App::FUNC_MAX			 ,0,NULL} //�R�}���h�e�[�u���I�[
};

//�֐����`�F�b�N
STDMETHODIMP App::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId)
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//���s
STDMETHODIMP App::Invoke(DISPID dispIdMember	//Function ID �ԍ�
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script������
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr)
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //�����`�F�b�N
		return S_FALSE;
	}		

	switch(dispIdMember){
		case FUNC_WAIT: //Wait
			{
				BOOL isOK = Wait( (DWORD)(pDispParams->rgvarg[0].ulVal) ); //Sleep����
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_CLICK_WAIT: //�}�E�X�N���b�N wait
			{
				BOOL isEnd = ClickWait();
				ReturnBool(wFlags,pVarResult,isEnd);
			}
			break;
		case FUNC_KEY_WAIT:		//�L�[�{�[�h���b�Z�[�W�҂�
			{
				int nID = GetValientInt(&pDispParams->rgvarg[0]); //�L�[�{�[�hID
				BOOL isEnd = KeyWait(nID);
				ReturnBool(wFlags,pVarResult,isEnd);
			}
			break;
		case FUNC_COUNTSTART:	//���Ԍv���J�n
			{
				m_ulCount = ::GetTickCount64();//���Ԍv���J�E���^�[
			}
			break;
		case FUNC_COUNTEND: //���Ԍv���I��
			{
				ULONGLONG ulCount = ::GetTickCount64();//���Ԍv���J�E���^�[
				UINT ulVal = (UINT)(ulCount - m_ulCount );
				ReturnUint(wFlags,pVarResult,ulVal);
			}
			break;
		case FUNC_STARTSCRIPTTIMER: //�X�N���v�g�^�C�}�[�J�n
			{
				CString strFilePath = pDispParams->rgvarg[0].bstrVal; 
				GetFrame()->ScriptStart(strFilePath);
			}
			break;
		case FUNC_STOPSCRIPTTIMER: //�X�N���v�g�^�C�}�[��~
			GetFrame()->KillTimer(TIMER_SCRIPT);
			break;
		case FUNC_SET_REPEAT_SCRIPT://�J��Ԃ��X�N���v�g�t�@�C���ݒ�
			{
				CString strFilePath = pDispParams->rgvarg[1].bstrVal; 
				int nElapse = GetValientInt(&pDispParams->rgvarg[0]);
				GetFrame()->ScriptRepeat(strFilePath,nElapse);
			}
			break;
		case FUNC_SETWINDOWTEXT:  //���C��Window Caption�ύX
			{
				CString strCaption = pDispParams->rgvarg[0].bstrVal; 
				GetFrame()->SetWindowText(strCaption);
			}
			break;
		case FUNC_DRAWRECT: //��`�`��
			{
				CString strName = pDispParams->rgvarg[8].bstrVal; //�`��I�u�W�F�N�g����
				int nLeft = GetValientInt(&pDispParams->rgvarg[7]); //��
				int nTop  = GetValientInt(&pDispParams->rgvarg[6]); //��
				int nRight = GetValientInt(&pDispParams->rgvarg[5]); //�E
				int nBottom = GetValientInt(&pDispParams->rgvarg[4]); //��
				int nA = 255;//A
				int nR = GetValientInt(&pDispParams->rgvarg[3]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[2]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[1]); //B
				BOOL isPaint = (BOOL)GetValientInt(&pDispParams->rgvarg[0]); //�h��Ԃ��܂����H				
				DrawRect(strName,nLeft,nTop,nRight,nBottom,nA,nR,nG,nB,isPaint);
			}
			break;
		case FUNC_DRAWLINE://�w�胉�C���`��
			{
				CString strName = pDispParams->rgvarg[7].bstrVal; //�`��I�u�W�F�N�g����
				int nXS = GetValientInt(&pDispParams->rgvarg[6]); //�n�_x
				int nYS  = GetValientInt(&pDispParams->rgvarg[5]); //�n�_y
				int nXE = GetValientInt(&pDispParams->rgvarg[4]); //�I�_x
				int nYE = GetValientInt(&pDispParams->rgvarg[3]); //�I�_y
				int nA = 255;//A
				int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
				DrawLine(strName,nXS,nYS,nXE,nYE,nA,nR,nG,nB);
			}
			break;
		//case FUNC_PAINTMASK:	//�}�X�N�̈�`��
		//	{
		//		int nCnum = GetValientInt(&pDispParams->rgvarg[3]); //�J�����ԍ�
		//		int nR    = GetValientInt(&pDispParams->rgvarg[2]); //R
		//		int nG    = GetValientInt(&pDispParams->rgvarg[1]); //G
		//		int nB    = GetValientInt(&pDispParams->rgvarg[0]); //B
		//		CameraEx *pCamera = GetView()->GetCameraPtr( nCnum );
		//		if(pCamera != NULL){
		//			CString strName;
		//			for(int i=0;i<pCamera->m_aryMask.GetSize();i++){
		//				strName.Format(L"Mask%d",i);
		//				CRect rect = pCamera->m_aryMask[i].m_rect;
		//				DrawRect(strName,rect.left,rect.top,rect.right,rect.bottom,255,nR,nG,nB,TRUE);
		//			}
		//		}				
		//	}
			break;
		case FUNC_RENDER:		//�����_�����O
			Render();
			break;
		case FUNC_REMOVEALL: //�S�`��I�u�W�F�N�g�폜
			RemoveAll();			
			break;
		case FUNC_REMOVE://�w��I�u�W�F�N�g���폜
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //�폜����
				GetView()->m_mgrVO.RemoveByName(strName.GetBuffer()); //�w��I�u�W�F�N�g���폜
			}
			break;
		case FUNC_REMOVEEXCEPT: //�w��I�u�W�F�N�g�ȊO���폜
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //�폜����
				RemoveExcept(strName);				
			}
			break;
		case FUNC_SETTEXTCOLOR: //�����F�̐ݒ�
			{
				int r = pDispParams->rgvarg[2].ulVal; //R
				int g = pDispParams->rgvarg[1].ulVal; //G
				int b = pDispParams->rgvarg[0].ulVal; //B
				SetTextColor(RGB(r,g,b));
			}
			break;
		case FUNC_SETFONTNAME: //Font���̐ݒ�
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //Font ����
				wcscpy_s(m_lf.lfFaceName,LF_FACESIZE,strName);		//Defalut�t�H���g
			}
			break;
		case FUNC_SETFONTSIZE: //FontSize�ݒ�
			{
				m_lf.lfHeight = pDispParams->rgvarg[0].ulVal; //Font Size
			}
			break;
		case FUNC_DRAWTEXT://������ݒ�
			{
				CString strID   = pDispParams->rgvarg[3].bstrVal; //ID������
				CString strText = pDispParams->rgvarg[2].bstrVal; //�\��������
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //x���W
				int nY = GetValientInt(&pDispParams->rgvarg[0]); //y���W
				DrawText(strID,strText,nX,nY);
			}
			break;
		case FUNC_DRAWCHAR:		//�����`��
			{	
				CString strID = pDispParams->rgvarg[3].bstrVal; //ID������
				int nChar = pDispParams->rgvarg[2].intVal; //�\������
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //x���W
				int nY = GetValientInt(&pDispParams->rgvarg[0]); //y���W
				CString strText;
				strText.Format(L"%c",nChar);
				GetView()->m_mgrVO.CreateStringObject(strID,nX,nY,strText,GetTextColor(),GetLogFont()); //��`�`��				
			}
			break;
		case FUNC_SETVISIBLE:	//�\��ON�EOFF�؂�ւ�
			{
				CString strID = pDispParams->rgvarg[1].bstrVal; //ID������
				BOOL isVisible = (BOOL)GetValientInt(&pDispParams->rgvarg[0]); //ON/OFF
				SetVisible(strID,(bool)isVisible);
			}
			break;
		case FUNC_SETTEXTRECT: //������\���̈�w��
			{
				CString strID = pDispParams->rgvarg[6].bstrVal; //ID������
				CString strH = pDispParams->rgvarg[5].bstrVal; //FormatH
				CString strV = pDispParams->rgvarg[4].bstrVal; //FormatV
				int nLeft   = GetValientInt(&pDispParams->rgvarg[3]); //��
				int nTop    = GetValientInt(&pDispParams->rgvarg[2]); //��
				int nRight  = GetValientInt(&pDispParams->rgvarg[1]); //�E
				int nBottom = GetValientInt(&pDispParams->rgvarg[0]); //��
				StringObject *pSO = (StringObject *)GetView()->m_mgrVO.GetVO(strID); 
				if( pSO != NULL ){
					pSO->SetRect(strH,strV,nLeft,nTop,nRight,nBottom); //�\���̈�ݒ�
				}
			}
			break;
		case FUNC_BINARYIMAGE: //2�l���摜�\��
			{
				int nTh = pDispParams->rgvarg[0].ulVal; //�������l
				GetView()->DoBinary(nTh);
			}
			break;
		case FUNC_SETKEYFILE: //�R�}���h�t�@�C���̃L�[���蓖��
			{
				int nVK = GetValientInt(&pDispParams->rgvarg[1]); //VirtualKey�ԍ�
				CString strFilePath = pDispParams->rgvarg[0].bstrVal; //�t�@�C��������				
				GetFrame()->m_mapKeyFilePath.SetAt(nVK,strFilePath); //�L�[�}�b�s���O�o�^
			}
			break;
		case FUNC_GETBITMAP://�w��t�H���_���r�b�g�}�b�v�̎擾
			{
				CString strFolder = pDispParams->rgvarg[2].bstrVal; //�t�H���_�p�X
				int nNo = GetValientInt(&pDispParams->rgvarg[1]); //�t�@�C���ԍ�				
				CString strExt = L"*.";
				strExt += pDispParams->rgvarg[0].bstrVal; //�g���q

				CString strRet;
				int nCnt = 0; //�r�b�g�}�b�v�t�@�C�����J�E���^�[
				CFileFind find;
				BOOL isWorking = find.FindFile(strExt);
				while(isWorking){
				   isWorking = find.FindNextFile();
				   if(nCnt==nNo){//�w��ԍ�����������
						strRet=find.GetFilePath(); //�t�@�C���p�X�擾
						break;
				   }
				   nCnt++;				   
				} 
				ReturnString(wFlags,pVarResult,strRet); //�߂�l�Z�b�g							
			}
			break;
		case FUNC_GETBITMAPNUM://�w��t�H���_���r�b�g�}�b�v���̎擾
			{
				CString strFolder = pDispParams->rgvarg[1].bstrVal; //�t�H���_�p�X
				CString strExt = L"*.";
				strExt += pDispParams->rgvarg[0].bstrVal; //�g���q
				int nCnt = 0; //�r�b�g�}�b�v�t�@�C�����J�E���^�[
				CFileFind find;
				BOOL isWorking = find.FindFile(strExt);
				while(isWorking){
				   isWorking = find.FindNextFile();
				   nCnt++;
				} 
				ReturnInt(wFlags,pVarResult,nCnt); //�X�N���v�g�ɓn��
			}
			break;
		case FUNC_TEXTOUTPUT: //�������ʏo��
			{
				CString strText; //�\��������
				COLORREF color = COLOR_WHITE; //�W�������F
				if(pDispParams->cArgs == 2 ){ //������2��
					int nNo = GetValientInt(&pDispParams->rgvarg[1]); //�\���s��
					CString strText = pDispParams->rgvarg[0].bstrVal; //�\��������
					GetOutputWnd()->OutputText(nNo,strText);
				}else if(pDispParams->cArgs == 1) {//�������P��
					CString strText = pDispParams->rgvarg[0].bstrVal; //�\��������
					GetOutputWnd()->OutputText(strText);
				}								
				
			}
			break;
		case FUNC_MESSAGE: //���b�Z�[�W�\��
			{
				if( pDispParams->cArgs == 5 ){ //������4��				
					int nNo = GetValientInt(&pDispParams->rgvarg[4]); //�\���s��
					CString strText = pDispParams->rgvarg[3].bstrVal; //�\��������
					int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
					int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
					int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
					Message(nNo,strText,RGB(nR,nG,nB));
				}else if(pDispParams->cArgs == 2 ){//������1��
					int nNo = GetValientInt(&pDispParams->rgvarg[1]); //�\���s��
					CString strText = pDispParams->rgvarg[0].bstrVal; //�\��������
					Message(nNo,strText,COLOR_WHITE);
				}
			}
			break;

		case FUNC_SETSTATUSBARTEXT: //�X�e�[�^�X�o�[�����Z�b�g
			{
				CString str = pDispParams->rgvarg[0].bstrVal; //�\��������
				//GetFrame()->GetStatusBarPtr()->SetPaneText(0,str);
			}
			break;
		case FUNC_OUTPUTCLEAR: //������o�̓N���A
			OutputClear();
			break;

		case FUNC_SETVIEWRATE:	//�\�����W�{���ݒ�
			{
				double dbRateX = GetValientDbl(&pDispParams->rgvarg[1]); //x�����{��
				double dbRateY = GetValientDbl(&pDispParams->rgvarg[0]); //y�����{��
				SetViewRate(dbRateX,dbRateY);
			}
			break;
		case FUNC_GETVIEWRATECX:	//�\���{���擾 x���� 
			{
				double dbRateCx = GetView()->GetRateCx();	
				ReturnDbl(wFlags,pVarResult,dbRateCx ); //�߂�l�Z�b�g
			}
			break;
		case FUNC_GETVIEWRATECY:	//�\���{���擾 y����
			{
				double dbRateCy = GetView()->GetRateCy();	
				ReturnDbl(wFlags,pVarResult,dbRateCy); //�߂�l�Z�b�g
			}
			break;
		case FUNC_SHOWWINDOW: //���C��Window�\����ԕύX
			{
				int nShow = GetValientInt(&pDispParams->rgvarg[0]); //�\�����
				GetFrame()->ShowWindow(nShow); //�\����ԕύX
			}
			break;
		case FUNC_MOVEWINDOW: //���C��Window�\���T�C�Y�ύX
			{
				int nX = GetValientInt(&pDispParams->rgvarg[3]); //x���W
				int nY = GetValientInt(&pDispParams->rgvarg[2]); //y���W
				int nW = GetValientInt(&pDispParams->rgvarg[1]); //��
				int nH = GetValientInt(&pDispParams->rgvarg[0]); //����
				GetFrame()->MoveWindow(nX,nY,nW,nH); //�\����ԕύX
			}
			break;
		case FUNC_VIEWHEIGHT:	//���ʏo�͍����ύX	
			{
				int nH = GetValientInt(&pDispParams->rgvarg[0]); //����
//				GetFrame()->SetViewHeight(nH);
			}
			break;
		case FUNC_VIEWWIDTH:		//���ʏo�͕��ύX
			{
				int nW = GetValientInt(&pDispParams->rgvarg[0]); //��
//				GetFrame()->SetViewWidth(nW);
			}
			break;
		case FUNC_SETTEXT:	//�ėp������L��
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				CString strVal = pDispParams->rgvarg[0].bstrVal; //�L��������擾
				SetText(strKey,strVal);//��������L��������
			}
			break;
		case FUNC_GETTEXT:	//�ėp������擾
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				CString strVal = GetText(strKey);	//������擾;
				ReturnString(wFlags,pVarResult,strVal); //�߂�l�Z�b�g
			}
			break;
		case FUNC_SETNUM:	//�ėp���l�ݒ�
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				int nVal = GetValientInt(&pDispParams->rgvarg[0]); //�ݒ�l
				SetNum(strKey,nVal);
			}
			break;
		case FUNC_GETNUM:	//�ėp���l�擾
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				int nVal = GetNum(strKey);//���l�擾
				ReturnInt(wFlags,pVarResult,nVal); //�߂�l�Z�b�g
			}
			break;
		case FUNC_SETDBL:	//�ėp�����ݒ�
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				double dbVal = GetValientDbl(&pDispParams->rgvarg[0]); //�ݒ�l
				SetDbl(strKey,dbVal);
			}
			break;
		case FUNC_GETDBL:	//�ėp�����擾
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				double dbVal = GetDbl(strKey);//���l�擾
				ReturnDbl(wFlags,pVarResult,dbVal); //�߂�l�Z�b�g
			}
			break;
		case FUNC_CREATE_FOLDER:	//�w��t�H���_�쐬
			{
				::SetCurrentDirectory( GetApp()->GetAppPath() ); //�J�����g�f�B���N�g���ݒ�
				CString strDirectory = pDispParams->rgvarg[0].bstrVal; //�t�H���_����
				if( strDirectory.Find(_T("TODAY")) >= 0 ){//�{���̓��t�Ńt�H���_�쐬
					CTime time = CTime::GetCurrentTime(); //���ݎ����擾
					CString strDate = time.Format(_T("%Y%m%d"));
					strDirectory.Replace(_T("TODAY"),strDate);
				}
				::CreateDirectory(strDirectory,NULL);
			}
			break;
		case FUNC_SENDMESSAGE:	//�R�}���h�Ăяo��
			{
				UINT unID = 0;
				WPARAM wParam = 0;
				LPARAM lParam = 0;
				int nIndex = pDispParams->cArgs;
				if( nIndex >= 1 ){ //������1�ȏ�
					unID = pDispParams->rgvarg[nIndex-1].ulVal; //���b�Z�[�WID
				}
				if(nIndex >= 2){//������2�ȏ�
					wParam = pDispParams->rgvarg[nIndex-2].ulVal; //wParam
				}
				if(nIndex >= 3){//������3�ȏ�
					lParam = pDispParams->rgvarg[nIndex-3].ulVal; //lParam
				}
				int nRet = SendMessage(unID,wParam,lParam);
				ReturnInt(wFlags,pVarResult,nRet); //�߂�l�Z�b�g
			}
			break;
		case FUNC_POSTMESSAGE:
			{
				UINT unID = 0;
				WPARAM wParam = 0;
				LPARAM lParam = 0;
				int nIndex = pDispParams->cArgs;
				if( nIndex >= 1 ){ //������1�ȏ�
					unID = pDispParams->rgvarg[nIndex-1].ulVal; //���b�Z�[�WID
				}
				if(nIndex >= 2){//������2�ȏ�
					wParam = GetValientInt(&pDispParams->rgvarg[nIndex-2]); //wParam
				}
				if(nIndex >= 3){//������3�ȏ�
					lParam = GetValientInt(&pDispParams->rgvarg[nIndex-3]); //lParam
				}
				GetFrame()->PostMessage(unID,wParam,lParam); //User���b�Z�[�W
			}
			break;
		case FUNC_SETRECT: //�̈�ݒ�
			{
				int nLeft   = pDispParams->rgvarg[3].ulVal; //��
				int nTop    = pDispParams->rgvarg[2].ulVal; //��
				int nRight  = pDispParams->rgvarg[1].ulVal; //�E
				int nBottom = pDispParams->rgvarg[0].ulVal; //��
				this->SetRect(nLeft,nTop,nRight,nBottom); //�̈�ݒ�
			}
			break;
		case FUNC_GETTIME:	//�}���e�B���f�A�^�C�}�[�擾
			{
			
				DWORD dwTime = timeGetTime();
				ReturnUint(wFlags,pVarResult,dwTime); //Uint
			}
			break;
		case FUNC_GETVALUE:	//�w��l�擾
			{
				double dbVal = 0;
				CString strType = pDispParams->rgvarg[0].bstrVal; //�l�w��
				if( strType == "CLICKLINE" ){
					dbVal = GetView()->GetClickLineLength();//�N���b�N���C���擾
				}
				ReturnDbl(wFlags,pVarResult,dbVal);
			}
			break;
		case FUNC_SETLINECOLOR://���F�ݒ�
			{
				int r = pDispParams->rgvarg[2].ulVal; //R
				int g = pDispParams->rgvarg[1].ulVal; //G
				int b = pDispParams->rgvarg[0].ulVal; //B
				Color color(r,g,b);
				GetView()->SetClickClineColor(color);
			}
			break;
		case FUNC_GETPRIVATEPROFILEINT://.ini�t�@�C���l�擾
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				int nDefault   = pDispParams->rgvarg[1].intVal; //Default�l
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);	
				int nRet = ::GetPrivateProfileInt(strSec,strKey,nDefault,strPath);
				ReturnInt(wFlags,pVarResult,nRet);				
			}
			break;
		case FUNC_WRITEPRIVATEPROFILEINT: //.ini�t�@�C���l��������
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				int nVal       = pDispParams->rgvarg[1].intVal; //�ݒ�l
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);	
				CString strVal;
				strVal.Format(L"%d",nVal);
				::WritePrivateProfileString(strSec,strKey,strVal,strPath);
			}
			break;
		case FUNC_GETPRIVATEPROFILEDBL:		//.ini�t�@�C��Dbl�l�擾
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				double dbVal = pDispParams->rgvarg[1].dblVal; //Double�l
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strDef;
				strDef.Format(L"%.f",dbVal);
				CString strRet = GetIniString(strSec,strKey,strDef,strFileName);
				double dbRet = _wtof(strRet);
				ReturnDbl(wFlags,pVarResult,dbRet);
			}
			break;
		case FUNC_WRITEPRIVATEPROFILEDBL:	//.ini�t�@�C��Dbl�l��������
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				double dbVal = pDispParams->rgvarg[1].dblVal; //Double�l
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);
				::WritePrivateProfileDbl(strSec,strKey,L"%.10f",dbVal,strPath);
			}
			break;
		case FUNC_GETPRIVATEPROFILESTRING://.ini�t�@�C��������擾
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				CString strDef = pDispParams->rgvarg[1].bstrVal; //������
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strRet = GetIniString(strSec,strKey,strDef,strFileName);
				ReturnString(wFlags,pVarResult,strRet);
			}
			break;
		case FUNC_WRITEPRIVATEPROFILESTRING://.ini�t�@�C�������񏑂�����
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //�Z�N�V����
				CString strKey = pDispParams->rgvarg[2].bstrVal; //�L�[
				CString strString = pDispParams->rgvarg[1].bstrVal; //������
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //�t�@�C������
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);					
				::WritePrivateProfileString(strSec,strKey,strString,strPath);
			}
			break;
		case FUNC_GETPROFILEINT: //���W�X�g���l�擾
			{
				CString strEntry = pDispParams->rgvarg[0].bstrVal; //�G���g��
				int nRet = GetProfileInt(strEntry);
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_WRITEPROFILEINT: //���W�X�g���l�ݒ�
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //�G���g��
				int nValue = pDispParams->rgvarg[0].intVal; //�������ޒl
				SetProfileInt(strEntry,nValue);
			}
			break;
		case FUNC_GETPROFILEDBL:	//���W�X�g���l�����擾
			{
				CString strEntry   = pDispParams->rgvarg[0].bstrVal; //�G���g��
				double dbRet = GetProfileDbl(strEntry);
				ReturnDbl(wFlags,pVarResult,dbRet);
			}
			break;
		case FUNC_WRITEPROFILEDBL:	//���W�X�g���l�����ݒ�
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //�G���g��
				double dbValue = GetValientDbl(&pDispParams->rgvarg[0]); //�������ޒl
				SetProfileDbl(strEntry,dbValue);
			}
			break;
		case FUNC_GETPROFILESTRING:	//���W�X�g���l�擾
			{
				CString strEntry   = pDispParams->rgvarg[0].bstrVal; //�G���g��
				CString strRet = this->GetProfileString(strEntry);
				ReturnString(wFlags,pVarResult,strRet);
			}
			break;
		case FUNC_WRITEPROFILESTRING:	//���W�X�g���l�ݒ�
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //�G���g��
				CString strValue   = pDispParams->rgvarg[0].bstrVal; //�������ޒl
				this->SetProfileString(strEntry,strValue);
			}
			break;
		case FUNC_SETINPUTNAME:		//GUI���͒l�ύX
			{
				CString strName = pDispParams->rgvarg[1].bstrVal; //Key
				int nValue = pDispParams->rgvarg[0].intVal; //Value
				//CKIBOFrame *pFrame = GetFrame();
				BOOL isOK = FALSE;// pFrame->SetInputName(strName, nValue);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_CENTERPOINT:	//���S�_�\���E��\��
			{
				BOOL isVisible = pDispParams->rgvarg[0].boolVal; //Value
				CenterPoint(isVisible);
			}
			break;
		case FUNC_ENABLEWINDOW:	//Menu���{�^�� Enable/DisEnable
			{
				BOOL isEnable = pDispParams->rgvarg[0].boolVal; //Value
				EnableWindow(isEnable);
			}
			break;
		case FUNC_GETPIXELSIZE: //1��f������̑傫�����擾
		{
			double dbPixel = GetValientDbl(&pDispParams->rgvarg[0]); //
			double dbRet = GetPixelSize(dbPixel);
			ReturnDbl(wFlags, pVarResult, dbRet);
		}
			break;
		case FUNC_SELECTFOLDER:	//�t�H���_�I��
			{
				CFolderDialog dlg;
				dlg.m_ofn.lpstrInitialDir = GetApp()->GetAppPath();
				dlg.DoModal();
				ReturnString(wFlags,pVarResult,dlg.m_strFolder); //�߂�l�Z�b�g
			}
			break;
		case FUNC_SELECTBITMAP:	//�r�b�g�}�b�v�t�@�C���I��
			{
				CString strFileName = ::SelectBitmapFile(TRUE,GetApp()->GetAppPath());
				ReturnString(wFlags,pVarResult,strFileName); //�߂�l�Z�b�g
			}
			break;
		case FUNC_EXEC: //exe�t�@�C�����s
			if (pDispParams->cArgs == 1) {//������1��
				CString strPath = pDispParams->rgvarg[0].bstrVal; //Exe File Path 
				::Exec(SW_HIDE,strPath.GetBuffer(),L"");
				ReturnBool(wFlags, pVarResult, TRUE);
			}
			break;
		case FUNC_READTEXT:	//������S�s�ǂݍ���
			{
				m_aryText.RemoveAll();
				//������1��
				CString strPath = pDispParams->rgvarg[0].bstrVal; //File Path

				CString line;
				FILE *pFile = fopen("result.txt", "r, ccs=UTF-8");
				if (pFile != NULL) {
					CStdioFile file(pFile);
					CString strLine;
					while (file.ReadString(strLine)) {
						m_aryText.Add(strLine);//�P�s�Âo�^
					}
				}
				if (pFile != NULL) {
					fclose(pFile);
				}
			}
			break;
		case FUNC_READSTRING: //������P�s�ǂݍ���
			{
				CString strPath = pDispParams->rgvarg[1].bstrVal; //File Path
				int nLine = pDispParams->rgvarg[0].iVal;
				CString strLine;
				if (nLine < m_aryText.GetCount()) {
					strLine = m_aryText.GetAt(nLine); //1�s������������o��
				}
				ReturnString(wFlags, pVarResult, strLine); //�߂�l�Z�b�g
			}
			break;
	}
	return S_OK;
}

//�R�}���h

//�S�폜
void App::RemoveAll()
{
	GetView()->m_mgrVO.RemoveAll(); //�`��v�fClear
}

//��`�`��
void App::DrawRect(CString strName,int nLeft,int nTop,int nRight,int nBottom,int nA,int nR,int nG,int nB,BOOL isPaint)
{
	CRect rect(nLeft,nTop,nRight,nBottom);
	Color color(nR,nG,nB);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,color,isPaint); //��`�`��
}
//�ȉ~�`��
void App::DrawCircle(CString strName, int nLeft, int nTop, int nRight, int nBottom, int nA, int nR, int nG, int nB, BOOL isPaint)
{
	CRect rect(nLeft, nTop, nRight, nBottom);
	Color color(nR, nG, nB);
	GetView()->m_mgrVO.CreateCircleObject(strName, rect, color, isPaint);
}
//���C���`��
void App::DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nA,int nR,int nG,int nB)
{
	DrawLine(strName,nXS,nYS,nXE,nYE,nR,nG,nB);
}
void App::DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nR,int nG,int nB)
{
	CLine line(nXS,nYS,nXE,nYE);
	Color color(nR,nG,nB);	
	GetView()->m_mgrVO.CreateLineObject(strName,line,color);//���C���`��
}
void App::DrawLine(CString strName, int nXS, int nYS, int nXE, int nYE, COLORREF cr)
{
	CLine line(nXS, nYS, nXE, nYE);
	Color color;
	color.SetFromCOLORREF(cr);
	GetView()->m_mgrVO.CreateLineObject(strName, line, color);//���C���`��
}

//�}�E�X�N���b�N�҂�
BOOL App::Wait(DWORD dwTime)
{
	DWORD dwStart = GetTickCount();
	MSG msg;
	while(IsWindowOK() && GetTickCount() >= dwStart && GetTickCount() - dwStart < dwTime ){
        if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
			if( msg.message == WM_CLOSE || GetApp()->IsEnd() ){
				return FALSE; //�ُ�I��
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
	}
	return TRUE;//����I��
}

//�}�E�X�N���b�N�҂�
BOOL App::ClickWait()
{
	MSG msg;
	while( IsWindowOK() ){
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CLOSE){
				return FALSE; //�ُ�I��
			}
			if(WM_LBUTTONDOWN == msg.message || msg.message == WM_RBUTTONDOWN){
				return TRUE; //����I��
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		Sleep(0);
	}
	return FALSE;//�ُ�I��
}
//�L�[�{�[�h���b�Z�[�W�҂�
BOOL App::KeyWait(int nKeyID)
{
	KIBOFrame *pFrame = ::GetFrame();
	MSG msg;
	while( IsWindowOK() ){
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CLOSE){
				return FALSE; //�ُ�I��
			}
			if(WM_KEYUP == msg.message){
				if(msg.wParam == nKeyID){
					return TRUE; //����I��
				}else if(msg.wParam == VK_ESCAPE){ //ESC Key
					return FALSE; //�L�����Z��
				}
				
			}
			if(!::TranslateAccelerator(pFrame->GetSafeHwnd(), m_hAccel, &msg) ){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		Sleep(0);
		if( nKeyID == 0 ){ //���[�v����
			if( GetAsyncKeyState(VK_ESCAPE) < 0 ){
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;//�ُ�I��
}

//�S��ʏ�������
void App::Render()
{
	GetView()->Render(TRUE);
}

//������擾
CString App::GetText(const CString &strKey)
{
	CString strValue = L"";
	m_mapString.Lookup(strKey,strValue);
	return strValue;
}
//��������L��������
void App::SetText(CString &strKey,CString &strVal)
{
	m_mapString.SetAt(strKey,strVal);
}
//�l�擾
int App::GetNum(const CString &strKey)
{
	CString strValue;
	if( m_mapString.Lookup(strKey,strValue) ){
		return _wtoi(strValue);
	}
	return 0;
}
//�l�ݒ�
void App::SetNum(CString strKey,int nVal)
{
	CString strVal; //������ϊ�
	strVal.Format(L"%d",nVal);
	m_mapString.SetAt(strKey,strVal);//������Ƃ��ċL��������
}
//�����ݒ�
void App::SetNum(int nVal,LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//���s�t�������񏑂�����
	SetNum(strFormat,nVal);
}
//�����擾 
double App::GetDbl(const CString &strKey)
{
	CString strValue;
	if( m_mapString.Lookup(strKey,strValue) ){
		return _wtof(strValue);
	}
	return 0;
}
double App::GetDbl(LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//���s�t�������񏑂�����
	return GetDbl(strFormat);
}
//�����ݒ�
void App::SetDbl(CString strKey,double dbVal)
{
	CString strVal; //������ϊ�
	strVal.Format(L"%f",dbVal);
	m_mapString.SetAt(strKey,strVal);//������Ƃ��ċL��������
}
 //�����ݒ�
void App::SetDbl(double dbVal,LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//���s�t�������񏑂�����
	SetDbl(strFormat,dbVal);
}
//���W�X�g���l�擾
int App::GetProfileInt(LPCWSTR strEntry) 
{
	return GetApp()->GetProfileInt(m_strSectionInt,strEntry,-1);
}
//���W�X�g���l�ݒ�
void App::SetProfileInt(LPCWSTR strEntry,int nValue) 
{
	GetApp()->WriteProfileInt(m_strSectionInt,strEntry,nValue);
}

//���W�X�g���l�擾
double App::GetProfileDbl(LPCWSTR strEntry) 
{
	CString strVal = GetProfileString(strEntry);
	return _wtof(strVal);
}
//���W�X�g���l�ݒ�
void App::SetProfileDbl(LPCWSTR strEntry,double dbValue) 
{
	CString strValue;
	strValue.Format(L"%f",dbValue);
	SetProfileString(strEntry,strValue);
}

//���W�X�g��������擾
CString App::GetProfileString(LPCWSTR strEntry) 
{
	return GetApp()->GetProfileString(m_strSectionString,strEntry,L"");
}

//���W�X�g��������ݒ�
void App::SetProfileString(LPCWSTR strEntry,CString& strValue) 
{
	GetApp()->WriteProfileString(m_strSectionString,strEntry,strValue);
}

//.exe�t�@�C�����̕�����擾
CString App::GetIniString(LPCTSTR lpszSec,LPCTSTR lpszKey,LPCTSTR lpszDef,LPCTSTR lpszFileName)
{
	CString strPath;
	strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),lpszFileName);
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));
	::GetPrivateProfileString(lpszSec,lpszKey,lpszDef,szBuf,sizeof(szBuf),strPath);
	return CString(szBuf);	
}
//���W�\���{���ݒ�
void App::SetViewRate(double dbRateX,double dbRateY) 
{
	GetView()->SetViewRate(dbRateX,dbRateY);
}

//�w��I�u�W�F�N�g�ȊO���폜
void App::RemoveExcept(LPCTSTR lpszName) 
{
	GetView()->m_mgrVO.RemoveExcept( lpszName ); 
}
void App::DrawText(LPCTSTR lpszID,LPCTSTR lpszText,int nX,int nY) 
{
	GetView()->m_mgrVO.CreateStringObject(lpszID,nX,nY,lpszText,GetTextColor(),GetLogFont()); //��`�`��
}
//���b�Z�[�W�\��
void App::Message(int nNo,LPCTSTR lpszText,COLORREF color)
{
//	GetFrame()->DlgBarMsg(nNo,lpszText,color);
}
//���b�Z�[�W����
int App::SendMessage(UINT unID,WPARAM wParam/*=0*/,LPARAM lParam/*=0*/)
{
	return (int)GetFrame()->SendMessage(unID,wParam,lParam); //User���b�Z�[�W
}

//�\���؂�ւ�
void App::SetVisible(CString &strID,bool isVisible)
{
	VisualObject* pVO = GetView()->m_mgrVO.GetVO(strID);
	if( pVO != NULL ){
		pVO->SetVisible(isVisible);
	}
}

//������o�̓N���A
void App::OutputClear()
{
	GetOutputWnd()->Clear();
}
//������o��
void App::TextOutput(CString &str)
{
	GetOutputWnd()->OutputText(str);
}
void App::TextOutput(LPCTSTR lpszText)
{
	CString str(lpszText);
	GetOutputWnd()->OutputText(str);
}
//�P��f������̑傫����mm�P�ʂŎ擾
double App::GetPixelSize(double dbPixel)
{
	CString strPath;
	strPath.Format(L"%s\\kibosystem.ini", GetApp()->GetAppPath());

	double dbDmin = DBL_MAX; //�ŏ��l
	double dbPerSelect = 0;//mm / pixel
	CString strSection = L"CALIBRATION"; //�L�����u���[�V�����Z�N�V����
	double dbPerPixel = ::GetPrivateProfileDbl(strSection, L"PERPIXEL", 0, strPath);
	return dbPerPixel;
}





CStringW ConvertUTF8ToUnicode(const CStringA& utf8)
{
	if (utf8.IsEmpty()) return L""; // nothing to do
	CStringW uni;
	int cc = 0;
	// get length (cc) of the new widechar excluding the \0 terminator first
	if ((cc = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0) - 1) > 0)
	{
		// convert
		wchar_t *buf = uni.GetBuffer(cc);
		if (buf) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, buf, cc);
		uni.ReleaseBuffer();
	}
	return uni;
}