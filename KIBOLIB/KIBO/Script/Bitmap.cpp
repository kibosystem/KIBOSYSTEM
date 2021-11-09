#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOView.h"
#include "../LIB/macro.h"
#include "App.h"
#include "Bitmap.h"
#include "ImageDevice.h"
#include "Labeling.h"

MapPDib::~MapPDib()
{
	Clear();
}
//�S�r�b�g�}�b�v�폜
void MapPDib::Clear()
{
	for (POSITION pos = GetStartPosition(); pos != NULL;){
		int nKey;
		CDIB *pPtr = NULL; //�r�b�g�}�b�v�|�C���^
		GetNextAssoc(pos, nKey, pPtr);
		if( pPtr != NULL ){
			delete pPtr;
		}
	}
	RemoveAll();
}

//�r�b�g�}�b�v�N���X�擾
CDIB* MapPDib::GetDibPtr(int nKey) 
{
	CDIB *pPtr = NULL;
	if( Lookup(nKey,pPtr) ){ //���łɓo�^�ς�
		if( pPtr != NULL ){
			return pPtr;
		}
	}
	pPtr = new CDIB;//�V�K�쐬
	try{
		//bitmap�쐬
		SetAt(nKey,pPtr);
	}catch( CException* e ){
		::ErrorMsg(L"Memory Error");
		e->Delete();
	}
	return pPtr;
}

static CommandTable s_table[] =
{
	{Bitmap24::FUNC_LOAD		, 1,L"Load"}		//�ǂݍ���
	,{Bitmap24::FUNC_SAVE		,-1,L"Save"}		//��������
	,{Bitmap24::FUNC_SHOW		,0,L"Show"}		//�\��
	,{Bitmap24::FUNC_SETTHRESHOLD,1,L"SetThreshold"}//�������l�ݒ�
	,{Bitmap24::FUNC_GETTHRESHOLD,-1,L"GetThreshold" }	//2�l���������l�擾
	,{ Bitmap24::FUNC_SETLINECOLOR,3,L"SetLineColor"}	//���C���F�ݒ�
	,{Bitmap24::FUNC_FROMIMAGEDEVICE,0,L"FromImageDevice"}	//�J�����摜�擾
	,{Bitmap24::FUNC_IMAGEDEVICEDIFF,1,L"ImageDeviceDiff"}	//�J�����摜��r
	,{Bitmap24::FUNC_TOIMAGEDEVICE,0,L"ToImageDevice"}	//�C���[�W�f�o�C�X�։摜���M
	,{Bitmap24::FUNC_COLORLENGTH, 3,L"ColorLength"}	//�F�����ϊ�
	,{Bitmap24::FUNC_CHANGECOLOR, 1,L"ChangeColor"}	//�F�ϊ�	
	,{Bitmap24::FUNC_GRADIENT	,-1,L"Gradient"}	//����
	,{Bitmap24::FUNC_SOBEL		,-1,L"Sobel"}		//�֊s���o �c�A��
	,{Bitmap24::FUNC_LAPLACIAN	,-1,L"Laplacian"}	//���v���V�A��
	,{Bitmap24::FUNC_PREWITT	,-1,L"Prewitt"}		//�֊s���o
	,{Bitmap24::FUNC_GAMMA		, 1,L"Gamma"}		//�K���}�␳
	,{Bitmap24::FUNC_INVERT		,0,L"Invert"}		//�e�F���]
	,{Bitmap24::FUNC_CHANGERANGE,-1, L"ChangeRange" }		//�R���g���X�g�ő剻
	,{Bitmap24::FUNC_EXPAND		,1,L"Expand"}		//�c��
	,{Bitmap24::FUNC_SHRINK		,1,L"Shrink"}		//���k
	,{Bitmap24::FUNC_COLLECTBITMAP, 1, L"CollectBitmap" }//�r�b�g�}�b�v��A�z�z��ɘA���o�^
	,{Bitmap24::FUNC_SETBITMAP	,1,L"SetBitmap"}		//�r�b�g�}�b�v��A�z�z��ɕۑ�
	,{Bitmap24::FUNC_GETBITMAP	,1,L"GetBitmap"}		//�A�z�z�񂩂�r�b�g�}�b�v�擾
	,{Bitmap24::FUNC_GETBITMAPCOUNT,0,L"GetBitmapCount" }//�A�z�z��J�E���g�擾
	,{Bitmap24::FUNC_DELTADIB	,2,L"DeltaDib"}		//�r�b�g�}�b�v����
	,{Bitmap24::FUNC_DIVDIB		,2,L"DivDib"}		//�r�b�g�}�b�v����Z
	,{Bitmap24::FUNC_AVERAGE	,0,L"Average"}		//���ω摜�̍쐬
	,{Bitmap24::FUNC_DIBSETCLEAR,0,L"DibSetClear"}	//�r�b�g�}�b�v�A�z�z��N���A
	,{Bitmap24::FUNC_HISTGRAM	,0,L"Histgram"}		//�q�X�g�O�����̕\��
	,{Bitmap24::FUNC_GETBINARYLEVEL,1,L"GetBinaryLevel"}//�w��ʐϊ����ɂȂ�Q�l�����x���̎擾
	,{Bitmap24::FUNC_GETBRIGHTNESS,5,L"GetBrightness"}	//���邳�ω��E�ʒu�擾
	,{Bitmap24::FUNC_MEDIAN		,0,L"Median"}		//���f�B�A���t�B���^
	,{Bitmap24::FUNC_MAPDELTA	,1,L"MapDelta"}		//�A�z�z���r�摜�쐬
	,{Bitmap24::FUNC_EDIT		,2,L"Edit"}			//�r�b�g�}�b�v�ҏW
	,{Bitmap24::FUNC_CALCTHRESHOOLD,0,L"CalcThreshold"} //�������l�����v�Z
	,{Bitmap24::FUNC_SELECTCOLOR,4,L"SelectColor"}//�F���o
	,{Bitmap24::FUNC_SELECT,2,L"Select"}//����Ɖ����I��
	,{Bitmap24::FUNC_FINDEDGE,6,L"FindEdge"} //�G�b�W���W���o
	,{Bitmap24::FUNC_DRAWLINE,8,L"DrawLine"} //���C���`��
	,{Bitmap24::FUNC_DRAWBITMAPLINE,-1,L"DrawBitmapLine"} //�r�b�g�}�b�v���C���`��
	,{Bitmap24::FUNC_MASKBORDER,1,L"MaskBorder"} //���x�����O�v�f�}�X�N
	,{Bitmap24::FUNC_CALCWIDTH,4,L"CalcWidth" }		//�����v�Z
	,{Bitmap24::FUNC_CALCHEIGHT,4,L"CalcHeight" }	//�������v�Z
	,{ Bitmap24::FUNC_CALCWIDTHDIRECT,3,L"CalcWidthDirect" }		//�ŏ������v�Z
	,{ Bitmap24::FUNC_CALCHEIGHTDIRECT,3,L"CalcHeightDirect" }	//�ŏ��������v�Z	
	,{Bitmap24::FUNC_RMAX,-1,L"Rmax"}	//R�v�f�ő�l
	,{Bitmap24::FUNC_RMIN,-1,L"Rmin"}	//R�v�f�ŏ��l
	,{Bitmap24::FUNC_GMAX,-1,L"Gmax"}	//G�v�f�ő�l
	,{Bitmap24::FUNC_GMIN,-1,L"Gmin"}	//G�v�f�ŏ��l
	,{Bitmap24::FUNC_BMAX,-1,L"Bmax"}	//B�v�f�ő�l
	,{Bitmap24::FUNC_BMIN,-1,L"Bmin"}	//B�v�f�ŏ��l
	,{Bitmap24::FUNC_DEFECTTEST,3,L"DefectTest"}//���׌���
	,{Bitmap24::FUNC_LENSCHECK,4,L"LensCheck"}//�����Y�`�F�b�N
	,{Bitmap24::FUNC_BINARYMATCHING, 5, L"BinaryMatching" }//2�l���}�b�`���O
	,{Bitmap24::FUNC_OUTPUTFILEPATH, -1, L"OutputFilePath" }//FilePath�o��
	,{Bitmap24::FUNC_OUTPUTFILENAME, -1, L"OutputFileName" }//FileName�o��
	,{Bitmap24::FUNC_MAX  ,0,NULL}	//�R�}���h��
};

static Bitmap24 *s_pInstance = NULL; //�B��̃N���X����
Bitmap24* Bitmap24::GetInstance()//���̎擾
{
	return s_pInstance;
}

Bitmap24::Bitmap24(void)
{
	s_pInstance = this;
	m_byTh = 128;	//�������l�ݒ�
}

Bitmap24::~Bitmap24(void)
{
}

