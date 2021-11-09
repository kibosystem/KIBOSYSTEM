#include "../stdafx.h"
#include "../KIBO.h"
#include "../LIB/functions.h"
#include "../LIB/ArrayPoint.h"
#include "../KIBOView.h"
#include "../OutputWnd.h"
#include "Labeling.h"
#include "App.h"
#include "Bitmap.h"

static CommandTable s_table[] =
{
	{Labeling::FUNC_SETBITMAP		,1,L"SetBitmap"}		//BITMAP�N���X�ŏ�����
	,{Labeling::FUNC_GETBITMAPSRC	,0,L"GetBitmapSrc"}		//���x�����O�\�[�X�摜�擾
	,{Labeling::FUNC_GETBITMAPDST	,0,L"GetBitmapDst"}		//���x�����O���ʉ摜�擾
	,{Labeling::FUNC_EXEC			,-1,L"Exec"}			//���x�����O���s
	,{Labeling::FUNC_ELEMENTEXEC	, 3,L"ElementLabeling"}  //�G�������g�����̃��x�����O
	,{Labeling::FUNC_SORT			, 2,L"Sort"}			//���x�����O���ʃ\�[�g
	,{Labeling::FUNC_OUTPUTRESULT	, 0,L"OutputResult"}		//���x�����O���ʂ̏o��
	,{Labeling::FUNC_DRAWRECT		,-1,L"DrawElementRect"}	//���x�����O�̈�`��
	,{Labeling::FUNC_PAINTELEMENT	, 4,L"PaintElement"}	//���x�����O�̈�`��
	,{Labeling::FUNC_LOADELEMENT	, 1,L"LoadElement"}		//�摜�t�@�C���ǂݍ���
	,{Labeling::FUNC_REMOVE_ALL_ELEM,0,L"RemoveAllElement"}	//���x�����O�v�f�폜
	,{Labeling::FUNC_INVERT			,0,L"Invert"}			//�������]
	,{Labeling::FUNC_EXPAND			,0,L"Expand"}			//�c��
	,{Labeling::FUNC_GETBORDER		,1,L"GetBorder"}		//���E�����o
	,{Labeling::FUNC_DRAWBORDER		,4,L"DrawBorder"}		//���E���`��
	,{Labeling::FUNC_SELECTBORDER	,3,L"SelectBorder"}		//���E���ꕔ���o
	,{Labeling::FUNC_SETWIDTHRANGE , 2,L"SetWidthRange" }	//�v�f�̕��w��
	,{Labeling::FUNC_SETHEIGHTRANGE ,2,L"SetHeightRange" }	//�v�f�̍����w��
	,{Labeling::FUNC_COUNT			,0,L"Count"}			//���x�����O�v�f���擾
	,{Labeling::FUNC_MAXELEMENT		,0,L"MaxElement"}		//�ő�v�f�ԍ��擾
	,{Labeling::FUNC_INELEMENT		,3,L"InElement"}		//�v�f���̓_�ł����H	
	,{Labeling::FUNC_GX				,1,L"Gx"}				//�d�S�����W
	,{Labeling::FUNC_GY				,1,L"Gy"}				//�d�S�����W
	,{Labeling::FUNC_MIN_X			,1,L"Xmin"}				//�ŏ������W
	,{Labeling::FUNC_MIN_Y			,1,L"Ymin"}				//�ŏ������W
	,{Labeling::FUNC_MAX_X			,1,L"Xmax"}				//�ő傘���W
	,{Labeling::FUNC_MAX_Y			,1,L"Ymax"}				//�ő備���W
	,{Labeling::FUNC_MIN_L			,3,L"Lmin"}				//�w��n�_����̍ŒZ����
	,{Labeling::FUNC_MAX_L			,3,L"Lmax"}				//�w��n�_����̍Œ�����
	,{Labeling::FUNC_WIDTH			,1,L"Width"}			//��
	,{Labeling::FUNC_HEIGHT			,1,L"Height"}			//����
	,{Labeling::FUNC_PIXELS			,1,L"Pixels"}			//��f��
	,{Labeling::FUNC_BORDER			,1,L"Border"}			//���͒�
	,{Labeling::FUNC_R_AVR			,1,L"Ravr"}				//���ϔ��a
	,{Labeling::FUNC_R_MAX			,1,L"Rmax"}				//�ő唼�a
	,{Labeling::FUNC_CIRCLE			,1,L"Circle"}			//�~�`�x
	,{Labeling::FUNC_MAX			,0,NULL}				//�R�}���h��
};

