#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOView.h"
#include "App.h"
#include "CharRecog.h"
#include "../LIB/typedef.h"
#include "../LIB/FilePlus.h"
#include "../LIB/Object.h"
#include "../LIB/macro.h"
#include "../LIB/Gdi.h"

#ifdef USE_CHARRECOG 

#define LABEL_MIN		  30	//�̈���������𔻒肷��Ƃ��̕����̍ŏ���f�� Default
#define LABEL_MAX		90000	//�̈���������𔻒肷��Ƃ��̕����̍ő��f�� Default
#define LABEL_MIN_CHAR	   5	//1�������̗̈�Ō������鉘��̑傫��
#define ADJUST_SIZE		   5	//�������J�n�T�C�Y
#define ADJUST_NUM		  10	//��������

//�̈悲�Ƃ̕����T�C�Y�擾
CSize CharRecog::GetCharPixels(int nRnum)
{
	CSize size(LABEL_MIN,LABEL_MAX); //Default�ݒ�
	m_mapCharPixels.Lookup(nRnum,size);
	return size;
}
//�̈悲�Ƃ̕����T�C�Y�ݒ�

void CharRecog::SetCharPixels(int nRnum,int nMin,int nMax)
{
	CSize size(nMin,nMax); 
	m_mapCharPixels.SetAt(nRnum,size);	
}

#define CHAR_SPACE_X	   3 //�����]��������
#define CHAR_SPACE_Y	   5 //�����]���c����

static TCHAR s_tcCheckRectName[] = L"�����̈�";
static TCHAR s_tcError = '?'; //�G���[����

//�R�}���h�ꗗ
static CommandTable s_table[] =
{
	 {CharRecog::FUNC_RECOGNIZE_CHAR	, 2,L"RecognizeChar"}		//�󎚌���
	,{CharRecog::FUNC_WHITE				, 2,L"White"}				//����������
	,{CharRecog::FUNC_SETERASEDOT		, 2,L"SetEraseDot"}			//�h�b�g�������폜��
	,{CharRecog::FUNC_DIRTY_CHECK		, 2,L"DirtyCheck"}			//���ꌟ��
	,{CharRecog::FUNC_INCLINATION		, 2,L"Inclination"}			//�X��
	,{CharRecog::FUNC_SET_COMMON		, 2,L"SetCommon"}			//�����̈悲�Ƃ̕�����𕪂��邩�ǂ���
	,{CharRecog::FUNC_SET_CHECK_RECT	, 6,L"SetCheckRect"}		//�����̈�̐ݒ�
	,{CharRecog::FUNC_READ_SAMPLE		,-1,L"ReadSample"}			//�T���v���摜�ǂݍ���
	,{CharRecog::FUNC_DRAWRESULT		, 3,L"DrawResult"}			//�F��������̕`��
	,{CharRecog::FUNC_SETCHARWIDTH		,-1,L"SetCharWidth"}		//�������w��
	,{CharRecog::FUNC_SETCHARINDEXWIDTH	,-1,L"SetCharIndexWidth"}	//�Œ蕶�����w��
	,{CharRecog::FUNC_DEFAULT_CHARWIDTH	,-1,L"SetDefaultCharWidth"} //�W���������w�� 
	,{CharRecog::FUNC_SET_CHAR_SIZE		, 3,L"SetCharSize"}			//�̈������������f���ݒ�
	,{CharRecog::FUNC_SET_AVENUM		,-1,L"SetAverageNum"}		//���ϗp�B�e�񐔎擾
	,{CharRecog::FUNC_SET_CONTRAST		, 2,L"SetContrast"}			//�R���g���X�g�̐ݒ�
	,{CharRecog::FUNC_SET_NUMBER		, 2,L"SetNumber"}			//���l�ݒ�
	,{CharRecog::FUNC_SET_ALPHA			, 2,L"SetAlpha"}			//�A���t�@�x�b�g�ݒ�
	,{CharRecog::FUNC_SET_INIFILE		, 1,L"SetIniFile"}			//IniFile�͎g�p���܂���
	,{CharRecog::FUNC_GET_CHECKSTRING	, 2,L"GetCheckString"}		//�w�茟���̈�̕�������擾
	,{CharRecog::FUNC_GET_MIN_WIDTH		, 2,L"GetMinWidth"}			//�w�茟���̈�̕������ŏ��l���擾
	,{CharRecog::FUNC_TEXTOUT_CHARWIDTH	, 1,L"TextOutCharWidth"}	//�������ݒ�o��
	,{CharRecog::FUNC_SET_LENGTH_TH		, 2,L"SetLengthTh"}			//�����F���������l�ݒ�
	,{CharRecog::FUNC_BIN_CHECK_RECT	, 3,L"BinaryCheckRect"}		//�����̈悲�Ƃ�2�l��
	,{CharRecog::FUNC_MAX,0,NULL}									//�R�}���h��
};

static CharRecog *s_pInstance = NULL; //�B���OPEN�N���X����
//CharRecog���̎擾
CharRecog* CharRecog::GetInstance() 
{
	return s_pInstance;
}

//�R���X�g���N�^
CharRecog::CharRecog(void)
{
	s_pInstance = this;	//���̂�ݒ�
	m_nAnum = 1; //���ώ擾�p�B�e��
	m_dwWaitTime = 100;//�L���v�`���t���[���Ԋu�^�C�� msec
	m_byContrastMin = MIN_CONVERT; //�R���g���X�g�ŏ��l�ݒ�
	m_byContrastMax = MAX_CONVERT; //�R���g���X�g�ő�l�ݒ�

	m_isIniFile = true; //IniFile�͕K�v�ł����H
}

//�f�X�g���N�^
CharRecog::~CharRecog(void)
{
}

//�֐����`�F�b�N
STDMETHODIMP CharRecog::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
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