//�֐����`�F�b�N
STDMETHODIMP Bitmap24::GetIDsOfNames(REFIID riid
						,LPOLESTR __RPC_FAR *rgszNames
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
STDMETHODIMP Bitmap24::Invoke(DISPID dispIdMember	//Function ID �ԍ�
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
		case FUNC_LOAD: //Load			
			{
				//����������擾
				CString strPath = pDispParams->rgvarg[0].bstrVal;// �r�b�g�}�b�v�t�@�C���p�X
				if( strPath[0] == L'.' ){ //�J�����g�f�B���N�g���w��
					strPath = GetApp()->GetCurrentPath() + strPath.Mid(1);
				}
				BOOL isOK = m_dib.Load(strPath);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_SAVE:	//��������
			{
				::SetCurrentDirectory( GetApp()->GetAppPath() ); //�J�����g�f�B���N�g���ݒ�
				CString strPath = pDispParams->rgvarg[0].bstrVal;// �r�b�g�}�b�v�t�@�C���p�X
				CTime time = CTime::GetCurrentTime(); //���ݎ����擾
				if( strPath.Find(_T("TODAY")) >= 0 ){
					CString strDate = time.Format(_T("%Y%m%d"));
					strPath.Replace(_T("TODAY"),strDate);
				}
				if( strPath.Find(_T("TIME")) >= 0 ){
					CString strTime = time.Format(_T("%H%M%S"));
					strPath.Replace(_T("TIME"),strTime);
				}
				//strPath.Replace(L"/",L"");
				//strPath.Replace(L"?",L"");
				CString strExt = strPath.Right(3);//�g���q�擾
				m_dib.WriteImage(strPath,strExt);//�摜�t�@�C���ۑ�
			}
			break;
		case FUNC_SHOW: //�\��
			{
				ShowBitmap(); //�r�b�g�}�b�v�\�����z�֐�
			}
			break;
		case FUNC_SETTHRESHOLD: //�������l�ݒ�
			{
				BYTE byTh = pDispParams->rgvarg[0].bVal;
				SetTh( byTh );
			}
			break;
		case FUNC_SETLINECOLOR:	//���C���F�ݒ�
		{
			BYTE byR = GetValientInt(&pDispParams->rgvarg[2]);//R
			BYTE byG = GetValientInt(&pDispParams->rgvarg[1]);//G
			BYTE byB = GetValientInt(&pDispParams->rgvarg[0]);//B
			SetLineColor(RGB(byR, byG, byB)); //
		}
			break;
		case FUNC_IMAGEDEVICEDIFF://�J�����摜��r
			{
				BOOL isDiff = ImageDeviceDiff(); //��r
				ReturnBool(wFlags,pVarResult,isDiff); //  != : true
			}
			break;
		case FUNC_FROMIMAGEDEVICE://�J�����摜�擾
			{
				FromImageDevice();
			}
			break;
		case FUNC_TOIMAGEDEVICE://�C���[�W�f�o�C�X�։摜���M
			{
				ToImageDevice();
			}
			break;
		case FUNC_COLORLENGTH: //�F�����ϊ�
			{
				int nR = pDispParams->rgvarg[2].ulVal; //R
				int nG = pDispParams->rgvarg[1].ulVal; //G
				int nB = pDispParams->rgvarg[0].ulVal; //B
				//�w��F�Ƌ߂��F���v�Z
				m_dib.ColorLength(nR,nG,nB); //�w��F�Ƃ̋����摜�ɕϊ�
			}
			break;
		case FUNC_CHANGECOLOR:	//�F�ϊ�
			{
				CString strMode = pDispParams->rgvarg[0].bstrVal; //�F���[�h
				int nType = CDIB::TYPE_COLOR; //Default Type
				if(strMode== _T("RGRAY")){
					nType = CDIB::TYPE_RGRAY;
				}else if(strMode== _T("GGRAY")){
					nType = CDIB::TYPE_GGRAY;
				}else if(strMode== _T("BGRAY")){
					nType = CDIB::TYPE_BGRAY;
				}else if(strMode== _T("R")){
					nType = CDIB::TYPE_R;
				}else if(strMode== _T("G")){
					nType = CDIB::TYPE_G;
				}else if(strMode== _T("B")){
					nType = CDIB::TYPE_B;
				}else if(strMode== _T("GRAY")){
					nType = CDIB::TYPE_GRAY;
				}else if(strMode== _T("Y")){
					nType = CDIB::TYPE_Y;
				}else if(strMode== _T("H")){
					nType = CDIB::TYPE_H;
				}else if(strMode== _T("S")){
					nType = CDIB::TYPE_S;
				}else if(strMode== _T("2")){//2�l��
					m_dib.ChangeBinary( GetTh() );
				}else{ //clear
					m_dib.PaintBlack();
					return S_OK;
				}
				m_dib.ChangeColor(nType); //�F�ϊ�
			}
			break;
		case FUNC_GRADIENT://����
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//���������݂���ꍇ
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Gradient(dbAmp); //����
			}
			break;
		case FUNC_SOBEL:	//�֊s���o �c�A��
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//���������݂���ꍇ
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Sobel(dbAmp); //����
			}
			break;
		case FUNC_LAPLACIAN://���v���V�A��
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//���������݂���ꍇ
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Laplacian(dbAmp);
			}
			break;
		case FUNC_GAMMA: //�K���}�␳
			{
				double dbExp = GetValientDbl(&pDispParams->rgvarg[0]);
				Gamma(dbExp);
			}
			break;
		case FUNC_PREWITT:	//�֊s���o
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//���������݂���ꍇ
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Prewitt(dbAmp); //�֊s���o
			}
			break;
		case FUNC_INVERT:	//�e�F���]
			m_dib.Invert(); //�e�F���]
			break;
		case FUNC_CHANGERANGE:	//�R���g���X�g�ő剻
			{
				if( pDispParams->cArgs == 0 ){
					BYTE byMin,byMax;
					if( m_dib.GetRange(byMin,byMax) ){ //�ő�l�ƍŏ��l�̎擾
						m_dib.ChangeRange(byMin, byMax);//Range���ŏ��l->0 ... �ő�l->255�֍L����
					}
				}else if(pDispParams->cArgs== 2){ //�ŏ��l�ƍő�l�̎w�肠��
					BYTE byMin = (BYTE)pDispParams->rgvarg[1].ulVal; //�ŏ��l�擾
					BYTE byMax = (BYTE)pDispParams->rgvarg[0].ulVal; //�ő�l�擾
					m_dib.ChangeRange(byMin, byMax);//Range���ŏ��l->0 ... �ő�l->255�֍L����
				}
			}
			break;
		case FUNC_EXPAND://�c��
			{
				int nNum = pDispParams->rgvarg[0].ulVal; //�c����
				Expand(nNum);
			}
			break;
		case FUNC_SHRINK://���k
			{
				int nNum = pDispParams->rgvarg[0].ulVal; //�c����
				Shrink(nNum);
			}
			break;
		case FUNC_COLLECTBITMAP://�r�b�g�}�b�v��A�z�z��ɘA���o�^
		{
			CString strPath = pDispParams->rgvarg[0].bstrVal; //Path
			CollectBitmap(strPath); //�w��t�H���_���r�b�g�}�b�v���W

		}
			break;
		case FUNC_SETBITMAP: //�r�b�g�}�b�v��A�z�z��ɕۑ�
			{
				int nNo = pDispParams->rgvarg[0].ulVal; //�r�b�g�}�b�v�ԍ�
				SetDib(nNo);
			}
			break;
		case FUNC_GETBITMAP://�A�z�z�񂩂�r�b�g�}�b�v�擾
			{
				int nNo = pDispParams->rgvarg[0].ulVal; //�r�b�g�}�b�v�ԍ�
				CDIB *pDib = m_mapDibPtr.GetDibPtr(nNo); //�r�b�g�}�b�v�N���X�擾
				if(pDib != NULL){
					pDib->Copy(m_dib); //pDib - > m_dib
				}								
			}
			break;
		case FUNC_GETBITMAPCOUNT:
			{
				int nCnt = GetMapCount();
				ReturnInt(wFlags, pVarResult, nCnt);
			}
			break;
		case FUNC_DELTADIB:	//�r�b�g�}�b�v����
			{
				int nNo1 = pDispParams->rgvarg[1].ulVal; //�r�b�g�}�b�v�ԍ�
				int nNo2 = pDispParams->rgvarg[0].ulVal; //�r�b�g�}�b�v�ԍ�
				CDIB *pDib1 = m_mapDibPtr.GetDibPtr(nNo1); //�r�b�g�}�b�v�N���X�擾
				CDIB *pDib2 = m_mapDibPtr.GetDibPtr(nNo2); //�r�b�g�}�b�v�N���X�擾
				DeltaDib(pDib1,pDib2); //�r�b�g�}�b�v�̈����Z
			}
			break;
		case FUNC_DIVDIB://�r�b�g�}�b�v����Z
			{
				int nNo1 = pDispParams->rgvarg[1].ulVal; //�r�b�g�}�b�v�ԍ�
				int nNo2 = pDispParams->rgvarg[0].ulVal; //�r�b�g�}�b�v�ԍ�
				CDIB *pDib1 = m_mapDibPtr.GetDibPtr(nNo1); //�r�b�g�}�b�v�N���X�擾
				CDIB *pDib2 = m_mapDibPtr.GetDibPtr(nNo2); //�r�b�g�}�b�v�N���X�擾
				DivDib(pDib1,pDib2); //�r�b�g�}�b�v�̈����Z		
			}
			break;
		case FUNC_AVERAGE: //���ω摜�̍쐬
			AverageFilter();
			break;
		case FUNC_DIBSETCLEAR:	//�r�b�g�}�b�v�A�z�z��N���A
			m_mapDibPtr.Clear();
			break;
		case FUNC_HISTGRAM: //�q�X�g�O�����̕\��
			ShowHistgram();
			break;
		case FUNC_GETBINARYLEVEL: //�w��ʐϊ����ɂȂ�Q�l�����x���̎擾
			{
				double dbPer = GetValientDbl(&pDispParams->rgvarg[0]);	//�w�芄��
				ReturnInt(wFlags,pVarResult,GetBinaryLevel(dbPer));
			}
			break;
		case FUNC_GETTHRESHOLD:  //2�l���������l�擾
			{
				int nTh  = this->GetTh(); //�ݒ肳�ꂽ�������l
				if( pDispParams->cArgs == 2 ){
					CString strColor = pDispParams->rgvarg[1].bstrVal;//���H���H
					int nPixels = pDispParams->rgvarg[0].ulVal; //�w���f��
					nTh = CalcThreshold(strColor,nPixels);
				}else if(pDispParams->cArgs == 1){
					CString strCommand = pDispParams->rgvarg[0].bstrVal;
					if( strCommand == "auto" || strCommand == "AUTO" ){
						nTh = CalcThreshold();						
					}
				}
				ReturnInt(wFlags,pVarResult,nTh);	
			}			
			break;
		case FUNC_GETBRIGHTNESS: //���邳�͈̔͂����߂�
			{
				CString strType = pDispParams->rgvarg[4].bstrVal;//MIN�HMAX�H
				CRect rect;
				rect.left   = GetValientInt(&pDispParams->rgvarg[3]);//��
				rect.top    = GetValientInt(&pDispParams->rgvarg[2]);//��
				rect.right  = GetValientInt(&pDispParams->rgvarg[1]);//�E
				rect.bottom = GetValientInt(&pDispParams->rgvarg[0]);//��
				int nVal = -1;
				if( strType == L"MIN"){
					nVal = GetBrightnessMin(rect);
				}else if(strType == L"MAX"){
					nVal = GetBrightnessMax(rect);
				}else if(strType == L"RANGE"){
					nVal = GetBrightnessRange(rect);
				}else{ //strType == L"DIFF"
					nVal = GetBrightnessDiff(rect);
				}
				ReturnInt(wFlags,pVarResult,nVal);	
			}
			break;
		case FUNC_MEDIAN:	//���f�B�A���t�B���^
			{
				m_dib.MedianFilter();
			}
			break;
		case FUNC_MAPDELTA:	//�A�z�z��ŏ��l�摜�쐬
			{
				double dbAmp = GetValientDbl(&pDispParams->rgvarg[0]); //�{��
				MapDelta(dbAmp);
			}
			break;
		case FUNC_EDIT:		//�r�b�g�}�b�v�ҏW
			{
				CString strEdit = pDispParams->rgvarg[1].bstrVal; //�r�b�g�}�b�v�ҏW�A�v���P�[�V����
				CString strFile = pDispParams->rgvarg[0].bstrVal; //�r�b�g�}�b�v�t�@�C������
				BOOL isOK = Exec(SW_HIDE,strEdit.GetBuffer(),strFile.GetBuffer());
				ReturnBool(wFlags,pVarResult,isOK);
			}			
			break;
		case FUNC_CALCTHRESHOOLD://�������l�����v�Z
			{
				int nRet = CalcThreshold();
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_SELECTCOLOR: //�F���o
			{
				double dbLength = GetValientDbl(&pDispParams->rgvarg[3]);//����
				BYTE byR = GetValientInt(&pDispParams->rgvarg[2]);//R
				BYTE byG = GetValientInt(&pDispParams->rgvarg[1]);//G
				BYTE byB =GetValientInt(&pDispParams->rgvarg[0]);//B
				SelectColor(dbLength,byR,byG,byB);
			}
			break;
		case FUNC_SELECT://����Ɖ����I��
			{
				double dbLow = GetValientDbl(&pDispParams->rgvarg[1]);
				double dbHigh= GetValientDbl(&pDispParams->rgvarg[0]);
				SelectLH(dbLow,dbHigh);
			}
			break;
		case FUNC_FINDEDGE: //�G�b�W���W���o
			{
				CRect rect;
				rect.left   = GetValientInt(&pDispParams->rgvarg[5]);//��
				rect.top    = GetValientInt(&pDispParams->rgvarg[4]);//��
				rect.right  = GetValientInt(&pDispParams->rgvarg[3]);//�E
				rect.bottom = GetValientInt(&pDispParams->rgvarg[2]);//��
				int nDirection = GetValientInt(&pDispParams->rgvarg[1]);//����
				int nColor = GetValientInt(&pDispParams->rgvarg[0]);//�� or ��
				double dbEdge = FindEdge(rect,nDirection,nColor); //Point�擾
				ReturnDbl(wFlags,pVarResult,dbEdge);
			}
			break;
		case FUNC_DRAWLINE:		//���C���`��
			{
				int nX1 = GetValientInt(&pDispParams->rgvarg[7]);//x1
				int nY1 = GetValientInt(&pDispParams->rgvarg[6]);//y1
				int nX2 = GetValientInt(&pDispParams->rgvarg[5]);//x2
				int nY2 = GetValientInt(&pDispParams->rgvarg[4]);//y2
				BYTE byR = pDispParams->rgvarg[3].intVal;//R
				BYTE byG = pDispParams->rgvarg[2].intVal;//G
				BYTE byB = pDispParams->rgvarg[1].intVal;//B
				int nWidth=pDispParams->rgvarg[0].intVal;//Line width
				DrawLine(nX1,nY1,nX2,nY2,RGB(byR,byG,byB),nWidth);
			}
			break;
		case FUNC_DRAWBITMAPLINE: //�r�b�g�}�b�v���C���`��
			{
				if (pDispParams->cArgs  > 0) {
					App *pApp = App::GetInstance();
					CString strMsg;
					strMsg.Format(L"%.2f,%.2f-%.2f,%.2f",m_ptS.X,m_ptS.Y,m_ptE.X,m_ptE.Y);
					pApp->TextOutput(strMsg);
				}
				DrawBitmapLine();
			}
			break;
		case FUNC_MASKBORDER: //���x�����O�v�f�}�X�N
			{
				int nEnum = pDispParams->rgvarg[0].intVal;//Element No
				MaskBorder(nEnum);
			}
			break;
		case FUNC_CALCWIDTH://�����v�Z
		{
			int nX = GetValientInt(&pDispParams->rgvarg[3]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[2]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[1]);//�͈͎w��
			CString strType = pDispParams->rgvarg[0].bstrVal; //�摜�v�����
			double dbWidth = CalcWidth(nX, nY, nRange, strType);
			ReturnDbl(wFlags, pVarResult, dbWidth);
		}
			break;
		case FUNC_CALCHEIGHT:	//�������v�Z
		{
			int nX = GetValientInt(&pDispParams->rgvarg[3]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[2]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[1]);//�͈͎w��
			CString strType = pDispParams->rgvarg[0].bstrVal; //�摜�v�����
			double dbHeight = CalcHeight(nX, nY, nRange, strType);
			ReturnDbl(wFlags, pVarResult, dbHeight);
		}
			break;
		case FUNC_CALCWIDTHDIRECT:		//�ŏ������v�Z
		{
			int nX = GetValientInt(&pDispParams->rgvarg[2]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[1]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[0]);//�͈͎w��
			double dbWidth = CalcWidthDirect(nX,nY,nRange, m_ptS,m_ptE);
			ReturnDbl(wFlags, pVarResult, dbWidth);
		}
			break;
		case FUNC_CALCHEIGHTDIRECT:		//�ŏ��������v�Z
		{
			int nX = GetValientInt(&pDispParams->rgvarg[2]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[1]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[0]);//�͈͎w��
			double dbHeight = CalcHeightDirect(nX, nY, nRange, m_ptS, m_ptE);
			ReturnDbl(wFlags, pVarResult, dbHeight);
		}
		break;

		case FUNC_RMAX://R�v�f�ő�l
			{
				int nRet = 0;
				if(pDispParams->cArgs==0){
					nRet = GetMaxElement(2);
					ReturnInt(wFlags,pVarResult,nRet);
				}else if( pDispParams->cArgs == 4 ){//�̈�w��
					int nXmin = GetValientInt(&pDispParams->rgvarg[3]);
					int nYmin = GetValientInt(&pDispParams->rgvarg[2]);
					int nXmax = GetValientInt(&pDispParams->rgvarg[1]);
					int nYmax = GetValientInt(&pDispParams->rgvarg[0]);
					nRet = GetMaxElement(2,nXmin,nYmin,nXmax,nYmax);					
				}
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_RMIN:	//R�v�f�ŏ��l
			{
				int nRet = 0;
				if( pDispParams->cArgs == 0 ){
					nRet = GetMinElement(2);					
				}else if( pDispParams->cArgs == 4 ){ //�̈�w��
					int nXmin = GetValientInt(&pDispParams->rgvarg[3]);
					int nYmin = GetValientInt(&pDispParams->rgvarg[2]);
					int nXmax = GetValientInt(&pDispParams->rgvarg[1]);
					int nYmax = GetValientInt(&pDispParams->rgvarg[0]);
					nRet = GetMinElement(2,nXmin,nYmin,nXmax,nYmax);
				}
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_GMAX:	//G�v�f�ő�l
			ReturnInt(wFlags,pVarResult,GetMaxElement(1));
			break;
		case FUNC_GMIN:	//G�v�f�ŏ��l
			ReturnInt(wFlags,pVarResult,GetMinElement(1));
			break;
		case FUNC_BMAX:	//B�v�f�ő�l
			ReturnInt(wFlags,pVarResult,GetMaxElement(0));
			break;
		case FUNC_BMIN:	//B�v�f�ŏ��l
			ReturnInt(wFlags,pVarResult,GetMinElement(0));
			break;
		case FUNC_DEFECTTEST:	//���׌���
			{
				int nTh    = pDispParams->rgvarg[2].intVal;//�������l
				int nRange = pDispParams->rgvarg[1].intVal;//�������l��
				int nMin   =  pDispParams->rgvarg[0].intVal;//�ŏ���f��
				DefectTest(nTh,nRange,nMin);//���׌���
			}
			break;
		case FUNC_LENSCHECK:	//�����Y�`�F�b�N
			{
				int nTh   = pDispParams->rgvarg[3].intVal;//�������l
				int nRange= pDispParams->rgvarg[2].intVal;//�������l��
				int nMin   =  pDispParams->rgvarg[1].intVal;//�ŏ���f��
				int nEdge = pDispParams->rgvarg[0].intVal;//���͉�f��
				DefectTest(nTh,nRange,nMin);//���׌���
				PaintBorder(nEdge);
			}
			break;
		case FUNC_BINARYMATCHING: //2�l���}�b�`���O
		{
			int nTh = pDispParams->rgvarg[4].intVal;//�������l
			double dbX = GetValientDbl(&pDispParams->rgvarg[3]);
			double dbY = GetValientDbl(&pDispParams->rgvarg[2]);
			int nMin = pDispParams->rgvarg[1].intVal;//�ŏ���f��
			int nMax = pDispParams->rgvarg[0].intVal;//�ő��f��
			double dbVal = BinaryMatching(nTh,dbX,dbY,nMin,nMax); //�}�b�`���O%
			ReturnDbl(wFlags, pVarResult, dbVal);
		}
			break;
		case FUNC_OUTPUTFILEPATH://FilePath�o��
		
		if (pDispParams->cArgs == 3){
			CString strID = pDispParams->rgvarg[2].bstrVal;//
			int nX = pDispParams->rgvarg[1].intVal;//
			int nY = pDispParams->rgvarg[0].intVal;//			
			App::GetInstance()->DrawText(strID, m_dib.GetFilePath(), nX, nY);
		}
		else{
			App::GetInstance()->TextOutput(m_dib.GetFilePath());
		}
			break;
		case FUNC_OUTPUTFILENAME: //FileName�o��
			if (pDispParams->cArgs == 3){
				CString strID = pDispParams->rgvarg[2].bstrVal;//
				int nX = pDispParams->rgvarg[1].intVal;//
				int nY = pDispParams->rgvarg[0].intVal;//			
				App::GetInstance()->DrawText(strID, m_dib.GetFileName(), nX, nY);
			}
			else{
				App::GetInstance()->TextOutput(m_dib.GetFileName());
			}
			break;
	}
	return S_OK;
}

//�w��r�b�g�}�b�v�̕��ς��v�Z
void Bitmap24::Average(CStringArray &strAryPath)
{
	int nBnum = (int)strAryPath.GetSize();
	int nSmax = 0; //�r�b�g�}�b�v�T�C�Y�ő�
	for(int i=0;i<nBnum;i++){//�t�@�C���̐��������[�v
		CDIB *pDib = m_mapDibPtr.GetDibPtr(i); //�r�b�g�}�b�v�N���X�擾
		if( pDib != NULL ){
			pDib->Load( strAryPath[i] ); //�ǂݍ���
			
			if( nSmax < pDib->GetSize() ){
				nSmax = pDib->GetSize();
			}
		}
	}
	if( nSmax > 0 ){
		//���v�l���v�Z
		m_bufCalc.Alloc(nSmax); //�v�Z�p�o�b�t�@�̍쐬		
		for(int i=0;i<nBnum;i++){
			CDIB *pDib = m_mapDibPtr.GetDibPtr(i);
			if(pDib == NULL){
				continue ;
			}
			int nW = pDib->GetWidth();
			int nH = pDib->GetHeight();
			BYTE *pSrc = pDib->GetBits();
			int *pDst = m_bufCalc.m_pBuf; //�]����
			int nSize = pDib->GetSize();
			for(int n=0;n<nSize;n++){
				*pDst++ += *pSrc++;
			}			
			if( nW != m_dib.GetWidth() || nH != m_dib.GetHeight() ){ 
				m_dib.Create(nW,nH);//�r�b�g�}�b�v�쐬
			}		

		}
		//���ʂ������o�r�b�g�}�b�vm_dib�ɃZ�b�g
		int *pSrc  = m_bufCalc.m_pBuf; //�]����
		BYTE *pDst = m_dib.GetBits();
		for(int n=0;n<nSmax;n++){
			*pDst++ = (BYTE)(*pSrc++ / nBnum);
		}
	}
}

//�c��
void Bitmap24::Expand(int nNum) 
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 + 3; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		for(int i=0;i<nNum;i++){
			Expand(nW,nH,nLine,nPad);
		}
	}
}
void Bitmap24::Expand(int nW,int nH,int nLine,int nPad)
{
	BYTE *pSrc = m_dib.GetBits();
	BYTE *pDst = m_dibTmp.GetBits(); //�擪�A�h���X�擾		

	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	//�W�������������Ă����Ƃ��傫�Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	BYTE byVal; //Src Value
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			byVal = *pSrc;
			if( byVal < *(pSrc-3)){//��
				byVal = *(pSrc-3);
			}
			if( byVal < *(pSrc+3)){//�E
				byVal = *(pSrc+3);
			}				
			if( byVal < *(pSrc-nLine-3)){//����
				byVal = *(pSrc-nLine-3);
			}
			if( byVal < *(pSrc-nLine)){//��
				byVal = *(pSrc-nLine);
			}
			if( byVal < *(pSrc-nLine+3)){//�E��
				byVal = *(pSrc-nLine+3);
			}
			if( byVal < *(pSrc+nLine-3)){//����
				byVal = *(pSrc+nLine-3);
			}
			if( byVal < *(pSrc+nLine)){//��
				byVal = *(pSrc+nLine);
			}
			if( byVal < *(pSrc+nLine+3)){//�E��
				byVal = *(pSrc+nLine+3);
			}
			//����
			pSrc++;
			*pDst++ = byVal; //�ő�lSet
		}
		pSrc += nPad;
		pDst += nPad;
	}
	m_dibTmp.Copy(m_dib); //m_dibTmp -> m_dib
}