static Labeling *s_pInstance = NULL; //�B��̃N���X����
Labeling* Labeling::GetInstance()//���̎擾
{
	return s_pInstance;
}

Labeling::Labeling(void)
{
	s_pInstance = this;
}

Labeling::~Labeling(void)
{
	m_isInitOK = false; //���������܂������H
}

//�֐����`�F�b�N
STDMETHODIMP Labeling::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
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
STDMETHODIMP Labeling::Invoke(DISPID dispIdMember	//Function ID �ԍ�
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
	//�֐��̎�
	switch(dispIdMember){
		case FUNC_SETBITMAP: //BITMAP�N���X�ŏ�����
			{
				int nTh = pDispParams->rgvarg[0].ulVal; //�������l
				SetBitmap(nTh);
			}
			break;
		case FUNC_GETBITMAPSRC: //���x�����O�摜�擾
			{
				Bitmap24 *pBitmap = Bitmap24::GetInstance();
				m_label.FillBitmapSrc(pBitmap->GetDib()); //���x�����O���ʉ摜���r�b�g�}�b�v�֓]��
			}
			break;
		case FUNC_GETBITMAPDST: //���x�����O���ʃr�b�g�}�b�v�擾
			{
				Bitmap24 *pBitmap = Bitmap24::GetInstance();
				m_label.FillBitmapDst(pBitmap->GetDib()); //���x�����O���ʉ摜���r�b�g�}�b�v�֓]��
			}
			break;
		case FUNC_EXEC: //���x�����O���s
			{
				int nRet = 0;
				if( pDispParams->cArgs >= 6 ){						
					nRet = Exec(
						 pDispParams->rgvarg[5].ulVal //�ŏ���f��
						,pDispParams->rgvarg[4].ulVal //�ő��f��
						,GetValientInt(&pDispParams->rgvarg[3]) //��
						,GetValientInt(&pDispParams->rgvarg[2]) //��
						,GetValientInt(&pDispParams->rgvarg[1]) //�E
						,GetValientInt(&pDispParams->rgvarg[0]) //��
					);
				}else if(pDispParams->cArgs >= 2 ){
					nRet = Exec(
						 pDispParams->rgvarg[1].ulVal	//�ŏ���f��
						,pDispParams->rgvarg[0].ulVal	 //�ő��f��
						);
				}
				ReturnInt(wFlags,pVarResult,nRet); //���x�����O�v�f����߂�
			}
			break;
		case FUNC_ELEMENTEXEC://�G�������g�����̃��x�����O
			{
				int nEnum = pDispParams->rgvarg[2].ulVal;//�v�f�ԍ�
				int nMin = pDispParams->rgvarg[1].ulVal;//�ŏ�
				int nMax = pDispParams->rgvarg[0].ulVal;//�ő�				
				int nRet = ElementExec(nEnum,nMin,nMax); //�G�������g�����̃��x�����O
				ReturnInt(wFlags,pVarResult,nRet); //���x�����O�v�f����߂�
			}
			break;
		case FUNC_SORT: //���x�����O���ʃ\�[�g
			{
				int nSnum = GetValientInt(&pDispParams->rgvarg[1]);
				BOOL isAscending = pDispParams->rgvarg[0].boolVal;
				Sort( nSnum,isAscending );
			}
			break;
		case FUNC_OUTPUTRESULT:	//���x�����O���ʂ̏o��
			OutputResult();
			break;
		case FUNC_DRAWRECT: //���x�����O�̈�`��
			if( pDispParams->cArgs >= 6 ){
				DrawElementRect(
					 pDispParams->rgvarg[5].ulVal	//�v�f�ԍ��擾
					,pDispParams->rgvarg[4].ulVal	//R
					,pDispParams->rgvarg[3].ulVal	//G
					,pDispParams->rgvarg[2].ulVal	//B
					,pDispParams->rgvarg[1].ulVal	//Dx
					,pDispParams->rgvarg[0].ulVal	//Dy
					);
			}else{
				DrawElementRect(
					 pDispParams->rgvarg[3].ulVal	//�v�f�ԍ��擾
					,pDispParams->rgvarg[2].ulVal	//R
					,pDispParams->rgvarg[1].ulVal	//G
					,pDispParams->rgvarg[0].ulVal	//B
					);
			}
			break;
		case FUNC_PAINTELEMENT: //���x�����O�̈�`��	
			{
				PaintElement(
					 pDispParams->rgvarg[3].ulVal	//�v�f�ԍ��擾
					,pDispParams->rgvarg[2].ulVal	//R
					,pDispParams->rgvarg[1].ulVal	//G
					,pDispParams->rgvarg[0].ulVal	//B
					);				
			}
			break;
		case FUNC_LOADELEMENT:	//�摜�t�@�C���ǂݍ���
			{
				CString strDir = pDispParams->rgvarg[0].bstrVal; //�f�B���N�g��Path
				CString strExt = strDir + L"\\*.bmp";
				int nNum = m_label.LoadCheckImage(strExt); //��r�p�̉摜��ǂݍ���
				ReturnInt(wFlags,pVarResult,nNum);
			}
			break;
		case FUNC_REMOVE_ALL_ELEM: //���x�����O�v�f�S����
			{
				CString strName;
				strName.Format(_T("Element"));
				GetView()->m_mgrVO.RemoveByName(strName.GetBuffer());
			}
			break;
		case FUNC_INVERT: //�������]����
			Invert();
			break;
		case FUNC_EXPAND: //�c��
			m_label.ExpandSrc(); //���x�����O�c��
			break;
		case FUNC_GETBORDER://���E�����o
			{
				int nEnum = pDispParams->rgvarg[0].ulVal;	//�v�f�ԍ�
				ArrayPoint *pAryPoint = GetAryPoint(nEnum);
				int nBorder = m_label.FindBorder(nEnum, *pAryPoint);
				ReturnInt(wFlags,pVarResult,nBorder); //Border�|�C���g����߂�
			}			
			break;
		case FUNC_DRAWBORDER:		//���E���`��
			{
				int nEnum = pDispParams->rgvarg[3].ulVal;	//�v�f�ԍ�
				int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
				ArrayPoint *pAryPoint = GetAryPoint(nEnum);
				if( pAryPoint != NULL ){
					CString strName;
					strName.Format(L"Border%d",nEnum);
					GetView()->m_mgrVO.CreatePolylineObject( strName,*pAryPoint,RGB(nR,nG,nB) );
				}
			}
			break;
		case FUNC_SELECTBORDER:		//���E���ꕔ���o
			{
				int nEnum = pDispParams->rgvarg[2].ulVal;	//�v�f�ԍ�
				CString strType = pDispParams->rgvarg[1].bstrVal; //�^�C�v����				
				int nW = pDispParams->rgvarg[0].ulVal;	//��
				SelectBorder(nEnum,strType,nW); //TypeBorder���o
			}
			break;
		case FUNC_SETWIDTHRANGE:	//�v�f�̕��w��
			{
				int nWmin = pDispParams->rgvarg[1].ulVal;	//�ŏ���
				int nWmax = pDispParams->rgvarg[0].ulVal;	//�ő啝
				SetWidthRange(nWmin,nWmax);
			}
			break;
		case FUNC_SETHEIGHTRANGE: //�v�f�̍����w��
		{
				int nHmin = pDispParams->rgvarg[1].ulVal;	//�ŏ���
				int nHmax = pDispParams->rgvarg[0].ulVal;	//�ő啝
				SetHeightRange(nHmin, nHmax);
		}
			break;
		//�v���p�e�B
		case FUNC_COUNT: //���x�����O�v�f���擾
			{
				int nCount = GetCount();
				ReturnInt(wFlags,pVarResult,nCount); //���x�����O�v�f����߂�
			}
			break;
		case FUNC_MAXELEMENT: //�ő�v�f�ԍ��擾
			{
				int nEnum = GetMaxElement(); //�ő�v�f�ԍ�
				ReturnInt(wFlags,pVarResult,nEnum); //���x�����O�v�f����߂�
			}
			break;
		case FUNC_INELEMENT:	//�v�f���̓_�ł����H	
			{
				int nEnum= GetValientInt(&pDispParams->rgvarg[2]); //�v�f�ԍ�
				int nX  = GetValientInt(&pDispParams->rgvarg[1]); //x���W
				int nY  = GetValientInt(&pDispParams->rgvarg[0]); //y���W
				BOOL isIn = FALSE;
				Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
				if( pElement != NULL ){
					isIn = pElement->IsContain(nX,nY); //�v�f���̓_�ł����H
				}
				ReturnBool(wFlags,pVarResult,isIn);
			}
			break;
		case FUNC_GX://�d�S�����W
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				double dbGx = Gx(nEnum);
				ReturnDbl(wFlags,pVarResult,dbGx);
			}
			break;
		case FUNC_GY: //�d�S�����W
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				double dbGy = Gy(nEnum);
				ReturnDbl(wFlags,pVarResult,dbGy);
			}
			break;
		case FUNC_MIN_L:	//�w��n�_����̍ŒZ����
			{
				int nEnum = pDispParams->rgvarg[2].ulVal; //�v�f�ԍ�
				double dbX = pDispParams->rgvarg[1].dblVal; //x���W
				double dbY = pDispParams->rgvarg[0].dblVal; //y���W
				Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
				if(pElement != NULL){
					double dbMin = pElement->GetLMin(dbX,dbY); 
					ReturnDbl(wFlags,pVarResult,dbMin); //�ŏ�������߂�
				}
			}
			break;
		case FUNC_MAX_L:	//�w��n�_����̍Œ�����
			{
				int nEnum = pDispParams->rgvarg[2].ulVal; //�v�f�ԍ�
				double dbX = pDispParams->rgvarg[1].dblVal; //x���W
				double dbY = pDispParams->rgvarg[0].dblVal; //y���W
				Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
				if(pElement != NULL){
					double dbMax = pElement->GetLMax(dbX,dbY); 
					ReturnDbl(wFlags,pVarResult,dbMax); //�ő勗����߂�
				}
			}
			break;
		case FUNC_PIXELS: //��f��
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				int nRet = Pixels(nEnum);				
				ReturnInt(wFlags,pVarResult,nRet); 
			}
			break;
		case FUNC_MIN_X: //�ŏ������W
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				int nRet = Xmin(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;					 
		case FUNC_MIN_Y: //�ŏ������W 
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				int nRet = Ymin(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_MAX_X: //�ő傘���W
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				int nRet = Xmax(nEnum);				
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_MAX_Y: //�ő備���W
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				int nRet = Ymax(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_WIDTH: //��
		case FUNC_HEIGHT://����
		case FUNC_BORDER: //Border
		case FUNC_R_AVR://���ϔ��a
		case FUNC_R_MAX: //�ő唼�a�擾
		case FUNC_CIRCLE://�~�`�x
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //�v�f�ԍ�
				Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
				pVarResult->vt = VT_INT;
				if( pElement != NULL ){
					switch( dispIdMember){
						case FUNC_WIDTH: //��
							pVarResult->intVal = pElement->GetWidth();
							break;
						case FUNC_HEIGHT://����
							pVarResult->intVal = pElement->GetHeight();
							break;
						case FUNC_BORDER: //���͒�
							{
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								pVarResult->intVal = GetBorderPixels(0,aryPoint); //���͒���Pixel�����擾
							}
							break;
						case FUNC_R_AVR://���ϔ��a
							{
								int nPixels = pElement->GetPixels(); //�ʐ�
								double dbGx = pElement->GetGx(); //�d�Sx
								double dbGy = pElement->GetGy(); //�d�Sy
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								double dbLen = 0;
								int i=0;
								for(int imax=(int)aryPoint.GetSize();i<imax;i++){
									if(aryPoint[i].x < 0){
										break;
									}
									dbLen += ::GetLength(dbGx,dbGy,aryPoint[i].x,aryPoint[i].y); //���a�̍��v
								}
								if( i > 0 ){
									dbLen /= i;
								}
								ReturnDbl(wFlags,pVarResult,dbLen);	
							}
							break;
						case FUNC_R_MAX: //�ő唼�a�擾
							{
								int nPixels = pElement->GetPixels(); //�ʐ�
								double dbGx = pElement->GetGx(); //�d�Sx
								double dbGy = pElement->GetGy(); //�d�Sy
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								double dbMax = 0;
								int i=0;
								for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
									if(aryPoint[i].x < 0){
										break;
									}
									double dbLen = ::GetLength(dbGx,dbGy,aryPoint[i].x,aryPoint[i].y); //���a�̍��v
									if( dbLen > dbMax ){ //�ő�l�ɍ���
										dbMax = dbLen;
									}
								}
								ReturnDbl(wFlags,pVarResult,dbMax);
							}
							break;
						case FUNC_CIRCLE://�~�`�x
							{
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								int nBorder = GetBorderPixels(0,aryPoint); //���͒���Pixel�����擾
								double dbCircle = pElement->GetRoundness(nBorder);
								ReturnDbl(wFlags,pVarResult,dbCircle);
							}
							break;

						default:
							pVarResult->intVal = -1;
							break;
					}
				}
			}
			break;
	}
	return S_OK;
}