//�������s
STDMETHODIMP CharRecog::Invoke(DISPID dispIdMember	//Function ID �ԍ�
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
		case FUNC_RECOGNIZE_CHAR: //�����F��
			{
				int nCnum = GetValientInt(&pDispParams->rgvarg[1]);//�J�����ԍ�
				int nTh = GetValientInt(&pDispParams->rgvarg[0]);//2�l���������l
				CString strRet = RecognizeChar(nCnum,nTh);
				if(IsPropertyGet(wFlags)){ //�߂�l����
					ReturnString(wFlags,pVarResult,strRet); //�������n��
				}
				return S_OK;
			}
			break;
		case FUNC_WHITE:	//����������
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//�̈�ԍ�
				m_mapIsWhite[nRnum] = pDispParams->rgvarg[0].boolVal ? true : false;//�������ł����H
				return S_OK;
			}
			break;
		case FUNC_SETERASEDOT: //�h�b�g�������폜
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//�̈�ԍ�
				int nEnum = GetValientInt(&pDispParams->rgvarg[0]);//�h�b�g�����폜��
				m_mapEraseDotNum[nRnum] = nEnum;//�h�b�g�����폜���ݒ�
				return S_OK;
			}
			break;
		case FUNC_DIRTY_CHECK:	//���ꌟ��
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//�̈�ԍ�
				bool isCheck = pDispParams->rgvarg[0].boolVal ? true : false; //���ꌟ�����s�����ǂ���
				m_mapIsDitrtyCheck[nRnum] = isCheck;
				return S_OK;
			}
			break;
		case FUNC_INCLINATION:	//�X��
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//�̈�ԍ�
				bool isInclination = pDispParams->rgvarg[0].boolVal ? true : false; //���ꌟ�����s�����ǂ���
				m_mapIsInclination[nRnum] = isInclination;
				return S_OK;
			}
			break;
		case FUNC_SET_COMMON:	//�����̈悲�Ƃ̕�����𕪂��邩�ǂ���
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//�̈�ԍ�
				BOOL isCommon = pDispParams->rgvarg[0].boolVal; //���ʕ���
				m_mapCommonChar[nRnum] = isCommon;
				return S_OK;
			}
			break;
		case FUNC_SET_CHECK_RECT: //�����̈�̐ݒ�
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[5]);	//�̈�ԍ�
				int nCharNum = GetValientInt(&pDispParams->rgvarg[4]); //������
				CheckRect crect; //�����̈�
				crect.m_nCharNum    = nCharNum;//������
				crect.m_rect.left   = GetValientInt(&pDispParams->rgvarg[3]); //��
				crect.m_rect.top    = GetValientInt(&pDispParams->rgvarg[2]); //��
				crect.m_rect.right  = GetValientInt(&pDispParams->rgvarg[1]); //�E
				crect.m_rect.bottom = GetValientInt(&pDispParams->rgvarg[0]); //��
				m_mapCheckRect[nRnum] = crect; //�����̈�Z�b�g
				m_mapCheckRectOld[nRnum] = crect.m_rect;
				//���x�����O�����̈�`��
				CString strName; //�`��I�u�W�F�N�g����
				strName.Format(L"%s%d",s_tcCheckRectName,nRnum);
				CKIBOView *pView = GetView();
				pView->m_mgrVO.CreateRectObject(strName,crect.m_rect,Color::Blue,false); //��`�`��
				return S_OK;
			}
			break;
		case FUNC_READ_SAMPLE: //�T���v���f�[�^�ǂݍ���
			{
				int nTh = GetValientInt(&pDispParams->rgvarg[2]); //�������l
				CString strFile = pDispParams->rgvarg[1].bstrVal;	//�t�@�C�����擾
				CString strNumber = pDispParams->rgvarg[0].bstrVal;	//�f�[�^�������				
				CString strFileBin = strFile; //�o�C�i���f�[�^�t�@�C��
				strFileBin.Replace(L".bmp",L".bin"); //�t�@�C�����쐬
				strFileBin.Replace(L".png",L".bin"); //�t�@�C�����쐬

				CStringArray sAry; //������z��
				int nRmax = SplitToken(' ',sAry,strNumber); //���p�󔒋�؂�
				for(int nRnum = 0;nRnum<nRmax;nRnum++){ //�����̈�̐��������[�v
					CString strNum = sAry[nRnum]; //�����̈悲�Ƃ̕���
					if( m_mapCheckRect[nRnum].m_nCharNum == 0 ){ //�Œ蕶�����̏ꍇ
						if( m_mapCheckRect[nRnum].m_str != strNum ){
							m_mapCheckRect[nRnum].m_str = strNum; //�Œ蕶���Z�b�g
						}
					}
				}
				CFileFind find;
				if(find.FindFile(strFileBin)){ //�t�@�C�������݂��鎞����
					find.FindNextFile();
					if( find.GetLength() > 0 ){//�t�@�C���T�C�Y�����݂���Ƃ�����
						ReadElement(strFileBin,sAry); //���������ǂݍ��݁@& m_aryCharElement�֒ǉ�
						return S_OK;
					}
					return S_FALSE;
				}else{ //�����L�^����Ă��Ȃ� or �o�[�W�����ύX			
					if( !ReadSample(nTh,strFile,strFileBin,strNumber) ){
						//�t�@�C�������Ă���Ƃ�
						::DeleteFile(strFileBin); //��ꂽ�t�@�C���͍폜����
						return S_FALSE;
					}
				}
				return S_OK;
			}
			break;
		case FUNC_DRAWRESULT://�F��������̕`��
			{
				CString strNumbers = pDispParams->rgvarg[2].bstrVal; //�F��������
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //�����`��J�nx���W
				int nHeight = GetValientInt(&pDispParams->rgvarg[0]); //�����̍���
				BOOL isOK = DrawResult(strNumbers,nX,nHeight);
				ReturnBool(wFlags,pVarResult,isOK); //Script�߂�l 
				return S_OK;
			}
			break;
		case FUNC_SETCHARWIDTH: //�������w��
			{
				CString strChar; //�w�蕶��
				int nRnum   = 0; //�̈�ԍ�
				double dbCx = 0; //��x 
				double dbCy = 0; //��y
				if(pDispParams->cArgs == 4){ //��x,��y�w��
					strChar = pDispParams->rgvarg[3].bstrVal; //�w�蕶��
					nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //�̈�ԍ�
					dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //��x 
					dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //��y
				}else if(pDispParams->cArgs == 3){ //���w��
					strChar = pDispParams->rgvarg[2].bstrVal; //�w�蕶��
					nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //�̈�ԍ�
					dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //��
					dbCy = 0; //�\��
				}
				if( strChar.GetLength() > 0 ){
					TCHAR tch = strChar[0];
					SetLineWidth(tch,nRnum,dbCx,dbCy);
					return S_OK;
				}
			}
			break;
		case FUNC_SETCHARINDEXWIDTH: //�Œ蕶�����w��
			{
				int nIndex  = -1;
				int nRnum   = 0; //�̈�ԍ�
				double dbCx = 0; //��x 
				double dbCy = 0; //��y
				if(pDispParams->cArgs == 4){ //��x,��y�w��
					nIndex  = GetValientInt(&pDispParams->rgvarg[3]); //�������я�index
					nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //�̈�ԍ�
					dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //��x 
					dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //��y
				}else if(pDispParams->cArgs == 3){ //���w��
					nIndex  = GetValientInt(&pDispParams->rgvarg[2]); //�������я�index
					nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //�̈�ԍ�
					dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //��x 
				}
				if( 0 <= nIndex ){
					SetLineWidthIndex(nIndex,nRnum,dbCx,dbCy);
					return S_OK;					
				}
			}
			break;
		case FUNC_DEFAULT_CHARWIDTH: //�W���������w��
			if(pDispParams->cArgs == 3){ //��x,��y�w��
				int nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //�̈�ԍ�
				double dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //��x 
				double dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //��y				
				SetLineWidth(DEFAULT_CHAR_NUM,nRnum,dbCx,dbCy); //DEFAULT_CHAR_NUM(0) is Default Number
				return S_OK;
			}else if(pDispParams->cArgs == 2){ //��x�̂ݎw��
				int nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //�̈�ԍ�
				double dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //��x 
				double dbCy = 0;
				SetLineWidth(DEFAULT_CHAR_NUM,nRnum,dbCx,dbCy); //DEFAULT_CHAR_NUM(0) is Default Number
				return S_OK;
			}
			break;

		case FUNC_SET_CHAR_SIZE://�̈������������f���ݒ�
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[2]);	//�̈�ԍ�
				int nMin = GetValientInt(&pDispParams->rgvarg[1]);	//�ŏ���f��
				int nMax = GetValientInt(&pDispParams->rgvarg[0]);	//�ő��f��
				SetCharPixels(nRnum,nMin,nMax);
				return S_OK;
			}
			break;
		case FUNC_SET_AVENUM://���ϗp�B�e�񐔎擾
			{
				if( pDispParams->cArgs == 1 ){
					int nAnum = GetValientInt(&pDispParams->rgvarg[0]);
					SetAverageNum(nAnum);//�񐔂̂ݐݒ�
					return S_OK;
				}else if(pDispParams->cArgs == 2 ){
					int nAnum = GetValientInt(&pDispParams->rgvarg[1]);
					int nTime = GetValientInt(&pDispParams->rgvarg[0]);
					SetAverage(nAnum,nTime);//�񐔂ƎB�e�Ԋu�ݒ�
					return S_OK;
				}				
			}
			break;
		case FUNC_SET_CONTRAST: //�R���g���X�g�̐ݒ�
			{
				BYTE byMin = (BYTE)pDispParams->rgvarg[1].ullVal; //�ŏ��l
				BYTE byMax = (BYTE)pDispParams->rgvarg[0].ullVal; //�ő�l
				SetContrast(byMin,byMax); //�R���g���X�g�̃����W�ݒ�
				return S_OK;
			}
			break;
		case FUNC_SET_NUMBER:	//���l�ݒ�
			{
				int nRnum = (int)pDispParams->rgvarg[1].ullVal; //�̈�ԍ�
				int nCnum = (int)pDispParams->rgvarg[0].ullVal; //�����ԍ�
				SetNumber(nRnum,nCnum);
				return S_OK;
			}
			break;
		case FUNC_SET_ALPHA:	//�A���t�@�x�b�g�ݒ�
			{
				int nRnum = (int)pDispParams->rgvarg[1].ullVal; //�̈�ԍ�
				int nCnum = (int)pDispParams->rgvarg[0].ullVal; //�����ԍ�
				SetAlpha(nRnum,nCnum);
				return S_OK;
			}
			break;
		case FUNC_SET_INIFILE:	//IniFile�͎g�p���܂���
			m_isIniFile = pDispParams->rgvarg[0].boolVal ? true : false;
			return S_OK;
			break;
		case FUNC_GET_CHECKSTRING:	//�w�茟���̈�̕�������擾
			{
				CString strText = pDispParams->rgvarg[1].bstrVal;	//�f�[�^������擾
				int nRnum = (int)pDispParams->rgvarg[0].ullVal; //�̈�ԍ�
				CStringArray sAry;
				if( ::SplitToken(' ',sAry,strText) > nRnum ){
					strText = sAry[nRnum]; //�̈��������擾
				}
				ReturnString(wFlags,pVarResult,strText); //�������n��
				return S_OK;
			}
			break;
		case FUNC_GET_MIN_WIDTH://�w�茟���̈�̕������ŏ��l���擾
			{
				CString strChar = pDispParams->rgvarg[1].bstrVal; //�w�蕶��
				int nRnum   = GetValientInt(&pDispParams->rgvarg[0]); //�̈�ԍ�
				if( strChar.GetLength() > 0 ){
					TCHAR tch = strChar[0];//�����擾
					double dbVal = GetMinWidth(tch,nRnum);	//�w�蕶�����擾
					ReturnDbl(wFlags,pVarResult,dbVal);	
					return S_OK;
				}				
			}
			break;
		case FUNC_TEXTOUT_CHARWIDTH: //�������ݒ�o��
			{
				int nRnum  = GetValientInt(&pDispParams->rgvarg[0]); //�̈�ԍ�
				TextOutMinWidth(nRnum); //�����̈���̑��݂��镶�����ׂĂ̍ŏ��������o��
			}
			return S_OK;
		case FUNC_SET_LENGTH_TH://�����F���������l�ݒ�
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);	//�̈�ԍ�
				double dbLen = GetValientDbl(&pDispParams->rgvarg[0]); //����
				m_mapLenTh[nRnum] = dbLen; //�����F�������擾
			}
			return S_OK;
		case FUNC_BIN_CHECK_RECT: //�����̈悲�Ƃ�2�l��
			{
				int nCnum = GetValientInt(&pDispParams->rgvarg[2]);	//�J�����ԍ�
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);	//�̈�ԍ�
				int nTh   = GetValientInt(&pDispParams->rgvarg[0]);	//�������l
				BinaryCheckRect(nCnum,nRnum,nTh);
			}
			break;
	}
	return S_FALSE;
}

//�F�����ʕ`��
BOOL CharRecog::DrawResult(CString& strNumbers,int nX,int nHeight)
{
	CKIBOView *pView = GetView();
	pView->m_mgrVO.RemoveByName(L"STRING"); //�S�`��I�u�W�F�N�g���폜
	CStringArray strAry;	//�F��������
	CStringArray strAryR;	//�G���[������
	int nCnt = CollectRecognizeChar(strNumbers,strAry,strAryR); //�F��������擾
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(LOGFONT));
	lf.lfHeight = nHeight;
	lf.lfCharSet = DEFAULT_CHARSET;
	wcscpy_s(lf.lfFaceName,_T("�l�r ����"));
	GetView()->m_mgrVO.CreateStringObject(_T("STRINGL"),nX, 0,_T("�F��������:"),COLOR_WHITE,lf);
	BOOL isOK = TRUE;
	CString strName;
	for(int i=1;i<=nCnt;i++){
		strName.Format(_T("STRING%d"),i); //Green������Name
		isOK &= CreateStringResult(strName,nX,i*nHeight,strAry[i-1],COLOR_GREEN,lf); //�����`�挋�ʕ\��
		strName.Format(_T("STRINR%d"),i); //Red������Name
		pView->m_mgrVO.CreateStringObject(strName,nX,i*nHeight,strAryR[i-1],COLOR_RED,lf);
	}
	for(int i=0;i<nCnt;i++){
		CString strRed = strAryR[i];
		for(int i=0;i<strRed.GetLength();i++){
			TCHAR tc = strRed[i];
			if( tc != ' ' && tc != ',' ){ //�G���[���������݂��邩�ǂ���
				isOK = FALSE; //NG���x��
			}
		}
	}
	//���茋�ʕ\��
	App *pApp = App::GetInstance();	

    COLORREF crResult = pApp->GetProfileInt(L"RESULT.OK.COLOR"); //OK�����F
	lf.lfHeight = pApp->GetProfileInt(L"RESULT.SIZE"); //�����T�C�Y�w��
	strName.Format(_T("STRING%d"),nCnt+1); //���ʕ�����Name
	if( strNumbers.Find(_T("?")) < 0 && isOK){
		pView->m_mgrVO.CreateStringObject(strName,nX,(nCnt+1)*nHeight,pApp->GetProfileString(L"RESULT.OK"),crResult,lf);
		return TRUE;
	}
	crResult = pApp->GetProfileInt(L"RESULT.NG.COLOR"); //NG�����F    
	pView->m_mgrVO.CreateStringObject(strName,nX,(nCnt+1)*nHeight,pApp->GetProfileString(L"RESULT.NG"),crResult,lf);
	return FALSE;
}

//�F��������̎擾
int CharRecog::CollectRecognizeChar(CString strNumber,CStringArray &strAry,CStringArray &strAryR)
{
	CString strRed = GetRight(strNumber,L","); //�ԕ����������擾
	strNumber = GetLeft(strNumber,L","); //�\�����������݂̂��擾
	int nCnt  = ::SplitToken(' ',strAry,strNumber);//�󔒕����ŕ���
	int nCntR = ::SplitToken(',',strAryR,strRed);//�J���}��؂�
	return min(nCnt,nCntR);
}

//�����̎w��
void CharRecog::SetLineWidth(TCHAR tChar,int nRnum,double dbCx,double dbCy)
{
	SizeDbl sizeDbl(dbCx,dbCy);
	LONG lKey = MAKELONG(tChar,nRnum); //�L�[���[�h�쐬
	m_mapAryLineWidth.SetAt(lKey,sizeDbl);
}
//�Œ蕶�������̎w��
void CharRecog::SetLineWidthIndex(int nIndex,int nRnum,double dbCx,double dbCy)
{
	SizeDbl sizeDbl(dbCx,dbCy);
	LONG lKey = MAKELONG(nIndex,nRnum); //�L�[���[�h�쐬
	m_mapAryIndexWidth.SetAt(lKey,sizeDbl);
}

//�ŏ������o�^
void CharRecog::SetMinWidth(TCHAR tChar,int nRnum,double dbWmin)
{
	LONG lKey = MAKELONG(tChar,nRnum); //�L�[���[�h�쐬
	double dbWidth;
	if( m_mapAryMinWidth.Lookup(lKey,dbWidth) ){ //���łɓo�^����Ă���ꍇ
		if(dbWmin < dbWidth){//��菬�����l�Ȃ��
			m_mapAryMinWidth[lKey] = dbWmin;	//�ŏ�����������map���
		}
	}else{//�������œo�^
		m_mapAryMinWidth[lKey] = dbWmin;	//�ŏ�����������map���
	}
}

//�ŏ������擾
double CharRecog::GetMinWidth(TCHAR tChar,int nRnum)
{
	LONG lKey = MAKELONG(tChar,nRnum); //�L�[���[�h�쐬
	double dbWidth = 0;
	if( m_mapAryMinWidth.Lookup(lKey,dbWidth) ){ //�o�^����Ă���ꍇ
		return dbWidth;
	}
	return 0;
}