//���k
void Bitmap24::Shrink(int nNum)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 + 3; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		for(int i=0;i<nNum;i++){
			Shrink(nW,nH,nLine,nPad);
		}
	}
}
//���k
void Bitmap24::Shrink(int nW,int nH,int nLine,int nPad)
{
	BYTE *pSrc = m_dib.GetBits();
	BYTE *pDst = m_dibTmp.GetBits(); //�擪�A�h���X�擾		
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	BYTE byVal; //Src Value
	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			byVal = *pSrc;
			if( byVal > *(pSrc-3)){//��
				byVal = *(pSrc-3);
			}
			if( byVal > *(pSrc+3)){//�E
				byVal = *(pSrc+3);
			}				
			if( byVal > *(pSrc-nLine-3)){//����
				byVal = *(pSrc-nLine-3);
			}
			if( byVal > *(pSrc-nLine)){//��
				byVal = *(pSrc-nLine);
			}
			if( byVal > *(pSrc-nLine+3)){//�E��
				byVal = *(pSrc-nLine+3);
			}
			if( byVal > *(pSrc+nLine-3)){//����
				byVal = *(pSrc+nLine-3);
			}
			if( byVal > *(pSrc+nLine)){//��
				byVal = *(pSrc+nLine);
			}
			if( byVal > *(pSrc+nLine+3)){//�E��
				byVal = *(pSrc+nLine+3);
			}
			//����
			pSrc++;
			*pDst++ = byVal; //�ő�lSet
		}
		pSrc += nPad;
		pDst += nPad;
	}
	m_dibTmp.Copy(m_dib); //m_dibTmp -> m_dib
}

