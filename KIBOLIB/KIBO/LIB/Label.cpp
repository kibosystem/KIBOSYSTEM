#include "../stdafx.h"
#include "../LIB/typedef.h"
#include "label.h"
#include "macro.h"

ArrayCheckImage Label::m_aryImageCheck; //��r�p�摜

Label::Label()
{
	m_nBufSize = 0;	//�o�b�t�@�T�C�Y
	m_nW = 0;		//���x�����O��
	m_nH = 0;		//���x�����O����
    m_pSrcBuf = NULL;	//���x�����O�\�[�X
    m_pDstBuf = NULL;	//���x�����O����
	m_pTmpBuf = NULL;	//���x�����O�\�[�X�e���|�����o�b�t�@
}

Label::~Label(void)
{
	Free();
}
//���x�����O�o�b�t�@���
void Label::Free()
{
	if( m_pSrcBuf != NULL ){
		delete m_pSrcBuf;
		m_pSrcBuf = NULL;
	}
	if( m_pDstBuf != NULL ){
		delete m_pDstBuf;
		m_pDstBuf = NULL;
	}
	if( m_pTmpBuf != NULL ){
		delete m_pTmpBuf;
		m_pTmpBuf = NULL;
	}
}
//���x�����O�p�̃o�b�t�@���쐬
BOOL Label::CreateBuffer(int nSize)
{
	//�o�b�t�@�̍쐬
	if( m_nBufSize < nSize ){ //�ȑO�쐬�����o�b�t�@�����傫�ȃT�C�Y���K�v�Ȏ�����
		Free();
		m_pSrcBuf = new int[ nSize ];
		if( m_pSrcBuf == NULL){
			return FALSE;
		}
		m_pDstBuf = new BYTE[ nSize ];
		if(m_pDstBuf == NULL){
			return FALSE;
		}
		m_pTmpBuf = new int[ nSize ];
		if(m_pTmpBuf == NULL){
			return FALSE;
		}
		m_nBufSize = nSize; //�T�C�Y�ۑ�
	}
	return TRUE;
}
//���x�����O������
BOOL Label::Init(int nW,int nH)
{
	m_nW = nW; //���x�����O�T�C�Y�o�^
	m_nH = nH;
	int nSize = nW*nH; //�g�p�͈̓N���A
	if( CreateBuffer(nSize) ){ //�K�v�Ȏ������쐬
		ZeroMemory(m_pSrcBuf,nSize*sizeof(int));
		ZeroMemory(m_pDstBuf,nSize);
		ZeroMemory(m_pTmpBuf,nSize*sizeof(int));
		return TRUE;
	}
	return FALSE;
}
//�}�X�N�̈�̃\�[�X�o�b�t�@���N���A
void Label::Mask(BYTE byVal,CRect &rect,BOOL isPaint/*�h��Ԃ�*/)
{
	if( m_pSrcBuf != NULL ){ //�\�[�X�o�b�t�@���`�F�b�N
		int nW = GetWidth();
		int nH = GetHeight();

		int nLeft  = mid(0,rect.left  ,nW);
		int nRight = mid(0,rect.right ,nW);
		int nBtm   = mid(0,nH-rect.top-1,nH-1);
		int nTop   = mid(0,nH-rect.bottom-1,nH-1);
		int nLine = nRight - nLeft; //1���C���T�C�Y		
		if( isPaint ){ //�h��Ԃ�����
			for(int y=nTop;y<=nBtm;y++){
				int *pData = m_pSrcBuf + (nH-y-1) * nW + nLeft;  //�����A�h���X
				memset(pData,byVal,nLine * sizeof(int)); //�PLine Clear
			}
		}else{ //�h��Ԃ�����
			int *pData = m_pSrcBuf + (nH-nTop-1) * nW + nLeft;  //�����A�h���X
			memset(pData,byVal,nLine * sizeof(int)); //�PLine Clear
			for(int y=nTop+1;y<nBtm;y++){
				*(m_pSrcBuf + (nH-y-1) * nW + nLeft ) = byVal; //Left  Clear
				*(m_pSrcBuf + (nH-y-1) * nW + nRight) = byVal; //Right Clear
			}
			pData = m_pSrcBuf + (nH-nBtm-1) * nW + nLeft;  //�����A�h���X
			memset(pData,byVal,nLine * sizeof(int)); //�PLine Clear
		}
	}
}
//���x�����O�S�̈�
int Label::Exec(int nMinSize,int nMaxSize)
{
	ZeroMemory(m_pDstBuf,GetSize());
	int nW = GetWidth();
	int nH = GetHeight();
	CRect rect(0,0,nW,nH);
	return CLabeling::Exec(m_pSrcBuf,nW,nH,nMinSize,nMaxSize,rect);
}