//���͒���Pixel�����擾
int Labeling::GetBorderPixels(int nBnum,ArrayPoint &aryPoint)
{
	int nBno = 0; //�{�[�_�[�ԍ�
	int nCnt = 0; //���͒�
	for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
		if( aryPoint[i].x < 0 ){
			if( nBno == nBnum ){
				return nCnt;
			}
			nCnt = 0;//reset
			nBno++; //���̃{�[�_�[�ԍ�������			
		}
		nCnt++;
	}
	return -1; //Error
}

//Bitmap�N���X�ŏ�����
BOOL Labeling::Init(CDIB &dib,int nTh) 
{
	m_label.Init(dib.GetWidth(), dib.GetHeight());//���x�����O�o�b�t�@�쐬						
	if (!m_label.CreateSrcBuf(dib, nTh)){//���x�����O�o�b�t�@�i2�l���摜�쐬)
		::ErrorMsg(_T("�摜�̎擾���o���܂���ł����B�J�����Ƃ̐ڑ����m�F���Ă��������B"));
		return FALSE;
	}
	return TRUE;
}

//�������]����
void Labeling::Invert()
{
	Labeling::GetInstance()->m_label.InvertSrcBuf(); //���x�����O�N���X�擾
}

//���x�����O���s
int Labeling::Exec(int nMin,int nMax,int nLeft,int nTop,int nRight,int nBottom)
{
	CRect rect;
	rect.left  = nLeft; //��
	rect.top   = nTop; //��
	rect.right = nRight; //�E
	rect.bottom= nBottom; //��
	return m_label.Exec(nMin,nMax,rect);//���x�����O���s
}
//���x�����O���s
int Labeling::Exec(int nMin,int nMax)
{	
	return m_label.Exec(nMin,nMax);//���x�����O���s
}			
//�G�������g�����̃��x�����O
int Labeling::ElementExec(int nEnum,int nMin,int nMax) 
{
	return m_label.ElementExec(nEnum, nMin, nMax);
}
//�\�[�g
void Labeling::Sort(int nSortNum,BOOL isAscending)
{
	m_label.Sort(nSortNum, isAscending); //�\�[�g�̎��s
}