//�ߎ��F���o
void Bitmap24::SelectColor(double dbLength,BYTE byR,BYTE byG,BYTE byB)
{
	int nSize = m_dib.GetSize();
	BYTE *pBits = m_dib.GetBits();
	for(int i=0;i<nSize;i+=3){
		double dbB = *pBits     - byB;
		double dbG = *(pBits+1) - byG;
		double dbR = *(pBits+2) - byR;
		double dbLen = sqrt(dbB*dbB+dbG*dbG+dbR*dbR);
		if( dbLen < dbLength){
			*pBits++ = 255;
			*pBits++ = 255;
			*pBits++ = 255;
		}else{
			*pBits++ = 0;
			*pBits++ = 0;
			*pBits++ = 0;			
		}
	}
}

//��������
void Bitmap24::Gradient(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( !m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //�擪�A�h���X�擾		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//�I�y���[�^�[
	static const int fx[9]={
		0,0, 0,
		0,1,-1,
		0,0, 0,
	};
	static const int fy[9]={
		0, 0,0,
		0, 1,0,
		0,-1,0,
	};
	int d[9]; //�v�Z����
	double dbDx,dbDy;
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDx = dbDy = 0;
			d[0] = *(pSrc+nLine-3);	//����
			d[1] = *(pSrc+nLine);	//��	
			d[2] = *(pSrc+nLine+3);	//�E��	
			d[3] = *(pSrc-3);		//��
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//�E
			d[6] = *(pSrc-nLine-3);	//����
			d[7] = *(pSrc-nLine);	//��
			d[8] = *(pSrc-nLine+3);	//�E��
			for(int i=0;i<9;i++){
				dbDx += d[i]*fx[i];
				dbDy += d[i]*fy[i];
			}
			*pDst++ = (BYTE)(dbAmp*sqrt(dbDx*dbDx+dbDy*dbDy));
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Src��ύX
}

//�֊s�����o�p����
void Bitmap24::Sobel(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( !m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //�擪�A�h���X�擾		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//�I�y���[�^�[
	static const int fx[9]={
		-1,0,1,
		-2,0,2,
		-1,0,1,
	};
	static const int fy[9]={
		-1,-2,-1,
		 0, 0, 0,
		 1, 2, 1,
	};

	int d[9]; //�v�Z����
	double dbDx,dbDy;
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDx = dbDy = 0;
			d[0] = *(pSrc+nLine-3);	//����
			d[1] = *(pSrc+nLine);	//��	
			d[2] = *(pSrc+nLine+3);	//�E��	
			d[3] = *(pSrc-3);		//��
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//�E
			d[6] = *(pSrc-nLine-3);	//����
			d[7] = *(pSrc-nLine);	//��
			d[8] = *(pSrc-nLine+3);	//�E��
			for(int i=0;i<9;i++){
				dbDx += d[i]*fx[i]; //x�����ϓ��l ///////////////////////////////////////////////////////////////////////////
				dbDy += d[i]*fy[i]; //y�����ϓ��l
			}
			*pDst++ = (BYTE)(dbAmp*sqrt(dbDx*dbDx+dbDy*dbDy));
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //�����摜�]��

	//////���������ύX//////////////////////////////////////FOR DEBUG
	////int nHalf = nLine / 2;
	////for(int y=0;y<nH;y++){
	////	BYTE *pDst = m_dib.GetPBitsB(0,y);
	////	BYTE *pSrc = m_dibTmp.GetPBitsB(0,y);
	////	for(int i=0;i<nHalf;i++,pDst++,pSrc++){
	////		*pDst = MIN(MAXBYTE,*pDst+*pSrc);
	////	}
	////}
	////
	////����
	//for(int y=0;y<nH;y++){
	//	BYTE *pDst = m_dib.GetPBitsB(0,y);
	//	BYTE *pSrc = m_dibTmp.GetPBitsB(0,y);
	//	for(int i=0;i<nLine;i++,pDst++,pSrc++){
	//		*pDst = MIN(MAXBYTE,*pDst+*pSrc);
	//	}
	//}
}

//���v���V�A��
void Bitmap24::Laplacian(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( !m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //�擪�A�h���X�擾		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//�I�y���[�^�[
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	int d[9]; //�v�Z����
	double dbDelta;
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDelta = 0;
			d[0] = *(pSrc+nLine-3);	//����
			d[1] = *(pSrc+nLine);	//��	
			d[2] = *(pSrc+nLine+3);	//�E��	
			d[3] = *(pSrc-3);		//��
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//�E
			d[6] = *(pSrc-nLine-3);	//����
			d[7] = *(pSrc-nLine);	//��
			d[8] = *(pSrc-nLine+3);	//�E��
			for(int i=0;i<9;i++){
				dbDelta += d[i]*c[i];
			}
			*pDst++ = (BYTE)abs(dbAmp*dbDelta);
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Src��ύX
}
//�K���}�␳
void Bitmap24::Gamma(double dbExp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nSize = m_dib.GetSize();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //�p�f�B���O�T�C�Y //x==nW-1�����l��

	//�ő�l�����߂�
	double dbMax = pow(MAXBYTE,dbExp);
	double dbAlpha = MAXBYTE / dbMax ; //�䗦

	BYTE *pSrc = m_dib.GetBits();   //�擪�A�h���X�擾		
	for(int i=0;i<nSize;i++){
		double dbTmp = pow( *pSrc,dbExp);
		*pSrc++ = (BYTE)(dbTmp * dbAlpha);
	}
}
//�֊s���o
void Bitmap24::Prewitt(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //�p�f�B���O�T�C�Y //x==nW-1�����l��
	if( !m_dibTmp.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //�擪�A�h���X�擾		
	m_dibTmp.PaintBlack();
	BYTE *pDst = m_dibTmp.GetBits();

	int d0,d1,d2,d3,d4,d5,d6,d7,d8; //�v�Z����1
	int m[9]; //�v�Z����2
	int nMax = 0;
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			nMax = 0;
			d0 = *(pSrc+nLine-3);//����
			d1 = *(pSrc+nLine);	 //��	
			d2 = *(pSrc+nLine+3);//�E��	
			d3 = *(pSrc-3);		 //��
			d4 = *pSrc * 2;		 //
			d5 = *(pSrc+3);		 //�E
			d6 = *(pSrc-nLine-3);//����
			d7 = *(pSrc-nLine);	 //��
			d8 = *(pSrc-nLine+3);//�E��
			m[0] =  d0 + d1 + d2 + d3 - d4 + d5 - d6 - d7 - d8;
			m[1] =  d0 + d1 + d2 + d3 - d4 - d5 + d6 - d7 - d8;
			m[2] =  d0 + d1 - d2 + d3 - d4 - d5 + d6 + d7 - d8;
			m[3] =  d0 - d1 - d2 + d3 - d4 - d5 + d6 + d7 + d8;
			m[4] = -d0 - d1 - d2 + d3 - d4 + d5 + d6 + d7 + d8;
			m[5] = -d0 - d1 + d2 - d3 - d4 + d5 + d6 + d7 + d8;
			m[6] = -d0 + d1 + d2 - d3 - d4 + d5 - d6 + d7 + d8;
			m[7] =  d0 + d1 + d2 - d3 - d4 + d5 - d6 - d7 + d8;
			//int nNo = 0;
			for(int i=0;i<8;i++){//�ő�l����
				if( nMax < m[i] ){
					nMax = m[i];
				}
			}
			*pDst++ = (BYTE)dbAmp*nMax;
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Src��ύX	
}

//�w�芄���̔��l�ŋ敪���邽�߂̂������l���擾
int Bitmap24::GetBinaryLevel(double dbPer)
{
	int nR[MAXBYTE+1];
	int nG[MAXBYTE+1];
	int nB[MAXBYTE+1];
	m_dib.CollectHistgram(nR,nG,nB);//�q�X�g�O�������W
	int nAll[MAXBYTE+1]; //�S�v�f�̉�f��
	for(int i=0;i<=MAXBYTE;i++){
		nAll[i] = nR[i] + nG[i] + nB[i];
	}

	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nPixels = (int)(nW*nH*3*dbPer+0.5); //�w���f��
	
	for(int i=MAXBYTE;i>=0;i--){
		nPixels -= nAll[i];
		if( nPixels <= 0 ){
			return i;
		}
	}
	return 0;
}

//RGB�v�f�ő�l�擾
int Bitmap24::GetMaxElement(int nRGB)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	BYTE byMax = 0;	
	for(int y=0;y<nH;y++){
		BYTE *pBits = m_dib.GetPBitsB(0,y);
		for(int x=0;x<nW;x++,pBits+=3){
			if( byMax < *(pBits+nRGB) ){//R or G or B
				byMax = *(pBits+nRGB);
			}
		}
	}
	return byMax;
}
//RGB�v�f�ő�l�擾 �̈�w��
int Bitmap24::GetMaxElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nXmin = MAX(0,nXmax);
	nYmin = MAX(0,nYmin);
	nXmax = MIN(nXmax,nW-1);
	nYmax = MIN(nYmax,nH-1);
	BYTE byMax = 0;	
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pBits = m_dib.GetPBitsB(nXmin,y);
		for(int x=nXmin;x<=nXmax;x++,pBits+=3){
			if( byMax < *(pBits+nRGB) ){//R or G or B
				byMax = *(pBits+nRGB);
			}
		}
	}
	return byMax;
}

//RGB�v�f�ŏ��l�擾
int Bitmap24::GetMinElement(int nRGB)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	BYTE byMin = MAXBYTE;
	for(int y=0;y<nH;y++){
		BYTE *pBits = m_dib.GetPBitsB(0,y);
		for(int x=0;x<nW;x++,pBits+=3){
			if( byMin > *(pBits+nRGB) ){//R or G or B
				byMin = *(pBits+nRGB);
			}
		}
	}
	return byMin;
}
//RGB�v�f�ő�l�擾 �̈�w��
int Bitmap24::GetMinElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nXmin = MAX(0,nXmax);
	nYmin = MAX(0,nYmin);
	nXmax = MIN(nXmax,nW-1);
	nYmax = MIN(nYmax,nH-1);
	BYTE byMin = MAXBYTE;
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pBits = m_dib.GetPBitsB(nXmin,y);
		for(int x=nXmin;x<=nXmax;x++,pBits+=3){
			if( byMin > *(pBits+nRGB) ){//R or G or B
				byMin = *(pBits+nRGB);
			}
		}
	}
	return byMin;
}