//���x�����O�̈�w��
int Label::Exec(int nMinSize,int nMaxSize,CRect &rect)
{
	ZeroMemory(m_pDstBuf,GetSize());
	int nW = GetWidth();
	int nH = GetHeight();
	CRect rcMax(0,0,nW,nH);
	CRect rcNorm; //���K��������`
	if( rcNorm.IntersectRect(rect,rcMax) ){
		return CLabeling::Exec(m_pSrcBuf,nW,nH,nMinSize,nMaxSize,rcNorm);
	}
	return 0;
}

//���x�����O�̈�w��
int Label::ElementExec(int nEnum,int nMinSize,int nMaxSize)
{
	Element *pElem = GetElement(nEnum);
	if( pElem != NULL ){
		CRect rect = pElem->GetRect(); //�v�f�S�̗̂̈�擾
		int nEmax = Exec(nMinSize,nMaxSize,rect);
		if(nEmax > 0){
			rect.DeflateRect(1,1); //�k�߂�
			//���݂̗v�f�����ׂđޔ�
			RIPVector vRip;
			vRip = m_result_region_info;

			//���x�����O�v�f�N���A
			m_result_region_info.clear();
			m_aryRip.Clear();
			m_nEmax = 0;
			
			for(int i=0,imax=(int)vRip.size();i<imax;i++){
				Element *pElem = m_aryRip.GetBack();
				pElem->Copy( vRip[i] ); //Src -> Dst
				CRect rcElem = pElem->GetRect();//�v�f���v�f�̗̈�擾
				if( rect.left < rcElem.left && rcElem.right < rect.right && rect.top < rcElem.top && rcElem.bottom < rect.bottom ){//�v�f�Ƃ��Ďc��
					m_result_region_info.push_back( pElem );
					m_nEmax++;//���x�����O�v�f�𑝂₷
				}else{//�v�f�Ƃ��Ďc���Ȃ�
					m_aryRip.Back();
				}
			}
			return GetEmax();
		}
	}
	return 0;
}

//���ёւ��N���X
class AscendMinX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinX() < pRight->GetMinX());
    }
};
class DescendMinX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinX() > pRight->GetMinX());
    }
};
class AscendMinY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinY() < pRight->GetMinY());
    }
};
class DescendMinY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinY() > pRight->GetMinY());
    }
};
class AscendMaxX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxX() < pRight->GetMaxX());
    }
};
class DescendMaxX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxX() > pRight->GetMaxX());
    }
};
class AscendMaxY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxY() < pRight->GetMaxY());
    }
};
class DescendMaxY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxY() > pRight->GetMaxY());
    }
};
class AscendGX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGx() < pRight->GetGx());
    }
};
class DescendGX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGx() > pRight->GetGx());
    }
};
class AscendGY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGy() < pRight->GetGy());
    }
};
class DescendGY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGy() > pRight->GetGy());
    }
};
class AscendPixels
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetPixels() < pRight->GetPixels());
    }
};
class DescendPixels
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetPixels() > pRight->GetPixels());
    }
};