//���x�����O���ʕ\��
void Labeling::OutputResult()
{
	OutputWnd *pOutput = GetOutputWnd();
	pOutput->Clear();

	CKIBOView* pView = GetView();
	CString str1; //�\��������
	for (int i = 0, imax = m_label.GetEmax(); i<imax; i++){//�S�v�f�擾
		Element *pElem = m_label.GetElement(i);
		str1.Format(L"�̈�(%d,%d,%d,%d) �ʐ�(%d) �d�S1(%.2f,%.2f) �d�S2(%.2f,%.2f)"
			,pElem->GetMinX(),pElem->GetMinY(),pElem->GetMaxX(),pElem->GetMaxY()
			,pElem->GetPixels()
			,pElem->GetGx(),pElem->GetGy()
			,pElem->GetGx()-pElem->GetMinX(),pElem->GetGy()-pElem->GetMinY()
			);
		pOutput->OutputText(str1);
	}
	pOutput->RedrawWindow();
}

//���x�����O�v�f����߂�
int Labeling::GetCount()
{	
	return m_label.GetEmax(); //���x�����O�v�f����߂�
}

//���x�����O�̈�`��
void Labeling::DrawElementRect(int nEnum,int nR,int nG,int nB,int nDx,int nDy)
{
	CString strName;
	strName.Format(_T("Element%d"),nEnum);
	Color color(nR,nG,nB);
	GetView()->SetElementRect(nEnum,strName,color,nDx,nDy);
}
void Labeling::DrawElementRect(int nEnum,int nR,int nG,int nB)
{
	CString strName;
	strName.Format(_T("Element%d"),nEnum);
	Color color(nR,nG,nB);
	GetView()->SetElementRect(nEnum,strName,color);	
}