//�����̈���̑��݂��镶�����ׂĂ̍ŏ��������o��
void CharRecog::TextOutMinWidth(int nRnum)
{
	COutputView *pOutput = GetOutputView();
	CString strText; //�o�̓��b�Z�[�W
	POSITION pos = m_mapAryMinWidth.GetStartPosition();
	LONG lKey=0;		//Key
	double dbVal=0;		//Value
	while (pos != NULL){
		m_mapAryMinWidth.GetNextAssoc(pos, lKey, dbVal);
		TCHAR tch = LOWORD(lKey); //����		
		if( nRnum == HIWORD(lKey) ){ //�����̈悪��v
			strText.Format(L"CharRecog.SetCharWidth('%c',%d,%.4f);",tch,nRnum,dbVal);
			pOutput->Output(strText,COLOR_WHITE,FALSE);
		}
	}    
}

//�����I�ȕ������̎擾
SizeDbl CharRecog::GetLineWidthAll(int nIndex,TCHAR tChar,int nRnum)
{	
	SizeDbl sizeDbl;
	//index�w�肪���݂���
	LONG lKey = MAKELONG(nIndex,nRnum); //�L�[���[�h�쐬
	if( m_mapAryIndexWidth.Lookup(lKey,sizeDbl) ){//�w�蕶���̐����擾
		return sizeDbl;
	}
	//�������Ƃ̎w�肪���݂���
	lKey = MAKELONG(tChar,nRnum); //�L�[���[�h�쐬
	if( m_mapAryLineWidth.Lookup(lKey,sizeDbl) ){//�w�蕶���̐����擾
		return sizeDbl;
	}
	//Default�ݒ���g�p
	return GetDefLineWidth(nRnum);
}

//�w�蕶���𐔒l�݂̂̐ݒ�ɕύX
void CharRecog::SetNumber(int nRnum,int nCnum)
{
	LONG lKey = MAKELONG(nRnum,nCnum); //�L�[���[�h�쐬
	m_mapIsNumber.SetAt(lKey,TRUE);
}
//�w�蕶���͐��l�ł���
BOOL CharRecog::IsNumber(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //�L�[���[�h�쐬

	BOOL isNumber;
	if( m_mapIsNumber.Lookup(lKey,isNumber) ){
		return isNumber; //���l���ǂ���
	}
	return FALSE;//���l�����Ƃ͌���Ȃ�
}
//�w�蕶�����A���t�@�x�b�g�݂̂̐ݒ�ɕύX
void CharRecog::SetAlpha(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //�L�[���[�h�쐬
	m_mapIsAlpha.SetAt(lKey,TRUE);
}
//�w�蕶���̓A���t�@�x�b�g�ł���
BOOL CharRecog::IsAlpha(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //�L�[���[�h�쐬

	BOOL isAlpha;
	if( m_mapIsAlpha.Lookup(lKey,isAlpha) ){
		return isAlpha; //�A���t�@�x�b�g���ǂ���
	}
	return FALSE;//�A���t�@�x�b�g�����Ƃ͌���Ȃ�
}

//�W�������̎擾
SizeDbl CharRecog::GetDefLineWidth(int nRnum)
{
	//�W���ԍ��̎擾
	SizeDbl sizeDbl;
	LONG lKey = MAKELONG(DEFAULT_CHAR_NUM,nRnum); //�L�[���[�h�쐬
	m_mapAryLineWidth.Lookup(lKey,sizeDbl); 
	return sizeDbl;
}

//�����`�挋�ʕ\��
BOOL CharRecog::CreateStringResult(CString strName,int x,int y,CString strString,COLORREF color,LOGFONT &lf)
{	
	BOOL isOK = TRUE;
	if( strString.IsEmpty()){
		strString = _T("�������G���[");	
		color = COLOR_RED;
		isOK = FALSE;
	}		
	GetView()->m_mgrVO.CreateStringObject(strName,x,y,strString,color,lf);
	return isOK;
}

//�����̈�g�ړ�
void CharRecog::MoveRectObject(int nRnum,CRect rect)
{
	CKIBOView *pView = GetView();
	CString strName; //�`��I�u�W�F�N�g����
	strName.Format(L"%s%d",s_tcCheckRectName,nRnum);
	RectObject *pRO = (RectObject *)pView->m_mgrVO.GetVO(strName); //�����̈�\���I�u�W�F�N�g�擾
	if(pRO != NULL ){
		m_mapCheckRect[nRnum].m_rect = rect; //�����̈�ύX
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();
		rect.left   = (int)(rect.left   * dbRateCx);
		rect.top    = (int)(rect.top    * dbRateCy);
		rect.right  = (int)(rect.right  * dbRateCx);
		rect.bottom = (int)(rect.bottom * dbRateCy);
		pRO->SetRect(rect);
	}
}

//���������݂���̈�����W����
int CharRecog::CollectCharRect(int nRnum,int nTh,CArray<CRect> &aryRect,CDIB &dibSrc) 
{
	aryRect.RemoveAll();
	
	m_dibGray.Copy(dibSrc);//Gray Image�쐬	
	//BYTE byCmin = GetContrastMin();//�R���g���X�g�ŏ��l�擾
	//BYTE byCmax = GetContrastMax();//�R���g���X�g�ő�l�擾
	//m_dibGray.MinMax(byCmin,byCmax);//�R���g���X�g�ő剻

	//���x�����O
	CKIBOView *pView = GetView();
	Label &label = pView->GetLabel(); //���x�����O�N���X�擾
	label.Init(m_dibGray.GetWidth(),m_dibGray.GetHeight());//���x�����O������
	if( !label.CreateSrcBuf(m_dibGray,nTh) ){//���x�����O�o�b�t�@�i2�l���摜�쐬)
		::ErrorMsg(_T("�摜�̎擾���o���܂���ł����B�J�����Ƃ̐ڑ����m�F���Ă��������B"));
		return 0;
	}
	if( !IsWhite(nRnum) ){ //�������łȂ��ꍇ�̂�
		label.InvertSrcBuf();//�����ϊ�	
	}
	
	//���x�����O�v�f����
	CSize sizeChar = GetCharPixels(nRnum);
	CRect rcCheck = GetCheckRectOld(nRnum); //�����ݒ莞�̗̈�ɖ߂�
	if(nRnum > 0){ //�ȑO��RECT�Ƃ̌����`�F�b�N
		CRect rcPrev = GetCheckRect(nRnum-1); //�ȑO�̗̈�
		CRect rectInter;
		if( rectInter.IntersectRect(rcCheck,rcPrev) ){ //��������
			int nH = rcCheck.Height(); //�����擾
			rcCheck.top = rcPrev.bottom + 1;
			rcCheck.bottom = rcCheck.top + nH;
		}
	}
	if( label.Exec(sizeChar.cx,sizeChar.cy,rcCheck) == 0 ){ //���x�����O���s
		return 0;
	}
	//�����̈悸�꒲��	������ 
	for(int i=0; i<ADJUST_NUM; i++){//������
		Element *pElemMax = label.GetMaxElement(); //�ő�̗v�f�擾
		if( pElemMax != NULL ){
			int nTopMax = pElemMax->GetMinY(); //��
			int nBtmMax = pElemMax->GetMaxY(); //��
			if( abs(nTopMax-rcCheck.top) < ADJUST_SIZE ){ //�オ�����Ă���
				//��Ɉړ�
				int nDelta = -ADJUST_SIZE;
				rcCheck.top		+= nDelta;
				rcCheck.bottom	+= nDelta;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //���x�����O�Ď��s
				MoveRectObject(nRnum,rcCheck); //�����̈�g�ړ�
			}else if( abs(nBtmMax-rcCheck.bottom) < ADJUST_SIZE ){//���������Ă���
				//���Ɉړ�
				int nDelta = ADJUST_SIZE;
				rcCheck.top		+= nDelta;
				rcCheck.bottom	+= nDelta;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //���x�����O�Ď��s
				MoveRectObject(nRnum,rcCheck); //�����̈�g�ړ�
			}else{ //�ő�v�f���g�̒�
				//�Z���^�����O
				int nHeight = rcCheck.Height();
				int nDelta = (rcCheck.Height() - (nBtmMax-nTopMax))/2;
				rcCheck.top		= nTopMax - nDelta;
				rcCheck.bottom	= rcCheck.top + nHeight;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //���x�����O�Ď��s
				MoveRectObject(nRnum,rcCheck); //�����̈�g�ړ�			
				break;
			}
		}
	}
	label.Sort(Label::SORT_MIN_X,true); //x���W�̏����Ƀ\�[�g
	int nEmax = label.GetEmax();//���x�����O�v�f���擾

	//���x�����O�v�f�̈�`��
	for(int i=0;i<nEmax;i++){
		Element *pElem = label.GetElement(i);
		aryRect.Add(pElem->GetRect()); //�����̈�����W
	}
	//Error�����`�F�b�N
	//���x�����O�v�f�̍������ׂĔ���
	

	return nEmax;
}

//1�������̃��x�����O���s
Element* CharRecog::LabelCharElement(int nRnum,CPoint &ptTL,Label &label,int nTh,Dib256 &dibGray,ArrayPoint &aryPoint)
{
	//�_�u�����x�����O
	label.Init(dibGray.GetWidth(),dibGray.GetHeight());//���x�����O������
	if( !label.CreateSrcBuf(dibGray,nTh) ){//���x�����O�o�b�t�@�i2�l���摜�쐬)		
		return NULL;
	}
	if( !IsWhite(nRnum) ){ //�������łȂ��ꍇ�̂�
		label.InvertSrcBuf();//�����ϊ�
	}
	if( IsEraseDot(nRnum) ){ //1�h�b�g�������������ǂ���
		label.EraseDot(); //1�h�b�g�����폜			
	}
	int nEnum = 0;
	CSize sizeChar = GetCharPixels(nRnum);
	int nEmax = label.Exec(LABEL_MIN_CHAR,sizeChar.cy);
	label.Sort(Label::SORT_PIXELS,FALSE); //0�Ԗڂ��ő�v�f�ɂȂ�悤��
	if( nEmax == 1 ){
		label.FindBorder(nEnum,aryPoint); //
		return label.GetElement(nEnum); //�P�������̃��x�����O�v�f�擾
	}else if(!IsDirtyCheck(nRnum) && label.GetMaxElement(&nEnum) != NULL){ //���ꌟ���Ȃ� and �ő�v�f�����݂���
		label.FindBorder(nEnum,aryPoint); //
		return label.GetElement(nEnum); //�P�������̃��x�����O�v�f�擾
	}
	//����A��
	//CKIBOView *pView = GetView();
	//CString strName;
	//for(int i=0;i<nEmax;i++){
	//	Element *pElem = label.GetElement(i);
	//	strName.Format(L"ErrorElement%d",i);
	//	CRect rect = pElem->GetRect();
	//	rect += ptTL;
	//	pView->m_mgrVO.CreateRectObject(strName,rect,Color::Red,false); //��`�`��
	//}
	aryPoint.RemoveAll(); //Border�Ȃ�
	return NULL;
}