//���x�����O���ʃ\�[�g
void Label::Sort(int nSnum,BOOL isAscending/*=true*/) 
{
	switch(nSnum){
		case SORT_MIN_X:	 //x���W��	
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMinX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMinX());
			}
			break;
		case SORT_MAX_X: //x���W��
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMaxX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMaxX());
			}
			break;
		case SORT_MIN_Y: //y���W��
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMinY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMinY());
			}
			break;
		case SORT_MAX_Y: //y���W��
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMaxY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMaxY());
			}
			break;
		case SORT_GX: //�d�Sx���W��
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendGX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendGX());
			}
			break;
		case SORT_GY: //�d�Sy���W��
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendGY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendGY());
			}
			break;
		case SORT_PIXELS: //�ʐϏ�
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendPixels());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendPixels());
			}
			break;
		default: //�s���Ȉ���
			return ;
	}
}

//���x�����O�\�[�X�o�b�t�@�̍쐬
//CDIB &dib:GRAYImage
BOOL Label::CreateSrcBuf(CDIB &dib,BYTE byTh)
{
	int *pSrcBuf = GetSrcBuf();
	BYTE *pDibBits = dib.GetBits(); //�擪�A�h���X�擾
	if(pSrcBuf != NULL && pDibBits != NULL){
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();
		int nLine = dib.GetLineSize();
		//Src Buf �쐬
		for(int y=0;y<nH;y++){
			BYTE *pBits = pDibBits + (nH-1-y)*nLine; //�㉺���]
			for(int x=0;x<nW;x++){
				BYTE byB = *pBits++;//B
				BYTE byG = *pBits++;//G
				BYTE byR = *pBits++;//R
				BYTE bySet = 0;
				if( (BYTE)(0.299*byR + 0.587*byG + 0.114*byB) >= byTh ){ //�t���J���[ -> 256�F
					bySet = 255; //�Q�l��
				}			
				*pSrcBuf++ = bySet;
			}
		}
		return TRUE;
	}
	return FALSE;
}
//���x�����O���ʃo�b�t�@���r�b�g�}�b�v�Ƃ��ď����o��
void Label::WriteDstBuf(LPCWSTR lpPath)
{
	int nW = GetWidth();
	int nH = GetHeight();
	CDIB dib;
	if( dib.Create(nW,nH) ){
		this->FillBitmapDst(dib);
		dib.WriteBitmap(lpPath);
	}
}
//���x�����OSrc�o�b�t�@���r�b�g�}�b�v�Ƃ��ď����o��
void Label::WriteSrcBuf(LPCWSTR lpPath)
{
	int nW = GetWidth();
	int nH = GetHeight();
	CDIB dib;
	if( dib.Create(nW,nH) ){
		this->FillBitmapSrc(dib);
		dib.WriteBitmap(lpPath);
	}
}

BOOL Label::CreateSrcBuf(Dib256 &dib,BYTE byTh)
{
	int *pSrcBuf = GetSrcBuf();
	BYTE *pDibBits = dib.GetBits(); //�擪�A�h���X�擾
	if(pSrcBuf != NULL && pDibBits != NULL){
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();
		int nLine = dib.GetLineSize();
		//Src Buf �쐬
		for(int y=0;y<nH;y++){
			BYTE *pBits = pDibBits + (nH-1-y)*nLine; //�㉺���]
			for(int x=0;x<nW;x++){
				*pSrcBuf++ = ( *pBits++ >= byTh ) ? MAXBYTE : 0;
			}
		}
		return TRUE;
	}
	return FALSE;
}