//�q�X�g�O�����\��
void Bitmap24::ShowHistgram()
{	
	if( !::IsWindow(m_dlgHistgram.GetSafeHwnd()) ){//Window�쐬�̕K�v����
		m_dlgHistgram.Create(IDD_HISTGRAM);		
	}
	if( ::IsWindow(m_dlgHistgram.GetSafeHwnd()) ){//Window�쐬�ɐ���
		//m_dib.CollectHistgram(m_dlgHistgram.m_nR,m_dlgHistgram.m_nG,m_dlgHistgram.m_nB);//�q�X�g�O�������W
		m_dib.CollectHistgramGray(m_dlgHistgram.m_nGray);//�q�X�g�O�������W
		m_dlgHistgram.Init();//������
		m_dlgHistgram.ShowWindow(SW_SHOW);
	}
}

//�ʐϔ䂩�炵�����l���v�Z
int Bitmap24::CalcThreshold(CString &strColor,int nPixels)
{
	int nAll = 0;
	int nR[256];
	int nG[256];
	int nB[256];
	m_dib.CollectHistgram(nR,nG,nB);//�q�X�g�O�������W
	if(strColor == "B" || strColor == "BLACK"){
		//�w���f���͍�
		for(int i=0;i<=MAXBYTE;i++){
			nAll += nR[i];
			nAll += nG[i];
			nAll += nB[i];
			if( nPixels < nAll ){ //�w���f���𒴂����Ƃ�
				return i;
			}
		}
	}else if(strColor == "W" || strColor == "WHITE"){
		//�w���f���͔�
		for(int i=MAXBYTE;i>=0;i--){
			nAll += nR[i];
			nAll += nG[i];
			nAll += nB[i];
			if( nPixels < nAll ){ //�w���f���𒴂����Ƃ�
				return i;
			}
		}
	}
	return -1;
}

//�����������l�v�Z
int Bitmap24::CalcThreshold()
{
	int nTh = 128;
	int nGray[256];
	m_dib.CollectHistgramGray(nGray);//�q�X�g�O�������W

	//128��艺�̍ő��f�������߂�
	int nMaxL = 0;
	int nIndexL = 0;
	for(int i=1;i<128;i++){
		if( nMaxL < nGray[i] ){ //�ő�l�̍����ւ�			
			nMaxL = nGray[i];
			nIndexL = i;
		}
	}
	//128�ȏ�̍ő��f�������߂�
	int nMaxH = 128;
	int nIndexH = 255;
	for(int i=128;i<255;i++){
		if( nMaxH < nGray[i] ){ //�ő�l�̍����ւ�			
			nMaxH = nGray[i];
			nIndexH = i;
		}		
	}
	//�ő�l�̒��S���擾
	return (int)( (nIndexH+nIndexL+0.5)/2.0 );
}

//�C���[�W�f�o�C�X�摜�擾
void Bitmap24::FromImageDevice()
{
	ImageDevice *pImageDevice = ImageDevice::GetInstance();
	if( pImageDevice != NULL ){
		CDIB &dib = pImageDevice->GetDib();
		dib.Copy(m_dib);
	}
}
//��r
BOOL Bitmap24::ImageDeviceDiff()
{
	ImageDevice *pImageDevice = ImageDevice::GetInstance();
	if( pImageDevice != NULL ){
		CDIB &dib = pImageDevice->GetDib();
		if( GetDib().IsEqual( dib ) ){ //DIB�ɕω��Ȃ�
			return FALSE;
		}
		return TRUE; //DIB�ɕω�����
	}
	return FALSE;
}
//�C���[�W�f�o�C�X�։摜���M
void Bitmap24::ToImageDevice()
{
	ImageDevice* pImageDevice = ImageDevice::GetInstance();
	pImageDevice->ToBitmap(m_dib);
}

//�w��͈͖��邳�ŏ��l�擾
int Bitmap24::GetBrightnessMin(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMin = INT_MAX;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start�ʒu
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F				
			if( by256 < nMin ){ //��菬�����l�ɍ����ւ�
				nMin = by256;
			}
		}
	}
	return nMin;
}
//�w��͈͖��邳�ő�l�擾
int Bitmap24::GetBrightnessMax(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMax = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start�ʒu
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F				
			if( nMax < by256 ){ //���傫���l�ɍ����ւ�
				nMax = by256;
			}
		}
	}
	return nMax;
}

//�w��͈͖��邳���擾
int Bitmap24::GetBrightnessRange(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMin = MAXBYTE;
	int nMax = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start�ʒu
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F				
			if( nMax < by256 ){ //���傫���l�ɍ����ւ�
				nMax = by256;
			}
			if( by256 < nMin  ){
				nMin = by256;
			}
		}
	}
	return (nMax-nMin);
}

//�w��I�y���[�^�[�̌v�Z
static int CalcOperator(const BYTE *pSrc,const int c[],int nLine)
{
	//B�̃��v���V�A��
	int nSum = *(pSrc+nLine-3) * c[0];//����
	nSum += *(pSrc+nLine)   * c[1];	//��
	nSum += *(pSrc+nLine+3) * c[2];	//�E��	
	nSum += *(pSrc-3)	    * c[3];	//��
	nSum += *pSrc		    * c[4];	//
	nSum += *(pSrc+3)	    * c[5];	//�E
	nSum += *(pSrc-nLine-3) * c[6];	//����
	nSum += *(pSrc-nLine)   * c[7];	//��
	nSum += *(pSrc-nLine+3) * c[8];	//�E��
	return nSum;
}

//�w��͈͖��邳�����l�擾
int Bitmap24::GetBrightnessDiff(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLine = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	//�v�Z�̈���擾
	int nTop  = MAX(1,rect.top);
	int nBtm  = MIN(nH-2,rect.bottom);
	int nLeft = MAX(1,rect.left);
	int nRight= MIN(nW-2,rect.right);

	//�I�y���[�^�[
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	//�w��͈͂̔����l�v�Z
	int nSumDiff = 0; //�����l���v
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pSrc = m_dib.GetPBitsB(nLeft,y);//Start�ʒu
		for(int x=nLeft;x<=nRight;x++){
			for(int i=0;i<3;i++){//RGB
				int nTmp = CalcOperator(pSrc++,c,nLine); //�I�y���[�^�[�v�Z
				nSumDiff += abs(nTmp);
			}
		}
	}
	return (int)(nSumDiff/8.0+0.5);
}

//���σt�B���^
void Bitmap24::AverageFilter()
{
	BYTE *pSrcTop = m_dib.GetBits(); //�擪�A�h���X
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	m_dib.Copy(m_dibTmp); //Copy��쐬
	BYTE *pDstTop = m_dibTmp.GetBits();

	for(int y=1,ymax=nH-1;y<ymax;y++){
		BYTE *pSrc = pSrcTop + y*nLineSize + 3;
		BYTE *pDst = pDstTop + y*nLineSize + 3;
		for(int x=1,xmax=nW-1;x<xmax;x++){
			for(int i=0;i<3;i++){ //BGR
				*pDst++ = Average(pSrc++,nLineSize);//B
			}
		}		
	}
	m_dibTmp.Copy(m_dib);
}

//���ϒl�擾
BYTE Bitmap24::Average(BYTE *pByte,int nLineSize)
{
	int nVal = *pByte;
	nVal += *(pByte-nLineSize-3);
	nVal += *(pByte-nLineSize);
	nVal += *(pByte-nLineSize+3);
	nVal += *(pByte-3);
	nVal += *(pByte+3);
	nVal += *(pByte+nLineSize-3);
	nVal += *(pByte+nLineSize);
	nVal += *(pByte+nLineSize+3);
	return (BYTE)(nVal/9+0.5); //����
}

//�A�z�z��摜�����쐬
void Bitmap24::MapDelta(double dbAmp)
{
	BYTE *pDst = m_dib.GetBits(); //�������ݐ�A�h���X
	int nSize = m_dib.GetSize();
	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	m_dib.PaintBlack(); //0 Clear
	for(POSITION pos = m_mapDibPtr.GetStartPosition(); pos != NULL;){
		int nNo=0;
		CDIB *pDIB = NULL; //�r�b�g�}�b�v�|�C���^
		m_mapDibPtr.GetNextAssoc(pos, nNo, pDIB);
		if( pDIB != NULL ){
			BYTE *pSrc1 = m_dibTmp.GetBits(); //���f�[�^�A�h���X
			BYTE *pSrc2 = pDIB->GetBits();
			for(int i=0;i<nSize;i++){
				BYTE byDelta = abs(*pSrc1++ - *pSrc2++);
				*pDst++ = (BYTE)(dbAmp * 100 * byDelta / 255 );
			}			
		}
	}
}