//���x�����O�v�f�h��Ԃ�
void Labeling::PaintElement(int nEnum,int nR,int nG,int nB)
{
	GetView()->PaintElement(nEnum,nR,nG,nB);
}

//�_�z��擾
ArrayPoint* Labeling::GetAryPoint(CString &str)
{
	ArrayPoint *pRet = m_mapAryPointPtr[str];
	if( pRet == NULL ){ //�����Ƃ��͍쐬
		pRet = new ArrayPoint;
		m_mapAryPointPtr[str] = pRet;
	}
	return pRet;
} 
ArrayPoint* Labeling::GetAryPoint(int n)
{
	CString str; //Key������쐬
	str.Format(L"%d",n); 
	return GetAryPoint(str);
}

//Type��Border���o
void Labeling::SelectBorder(int nEnum,CString strType,int nW)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){
		CString strName;
		strName.Format(L"%s%d",strType,nEnum);
		ArrayPoint *pAryCpy = GetAryPoint(strName); //�ꕔ�݂̗̂̈�擾
		pAryCpy->Copy(*pAryPoint); //�S�̃R�s�[
		if( strType == "LEFT" ){			
			CPoint pt = pAryPoint->GetLeft(); //���[�擾
			pAryCpy->RangeX(pt.x,pt.x+nW); //���[�̂ݒ��o
			pAryCpy->SortY();
		}else if(strType == "RIGHT"){
			CPoint pt = pAryPoint->GetRight(); //�E�[�擾
			pAryCpy->RangeX(pt.x-nW,pt.x); //�E�[�̂ݒ��o
			pAryCpy->SortY();
		}else if(strType == "TOP"){
			CPoint pt = pAryPoint->GetTop();
			pAryCpy->RangeY(pt.y,pt.y+nW); //��[�̂ݒ��o
			pAryCpy->SortX();
		}else if(strType == "BOTTOM"){
			CPoint pt = pAryPoint->GetBottom(); //���[�擾
			pAryCpy->RangeX(pt.y-nW,pt.y); //���[�̂ݒ��o
			pAryCpy->SortX();
		}else if(strType == "ALL"){
			pAryCpy->RangeX(0,nW); //�v���X�̈�̂ݒ��o
		}
	}
}