//�����������當����\�z
double CharRecog::GetPredictElement(CharElement &celem,WORD wRnum,WORD wCnum)
{
	TCHAR tchar = 0; //16bit����
	int nIndex = 0;//�ŒZ����������index
	double dbMin = DBL_MAX; //�ŏ�����
	for(int i=0;i<m_aryCharElement.GetSize();i++){ //�S�T���v���f�[�^����
		CharElement& elem = m_aryCharElement[i];
		if( elem.m_wRnum == wRnum || (IsCommon(elem.m_wRnum) && IsCommon(wRnum)) ){ //�����̈挟���`�F�b�N �̈悲�Ƃɕ�����ނ��قȂ�ꍇ
			if( iswalpha(elem.m_tChar) ){ //�A���t�@�x�b�g�ł��B
				if( IsNumber(wRnum,wCnum) ){ //�A���t�@�x�b�g�Ȃ̂ɐ��l�w�肪����
					continue;
				}
			}else{ //���l�ł��B
				if( IsAlpha(wRnum,wCnum) ){ //���l�Ȃ̂ɃA���t�@�x�b�g�w�肪����
					continue;
				}
			}
			double dbDelta = CalcDelta(elem,celem); //�����ʊԂ̋������擾
			if( dbDelta < dbMin ){
				dbMin = dbDelta; //�ŏ������̓���ւ�
				tchar = elem.m_tChar; //�\�z����
				nIndex = i;
			}
		}
	}
	celem.m_tChar = tchar;	//�\�z����

/////////////////////////////FOR DEBUG//////////////////////////////
#ifdef CALC_DELTA_CHECK
	CharElement& elem = m_aryCharElement[nIndex];
	CalcDelta(elem,celem,TRUE);
#endif
/////////////////////////////FOR DEBUG//////////////////////////////

	return dbMin; //����
}

double AreaMax(double d1,double d2,double d3,double d4)
{
	return max(max(d1,d2), max(d3,d4));
}
double AreaMin(double d1,double d2,double d3,double d4)
{
	return min(min(d1,d2), min(d3,d4));
}

//�ʐϓ����l���v�Z����
static double CalcAreaDelta(double dbA1,double dbA2)
{
	double dbDelta = dbA1 - dbA2;
	double dbVal = 4000*(dbDelta)/dbA2;	//�ʐ�All �w�K�f�[�^���̃T�C�Y�Ő��K��
	return min(dbDelta*4,dbVal); //�ʐϒl���� or �䗦
}

//�����ʊԂ̋������擾    �w�K�f�[�^,       �\�������̓���, 
double CharRecog::CalcDelta(CharElement &elem,CharElement &celem,BOOL isDebug)
{
	if( celem.m_nAreaA <= 0 ){
		return SHRT_MAX; //�ő�
	}
	double d[23]; //delta
	ZeroMemory(d,sizeof(d)); //0clear
	d[ 0] = CalcAreaDelta(celem.m_nAreaA,elem.m_nAreaA);//�ʐ�All �w�K�f�[�^���̃T�C�Y�Ő��K��
	d[ 1] = CalcAreaDelta(celem.m_dbAreaT,elem.m_dbAreaT);//�ʐϏ�
	d[ 2] = CalcAreaDelta(celem.m_dbAreaB,elem.m_dbAreaB);//�ʐω�
	d[ 3] = CalcAreaDelta(celem.m_dbAreaL,elem.m_dbAreaL);//�ʐύ�
	d[ 4] = CalcAreaDelta(celem.m_dbAreaR,elem.m_dbAreaR);//�ʐωE
	d[ 5] = 100*(celem.m_dbGx-elem.m_dbGx);	//�d�Sx
	d[ 6] = 100*(celem.m_dbGy-elem.m_dbGy);	//�d�Sy	
	d[ 7] = ((double)celem.m_wContourNum-elem.m_wContourNum)*200;//���E���̐�
	d[ 8] = 40*100.0*(celem.m_dbContourOut - elem.m_dbContourOut)/elem.m_nAreaA;//���E���̍��v����
	d[ 9] = 40*100.0*(celem.m_dbContourIn  - elem.m_dbContourIn )/elem.m_nAreaA;//�����̋��E��
	d[10] = (celem.m_dbCircle - elem.m_dbCircle)*5000;//�~�`�x
	d[11] = (celem.m_byEdgeLR - elem.m_byEdgeLR)*190; //�G�b�W�E����
	d[12] = (celem.m_byEdgeRL - elem.m_byEdgeRL)*190; //�G�b�W������
	d[13] = (celem.m_byEdgeTB - elem.m_byEdgeTB)*180; //�G�b�W������
	d[14] = (celem.m_byEdgeBT - elem.m_byEdgeBT)*180; //�G�b�W�����
	CRect rcL = elem.m_rect; //�w�K�f�[�^�̈�
	CRect rcR = celem.m_rect;//�\�������̈�
	d[15] = 25*( rcL.Width() -rcR.Width()  ); //�T�C�Y��
	d[16] = 25*( rcL.Height()-rcR.Height() ); //�T�C�Y�c
	d[17] = 5*( (rcL.CenterPoint().x-elem.m_dbGx) - (rcR.CenterPoint().x-celem.m_dbGx) );
	d[18] = 5*( (rcL.CenterPoint().y-elem.m_dbGy) - (rcR.CenterPoint().y-celem.m_dbGy) );

	d[19] = (celem.GetLengthLeft()  -elem.GetLengthLeft()  )*1000 / celem.m_nAreaA;
	d[20] = (celem.GetLengthTop()   -elem.GetLengthTop()   )*1000 / celem.m_nAreaA;
	d[21] = (celem.GetLengthRight() -elem.GetLengthRight() )*1000 / celem.m_nAreaA;
	d[22] = (celem.GetLengthBottom()-elem.GetLengthBottom())*1000 / celem.m_nAreaA;

	//�����v�Z
	double dbLength = 0;
	for(int j=0,jmax=sizeof(d)/sizeof(double);j<jmax;j++){
		d[j] = mid(-300,d[j],300); //����ݒ�
		dbLength += d[j] * d[j]; 
	}

#ifdef CALC_DELTA_CHECK
	if( isDebug ){
		TRACE("\n%c\n",elem.m_tChar);
		TRACE("%.2f\n",sqrt(dbLength));
		for(int n=0;n<sizeof(d)/sizeof(double);n++){
			TRACE("d[%d]:%.2f, ",n,d[n]);
		}		
		TRACE("Area:%d-%d\n",celem.m_nAreaA,elem.m_nAreaA	);	//�ʐ�All 
		TRACE("AreaT:%.2f-%.2f\n",celem.m_dbAreaT,elem.m_dbAreaT);//�ʐϏ�			
		TRACE("AreaB:%.2f-%.2f\n",celem.m_dbAreaB,elem.m_dbAreaB);//�ʐω�
		TRACE("AreaL:%.2f-%.2f\n",celem.m_dbAreaL,elem.m_dbAreaL);//�ʐύ�
		TRACE("AreaR:%.2f-%.2f\n",celem.m_dbAreaR,elem.m_dbAreaR);//�ʐωE
		TRACE("Gx:%.2f-%.2f\n",celem.m_dbGx,elem.m_dbGx);	//�d�Sx
		TRACE("Gy:%.2f-%.2f\n",celem.m_dbGy,elem.m_dbGy);	//�d�Sy
		TRACE("ContourNum:%u-%u\n",celem.m_wContourNum,elem.m_wContourNum);//���E���̐�
		TRACE("ContourOut:%.2f-%.2f\n",celem.m_dbContourOut,elem.m_dbContourOut);//���E���̍��v����
		TRACE("ContourIn :%.2f-%.2f\n",celem.m_dbContourIn ,elem.m_dbContourIn);//���E���̍���
		TRACE("Circle:%.2f-%.2f\n",celem.m_dbCircle,elem.m_dbCircle);	//�~�`�x		
		TRACE("EdgeLR %u - %u\n",celem.m_byEdgeLR,elem.m_byEdgeLR);
		TRACE("EdgeRL %u - %u\n",celem.m_byEdgeRL,elem.m_byEdgeRL);
		TRACE("EdgeTB %u - %u\n",celem.m_byEdgeTB,elem.m_byEdgeTB);
		TRACE("EdgeBT %u - %u\n",celem.m_byEdgeBT,elem.m_byEdgeBT);
		TRACE("Width %u - %u\n",celem.m_rect.right-celem.m_rect.left,elem.m_rect.right-elem.m_rect.left);
		TRACE("Heght %u - %u\n",celem.m_rect.bottom-celem.m_rect.top,elem.m_rect.bottom-elem.m_rect.top);
		TRACE("\n");
	}
#endif
	return sqrt(dbLength); //����
}

//���E���̐����擾
static int GetContourNum(ArrayPoint &aryPoint)
{
	ArrayPoint aryRet; //�ꎞ�I�Ȕz��
	int nStart = 0;//�ǉ��J�n���W
	int nCnt = 0; //���E���̐�
	int nLen = 0; //���E���̒���
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x < 0 ){
			if( nLen >= MIN_CONTOUR_LEN){ //����������Ă��鋫�E������
				int nEnd = i;
				for(int n=nStart;n<=nEnd;n++){ //OK�̎������ǉ�
					aryRet.Add( aryPoint[n] );
				}
				nCnt++;	
			}					
			nLen = 0; //�����N���A
			nStart = i+1;
		}else{
			nLen++;
		}
	}
	aryPoint.RemoveAll();
	aryPoint.Copy( aryRet ); //���W�|�C���g�ύX
	return nCnt;
}

//�ő勫�E���擾
static double GetContourMax(ArrayPoint &aryPoint,double &dbAll)
{
	dbAll = 0; //clear
	double dbMax = 0; //�ő勫�E��
	double dbCnt = 0; //�������̋��E��
	CPoint ptPrev(-1,-1);
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( ptPrev.x >= 0 ){ //�����v�Z�\�Ȏ�����
			if( pt.x < 0 ){ 
				if( dbMax < dbCnt){//���傫�����E������
					dbMax = dbCnt;
				}
				dbAll += dbCnt;//���v�l
				dbCnt = 0;			
			}else{
				int nCheck = abs(pt.x-ptPrev.x)+abs(pt.y-ptPrev.y);
				if( nCheck == 1 ){
					dbCnt += 1;
				}else{
					dbCnt += 1.414213562; //sqrt(2)
				}
			}
		}
		ptPrev = pt; //���̓_��
	}
	return dbMax;
}
//���E����`�̈�擾
static double GetContourRect(ArrayPoint &aryPoint)
{
	CRect rcMin(INT_MAX,INT_MAX,INT_MIN,INT_MIN); //�ŏ���`
	CArray<double> aryArea; //�ʐϋL�^
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x < 0 ){ //�I�[
			rcMin.NormalizeRect(); //���K��
			aryArea.Add( rcMin.Width()*rcMin.Height() ); //�ʐϓo�^
			rcMin.left = INT_MAX;
			rcMin.top  = INT_MAX;
			rcMin.right= INT_MIN;
			rcMin.bottom=INT_MIN;
			continue ;
		}
		//��菬�������̂ɍ����ւ�
		if( rcMin.left > pt.x ){
			rcMin.left = pt.x;
		}
		if( rcMin.top > pt.y ){
			rcMin.top = pt.y;
		}
		//���傫�����̂ɍ����ւ�
		if( rcMin.right < pt.x ){
			rcMin.right = pt.x;
		}
		if( rcMin.bottom < pt.y ){
			rcMin.bottom = pt.y;
		}
	}
	double dbSum = 0; //�S�ʐύ��v
	double dbMax = 0; //�ő�ʐϒl
	double dbMin = DBL_MAX;
	for(int i=0,imax=aryArea.GetSize();i<imax;i++){
		double dbArea = aryArea[i];
		if( dbMin > dbArea){
			dbMin = dbArea;
		}
		dbSum += dbArea;
	}
	//return (dbMax - (dbSum-dbMax)); //�ő�ʐς��炻��ȊO�̖ʐς�����
	return dbMin;
}