//���x�����O�\�[�X�o�b�t�@�̔������]
void Label::InvertSrcBuf()
{
	int *pSrcBuf = GetSrcBuf();
	for(int y=0,ymax=GetHeight();y<ymax;y++){
		for(int x=0,xmax=GetWidth();x<xmax;x++){
			*pSrcBuf++ = (*pSrcBuf) ? 0 : 255;
		}
	}	
}
//�h�b�g�����폜
void Label::EraseDot() 
{
	int *pSrcBuf = GetSrcBuf();
	int nH = GetHeight();
	int nW = GetWidth();
	pSrcBuf += nW;//y==0
	for(int y=1,ymax=nH-1;y<ymax;y++){
		pSrcBuf++; //x==0
		for(int x=1,xmax=nW-1;x<xmax;x++){
			if( *pSrcBuf == 0 && *(pSrcBuf+1) && *(pSrcBuf-1) && *(pSrcBuf+nW) && *(pSrcBuf-nW)){
				*pSrcBuf = 255; //�h�b�g����
			}
			pSrcBuf++;
		}
		pSrcBuf++; //x==nW-1
	}	
}

//���x�����O���ʃo�b�t�@�̍쐬
BOOL Label::FillDstBuf(Element *pElem,BYTE byVal)
{
	if( pElem != NULL ){		
		FillFrameByte(m_pDstBuf,pElem,byVal);
		return TRUE;
	}
	return FALSE;
}
BOOL Label::FillDstBuf(int nEnum,BYTE byVal)
{
	Element *pElem = GetElement(nEnum);
	if( pElem != NULL ){		
		FillFrameByte(m_pDstBuf,pElem,byVal);
		return TRUE;
	}
	return FALSE;
}

//���x�����OSrc�摜���r�b�g�}�b�v�֓]��
//�r�b�g�}�b�v�͕K�����x�����O�T�C�Y�ƈ�v���邱��
BOOL Label::FillBitmapSrc(CDIB &dib) 
{
	BYTE *pBits = dib.GetBits();
	int *pData = m_pSrcBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //�p�f�B���O�T�C�Y
	if( GetWidth() != nW || GetHeight() != nH ){ //�T�C�Y�`�F�b�N
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		for(int x=0;x<nW;x++){
			*pDst++	  = *pData;//B
			*pDst++ = *pData;//G
			*pDst++ = *pData++;//R
		}
	}	
	return TRUE;
}
//���x�����O���ʉ摜���r�b�g�}�b�v�֓]��
//�r�b�g�}�b�v�͕K�����x�����O�T�C�Y�ƈ�v���邱��
BOOL Label::FillBitmapDst(CDIB &dib) 
{
	BYTE *pBits = dib.GetBits();
	BYTE *pData = m_pDstBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //�p�f�B���O�T�C�Y
	if( GetWidth() != nW || GetHeight() != nH ){ //�T�C�Y�`�F�b�N
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		for(int x=0;x<nW;x++){
			*pDst++	= *pData;//B
			*pDst++ = *pData;//G
			*pDst++ = *pData++;//R
		}
	}	
	return TRUE;
}

//���x�����O���ʉ摜���r�b�g�}�b�v�֓]��
//�r�b�g�}�b�v�͕K�����x�����O�T�C�Y�ƈ�v���邱��
BOOL Label::FillBitmapDst(Dib256 &dib) 
{
	BYTE *pBits = dib.GetBits();
	BYTE *pData = m_pDstBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //�p�f�B���O�T�C�Y
	if( GetWidth() != nW || GetHeight() != nH ){ //�T�C�Y�`�F�b�N
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		memcpy(pDst,pData,nW);
		pData+=nW;
	}	
	return TRUE;
}

//���x�����O���ʂ̎擾(Point)
void Label::FillFramePoint(Element *pElem,ArrayPoint &aryPoint)
{
	aryPoint.RemoveAll();
	RSPList& rspl = pElem->GetRasterSegmentList();
	for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//�S�Z�O�����g���X�g����
		RasterSegment *pRs = *rspi;
		int	lx = pRs->GetLeftX();
		int	rx = pRs->GetRightX();
		int	y = pRs->GetY();
		for(int x=lx;x<=rx;x++){
			aryPoint.AddPoint(x,y);
		}
	}
}