//�v�f���w��
void Labeling::SetWidthRange(int nWmin, int nWmax)
{
	m_label.SetWmin(nWmin);
	m_label.SetWmax(nWmax);
}
//�v�f�����w��
void Labeling::SetHeightRange(int nHmin, int nHmax)
{
	m_label.SetHmin(nHmin);
	m_label.SetHmax(nHmax);
}

//�w��Y���W�̍��[�_�����W
void Labeling::CollectPointXmin(int nEnum,ArrayPoint &aryPtXmin,int nYS,int nYE)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){
		int imax = (int)pAryPoint->GetSize();//�S���E����
		for(int y=nYS;y<=nYE;y++){//�Sy���W���Ƃ�x���W���擾
			int nXmin = INT_MAX;
			for(int i=0;i<imax;i++){//�S���E������
				if( pAryPoint->GetAt(i).y == y ){ //y���W����
					int nX = pAryPoint->GetAt(i).x;
					if( nX < nXmin ){
						nXmin = nX;
					}
				}
			}
			if( nXmin < INT_MAX ){
				aryPtXmin.AddPoint(nXmin,y); //y���W�o�^			
			}
		}
	}
}

//�w��Y���W�̉E�[�_�����W
void Labeling::CollectPointXmax(int nEnum,ArrayPoint &aryPtXmax,int nYS,int nYE)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){		
		int imax = (int)pAryPoint->GetSize();//�S���E����
		for(int y=nYS;y<=nYE;y++){//�Sy���W���Ƃ�x���W���擾
			int nXmax = -1;
			for(int i=0;i<imax;i++){//�S���E������
				if( pAryPoint->GetAt(i).y == y ){ //y���W����
					int nX = pAryPoint->GetAt(i).x;
					if( nXmax < nX ){
						nXmax = nX;
					}
				}
			}
			if( nXmax > 0 ){
				aryPtXmax.AddPoint(nXmax,y); //y���W�o�^			
			}
		}
	}
}