//���x�����O�Ƌ��E�����當���̓������擾
CharElement CharRecog::GetCharElement(CRect &rect,Label &label,Element *pElem,TCHAR tchar,WORD wRnum,ArrayPoint &aryPoint)
{
	CharElement element;
	ZeroMemory(&element,sizeof(CharElement));
	if( pElem != NULL ){
		element.m_tChar   = tchar; //��������
		element.m_wRnum   = wRnum; //�̈�ԍ�
		element.m_nAreaA  = pElem->GetPixels();	//�ʐ�All 
		element.m_dbAreaT = (REAL)pElem->GetPixelsTop();	//�ʐϏ�
		element.m_dbAreaB = (REAL)pElem->GetPixelsBottom();	//�ʐω�
		element.m_dbAreaL = (REAL)pElem->GetPixelsLeft();	//�ʐύ�
		element.m_dbAreaR = (REAL)pElem->GetPixelsRight();	//�ʐωE
		element.m_dbGx = (REAL)pElem->GetGravityX()-pElem->GetMinX();//�d�S�̑��Έʒux
		element.m_dbGy = (REAL)pElem->GetGravityY()-pElem->GetMinY();//�d�S�̑��Έʒuy
		int nCnt = GetContourNum(aryPoint);
		element.m_wContourNum = (WORD)nCnt;	//���E���̐�
		double dbLenAll;//���E���̍��v����
		double dbLenMax = GetContourMax(aryPoint,dbLenAll); //�ő勫�E���̎擾
		element.m_dbContourOut = (REAL)dbLenMax;//�ő勫�E�����O�����E���Ƃ���
		element.m_dbContourIn  = (REAL)abs(dbLenAll-element.m_dbContourOut); //�ő勫�E���Ƃ��̑��̋��E���̍���
		element.m_dbCircle = (REAL)(4*PI*element.m_nAreaA /(dbLenMax * dbLenMax));//�~�`�x
		AryLine aryLineV; //���������������C���z��
		CollectLineV(aryLineV,label);
		element.m_byEdgeTB = (BYTE)GetEdgeNumTopToBtm(wRnum,NULL,aryLineV,label,rect.TopLeft());	 //�ォ�牺�����̃G�b�W��
		element.m_byEdgeBT = (BYTE)GetEdgeNumBtmToTop(wRnum,NULL,aryLineV,label,rect.TopLeft());	 //�����������̃G�b�W��
		AryLine aryLineH; //���������������C���z��
		CollectLineH(aryLineH,label);		
		element.m_byEdgeLR = (BYTE)GetEdgeNumLeftToRight(wRnum,                 0,NULL,aryLineH,label,rect.TopLeft()); //������E�����̃G�b�W��
		element.m_byEdgeRL = (BYTE)GetEdgeNumRightToLeft(wRnum,label.GetWidth()-1,NULL,aryLineH,label,rect.TopLeft());  //�E���獶�����̃G�b�W��
		
		element.m_rect.left  = pElem->GetMinX();
		element.m_rect.top   = pElem->GetMinY();
		element.m_rect.right = pElem->GetMaxX();
		element.m_rect.bottom= pElem->GetMaxY();

		//��`���_����̋��������߂�
		CRect rcElem = element.m_rect; //�v�f�̈�
		CPoint ptLT(rcElem.left ,rcElem.top);//����
		CPoint ptRT(rcElem.right,rcElem.top);//�E��
		CPoint ptLB(rcElem.left ,rcElem.bottom);//����
		CPoint ptRB(rcElem.right,rcElem.bottom);//�E��		
		element.SetLeft	 (aryPoint,ptLT,ptLB);//�������_����̋���
		element.SetTop	 (aryPoint,ptLT,ptRT);//������_����̋���
		element.SetRight (aryPoint,ptRT,ptRB);//�E�����_����̋���
		element.SetBottom(aryPoint,ptLB,ptRB);//�������_����̋���
	}
	return element; //�����ʎ擾
}

//�摜�t�@�C������f�[�^�̎擾
//�����̈�̃��x�����O -> �������Ƃ̃��x�����O -> ���E���T�� -> �f�[�^�쐬
BOOL CharRecog::ReadSample(int nThAll,const CString &strFile,const CString &strFileBin,CString &strNumber)
{
	Dib256 dib256;
	if( dib256.LoadFile(strFile) ){ //�t�@�C������ǂݍ���
		::ErrorMsg(_T("�摜�t�@�C��%s�̑��݂��m�F���Ă��������B"),strFile);
		return FALSE;
	}
	CDIB dib;
	dib256.Paste(dib);//�摜�ǂݍ���


	FilePlus file;
	if( !file.Open(strFileBin,FilePlus::modeBinOut) ){ //
		::ErrorMsg(_T("�f�[�^�t�@�C��%s���쐬�ł��܂���B"),strFileBin);
		return FALSE;
	}
	CArchive ar(&file, CArchive::store); //���������f�[�^�Z�[�u

	CKIBOView *pView = GetView();
	int nWmax = dib.GetWidth();
	int nHmax = dib.GetHeight();

	CStringArray sAry; //������z��
	SplitToken(' ',sAry,strNumber); //���p�󔒋�؂�
	Label &label = GetView()->GetLabel(); //���x�����O�N���X�擾
	for(int nRnum=0;nRnum<sAry.GetSize();nRnum++){ //�����̈�̐��Ń��[�v
		CRect rcCheck = GetCheckRect(nRnum); //�����̈�擾
		if( rcCheck.IsRectEmpty() ){
			continue ;
		}
		CString strNum = sAry[nRnum]; //�T���v�����𕶎���
		CArray<CRect> aryRect; //�̈�
		int nEmax = CollectCharRect(nRnum,nThAll,aryRect,dib); //���������݂���̈�����W����
		if( nEmax != strNum.GetLength()){
			::ErrorMsg(_T("%s \n �T���v���f�[�^�[(%s)�������G���[ \n�̈�:%d %d->%d"),dib.GetFilePath(),strNum,nRnum,strNum.GetLength(),nEmax );
			//::ErrorMsg(_T("�w�K�摜�t�@�C�� \n %s \n �͎g�p�o���܂���B"),dib.GetFilePath());
#ifdef DEBUG
			//���x�����O�Ɏg�p�����摜��\������
			CDIB dibErr;//�G���[�摜
			dibErr.Create(label.GetWidth(),label.GetHeight());
			label.FillBitmapSrc(dibErr);
			::DrawRectangle(dibErr,rcCheck,COLOR_RED);//�����̈�`��
			::DrawRectangles(dibErr,aryRect,COLOR_GREEN); //�w�肳�ꂽ��`���r�b�g�}�b�v�ɕ`��
			dibErr.WriteBitmap(L"c:\\tmp.bmp");

			ImageDlg dlg; //�C���[�W�\���_�C�A���O
			dlg.SetBitmap(dibErr);
			dlg.DoModal();
#endif
			::DeleteFile(strFileBin); //�o�C�i���t�@�C���̍폜
			return FALSE;
		}
		rcCheck = GetCheckRect(nRnum); //�����̈�Ď擾
		int nTh = CalcThreshold(nThAll,dib,rcCheck);//�������l�擾
		m_aryPoint.RemoveAll();//���E���_�񏉊���
		for(int i=0;i<nEmax;i++){ //������`�v�f�̐��������[�v
			CRect rect = aryRect[i];
			rect.left  = max(0,rect.left-CHAR_SPACE_X);
			rect.top   = max(0,rect.top -CHAR_SPACE_Y);
			rect.right = min(nWmax-1,rect.right +CHAR_SPACE_X);
			rect.bottom= min(nHmax-1,rect.bottom+CHAR_SPACE_Y);

			m_dibGray.Cut(dib,rect);//1�������̃r�b�g�}�b�v�쐬
			Element *pElem = LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint); //1�������̃��x�����O���s
			if(pElem != NULL && m_aryPoint.GetSize() > 0 ){
				CharElement celem = GetCharElement(rect,label,pElem,strNum[i],nRnum,m_aryPoint); //�����v�f�\���̂̎擾
				m_aryCharElement.Add(celem); //���������ǉ�
				//�v�f�f�[�^�̕ۑ�
				Serialize(ar,celem); //����������������
			}
			///////////////////////FOR DEBUG/////////////////////////////////////
			else{
				m_dibGray.ChangeBinary(nTh);
				m_dibGray.SaveFile(L"c:\\cut.bmp");
				return FALSE;
			}
			///////////////////////FOR DEBUG/////////////////////////////////////
		}
	}
	return TRUE;
}

//�����F���R�}���h
//�߂�l�@�F��������
CString CharRecog::RecognizeChar(int nCnum,int nThAll)
{
	m_mapPrevElement.clear(); //�ŐV�f�[�^clear
	CString strRet = L""; //�߂�l
	CString strRed = L""; //�ԕ���
	COutputView *pOutput = GetOutputView(); //���ʕ\��
	pOutput->Clear();
	CKIBOView *pView = GetView();
	CameraEx *pCameraEx = pView->GetCameraPtr(nCnum);

	//�����摜�擾
	int nAnum = GetAverageNum();
	if( nAnum ==1){ //�B�����ω�
		pCameraEx->Capture(); //0�ԃJ��������摜�擾
	}else{
		pCameraEx->CaptureAverage(nAnum,GetWaitTime());
	}

	int nWmax = pCameraEx->GetWidth();
	int nHmax = pCameraEx->GetHeight();

	CDIB &dibSrc = pCameraEx->GetDib();	
	Label &label = GetView()->GetLabel(); //���x�����O�N���X
	CArray<CRect> aryRect; //���W����镶���̈�	
	int nRectSize = m_mapCheckRect.GetSize();//�����̈��
	CString str1;
	for(int nRnum=0;nRnum<nRectSize;nRnum++){
		CRect rcCheck = GetCheckRect(nRnum); //�����̈�擾 
		if( rcCheck.IsRectEmpty() ){
			continue ;
		}
		double dbLenTh = GetLengthTh(nRnum);//�����ǔ۔��苗���擾
		int nEmax = CollectCharRect(nRnum,nThAll,aryRect,dibSrc); //���������݂���̈�����W����
		int nCharNum = GetCharNum(nRnum); //���𕶎����擾
		CString strFix = GetCharString(nRnum); //�Œ蕶���擾
		int nFixNum = strFix.GetLength();//�Œ蕶����
		if( nCharNum < 0 || nCharNum == nEmax || nFixNum == nEmax){ //���������ϓ�����ꍇ || �������`�F�b�NOK			
			 rcCheck = GetCheckRect(nRnum); //�����̈�擾 
			int nTh = CalcThreshold(nThAll,dibSrc,rcCheck); //�������l�ύX
			//���E���T��
			m_aryPoint.RemoveAll(); //���E���_�񏉊���
			for(int i=0;i<nEmax;i++){ //�v�f�̐��������[�v
				CRect rect = aryRect[i];
				rect.left  = max(0,rect.left-CHAR_SPACE_X);
				rect.top   = max(0,rect.top -CHAR_SPACE_Y);
				rect.right = min(nWmax-1,rect.right +CHAR_SPACE_X);
				rect.bottom= min(nHmax-1,rect.bottom+CHAR_SPACE_Y);
				m_dibGray.Cut(dibSrc,rect);//1�������̃r�b�g�}�b�v�쐬

				Element *pElem = LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint);//�P�������̃��x�����O�v�f�擾
				CharElement celem = GetCharElement(rect,label,pElem,0,nRnum,m_aryPoint); //�����v�f�̎擾
#ifdef LINECHECK
				CreateBorderObject(rect.TopLeft(),m_aryPoint);//���E���\���I�u�W�F�N�g�쐬
#endif
				//�����`�F�b�N
				double dbLen = GetPredictElement(celem,nRnum,i); //���x�����O�Ƌ��E�����當����𔻒f
				if( nCharNum == 0 ){ //�Œ蕶���̎�
					if( i < nFixNum ){
						celem.m_tChar = strFix[i]; //�Œ蕶���擾
					}
				}
				//�����܂ł̔F�����ʂ��L�^
				SetPrevElement(celem.m_wRnum,i,celem);
				if( dbLenTh < dbLen ){
					CString strMes;
					strMes.Format(L"?: �����F���G���[ %.1f",dbLen);
					pOutput->Output(strMes,COLOR_RED);//���b�Z�[�W�\��
				}
				CheckNickData data;
				//�����ɂ��G���[�`�F�b�N
				if( dbLenTh < dbLen || //�G���[�����֕ύX
					!CheckElement(celem,nRnum,i) ){ //�����l�`�F�b�N�����s������
					celem.m_tChar = s_tcError;
				}else{
					//������������
					data = CheckNick(i,label,celem,rect,m_aryPoint,nRnum,dbLen,nTh);
					celem.m_tChar = data.m_tChar;
					if( data.m_isOK && IsExpandCharTop(celem.m_tChar) ){//i��j�̃`�F�b�N
						rect.top = rcCheck.top;
						rect.bottom = rcCheck.bottom;
						m_dibGray.Cut(dibSrc,rect);//1�������̃r�b�g�}�b�v�쐬
						LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint);//�P�������̃��x�����O�v�f�擾
						data.m_isOK = (label.GetEmax()==2);//�������݂��鎞����OK
						if(!data.m_isOK){ //�G���[
							pOutput->RemoveLastMessage();
							CString strMes;
							pOutput->Output(false,L"%c: �@�ُ�l���o�F�\���v�f�s��",celem.m_tChar);
						}
					}
				}
				//���ʂ̏o��
				strRet += celem.m_tChar;
				if( data.m_isOK && celem.m_tChar != s_tcError){
					strRed += ' ';
				}else{
					strRed += celem.m_tChar; //�ԕ����ŕ\��
					CreateErrorRect(nRnum,i,rect); //�̈�S�̂̃G���[��`�`��
				}
			}
		}else if( nCharNum != nEmax ){ //���������قȂ�
			str1.Format(_T("�̈���������G���[ ��:%d -> %d �ʐ� "),nCharNum,nEmax);			
			for(int i=0;i<aryRect.GetSize();i++){
				CreateErrorRect(nRnum,i,aryRect[i]); //�̈�S�̂̃G���[��`�`��
			}
			CString strTmp;
			for(int i=0;i<nEmax;i++){
				Element *pElem = label.GetElement(i);
				if( pElem != NULL ){
					strTmp.Format(L"%d ",pElem->GetPixels()); //��f���\��
					str1 += strTmp;
				}
			}
			pOutput->Output(str1,COLOR_RED); //�o��			
		}
		str1 = "";
		pOutput->Output(str1,COLOR_WHITE); //�o��
		strRet += ' ';
		strRed += ',';
	}
	CString strConnect;
	strConnect.Format(L"%s,%s",strRet,strRed);
	return strConnect;
}