//R��������
void Bitmap24::ClearR() 
{
	BYTE *pSrc = m_dib.GetBits(); //�擪�A�h���X
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*(pSrc+2) = 0; //Clear R
		pSrc += 3;
	}
}
//G��������
void Bitmap24::ClearG() 
{
	BYTE *pSrc = m_dib.GetBits(); //�擪�A�h���X
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*(pSrc+1) = 0; //Clear G
		pSrc += 3;
	}
}
//B��������
void Bitmap24::ClearB() 
{
	BYTE *pSrc = m_dib.GetBits(); //�擪�A�h���X
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*pSrc = 0; //Clear B
		pSrc += 3;
	}
}

//static ImageDlg s_dlg;
//
////�_�C�A���O�\��
//void Bitmap24::ShowDialog(int nCx,int nCy)
//{
//	CRect rect;
//	GetView()->GetWindowRect(&rect); //�\���ʒu
//	rect.right  = rect.left + nCx;
//	rect.bottom = rect.top  + nCy;
//
//	if( s_dlg.GetSafeHwnd() == NULL ){
//		s_dlg.Create(ImageDlg::IDD,GetView());
//	}
//	s_dlg.SetBitmap(m_dib);
//	s_dlg.CalcWindowRect(&rect);
//	s_dlg.MoveWindow(rect);
//	s_dlg.ShowWindow(SW_SHOW);
//}
//
////�_�C�A���O��\��
//void Bitmap24::HideDialog()
//{
//	if( s_dlg.GetSafeHwnd() != NULL ){
//		s_dlg.ShowWindow(SW_HIDE);
//	}
//}

//����Ɖ����̑I��
void Bitmap24::SelectLH(double dbLow,double dbHigh)
{
	//�O����
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nGray[256];
	m_dib256.Copy(m_dib);	//256�F�r�b�g�}�b�v�̍쐬
	m_dib256.CollectHistgram( nGray );//�q�X�g�O�������W
	BYTE *pDib	= m_dib.GetBits(); //24bit color Top
	int nPadF = m_dib.GetLineSize() - nW*3;
	BYTE *pGray = m_dib256.GetBits(); //Gray Top
	int nPadG = m_dib256.GetLineSize() - nW; //�p�f�B���O�T�C�Y
	int nSize = m_dib256.GetSize();
	int nLowSize  = (int)(nSize * dbLow  / 100);
	int nHighSize = (int)(nSize * dbHigh / 100);
	//High	
	int nHighCount = 0;
	int i=255;
	for(;i>=0;i--){
		nHighCount += nGray[i];
		if( nHighCount >= nHighSize ){
			break;
		}
	}
	BYTE byVal = (BYTE)i;
	for(int y=0;y<nH;y++){		
		for(int x=0;x<nW;x++){			
			if( *pGray++ >= byVal ){
				*pDib++ = 255;//B
				*pDib++ = 255;//G
				*pDib++ = 255;//R
			}else{
				pDib+=3; //skip
			}
		}
		 //1Line�I��
		pGray += nPadG;
		pDib  += nPadF;
	}

	pDib  = m_dib.GetBits(); //24bit color Top
	pGray = m_dib256.GetBits(); //Gray Top
	//LOW
	int nLowCount  = 0;
	for(i=0;i<256;i++){
		nLowCount += nGray[i];
		if( nLowCount >= nLowSize ){
			break;
		}
	}
	byVal = (BYTE)i;
	for(int y=0;y<nH;y++){		
		for(int x=0;x<nW;x++){			
			if( *pGray++ <= byVal ){
				*pDib++ = 0;//B
				*pDib++ = 0;//G
				*pDib++ = 0;//R
			}else{
				pDib+=3;
			}
		}
		 //1Line�I��
		pGray += nPadG;
		pDib  += nPadF;
	}		
}
//�r�b�g�}�b�v�̈����Z
BOOL Bitmap24::DeltaDib(CDIB *pDib1,CDIB *pDib2)
{
	if( pDib1 != NULL && pDib2 != NULL ){		
		BYTE *pBits1 = pDib1->GetBits();
		BYTE *pBits2 = pDib2->GetBits();
		int nSize1 = pDib1->GetSize();
		int nSize2 = pDib2->GetSize();
		if( nSize1 == nSize2 ){
			int nW = pDib1->GetWidth();
			int nH = pDib1->GetHeight();
			m_dib.Create(nW,nH);
			BYTE *pDst = m_dib.GetBits();
			for(int i=0;i<nSize1;i++){
				int nDelata = *pBits1++ - *pBits2++;
				*pDst++ = mid(0,nDelata,MAXBYTE);
			}
			return TRUE;
		}
	}
	return FALSE;
}

//�r�b�g�}�b�v�̊���Z
BOOL Bitmap24::DivDib(CDIB *pDib1,CDIB *pDib2)
{
	if( pDib1 != NULL && pDib2 != NULL ){	
		BYTE *pBits1 = pDib1->GetBits();
		BYTE *pBits2 = pDib2->GetBits();
		int nSize1 = pDib1->GetSize();
		int nSize2 = pDib2->GetSize();
		if( nSize1 == nSize2 ){
			int nW = pDib1->GetWidth();
			int nH = pDib1->GetHeight();
			double dbValMax = 0;
			for(int i=0;i<nSize1;i++,pBits1++,*pBits2++){
				if(*pBits2 > 0){
					double dbVal = (double)*pBits1 / *pBits2;
					if( dbValMax < dbVal ){
						dbValMax = dbVal; //�ő�l�L��
					}
				}
			}
			if( dbValMax > 0 ){
				pBits1 = pDib1->GetBits();
				pBits2 = pDib2->GetBits();
				m_dib.Create(nW,nH);
				BYTE *pDst = m_dib.GetBits();
				for(int i=0;i<nSize1;i++,pBits1++,*pBits2++,pDst++ ){
					if(*pBits2 > 0){
						*pDst = (BYTE)((*pBits1 * MAXBYTE ) / (*pBits2 * dbValMax)); //Range���L����
					}
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

//���C���`��
void Bitmap24::DrawLine(int nX1,int nY1,int nX2,int nY2,COLORREF color,int nWidth)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CPen pen(PS_SOLID,nWidth,color);
	CPen *pPenOld = dcMem.SelectObject(&pen);
	CBitmap *pBmpOld = dcMem.SelectObject(m_dib.GetBitmap());
	dcMem.MoveTo(nX1,nY1);
	dcMem.LineTo(nX2,nY2);
	//��n��
	dcMem.SelectObject(pBmpOld);
	dcMem.SelectObject(pPenOld);
	pDC->DeleteDC();
}
//�r�b�g�}�b�v���C���`��
void Bitmap24::DrawBitmapLine()
{
	this->DrawLine((int)(m_ptS.X+0.5), (int)(m_ptS.Y+0.5), (int)(m_ptE.X+0.5), (int)(m_ptE.Y+0.5), GetLineColor(),1);
}

//���x�����O�}�X�N
void Bitmap24::MaskBorder(int nEnum)
{
	static CPoint s_ptAry[10000]; 
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	Label &label = Labeling::GetInstance()->m_label; //���x�����O�N���X�擾
	Element *pElem = label.GetElement(nEnum);
	if( pElem == NULL ){
		return ;
	}
	ArrayPoint aryPoint; //���E��
	if( label.FindBorder(pElem,aryPoint) > 0 ){	//���E�����o
		int nCount = 0;
		for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
			if( aryPoint[i].x < 0 ){ //�I��
				break;
			}
			s_ptAry[i] = aryPoint[i];
			nCount++;
		}
		CRgn rgn;
		rgn.CreatePolygonRgn(s_ptAry,nCount,ALTERNATE);
		CRgn rgnRect;
		rgnRect.CreateRectRgn(0,0,nW-1,nH-1);
		rgn.CombineRgn(&rgnRect,&rgn,RGN_DIFF); //Paint�̈��I��

		//Clear�`��
		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL); 
		CBitmap *pOldBmp = (CBitmap*)dcMem.SelectObject( m_dib.GetBitmap() );
		CBrush br; //�u���V
		br.CreateSolidBrush( COLOR_BLACK );
		CBrush *pOldBrush = dcMem.SelectObject( &br );
		dcMem.FillRgn(&rgn,&br); //�}�X�N�h��Ԃ�
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldBrush);
	}
}

//���׌��� 
void Bitmap24::DefectTest(int nTh,int nRange,int nMin)
{
	m_dib.Copy(m_dibTmp);//�ޔ�
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	//10�ȏ���c������ȊO�͂��ׂ�0�ɂ���
	int nSize = m_dib.GetSize();
	BYTE *pBits = m_dib.GetBits();
	for(int i=0;i<nSize;i++,pBits++){
		if( *pBits < 10 ){
			*pBits = 0;
		}
	}

	if( !m_dib256.Create(nW,nH) ){ //�v�Z�p���ʃo�b�t�@�擾
		return ;
	}

	m_dib256.Copy(m_dib); //256�K���ϊ�
	BYTE *pSrc = m_dib256.GetBits();   //�擪�A�h���X�擾		
	BYTE *pDst = m_dib.GetBits();
	int nLine256 = m_dib256.GetLineSize();//���m�N�����C���T�C�Y
	int nLine = m_dib.GetLineSize();//�t���J���[1���C���T�C�Y
	int nLen = 1; //���͂P��f�����������Œ�
	int nSide = nLen*2+1; //1�ӂ̒���
	int nPoints = nSide * nSide; //�����G���A��f
	
	int ary[36];//�v�Z����
	ZeroMemory(ary,sizeof(ary));

	BYTE byDelta;	
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	//y==0	
	pSrc += nLine256;
	pDst += nLine ;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc++;
		pDst+=3;
		for(int x=1,xmax=nW-1;x<nW-1;x++){
			byDelta = 0;
			BYTE *pData = pSrc-nLine256*nLen-nLen;	//����[
			for(int i=0;i<nSide;i++){
				for(int j=0;j<nSide;j++){
					if( *pData == 0 ){
						ZeroMemory(ary,sizeof(ary));
						i=nSide;
						break;
					}else if(*pSrc < *pData ){
						ary[i+j*nSide] = 0;
					}else{
						int nDelta = *pSrc - *pData;//�����v�Z
						//ary[i+j*nSide] = (255-*pData) * 10 / *pSrc; 
						ary[i+j*nSide] = *pSrc - *pData + (255-*pData)*(255-*pData)/900; //�����v�Z
					}
					pData++;
				}
				pData -= nSide;//���[��
				pData += nLine256;//���̍s��
			}
			for(int i=0;i<nPoints;i++){
				if(byDelta < ary[i]){ //�ő�l���o
					byDelta =  ary[i];
				}
				//byDelta += ary[i];
			}
			if( byDelta > 0 ){
				BYTE byVal = *pSrc + byDelta;
				*pDst++ = byVal;
				*pDst++ = byVal;
				*pDst++ = byVal;
			}else{
				*pDst++ = 0;
				*pDst++ = 0;
				*pDst++ = 0;
			}
			pSrc++;
		}
		//x==nW-1
		pSrc++;
		pDst+=3;
	}
	//2�l��
	m_dib256.Copy(m_dib); //256�K���ϊ�
	m_dib.ChangeBinary(nTh); //�R�A�쐬

	//m_dib256.SaveFile(L"F:\\tmp\\test.bmp");

	for(int n=0;n<nRange;n++){ //�������l�����[�v
		pSrc = m_dib256.GetBits();   //�擪�A�h���X�擾		
		pDst = m_dib.GetBits();// 
		for(int y=0;y<nH;y++){
			for(int x=0;x<nW;x++){
				if( *pSrc > nTh && x>0 && y>0 && x<nW-1 && y<nH-1){//�R�A�̊m�F
					if( *pDst || *(pDst-3) || *(pDst+3) 
					|| *(pDst-nLine-3) || *(pDst-nLine+3) || *(pDst-nLine)
					|| *(pDst+nLine-3) || *(pDst+nLine+3) || *(pDst+nLine) ){
						*pDst = 255;
						*(pDst+1) = 255;
						*(pDst+2) = 255;
					}					
				}
				pSrc++;
				pDst+=3;
			}
		}
		nTh--;
	}

	//���x�����O ��
	Label &label = Labeling::GetInstance()->m_label; //���x�����O�N���X�擾
	label.Init(nW,nH);
	label.CreateSrcBuf(m_dib,MAXBYTE-1);
	int nEnum = label.Exec(nMin,9999);////////////////////////////////////////////////////////////���ŏ���f��
	for(int i=0;i<nEnum;i++){
		Element *pE = label.GetElement(i);
		label.PaintElementColor(pE,m_dib,255,0,0);
	}
}