//BITMAP�N���X�ŏ�����
BOOL Labeling::SetBitmap(int nTh)
{
	Bitmap24 *pBitmap = Bitmap24::GetInstance();
	if( pBitmap != NULL ){
		return Init(pBitmap->GetDib(),nTh);
	}
	return FALSE;
}

//�d�Sx���W
double Labeling::Gx(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetGx(); 
	}
	return 0;	
}
//�d�Sy���W
double Labeling::Gy(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetGy(); 
	}
	return 0;		
}
//��f��
int Labeling::Pixels(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetPixels();
	}
	return 0;			
}
//�ŏ������W	
int Labeling::Xmin(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetMinX();
	}
	return 0;			
}
//�ŏ������W
int Labeling::Ymin(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetMinY();
	}
	return 0;			
}
//�ő傘���W
int Labeling::Xmax(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetMaxX();
	}
	return 0;			
}
//�ő備���W
int Labeling::Ymax(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //�ő�v�f�擾
	if( pElement != NULL ){
		return pElement->GetMaxY();
	}
	return 0;			
}
//�ő�v�f�ԍ��擾
int Labeling::GetMaxElement()
{
	int nEnum = - 1; //�ő�v�f�ԍ�
	m_label.GetMaxElement(&nEnum); //�ő�v�f�ԍ��Z�b�g
	return nEnum;
}