//�G���[���C���`��
void CharRecog::CreateErrorRect(int nRnum,int nEnum,CRect &rect) 
{
	CString strName;
	strName.Format(_T("ErrorRect%d-%d"),nRnum,nEnum);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,Color::Red,false); //��`�`���
	TRACE(_T("Error RECT:%d %d %d %d\n"),rect.left,rect.top,rect.right,rect.bottom);
}

//���E���\���I�u�W�F�N�g�쐬
void CharRecog::CreateBorderObject(CPoint &ptTL,ArrayPoint &aryPoint) 
{
	int nCnt = 0;
	ArrayPoint ptAry;
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x >= 0 ){
			ptAry.Add(pt+ptTL);
		}else{//�I�[����
			CString strContourName;
			strContourName.Format(L"Contour%d_%d%d",nCnt,ptTL.x,ptTL.y);
			GetView()->m_mgrVO.CreatePolylineObject(strContourName,ptAry,nCnt==0 ? COLOR_GREEN : nCnt==1 ? COLOR_YELLOW : COLOR_RED); //��`�`��
			ptAry.RemoveAll();
			nCnt++;
		}
	}
}


//�w��ԍ��̉摜���擾
BOOL CharRecog::GetCharImage(CDIB &dibDst,int nRnum,int nCnum)
{
	CKIBOView *pView = GetView();
	CharElement& celem = GetPrevElement(nRnum,nCnum); //�ȑO�̃f�[�^�擾
	CDIB &dibSrc = pView->GetDib(); //�\���r�b�g�}�b�v�擾
	CRect rc = celem.m_rect;//�摜�`��̈�
	int nW = rc.Width();
	int nH = rc.Height();
	if( dibDst.Create(nW,nH) ){		
		CDC *pDC = ::AfxGetMainWnd()->GetDC();
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pOldBmpSrc = dcSrc.SelectObject( dibSrc.GetBitmap() );

		CDC dcDst;
		dcDst.CreateCompatibleDC(pDC);
		CBitmap *pOldBmpDst = dcDst.SelectObject( dibDst.GetBitmap() );
		dcDst.BitBlt(0,0,nW,nH,&dcSrc,rc.left,rc.top,SRCCOPY);

		//��n��
		dcSrc.SelectObject( pOldBmpSrc);
		dcDst.SelectObject( pOldBmpDst);
		pDC->DeleteDC();
		return TRUE;
	}
	return FALSE;
}

//��ɂ����ꕶ������܂����H
BOOL CharRecog::IsExpandCharTop(TCHAR tc)
{
	switch(tc){
		case 'i':
		case 'j':
			return TRUE;
	}
	return FALSE;
}

//���������������C�����W
int CharRecog::CollectLineH(AryLine &aryLineH,Label &label)
{
	aryLineH.RemoveAll(); //clear

	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	CLine line;
	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx; //�����J�n�A�h���X
		for(int x=0;x<nCx;x++,pFind++){
			if( IsInPointH(pFind,x,nCx,nCy) ){ //In Point����������
				line.m_ptS.x = x; //�J�n�_�o�^
				line.m_ptS.y = y;
				for(;x<nCx;x++,pFind++){
					if( IsOutPointH(pFind,x,nCx,nCy) ){ //Out Point����������
						line.m_ptE.x = x-1;
						line.m_ptE.y = y;
						aryLineH.Add(line); //���o�^
						break;
					}
				}
				if( x==nCx ){ //�[�܂œ��B�����ꍇ					
					line.m_ptE.x = x-1;
					line.m_ptE.y = y;
					aryLineH.Add(line); //�[�܂ł̐���o�^
				}
			}
		}
	}
	return aryLineH.GetSize();
}

//�������J�n�|�C���g
BOOL CharRecog::IsInPointH(BYTE *pFind,int x,int nCx,int nCy)
{
	int imax = max(2,nCx / 30 );
	BOOL isOK = TRUE;
	for(int i=0;i<imax && x<nCx;i++,x++,pFind++){ //�c�����R��f����
		isOK &= (*pFind==MAXBYTE);
	}
	return isOK;
}
//�������I���|�C���g
BOOL CharRecog::IsOutPointH(BYTE *pFind,int x,int nCx,int nCy)
{
	//int imax = max(2,nCx / 30 );
	//BOOL isOK = TRUE;
	//for(int i=0;i<imax && x<nCx;i++,x++,pFind++){ //�c�����R��f����
	//	isOK &= !(*pFind);
	//}
	return !(*pFind);
}
//�c�����J�n�|�C���g
BOOL CharRecog::IsInPointV(BYTE *pFind,int y,int nCx,int nCy)
{
	int imax = max(2,nCy / 30 );
	BOOL isOK = TRUE;
	for(int i=0;i<imax && y<nCy;i++,y++,pFind+=nCx){ //�c�����R��f����
		isOK &= (*pFind==MAXBYTE);
	}
	return isOK;
}
//�c�����I���|�C���g
BOOL CharRecog::IsOutPointV(BYTE *pFind,int y,int nCx,int nCy)
{
	//int imax = max(2,nCy / 30 );
	//BOOL isOK = TRUE;
	//for(int i=0;i<imax && y<nCy;i++,y++,pFind+=nCx){ //�c�����R��f����
	//	isOK &= !(*pFind);
	//}
	return !(*pFind);
}
//���������������C�����W
int CharRecog::CollectLineV(AryLine &aryLineV,Label &label)
{
	aryLineV.RemoveAll(); //clear

	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾

	CLine line;
	int nYs = 0;
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + x; //�����J�n�A�h���X
		for(int y=nYs;y<nCy;y++,pFind+=nCx){			
			if( IsInPointV(pFind,y,nCx,nCy) ){ //In Point����������
				line.m_ptS.x = x; //�J�n�_�o�^
				line.m_ptS.y = y;				
				for(;y<nCy;y++,pFind+=nCx){
					if( IsOutPointV(pFind,y,nCx,nCy) ){ //Out Point����������
						line.m_ptE.x = x;
						line.m_ptE.y = y-1;
						aryLineV.Add(line); //���o�^
						break;
					}					
				}
				if( y==nCx ){ //�[�܂œ��B�����ꍇ					
					line.m_ptE.x = x;
					line.m_ptE.y = y-1;
					aryLineV.Add(line); //�[�܂ł̐���o�^
				}
			}
		}
	}
	return aryLineV.GetSize();
}