//���E���h��Ԃ�
void Bitmap24::PaintBorder(int nEdge)
{
	Label &label = Labeling::GetInstance()->m_label; //���x�����O�N���X�擾

	//�O�~���o
	//label.CreateSrcBuf(m_dibTmp,nTh);
	//label.Exec(1000,1000000);
	Element *pE = label.GetMaxElement();
	if( pE == NULL ){
		return ;
	}
	int nLineCx = nEdge; //���C����
	CRect rcEllipse = pE->GetRect(); //�v�f��`�̈�
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL); 
	CBitmap *pOldBmp = (CBitmap*)dcMem.SelectObject( m_dib.GetBitmap() );
	CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);
	//CPen pen(PS_SOLID,nLineCx,COLOR_GREEN);
	CPen pen(PS_SOLID,nLineCx,COLOR_BLACK);
	CPen *pOldPen = dcMem.SelectObject(&pen);
	dcMem.Ellipse(rcEllipse); 	//�Ή~�`��
	dcMem.SelectObject( pOldBmp );
	dcMem.SelectObject( pOldBrush);
	dcMem.SelectObject( pOldPen);
}

//���������G�b�W���o
double Bitmap24::FindEdgeH(int nTh,int nX,int nY,int nDelta) 
{
	BYTE *pTop = m_dib.GetBits() ;
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nX = MIN(nX, nW - 1);
	if( pTop != NULL && nY < nH ){
		for(int x=nX;0<=x && x<nW;x+=nDelta){
			int nVal = m_dib.GetBitsB(x,nY);
			if( nVal >= nTh ){//�����o
				int nXPrev = x-nDelta;
				int nVPrev = m_dib.GetBitsB(nXPrev,nY);
				double dbDv = nVal-nVPrev;
				double dbX = nXPrev + (nTh-nVPrev) / dbDv; //x���W�擾
				return dbX;				
			}
		}
	}
	return -1;
}
//���������G�b�W���o
double Bitmap24::FindEdgeV(int nTh,int nX,int nY,int nDelta) 
{
	BYTE *pTop = m_dib.GetBits() ;
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	if( pTop != NULL && nY < nH ){
		for(int y=nY;0<=y && y<nH;y+=nDelta){
			int nVal = m_dib.GetBitsB(nX,y);
			if( nVal >= nTh ){//�����o
				int nYPrev = y-nDelta;
				int nVPrev = m_dib.GetBitsB(nX,nYPrev);
				double dbDv = nVal-nVPrev;
				double dbY = nYPrev + (nTh-nVPrev) / dbDv; //x���W�擾
				return dbY;				
			}
		}
	}
	return -1;
}

//�w��t�H���_���r�b�g�}�b�v���W
void Bitmap24::CollectBitmap(CString &strPath)
{
	int nCnt = 0;
	CFileFind find;
	BOOL isOK = find.FindFile(strPath + _T("\\*.*"));
	while (isOK){
		isOK = find.FindNextFile();
		if (!find.IsDirectory()){ //File
			CString strFilePath = find.GetFilePath();
			if (m_dib.Load(strFilePath)){
				SetDib(nCnt);
				nCnt++;
			}
		}
	}
}

//�w��ԍ��Ƀr�b�g�}�b�v�L��
BOOL Bitmap24::SetDib(int nNo)
{
	CDIB *pDib = m_mapDibPtr.GetDibPtr(nNo); //�r�b�g�}�b�v�N���X�擾
	if(pDib != NULL){
		m_dib.Copy(*pDib); //m_dib -> pDib
		return TRUE;
	}
	return FALSE;
}

//����
enum{
	 D_FROM_TOP = 0
	,D_FROM_BTM
	,D_FROM_LEFT
	,D_FROM_RIGHT
	,D_NUM
};

//Point�擾
double Bitmap24::FindEdge(CRect &rect,int nDirection,int nColor)
{
	double dbRet = 0;
	double dbX = 0;
	double dbY = 0;
	double dbYmax = 0;	
	int nCnt = 0;
	switch(nDirection){
		case D_FROM_TOP:

			break;
		case D_FROM_BTM:
			for(int x=rect.left;x<=rect.right;x++){
				dbY = FindEdgeV(m_byTh,x,rect.top,1) ;
				if( dbYmax < dbY ){
					dbYmax = dbY;
				}
				nCnt++;
			}
			dbRet = dbYmax; //�ő�l�擾
			break;
		case D_FROM_LEFT:
			for(int y=rect.top;y<=rect.bottom;y++){
				dbX += FindEdgeH(m_byTh,rect.left,y,1) ;
				nCnt++;
			}
			dbRet = dbX / nCnt;
			break;
		case D_FROM_RIGHT:
			for(int y=rect.top;y<=rect.bottom;y++){
				dbX += FindEdgeH(m_byTh,rect.right,y,-1) ;
				nCnt++;
			}
			dbRet = dbX / nCnt;
			break;
	}
	return dbRet;
}