//�k��
void Label::ContractSrc()
{
	int *pOut = GetTmpBuf();
	int *pIn  = GetSrcBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	ZeroMemory(pOut,nW*nH*sizeof(int)); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( *pIn ){ //�l�����݂���
				if( *(pIn-1) && *(pIn+1)
					&& *(pIn-nW  ) && *(pIn+nW)
					&& *(pIn-nW-1) && *(pIn+nW-1) 
					&& *(pIn-nW+1) && *(pIn+nW+1) )
				{
					*pOut = *pIn;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //�k����Ԃ�Copy
}
void Label::ContractDst()
{
	BYTE *pOut = (BYTE *)GetTmpBuf();
	BYTE *pIn  = GetDstBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	ZeroMemory(pOut,nW*nH); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( *pIn ){ //�l�����݂���
				if( *(pIn-1) && *(pIn+1)
					&& *(pIn-nW  ) && *(pIn+nW)
					&& *(pIn-nW-1) && *(pIn+nW-1) 
					&& *(pIn-nW+1) && *(pIn+nW+1) )
				{
					*pOut = *pIn;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //�k����Ԃ�Copy
}

//�g��
void Label::ExpandSrc()
{
	int *pOut = GetTmpBuf();
	int *pIn  = GetSrcBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	CopyMemory(pOut,pIn,nW*nH*sizeof(int)); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( !*pIn ){ //�l�����݂��Ȃ�
				if( *(pIn-1) || *(pIn+1)
					|| *(pIn-nW  ) || *(pIn+nW)
					|| *(pIn-nW-1) || *(pIn+nW-1) 
					|| *(pIn-nW+1) || *(pIn+nW+1) )
				{
					*pOut = 255;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //�g���Ԃ�Copy
}

//X�����̂݊g��
void Label::ExpandSrcX(int nNum)
{
	for(int i=0;i<nNum;i++){
		int *pOut = GetTmpBuf();
		int *pIn  = GetSrcBuf();
		int nW = GetWidth();
		int nH = GetHeight();

		CopyMemory(pOut,pIn,nW*nH*sizeof(int)); //out is clear
		pOut += nW;// y==0
		pIn  += nW;
		for(int y=1;y<nH-1;y++){
			pIn++;
			pOut++; //x==0
			for(int x=1;x<nW-1;x++){
				if( !*pIn ){ //�l�����݂��Ȃ�
					if( *(pIn-1) || *(pIn+1) ){
						*pOut = 255;
					}
				}
				pIn++;
				pOut++;
			}
			pIn++;
			pOut++; //x==nW-1
		}
		CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //�g���Ԃ�Copy
	}
}

void Label::ExpandDst()
{
	BYTE *pOut = (BYTE *)GetTmpBuf(); //�\�[�X�p�e���|�����o�b�t�@
	BYTE *pIn  = GetDstBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	CopyMemory(pOut,pIn,nW*nH); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( !*pIn ){ //�l�����݂��Ȃ�
				if( *(pIn-1) || *(pIn+1)
				|| *(pIn-nW  ) || *(pIn+nW)
				|| *(pIn-nW-1) || *(pIn+nW-1) 
				|| *(pIn-nW+1) || *(pIn+nW+1) )
				{
					*pOut = 255;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //�g���Ԃ�Copy
}

//X�����݂̂̊g��
void Label::ExpandDstX(int nNum)
{
	for(int i=0;i<nNum;i++){
		BYTE *pOut = (BYTE *)GetTmpBuf(); //�\�[�X�p�e���|�����o�b�t�@
		BYTE *pIn  = GetDstBuf();
		int nW = GetWidth();
		int nH = GetHeight();

		CopyMemory(pOut,pIn,nW*nH); //out is clear
		pOut += nW;// y==0
		pIn  += nW;
		for(int y=1;y<nH-1;y++){
			pIn++;
			pOut++; //x==0
			for(int x=1;x<nW-1;x++){
				if( !*pIn ){ //�l�����݂��Ȃ�
					if( *(pIn-1) || *(pIn+1) )
					{
						*pOut = 255;
					}
				}
				pIn++;
				pOut++;
			}
			pIn++;
			pOut++; //x==nW-1
		}
		CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //�g���Ԃ�Copy
	}
}

//���E���ǐ�(8-�A��)
int Label::FindBorder(int nEnum,ArrayPoint &aryPoint)
{
	Element *pElem = GetElement(nEnum);
	return FindBorder(pElem,aryPoint);
}
int Label::FindBorder(Element *pElem,ArrayPoint &aryPoint)
{
	aryPoint.RemoveAll();
	int nW = GetWidth();
	int nH = GetHeight();
	FillDstBuf(pElem,MAXBYTE); //���x�����O���ʂ�255
	BYTE byBnum = 1; //���E���ԍ�
	for(int y=1;y<nH-1;y++){
		BYTE *pBuf = GetDstBuf() + y*nW + 1;
		for(int x=1;x<nW-1;x++){
			if( *pBuf == MAXBYTE ){ //���x�����O���ʂ̎�
				if( *(pBuf-1) == 0 ){ //���������E
					ChaseBorder(aryPoint,x,y,BORDER_LEFTDOWN,pBuf,byBnum); //�O�����E�T��
					byBnum++;
				}else if(*(pBuf+1) == 0 ){ //�E�������E
					ChaseBorder(aryPoint,x,y,BORDER_RIGHTUP,pBuf,byBnum); //�E�����E�T��
					byBnum++;
				}
			}
			pBuf++;
		}
	}
	return (byBnum-1);
}

//���E���ǐ�(8-�A��)
int Label::ChaseBorder(ArrayPoint &aryPoint,int x,int y,BORDER nDirection,BYTE *pBin,BYTE byBnum)
{
	int nW = GetWidth();	//���x�����O���擾
	int nH = GetHeight();//���x�����O�����擾

	int xStart = x; //�n�_
	int yStart = y;

	int xNow = x; //���݈ʒu
	int yNow = y;

	int xCheck = -1;//�T���ʒu
	int yCheck = -1; 

	int nCnt = 0;
	while(xCheck != xStart || yCheck != yStart){ //�n�_�ɖ߂�܂�
		BOOL isMove = FALSE; //�ړ��͂���܂������H
		switch(nDirection){
            case BORDER_RIGHT: //�E�𒲍�				
				xCheck = xNow + 1; //�E���`�F�b�N
				yCheck = yNow;
				if( xNow+1 < nW && *(pBin+1) ){ //�E��
					pBin	+= 1;
					nDirection = BORDER_RIGHTDOWN;//�E����
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHTUP; //�E��`�F�b�N
				}
				break;
			case BORDER_RIGHTUP:		//�E��𒲍�
				xCheck = xNow + 1; //�E���`�F�b�N
				yCheck = yNow - 1;
				if( xNow+1 < nW && yNow-1>=0 && *(pBin-nW+1) ){ //�E���
					pBin	+= (-nW+1);
					nDirection = BORDER_RIGHTDOWN;//�E����
					isMove = TRUE;
				}else{
					nDirection = BORDER_UP; //��`�F�b�N
				}
				break;
            case BORDER_UP: //��𒲍�
				xCheck = xNow;
				yCheck = yNow - 1;
				if( yNow-1>=0 && *(pBin-nW) ){ //���
					pBin += -nW;
					nDirection = BORDER_RIGHTUP;
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFTUP;
				}
				break;
			case BORDER_LEFTUP:		//����𒲍�
				xCheck = xNow - 1;
				yCheck = yNow - 1;
				if( xNow-1>=0 && yNow-1>=0 && *(pBin-nW-1) ){ //���
					pBin += -nW-1;
					nDirection = BORDER_RIGHTUP;
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFT;
				}
				break;
            case BORDER_LEFT: //���𒲍�
				xCheck = xNow - 1;
				yCheck = yNow;
				if( xNow-1>=0 && *(pBin-1) ){
					pBin += -1;
					nDirection = BORDER_LEFTUP;	                    
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFTDOWN;
				}
				break;
			case BORDER_LEFTDOWN:	//�����𒲍�
				xCheck = xNow - 1;
				yCheck = yNow + 1;
				if( xNow-1>=0 && yNow+1<nH && *(pBin+nW-1) ){
					pBin += nW-1;
					nDirection = BORDER_LEFTUP;	
					isMove = TRUE;
				}else{
					nDirection = BORDER_DOWN;
				}
				break;				
            case BORDER_DOWN: //���𒲍�				
				xCheck = xNow;		//�T���ʒu�͌��݂̉���
				yCheck = yNow + 1;
				if( yNow+1<nH && *(pBin+nW) ){ //����
					pBin += nW; //�A�h���X�ǉ�
					nDirection = BORDER_LEFTDOWN; //������
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHTDOWN; //�E����
				}
				break;
			case BORDER_RIGHTDOWN:	//�E���𒲍�
				xCheck = xNow + 1;
				yCheck = yNow + 1;
				if( xNow+1<nW && yNow+1<nH && *(pBin+nW+1) ){ //�E����
					pBin += nW+1; //�A�h���X�ǉ�
					nDirection = BORDER_LEFTDOWN; //������
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHT; //�E��
				}
				break;
			default:
				ASSERT(FALSE);
				break;
		}
		if( isMove ){//�ړ����������Ƃ�����
			*pBin = byBnum; //���E���ԍ��Z�b�g
			aryPoint.AddPoint(xNow,yNow); //���E�_�o�^
			//TRACE("x,y=%d,%d\n",xNow,yNow);
			xNow = xCheck;	//���݈ʒu�̍X�V
			yNow = yCheck; 
			nCnt++;
		}		
	}
	aryPoint.AddPoint(-1,-1); //�I�_�o�^
	return nCnt;
}

//�̈���v�f���W
int Label::FindElement(CRect &rect,AryInt &aryElement)
{
	aryElement.RemoveAll();
	int nEmax = GetEmax();
	CRect rcElem;
	for(int i=0;i<nEmax;i++){
		Element *pElem = GetElement(i);
		rcElem.left  = pElem->GetMinX();
		rcElem.right = pElem->GetMaxX();
		rcElem.top   = pElem->GetMinY();
		rcElem.bottom= pElem->GetMaxY();
		//���S�Ɋ܂܂��
		if( rect.left <= rcElem.left && rect.top <= rcElem.top && rcElem.right <= rect.right && rcElem.bottom <=rect.bottom){
			aryElement.Add( i ); //�v�f�ԍ����Z�b�g
		}
	}
	return (int)aryElement.GetSize();
}

//�w��l�̃J�E���g
int Label::CountDst(BYTE byVal) 
{
	int nCnt = 0;
	int nSize = GetSize();
	BYTE *pDst = GetDstBuf();	
	for(int i=0;i<nSize;i++){
		if( *pDst++ == byVal ){
			nCnt++;
		}
	}	
	return nCnt;
}

//���x�����O���ʂ̕`��
void Label::PaintElementColor(Element *pElem,CDIB &dib,BYTE byR,BYTE byG,BYTE byB)
{
	BYTE *pTop = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	if(GetWidth() == nW && GetHeight() == nH){
		int nLine = dib.GetLineSize();
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//�S�Z�O�����g���X�g����
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pBits = pTop + (nH-1-y)*nLine + lx*3; //�㉺���]
			for(int x=lx;x<=rx;x++){
				*pBits++ = byB; //B
				*pBits++ = byG; //G
				*pBits++ = byR; //R
			}
		}
	}
}
void Label::PaintElementBitmap(Element *pElem, CDIB &dibDst,CDIB &dibSrc)
{
	BYTE *pBitsD = dibDst.GetBits();
	BYTE *pBitsS = dibSrc.GetBits();
	int nW = dibDst.GetWidth();
	int nH = dibDst.GetHeight();
	if (GetWidth() == nW && GetHeight() == nH) {
		int nLine = dibDst.GetLineSize();
		RSPList& rspl = pElem->GetRasterSegmentList();
		for (RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++) {//�S�Z�O�����g���X�g����
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pDst = pBitsD + (nH - 1 - y)*nLine + lx * 3; //�㉺���]
			BYTE *pSrc = pBitsS + (nH - 1 - y)*nLine + lx * 3; //�㉺���]
			
			for (int x = lx; x <= rx; x++) {
				if (*pDst < *pSrc) {
					*pDst++ = *pSrc++; //B
					*pDst++ = *pSrc++; //G
					*pDst++ = *pSrc++; //R
				}
				else {
					pDst += 3;
					pSrc += 3;
				}
			}
		}
	}
}


//Element�����w��l�œh��Ԃ�
void Label::FillElement(CDIB &dib,BYTE byVal)
{
	BYTE *pBitsT = dib.GetBits();

	int nW = dib.GetWidth();//���擾
	int nH = dib.GetHeight(); //�����擾
	int nLine = dib.GetLineSize();
	int nSize = dib.GetSize();//�T�C�Y�擾
	int nMax = GetEmax();
	for(int i=0;i<nMax;i++){
		Element *pElem = GetElement(i);
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//�S�Z�O�����g���X�g����
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pSrc = pBitsT + (nH-y-1) * nLine + lx*3; 
			memset(pSrc,byVal,(rx-lx)*3);
		}		
	}	
}

//��r�p�̉摜��ǂݍ���
int Label::LoadCheckImage(CString& strName) 
{
	CFileFind find;

	BOOL isWorking = find.FindFile(strName);
	while(isWorking){
	   isWorking = find.FindNextFile();
		if( find.GetLength() > 0 ){//�t�@�C���T�C�Y�����݂���Ƃ�����
			Dib256 dib;
			if( dib.LoadFile( find.GetFilePath() ) ){
				CString strTitle = find.GetFileTitle();
				TRACE(L"%c\n",strTitle[0]);
				m_aryImageCheck.AddData(strTitle[0],dib); 							
			}
		}
	}
	return (int)m_aryImageCheck.GetSize(); 
}
//�}�b�`���O����
//�摜�f�[�^�̔�r
int Label::MatchingImage(Dib256 &dib,int nTh,double &dbPer)
{
	dib.ChangeBinary(nTh);
	BYTE *pTop = dib.GetBits();
	int nSize1 = dib.GetSize();
	int nCmax = 0; //�J�E���^�[ Max
	int nIndex = -1;
	for( int i=0;i<m_aryImageCheck.GetSize();i++){ //��r�p�摜�f�[�^		
		Dib256 *pDibData = m_aryImageCheck[i].m_pDib256;
		int nSize2 = pDibData->GetSize();
		int nSize = min(nSize1,nSize2);

		BYTE *pData = pDibData->GetBits();//��r�f�[�^
		BYTE *pSrc  = pTop; //�B�e�f�[�^
		int nGood = 0; //
		int nErr = 0;
		for(int n=0;n<nSize;n++){ //�S�f�[�^���[�v
			BYTE bySrc  = *pSrc++;
			BYTE byData = *pData++;

			if( bySrc == 0 && byData == 0 ){
				nGood++;
			}else if( bySrc == 0){
				nErr++;
			}else if( byData == 0){
				nErr++;
			}			
		}
		int nCnt = nGood - nErr;
		if( nCmax < nCnt ){ //�ő�l�ύX
			nCmax = nCnt;
			nIndex = i;
		}
	}
	int nCount = dib.CountPixels(nTh);
	dbPer = 100.0 * nCmax / nCount; //%
	return nIndex;
}