//�v�f�`�F�b�N
BOOL CharRecog::CheckElement(CharElement &celem,int nRnum,int nCnum)
{
	//�e�v�f�̍ŏ��l
	double	dbAreaAMin	= INT_MAX;	//�ʐ�All
	WORD	wContourMin = MAXWORD;	//���E����
	double	dbWidthMin	= INT_MAX;	//�������ŏ�
	double	dbHeightMin	= INT_MAX;	//���������ŏ�
	double  dbLenLMin[CharElement::POINT_NUM]; //= INT_MAX;	//���[����̋���
	double  dbLenTMin[CharElement::POINT_NUM]; //= INT_MAX;	//��[����̋���
	double  dbLenRMin[CharElement::POINT_NUM]; //= INT_MAX;	//���[����̋���
	double  dbLenBMin[CharElement::POINT_NUM]; //= INT_MAX;	//���[����̋���
	double  dbLenDxMin = INT_MAX;  //���E�����̍���
	double  dbLenDyMin = INT_MAX;  //�㉺�����̍���

	//�e�v�f�̍ő�l
	double	dbAreaAMax	= 0;	//�ʐ�All
	WORD	wContourMax = 0;	//���E����
	double	dbWidthMax	= 0;	//�������ŏ�
	double	dbHeightMax	= 0;	//���������ŏ�
	double  dbLenLMax[CharElement::POINT_NUM]; //= 0;	//���[����̋���
	double  dbLenTMax[CharElement::POINT_NUM]; //= 0;	//��[����̋���
	double  dbLenRMax[CharElement::POINT_NUM]; //= 0;	//���[����̋���
	double  dbLenBMax[CharElement::POINT_NUM]; //= 0;	//���[����̋���
	double  dbLenDxMax = 0;  //���E�����̍���
	double  dbLenDyMax = 0;   //�㉺�����̍���

	//�z��ݒ�
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMin[i] = INT_MAX;	//���[����̋���
		dbLenTMin[i] = INT_MAX;	//��[����̋���
		dbLenRMin[i] = INT_MAX;	//���[����̋���
		dbLenBMin[i] = INT_MAX;	//���[����̋���
		dbLenLMax[i] = 0;	//���[����̋���
		dbLenTMax[i] = 0;	//��[����̋���
		dbLenRMax[i] = 0;	//���[����̋���
		dbLenBMax[i] = 0;	//���[����̋���
	}
	BOOL isSyUD = IsSymmetryUpDown(celem.m_tChar);	//�㉺�Ώ̕����ł����H
	//BOOL isSyLR = IsSymmetryLR(celem.m_tChar);		//���E�Ώ̕����ł����H
	BOOL isInc  = IsInclination(nRnum); //�΂ߕ����ł����H
	for(int i=0;i<m_aryCharElement.GetSize();i++){ //�S�T���v���f�[�^����
		CharElement& elem = m_aryCharElement[i]; //�v�f�擾
		if( elem.m_wRnum == nRnum || (IsCommon(elem.m_wRnum) && IsCommon(nRnum)) ){ //�����̈挟���`�F�b�N �̈悲�Ƃɕ�����ނ��قȂ�ꍇ
			if( elem.m_tChar == celem.m_tChar ){ //���ꕶ��
				//�����̈�
				double dbWidth  = (elem.m_rect.right -elem.m_rect.left+1);
				double dbHeight = (elem.m_rect.bottom-elem.m_rect.top +1);
				//��菬�������ɒu��������
				if(elem.m_nAreaA < dbAreaAMin){ //�ʐ�All�ŏ�
					dbAreaAMin = elem.m_nAreaA; 
				}
				if(elem.m_nAreaA > dbAreaAMax){ //�ʐ�All�ő�
					dbAreaAMax = elem.m_nAreaA; 
				}
				if(elem.m_wContourNum < wContourMin){ //���E���̍��v�����ŏ�
					wContourMin = elem.m_wContourNum;
				}
				if(elem.m_wContourNum > wContourMax){//���E���̍��v�����ő�
					wContourMax= elem.m_wContourNum; 
				}
				if(dbWidth < dbWidthMin){//�������ŏ�
					dbWidthMin = dbWidth; 
				}
				if(dbWidth > dbWidthMax){//�������ő�
					dbWidthMax = dbWidth; 
				}
				if(dbHeight < dbHeightMin){//���������ŏ�
					dbHeightMin = dbHeight;
				}
				if(dbHeight > dbHeightMax){//���������ő�
					dbHeightMax = dbHeight;
				}

				for(int j=0;j<CharElement::POINT_NUM;j++){
					if(elem.m_rLeft[j] < dbLenLMin[j]){ //���[����̋���
						dbLenLMin[j] = elem.m_rLeft[j];
					}
					if(elem.m_rLeft[j] > dbLenLMax[j]){ //���[����̋���
						dbLenLMax[j] = elem.m_rLeft[j];
					}
					if(elem.m_rTop[j] < dbLenTMin[j]){ //��[����̋���
						dbLenTMin[j] = elem.m_rTop[j];
					}
					if(elem.m_rTop[j] > dbLenTMax[j]){ //��[����̋���
						dbLenTMax[j] = elem.m_rTop[j];
					}
					if(elem.m_rRight[j] < dbLenRMin[j]){ //�E�[����̋���
						dbLenRMin[j] = elem.m_rRight[j];
					}
					if(elem.m_rRight[j] > dbLenRMax[j]){ //�E�[����̋���
						dbLenRMax[j] = elem.m_rRight[j];
					}
					if(elem.m_rBottom[j] < dbLenBMin[j]){ //���[����̋���
						dbLenBMin[j] = elem.m_rBottom[j];
					}
					if(elem.m_rBottom[j] > dbLenBMax[j]){ //���[����̋���
						dbLenBMax[j] = elem.m_rBottom[j];
					}
				}
				if( !isInc ){ //�΂ߕ����ȊO�̂Ƃ�����
					if( isSyUD ){//�㉺�Ώ̕����̎�
						double dbTop   = elem.GetLengthTop();	//������_����̋����擾
						double dbBottom= elem.GetLengthBottom();//�������_����̋����擾
						double dbDy = abs(dbTop-dbBottom);
						if(dbDy < dbLenDyMin){ //�㉺�����ŏ�
							dbLenDyMin = dbDy;
						}
						if(dbDy > dbLenDyMax){ //�㉺�����ő�
							dbLenDyMax= dbDy;
						}
					}
					//if( isSyLR ){ //���E�Ώ̕����̎�
					//	double dbLeft  = elem.GetLengthLeft();	//�������_����̋����擾
					//	double dbRight = elem.GetLengthRight();	//�E�����_����̋����擾
					//	double dbDx = abs(dbLeft-dbRight);
					//	if(dbDx < dbLenDxMin){ //�㉺�����ŏ�
					//		dbLenDxMin = dbDx;
					//	}
					//	if(dbDx > dbLenDxMax){ //�㉺�����ő�
					//		dbLenDxMax= dbDx;
					//	}
					//}					
				}
			}
		}
	}
	//�e�v�f�̍ő�l
	dbAreaAMax		+= DEFAULT_CHECK_SPACE;
	dbWidthMax		+= DEFAULT_CHECK_SPACE;
	dbHeightMax		+= DEFAULT_CHECK_SPACE;
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMax[i]+= DEFAULT_CHECK_SPACE; //���[����̋���
		dbLenTMax[i]+= DEFAULT_CHECK_SPACE; //��[����̋���
		dbLenRMax[i]+= DEFAULT_CHECK_SPACE; //�E�[����̋���
		dbLenBMax[i]+= DEFAULT_CHECK_SPACE; //���[����̋���
	}
	dbLenDyMax		+= (DEFAULT_CHECK_SPACE+1);

	//�e�v�f�̍ŏ��l
	dbAreaAMin		-= DEFAULT_CHECK_SPACE;
	dbWidthMin		-= DEFAULT_CHECK_SPACE;
	dbHeightMin		-= DEFAULT_CHECK_SPACE;
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMin[i]-= DEFAULT_CHECK_SPACE; //���[����̋���
		dbLenTMin[i]-= DEFAULT_CHECK_SPACE; //��[����̋���
		dbLenRMin[i]-= DEFAULT_CHECK_SPACE; //�E�[����̋���
		dbLenBMin[i]-= DEFAULT_CHECK_SPACE; //���[����̋���
	}
	dbLenDyMin -= (DEFAULT_CHECK_SPACE+1);

	CString strErrType;
	BOOL isOK = TRUE;
	COutputView *pOutput = GetOutputView(); //���ʕ\��
	double dbWidth  = (celem.m_rect.right -celem.m_rect.left+1);
	double dbHeight = (celem.m_rect.bottom-celem.m_rect.top +1);
	//���͈͓��ɂ��邩�ǂ���
	if(celem.m_wContourNum < wContourMin){
		strErrType = L"���E�����s��";
		isOK = FALSE;
	}else if(celem.m_wContourNum > wContourMax){
		strErrType = L"���E������";
		isOK = FALSE;
	}else if( dbWidth < dbWidthMin ){
		strErrType = L"�������s��";
		isOK = FALSE;
	}else if(dbWidth > dbWidthMax){
		strErrType = L"��������";
		isOK = FALSE;
	}else if( dbHeight < dbHeightMin ){
		strErrType = L"���������s��";
		isOK = FALSE;
	}else if(dbHeight > dbHeightMax){
		strErrType = L"����������";
		isOK = FALSE;
	}else if(celem.m_nAreaA < dbAreaAMin){ //�ʐύŏ��l�`�F�b�N
		strErrType = L"�ʐϕs��";
		isOK = FALSE;
	}else if( !celem.IsLeftOK(dbLenLMin,dbLenLMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsTopOK(dbLenTMin,dbLenTMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsRightOK(dbLenRMin,dbLenRMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsBottomOK(dbLenBMin,dbLenBMax,strErrType) ){
		isOK = FALSE;
	}else if( !isInc ){ //�΂ߕ����ȊO�̎�����
		if( isSyUD ){//�㉺�Ώ̕����̎�
			double dbTAll = celem.GetLengthTop();
			double dbBAll = celem.GetLengthBottom();
			double dbDy = abs(dbTAll-dbBAll);			
			if( dbDy < dbLenDyMin || dbLenDyMax < dbDy ){ //�㉺�̋����̍��ق��w�K�f�[�^�͈͓̔����ǂ���
				strErrType = L"�㉺�Ώ̃G���[";
				isOK = FALSE;
			}
		}
		//if( isSyLR ){//���E�Ώ̕����̎�����
		//	double dbLAll = celem.GetLengthLeft();
		//	double dbRAll = celem.GetLengthRight();
		//	double dbDx = abs(dbRAll-dbLAll);
		//	if( dbDx < dbLenDxMin || dbLenDxMax < dbDx ){ //�㉺�̋����̍��ق��w�K�f�[�^�͈͓̔����ǂ���
		//		strErrType = L"���E�Ώ̃G���[";
		//		isOK = FALSE;
		//	}
		//}
	}
	if(!isOK){
		pOutput->Output(false,L"%c: �ُ�l���o�F%s",celem.m_tChar,strErrType);
		TRACE(L"\n�����l�ɂ��G���[���o\n");
		TRACE(L"%c %d\n",celem.m_tChar,nRnum);
		TRACE(L"�ʐρF%d %.1f-%.1f\n",celem.m_nAreaA,dbAreaAMin,dbAreaAMax);
		TRACE(L"���E�����F%u %u-%u\n",celem.m_wContourNum,wContourMin,wContourMax); //���E���̍��v����
		TRACE(L"�����̈�F%.1f %.1f-%.1f\n",dbWidth	,dbWidthMin	,dbWidthMax	 ); //�����̈�
		TRACE(L"�����̈�F%.1f %.1f-%.1f\n",dbHeight,dbHeightMin,dbHeightMax ); //�����̈�
		TRACE(L"��:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rLeft[i]);
		}
		TRACE(L"\n��:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rTop[i]);
		}
		TRACE(L"\n�E:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rRight[i]);
		}
		TRACE(L"\n��:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rBottom[i]);
		}
	}
	return isOK;
}

//
//CharRecog.ini
//[400001105.bmp]
//TIME=YYYYMMDDhhmmss
//�̈�ԍ�-�����ԍ�=1,1,1000,100,100.0,198...
//ELEMENT2=1,1,1000,100,100.0,198...
//ELEMENT3=1,1,1000,100,100.0,198...

//�Z�N�V��������v�f�f�[�^�̏�������
BOOL CharRecog::ReadProfileElement(const CString &strFile,int nRnum,int nCnum,CharElement &elem)
{
	CString strKey;
	strKey.Format(L"%02d-%02d",nRnum,nCnum);//�̈�ԍ�-�����ԍ�		
	CString strElement = ReadIniString(strFile,strKey);
	CStringArray sAry; //������z��
	SplitToken(',',sAry,strElement); //�J���}��؂�	
	if(sAry.GetSize() >= (21+CharElement::POINT_NUM*4) ){
		elem.m_tChar   = sAry[0].GetAt(0);	//��������
		elem.m_wRnum   = _wtoi(sAry[1]);	//�̈�ԍ�
		elem.m_nAreaA  = _wtoi(sAry[2]);	//�ʐ�All		
		elem.m_dbAreaT = (REAL)_wtof(sAry[3]);	//�ʐϏ�
		elem.m_dbAreaB = (REAL)_wtof(sAry[4]);	//�ʐω�
		elem.m_dbAreaL = (REAL)_wtof(sAry[5]);	//�ʐύ�
		elem.m_dbAreaR = (REAL)_wtof(sAry[6]);	//�ʐωE
		elem.m_dbGx	   = (REAL)_wtof(sAry[7]);	//�d�Sx
		elem.m_dbGy	   = (REAL)_wtof(sAry[8]);	//�d�Sy
		elem.m_wContourNum  = _wtoi(sAry[9] );//���E���̐�
		elem.m_dbContourOut = (REAL)_wtof(sAry[10]);//���E���̍��v����
		elem.m_dbContourIn  = (REAL)_wtof(sAry[11]);//�O�����E��-�������E��
		elem.m_dbCircle		= (REAL)_wtof(sAry[12]);//�~�`�x
		elem.m_byEdgeLR		= _wtoi(sAry[13]);//������E�����̃G�b�W��
		elem.m_byEdgeRL		= _wtoi(sAry[14]);	//�E���獶�����̃G�b�W��
		elem.m_byEdgeTB		= _wtoi(sAry[15]);	//�ォ�牺�����̃G�b�W��
		elem.m_byEdgeBT		= _wtoi(sAry[16]);	//�����������̃G�b�W��
		elem.m_rect.left	= _wtoi(sAry[17]);	//�����̈捶
		elem.m_rect.top		= _wtoi(sAry[18]);	//�����̈��
		elem.m_rect.right	= _wtoi(sAry[19]);	//�����̈�E
		elem.m_rect.bottom	= _wtoi(sAry[20]);	//�����̈扺
		int nIndex = 21;
		for(int i=0;i<CharElement::POINT_NUM;i++){
			elem.m_rLeft[i]	 = (REAL)_wtof(sAry[nIndex+i]);//�������_����̋���
			elem.m_rTop[i]   = (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM]);//������_����̋���
			elem.m_rRight[i] = (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM*2]);//�E�����_����̋���
			elem.m_rBottom[i]= (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM*3]);//�������_����̋���
		}
		return TRUE;
	}
	return FALSE;
}
//�Z�N�V�����ɗv�f�f�[�^�̏�������
void CharRecog::WriteProfileElement(const CString &strFile,CharElement &elem,int nCnum)
{
	CString strKey;
	strKey.Format(L"%02d-%02d",elem.m_wRnum,nCnum);//�̈�ԍ�-�����ԍ�

	CString strVal;
	strVal.Format(L"%c,%u,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%u,%.2f,%.2f,%.2f,%u,%u,%u,%u,%d,%d,%d,%d,"
					,elem.m_tChar		//��������
					,elem.m_wRnum		//�̈�ԍ�
					,elem.m_nAreaA		//�ʐ�All		
					,elem.m_dbAreaT		//�ʐϏ�
					,elem.m_dbAreaB		//�ʐω�
					,elem.m_dbAreaL		//�ʐύ�
					,elem.m_dbAreaR		//�ʐωE
					,elem.m_dbGx		//�d�Sx
					,elem.m_dbGy		//�d�Sy
					,elem.m_wContourNum	//���E���̐�
					,elem.m_dbContourOut//�O�����E���̒���
					,elem.m_dbContourIn //�������E���̒���
					,elem.m_dbCircle	//�~�`�x
					,elem.m_byEdgeLR	//������E�����̃G�b�W��
					,elem.m_byEdgeRL	//�E���獶�����̃G�b�W��
					,elem.m_byEdgeTB	//�ォ�牺�����̃G�b�W��
					,elem.m_byEdgeBT	//�����������̃G�b�W��
					,elem.m_rect.left	//�����̈捶
					,elem.m_rect.top	//�����̈��		
					,elem.m_rect.right	//�����̈�E	
					,elem.m_rect.bottom	//�����̈扺	
					);
	strVal += elem.GetLeftCsv();
	strVal += ',';
	strVal += elem.GetTopCsv();
	strVal += ',';
	strVal += elem.GetRightCsv();
	strVal += ',';
	strVal += elem.GetBottomCsv();
	WriteIniString(strFile,strKey,strVal);
}