//2�l���}�b�`���O
double Bitmap24::BinaryMatching(int nTh, double dbX, double dbY,int nMin,int nMax)
{
	double dbPerMax = 0;
	double dbPer = 0; //
	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256�F�쐬
	Labeling *pLabel = Labeling::GetInstance();
	Label&label =  pLabel->m_label;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nSize = m_dib256.GetSize();
	int nBnum = (int)m_mapDibPtr.GetCount(); //�ő吔
	Dib256 dibDic; //�����f�[�^
	for (int i = 0; i < nBnum; i++){//�t�@�C���̐��������[�v
		CDIB *pDib = m_mapDibPtr.GetDibPtr(i); //�r�b�g�}�b�v�N���X�擾			
		if (pDib->GetWidth()==nW && pDib->GetHeight() == nH){ //�T�C�Y�`�F�b�N
			label.Init(nW, nH);
			label.CreateSrcBuf(*pDib, nTh);			
			if (label.Exec(nMin,nMax) > 0){
				BYTE *pCamera = m_dib256.GetBits(); //�J�����f�[�^�擾
				int nEmax = pLabel->GetMaxElement(); //�ő�v�f�ԍ�
				double dbGx = pLabel->Gx(nEmax); //�d�S�擾
				double dbGy = pLabel->Gy(nEmax);
				pDib->Copy(m_dibTmp); //�f�[�^�ޔ�
				pDib->Shift(m_dibTmp,dbX-dbGx, dbY-dbGy); //xy�����ړ�				
				dibDic.Copy(*pDib); //256�F�ϊ�
				BYTE *pDic = dibDic.GetBits();//�擪�A�h���X
				int nSame = 0;
				int nDiff = 0;
				for (int n = 0; n < nSize; n++, pCamera++, pDic++){
					if (*pDic >= nTh){
						if (*pCamera >= nTh){ //
							nSame++;
						}
						else{
							nDiff++;
						}
					}
					else{//*pDic < nTh
						if (*pCamera >= nTh ){ //
							nDiff++;
						}
					}
				}
				dbPer = 100.0 * nSame / (nSame + nDiff);
				if (dbPerMax < dbPer){ //�ő�l�L�^
					dbPerMax = dbPer;
					m_dib.SetFilePath(pDib->GetFilePath()); //�t�@�C���p�X�L�^
				}
				m_dibTmp.Copy(*pDib); //���ɖ߂�
			}
		}
	}
	return dbPerMax;
}
//���������ő勗���擾
double GetWidthMax(BYTE byTh,CDIB &dib,int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMax = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//�w����W�̒l���擾
	double dbRight=nW-1, dbLeft=0, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int y = ys; y <=ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);			
			for (int x = nX; x < nW; x++,pG+=3) { //�{�����T��				
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMax < dbLen) { //�ő�l�ɕϊ�
				dbLenMax = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	else { //���̎�
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbRight = x - 1 + (byPrev- byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbLeft = x + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMax < dbLen) { //�ő�l�ɕϊ�
				dbLenMax = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	return dbLenMax;
}
//���������ő勗���擾
double GetHeightMax(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbHMax = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); 
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	//�w����W�̒l���擾
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int x = xs; x <= xe; x++) { 
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++,pG-=nLine) {//+�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG+=nLine) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbHMax < dbLen) { //�ő�l�ɕϊ�
				dbHMax = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	else { //���̎�
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) {//+�����T��
				if (*pG < byTh) { //������
					dbDeltaV = byPrev- *pG;
					dbBtm = y - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = byPrev  - *pG;
					dbTop = y + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbHMax < dbLen) { //�ő�l�ɕϊ�
				dbHMax = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	return dbHMax;
}

double GetWidthMin(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMin = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//�w����W�̒l���擾
	double dbRight, dbLeft, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			dbRight = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			dbLeft = 0;
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMin > dbLen) { //�ŏ��l�ɕϊ�
				dbLenMin = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	else { //���̎�
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbRight = x - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbLeft = x + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMin > dbLen) { //�ŏ��l�ɕϊ�
				dbLenMin = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	return dbLenMin;
}
double GetHeightMin(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMin = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	//�w����W�̒l���擾
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; x < nH; x++, pG -= nLine) { //�{�����T��				
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbLenMin > dbLen) { //�ŏ��l�ɕϊ�
				dbLenMin = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	else { //���̎�
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; x < nH; x++, pG -= nLine) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbBtm = y - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					dbTop = y + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbLenMin > dbLen) { //�ŏ��l�ɕϊ�
				dbLenMin = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	return dbLenMin;
}
//���ϒl�擾�F��
double GetWidthAvg(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenAvg = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	int nCnt = 0;
	//�w����W�̒l���擾
	double dbRight, dbLeft, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;

	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	else { //���̎�
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;

			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	ptS.X = (REAL)dbLeft;
	ptE.X = (REAL)dbRight;
	ptS.Y = ptE.Y = (REAL)nY;
	return dbLenAvg;
}
//���ϒl�擾�F����
double GetHeightAvg(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenAvg = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nH - 1);
	int nCnt = 0;
	//�w����W�̒l���擾
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;

	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) { //�{�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG += nLine) { //-�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	else { //���̎�
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) { //�{�����T��
				if (*pG < byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;

			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG += nLine) { //-�����T��
				if (*pG < byTh) { //������
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	ptS.Y = (REAL)dbTop;
	ptE.Y = (REAL)dbBtm;
	ptS.X = ptE.X = (REAL)nX;
	return dbLenAvg;
}

//�����v�Z
double Bitmap24::CalcWidth(int nX, int  nY, int  nRange, CString& strType)
{
	CDIB &dib = GetDib();

	//�^�C�v
	BYTE byTh = GetTh();
	double dbLength = 0;
	CString strTypeLow = strType.MakeLower();
	if (strTypeLow = L"max") {
		dbLength = GetWidthMax(byTh,dib,nX, nY, nRange, m_ptS, m_ptE);
	}
	else if (strTypeLow = L"min") {
		dbLength = GetWidthMin(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else { //strTypeLow = L"avg" 
		dbLength = GetWidthAvg(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	return dbLength;
}

//�������v�Z
double Bitmap24::CalcHeight(int nX, int  nY, int  nRange, CString& strType)
{
	CDIB &dib = GetDib();

	//�^�C�v
	BYTE byTh = GetTh();
	double dbLength = 0;
	CString strTypeLow = strType.MakeLower();
	if (strTypeLow = L"max") {
		dbLength = GetHeightMax(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else if (strTypeLow = L"min") {
		dbLength = GetHeightMin(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else { //strTypeLow = L"avg" 
		dbLength = GetHeightAvg(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	return dbLength;
}
//�ŏ������v�Z
double Bitmap24::CalcWidthDirect(int nX, int  nY, int  nRange,PointF &ptL,PointF &ptR)
{
	BYTE byTh = GetTh();

	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256�F�r�b�g�}�b�v��

	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//�w����W�̒l���擾
	double dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	CArray<PointF> aryPtRT; //�E�[�㔼��
	CArray<PointF> aryPtRB; //�E�[������
	
	double dbYC = (ys + ye) / 2.0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			ptR.X = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					ptR.X = (REAL)(x - 1 + (byTh - byPrev) / dbDeltaV);
					ptR.Y = (REAL)y;
					break;
				}
				byPrev = *pG;
			}
			if (ptR.X > 0) {
				if (y < dbYC) { //�㔼��
					aryPtRT.Add(ptR);
				}
				else {//������
					aryPtRB.Add(ptR);
				}
			}
		}
	}
	else { //���̎�
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			ptR.X = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					ptR .X = (REAL)(x - 1 + (byPrev - byTh) / dbDeltaV);
					ptR.Y = (REAL)y;
					break;
				}
				byPrev = *pG;
			}
			if (ptR.X > 0) {
				if (y < dbYC) { //�㔼��
					aryPtRT.Add(ptR);
				}
				else {//������
					aryPtRB.Add(ptR);
				}
			}
		}
	}
	//
	PointF ptRTAvg; //�d�S
	for (int i = 0; i < aryPtRT.GetSize(); i++) {
		ptRTAvg = ptRTAvg + aryPtRT[i];
	}
	ptRTAvg.X /= aryPtRT.GetSize();
	ptRTAvg.Y /= aryPtRT.GetSize();

	PointF ptRBAvg; //�d�S
	for (int i = 0; i < aryPtRB.GetSize(); i++) {
		ptRBAvg = ptRBAvg + aryPtRB[i];
	}
	ptRBAvg.X /= aryPtRB.GetSize();
	ptRBAvg.Y /= aryPtRB.GetSize();

	int nX1 = (int)(ptRTAvg.X + 0.5);
	int nY1 = (int)(ptRTAvg.Y + 0.5);
	int nX2 = (int)(ptRBAvg.X + 0.5);
	int nY2 = (int)(ptRBAvg.Y + 0.5);
	this->DrawLine(nX1, nY1, nX2, nY2, COLOR_YELLOW, 3); //YELLOW LINE

	//�T���x�N�g���̎n�_
	PointF ptS((REAL)((ptRTAvg.X + ptRBAvg.X)*0.5), (REAL)((ptRTAvg.Y + ptRBAvg.Y)*0.5)); //���_
	PointF vecV(ptRBAvg.X - ptRTAvg.X, ptRBAvg.Y - ptRTAvg.Y);

	//�P�ʃx�N�g����
	double dbSize = sqrt(vecV.X*vecV.X + vecV.Y * vecV.Y);
	vecV.X = (REAL)(vecV.X / dbSize);
	vecV.Y = (REAL)(vecV.Y / dbSize);

	PointF vecF = vecV; //�T���x�N�g��
	//90�x��] 
	vecF.X = -vecV.Y;
	vecF.Y = vecV.X ;


	//�T��
	double dbLenMin = 0;//�ŏ��l
	double dbPrev = 0; //�ȑO�̒l
	PointF ptF = ptS + vecF; //�T�����W
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (ptF.X > 0 &&  ptF.Y > 0 ){
			dbPrev = dbValue;
			ptF = ptF + vecF; //���̓_��
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
			if (byTh <= dbValue) { 
				break;
			}
		}
		//������
		double dbRate = abs( (byTh - dbValue) / (dbPrev - dbValue) ); //���`��Ԕ䗦�����߂�
		ptF.X =(REAL)( ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y-(vecF.Y * dbRate));
	}
	else { //���̂Ƃ�
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (dbValue  > byTh) { //���̊�
			dbPrev = dbValue;
			ptF = ptF + vecF; //���̓_��
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		}
		//������
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //���`��Ԕ䗦�����߂�
		ptF.X = (REAL)(ptF.X-(vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y-(vecF.Y * dbRate));
	}
	ptR = ptS;
	ptL = ptF;
	double dbLength  = ::GetLength(ptF.X, ptF.Y, ptS.X, ptS.Y); //�Q�_�ԋ���	
	return dbLength;
}
double Bitmap24::CalcHeightDirect(int nX, int  nY, int  nRange, PointF &ptT, PointF &ptB)
{
	BYTE byTh = GetTh();
	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256�F�r�b�g�}�b�v��

	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	
//�w����W�̒l���擾
	double dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	CArray<PointF> aryPtBL; //��������
	CArray<PointF> aryPtBR; //���E����
	double dbXC = (xs + xe) / 2.0;
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			ptB.Y = 0;
			for (int y = nY; y < nH; y++, pG -= nLine) { //�{�����T��
				if (*pG >= byTh) { //������
					dbDeltaV = *pG - byPrev;
					ptB.X = (REAL)x;
					ptB.Y = (REAL)(y - 1 + (byTh - byPrev) / dbDeltaV);
					break;
				}
				byPrev = *pG;
			}
			if (ptB.Y > 0) {
				if (x < dbXC) { //������
					aryPtBL.Add(ptB);
				}//�E����
				else {
					aryPtBR.Add(ptB);
				}
			}
		}
	}
	else { //���̎�
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			ptB.Y = 0;
			for (int y = nY; y < nH; y++, pG -= nLine) { //�{�����T��				
				if (*pG < byTh) { //������
					dbDeltaV = byPrev - *pG;
					ptB.X = (REAL)x;
					ptB.Y = (REAL)(y - 1 + (byPrev - byTh) / dbDeltaV);
					break;
				}
				byPrev = *pG;
			}
			if (ptB.Y > 0) {
				if (x < dbXC) { //������
					aryPtBL.Add(ptB);
				}//�E����
				else {
					aryPtBR.Add(ptB);
				}
			}
		}
	}
	//
	PointF ptBLAvg; //�d�S
	for (int i = 0; i < aryPtBL.GetSize(); i++) {
		ptBLAvg = ptBLAvg + aryPtBL[i];
	}
	ptBLAvg.X /= aryPtBL.GetSize();
	ptBLAvg.Y /= aryPtBL.GetSize();

	PointF ptBRAvg; //�d�S
	for (int i = 0; i < aryPtBR.GetSize(); i++) {
		ptBRAvg = ptBRAvg + aryPtBR[i];
	}
	ptBRAvg.X /= aryPtBR.GetSize();
	ptBRAvg.Y /= aryPtBR.GetSize();

	int nX1 = (int)(ptBLAvg.X + 0.5);
	int nY1 = (int)(ptBLAvg.Y + 0.5);
	int nX2 = (int)(ptBRAvg.X + 0.5);
	int nY2 = (int)(ptBRAvg.Y + 0.5);
	this->DrawLine(nX1, nY1, nX2, nY2, COLOR_YELLOW, 3); //YELLOW LINE

														 //�T���x�N�g���̎n�_
	PointF ptS((REAL)((ptBLAvg.X + ptBRAvg.X)*0.5), (REAL)((ptBLAvg.Y + ptBRAvg.Y)*0.5)); //���_
	PointF vecV(ptBLAvg.X - ptBRAvg.X, ptBLAvg.Y - ptBRAvg.Y);

	//�P�ʃx�N�g����
	double dbSize = sqrt(vecV.X*vecV.X + vecV.Y * vecV.Y);
	vecV.X = (REAL)(vecV.X / dbSize);
	vecV.Y = (REAL)(vecV.Y / dbSize);

	PointF vecF = vecV; //�T���x�N�g��
						//90�x��] 
	vecF.X = -vecV.Y;
	vecF.Y = vecV.X;


	//�T��
	double dbLenMin = 0;//�ŏ��l
	double dbPrev = 0; //�ȑO�̒l
	PointF ptF = ptS + vecF; //�T�����W
	if (byData < byTh) { //�w��|�C���g�����̂Ƃ�		
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (ptF.X > 0 && ptF.Y > 0) {
			dbPrev = dbValue;
			ptF = ptF + vecF; //���̓_��
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
			if (byTh <= dbValue) {
				break;
			}
		}
		//������
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //���`��Ԕ䗦�����߂�
		ptF.X = (REAL)(ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y - (vecF.Y * dbRate));
	}
	else { //���̂Ƃ�
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (dbValue  > byTh) { //���̊�
			dbPrev = dbValue;
			ptF = ptF + vecF; //���̓_��
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		}
		//������
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //���`��Ԕ䗦�����߂�
		ptF.X = (REAL)(ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y - (vecF.Y * dbRate));
	}
	ptB = ptS;
	ptT = ptF;
	double dbLength = ::GetLength(ptF.X, ptF.Y, ptS.X, ptS.Y); //�Q�_�ԋ���	
	return dbLength;

}