//.ini�t�@�C���ɂɕ�����̏�������
void CharRecog::WriteIniString(const CString &strSection,const CString &strKey,LPCWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);

	CString strPath;
	strPath.Format(L"%s\\CharRecog.ini",GetApp()->GetAppPath());	
	::WritePrivateProfileString(strSection,strKey,strFormat,strPath);
}

//.ini�t�@�C�����當����̓ǂݍ���
CString CharRecog::ReadIniString(const CString &strSection,const CString &strKey)
{
	CString strPath;
	strPath.Format(L"%s\\CharRecog.ini",GetApp()->GetAppPath());	

	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));
	::GetPrivateProfileString(strSection,strKey,g_strEmpty,szBuf,sizeof(szBuf),strPath);
	CString strRet = szBuf; //�߂�l�쐬
	return strRet;
}
//.bin�t�@�C�����當����f�[�^�̓ǂݍ���
//�t�@�C���� -> �w�K�f�[�^\SN123456789_134826.bin
//�̈�ԍ�,�������ԍ�,����

//�㉺�Ώ̕����̎�
BOOL CharRecog::IsSymmetryUpDown(TCHAR tc)
{
	switch(tc){
		case '0':
		case '3':
		case '8':
		case 'N':
		case 'C':
		case 'D':
		case 'E':
		case 'H':
		case 'O':
		case 'S':
		case 'X':
		case 'Z':
		case 'l':
		case 'o':
		case 'x':
		case 'z':
			return TRUE;
			break;
	}
	return FALSE;
}

//���E�Ώ̕����̎�
BOOL CharRecog::IsSymmetryLR(TCHAR tc)
{
	switch(tc){
		case '0':
		case '8':
		case 'H':
		case 'O':
		case 'X':
		case 'l':
		case 'o':
		case 'x':
			return TRUE;
			break;
	}
	return FALSE;
}


//���������ǂݍ��݁@& m_aryCharElement�֒ǉ�
BOOL CharRecog::ReadElement(CString &strFileBin,CStringArray &sAry)
{
	FilePlus file;
	if( file.Open(strFileBin,FilePlus::modeBinIn) ){
		CharElement elem; //��������
		CArchive ar(&file, CArchive::load);
		int nRmax = sAry.GetSize();
		for(int i=0;i<nRmax;i++){//�����̈�̐��������[�v
			if( !GetCheckRect(i).IsRectEmpty() ){
				CString strText = sAry[i]; //�󎚕�����擾
				int nLen = strText.GetLength();
				for(int n=0;n<nLen;n++){ //������̐��������[�v
					if( Serialize(ar,elem) ){
						m_aryCharElement.Add(elem);
					}
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}
//�V���A���C�Y
BOOL CharRecog::Serialize(CArchive& ar,CharElement &elem)
{
	if (ar.IsStoring()){ //Save
		ar << elem.m_tChar		;	//��������
		ar << elem.m_wRnum		;	//�̈�ԍ�
		ar << elem.m_nAreaA		;	//�ʐ�All
		ar << elem.m_dbAreaT	;	//�ʐϏ�
		ar << elem.m_dbAreaB	;	//�ʐω�
		ar << elem.m_dbAreaL	;	//�ʐύ�
		ar << elem.m_dbAreaR	;	//�ʐωE
		ar << elem.m_dbGx		;	//�d�Sx
		ar << elem.m_dbGy		;	//�d�Sy
		ar << elem.m_wContourNum;	//���E���̐�
		ar << elem.m_dbContourOut;	//�O�����E���̒���
		ar << elem.m_dbContourIn;	//�������E���̒���
		ar << elem.m_dbCircle	;	//�~�`�x
		ar << elem.m_byEdgeLR	;	//������E�����̃G�b�W��
		ar << elem.m_byEdgeRL	;	//�E���獶�����̃G�b�W��
		ar << elem.m_byEdgeTB	;	//�ォ�牺�����̃G�b�W��
		ar << elem.m_byEdgeBT	;	//�����������̃G�b�W��
		ar << elem.m_rect.left	;	//�����̈�
		ar << elem.m_rect.top	;	//�����̈�
		ar << elem.m_rect.right	;	//�����̈�
		ar << elem.m_rect.bottom;	//�����̈�
		for(int i=0;i<CharElement::POINT_NUM;i++){
			ar << elem.m_rLeft[i];	//�������_����̋���
			ar << elem.m_rTop[i];	//������_����̋���
			ar << elem.m_rRight [i];//�E�����_����̋���
			ar << elem.m_rBottom[i];//�������_����̋���
		}
		return TRUE;
	}else{ //Load
		ar >> elem.m_tChar		;	//��������
		ar >> elem.m_wRnum		;	//�̈�ԍ�
		ar >> elem.m_nAreaA		;	//�ʐ�All
		ar >> elem.m_dbAreaT	;	//�ʐϏ�
		ar >> elem.m_dbAreaB	;	//�ʐω�
		ar >> elem.m_dbAreaL	;	//�ʐύ�
		ar >> elem.m_dbAreaR	;	//�ʐωE
		ar >> elem.m_dbGx		;	//�d�Sx
		ar >> elem.m_dbGy		;	//�d�Sy
		ar >> elem.m_wContourNum;	//���E���̐�
		ar >> elem.m_dbContourOut;	//�O�����E���̒���
		ar >> elem.m_dbContourIn;	//�������E���̒���
		ar >> elem.m_dbCircle	;	//�~�`�x
		ar >> elem.m_byEdgeLR	;	//������E�����̃G�b�W��
		ar >> elem.m_byEdgeRL	;	//�E���獶�����̃G�b�W��
		ar >> elem.m_byEdgeTB	;	//�ォ�牺�����̃G�b�W��
		ar >> elem.m_byEdgeBT	;	//�����������̃G�b�W��
		ar >> elem.m_rect.left	;	//�����̈�
		ar >> elem.m_rect.top	;	//�����̈�
		ar >> elem.m_rect.right	;	//�����̈�
		ar >> elem.m_rect.bottom;	//�����̈�
		for(int i=0;i<CharElement::POINT_NUM;i++){
			ar >> elem.m_rLeft[i];	//�������_����̋���
			ar >> elem.m_rTop[i];	//������_����̋���
			ar >> elem.m_rRight [i];//�E�����_����̋���
			ar >> elem.m_rBottom[i];//�������_����̋���
		}
		return TRUE;
	}	
	return FALSE;
}

//�̈悲�Ƃ�2�l��
void CharRecog::BinaryCheckRect(int nCnum,int nRnum,int nTh)
{
	COutputView *pOutput = GetOutputView(); //���ʕ\��

	CameraEx *pCameraEx = GetView()->GetCameraPtr(nCnum);
	CDIB &dibSrc = pCameraEx->GetDib();	
	if( GetCheckRect(nRnum).IsRectEmpty() ){//�w�茟���̈�ݒ�`�F�b�N
		return ;
	}
	CArray<CRect> aryRect; //���W����镶���̈�	
	int nEmax = CollectCharRect(nRnum,nTh,aryRect,dibSrc); //���������݂���̈�����W����
	CRect rect = GetCheckRect(nRnum); //�����̈�擾
	nTh = CalcThreshold(nTh,dibSrc,rect);
	CDIB dibDst;
	m_dibGray.Paste(dibDst);
	dibDst.ChangeBinary(nTh); //2�l���摜�쐬
	dibSrc.SetDib(rect.left,rect.top,dibDst); //2�l���摜�\��t��
}

//�̈���̂������l�v�Z
int CharRecog::CalcThreshold(int nTh,CDIB &dibSrc,CRect &rcCheck)
{
	//�̈悲�Ƃ�2�l��
	if( m_dibGray.Cut(dibSrc,rcCheck) ){//�w��̈�r�b�g�}�b�v�쐬					
		int nBlack = 0; //���̉�f��
		int nHist[256];
		m_dibGray.CollectHistgram(nHist);//�q�X�g�O�������W
		//nTh�ȏ�̍ő�v�f�擾		
		int nMax = 0;
		int nImax = 0;
		for(int i=nTh;i<=MAXBYTE;i++){
			if( nMax < nHist[i] ){//���傫���ق���
				nMax = nHist[i];
				nImax = i;
			}
		}
		//nTh��艺�̍ő�v�f
		nMax = 0;
		int nImin = 0;
		for(int i=nTh-1;i>=0;i--){
			if( nMax < nHist[i] ){ //���傫������
				nMax = nHist[i];
				nImin = i;
			}
		}
		////nImin����nImax�Ԃ̍ŏ��l�����߂�
		//int nMin = INT_MAX;
		//int nTh1 = nTh;
		//for(int i=nImin;i<=nImax;i++){
		//	if( nMin > nHist[i] ){ //��菬��������
		//		nMin = nHist[i];
		//		nTh1 = i;
		//	}	
		//}
		////nImax����nImin�Ԃ̍ŏ��l�����߂�
		//nMin = INT_MAX;
		//int nTh2 = nTh;
		//for(int i=nImax;i>=nImin;i--){
		//	if( nMin > nHist[i] ){ //��菬��������
		//		nMin = nHist[i];
		//		nTh2 = i;
		//	}	
		//}
		nTh = int( (nImin + nImax) / 2.0 + 0.5 ); //�����炵���������l
	}
	//////FOR DEBUG///////////////////
	TRACE("TH:%d,left:%d,top:%d,right:%d,bottom:%d\n",nTh,rcCheck.left,rcCheck.top,rcCheck.right,rcCheck.bottom);
	//////FOR DEBUG///////////////////
	return nTh;
}

#endif


