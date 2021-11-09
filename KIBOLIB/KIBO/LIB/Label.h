#pragma once

#pragma warning(disable : 4995)

#include "dib.h"
#include "dib256.h"
#include "CLabeling.h"
#include "ArrayPoint.h"
#include "typedef.h"
#include "ArrayPoint.h"

enum BORDER
{
     BORDER_RIGHT= 0	//�E�@�𒲍�
	,BORDER_RIGHTUP		//�E��𒲍�
	,BORDER_UP			//��@�𒲍�
	,BORDER_LEFTUP		//����𒲍�
	,BORDER_LEFT		//���@�𒲍�
	,BORDER_LEFTDOWN	//�����𒲍�
	,BORDER_DOWN		//���𒲍�
	,BORDER_RIGHTDOWN	//�E���𒲍�
};

//�d�˂��킹�p�摜�f�[�^
class CheckImage
{
public:
	TCHAR m_Char;		//���𕶎�
	Dib256 *m_pDib256; //�摜�f�[�^
public:
	CheckImage(){//�R���X�g���N�^
		m_Char = 0;			//���𕶎�
		m_pDib256 = NULL;	//�摜�f�[�^
	}
	~CheckImage(){} //�f�X�g���N�^
	CheckImage& operator=(const CheckImage&t)
	{
		m_Char = t.m_Char;
		m_pDib256 = t.m_pDib256;
		return *this;
	}
};

//��r�p�摜�f�[�^
class ArrayCheckImage : public CArray<CheckImage> 
{
public:
	ArrayCheckImage(){}
	~ArrayCheckImage(){
		for(int i=0;i<GetSize();i++){
			delete GetAt(i).m_pDib256;
		}
	}

	void AddData(TCHAR t,Dib256& dib256){
		CheckImage check;
		check.m_Char = t;
		check.m_pDib256 = new Dib256(); 
		if( check.m_pDib256 != NULL ){
			check.m_pDib256->Copy( dib256 ); //�摜�f�[�^�R�s�[
			this->Add( check ); //�ǉ�
		}
	}
};

class Label : public CLabeling
{
private:
    int		*m_pSrcBuf;	//���x�����O�\�[�X
    BYTE	*m_pDstBuf;	//���x�����O����
    int		*m_pTmpBuf; //���x�����O�\�[�X�e���|�����o�b�t�@
	int		m_nW;		//���x�����O��
	int		m_nH;		//���x�����O����
	int		m_nBufSize; //�o�b�t�@�T�C�Y
public:
	static ArrayCheckImage m_aryImageCheck; //��r�p�摜
public:
	Label(void);
	~Label(void);

	//�\�[�g�^�C�v
	//���x�����O���ʃ\�[�g
	enum{
		 SORT_MIN_X = 0	//x���W��������
		,SORT_MAX_X		//x���W�傫����
		,SORT_MIN_Y		//y���W��������
		,SORT_MAX_Y		//y���W�傫����
		,SORT_GX		//�d�Sx���W��
		,SORT_GY		//�d�Sy���W��
		,SORT_PIXELS	//�ʐϏ�
		,SORT_MAX
	};

	int*	GetSrcBuf(){return m_pSrcBuf;} //���x�����O�\�[�X
	BYTE*	GetDstBuf(){return m_pDstBuf;} //���x�����O����
	int*	GetTmpBuf(){return m_pTmpBuf;} //�e���|�����o�b�t�@ �i�\�[�X�o�b�t�@�T�C�Y�j
	void ClearTmpBuf(){ZeroMemory(m_pTmpBuf,GetSize()*sizeof(int));} //���x�����O�\�[�X�o�b�t�@�N���A

	void WriteSrcBuf(LPCWSTR lpPath); //���x�����OSrc�o�b�t�@���r�b�g�}�b�v�Ƃ��ď����o��
	void WriteDstBuf(LPCWSTR lpPath); //�o�b�t�@���r�b�g�}�b�v�Ƃ��ď����o��

	int GetWidth() {return m_nW;} //���x�����O���擾
	int GetHeight(){return m_nH;} //���x�����O�����擾
	int GetSize(){return m_nW * m_nH;} //���x�����O�T�C�Y�擾
	void Free(); //���x�����O�o�b�t�@���
	BOOL Init(int nW,int nH);	//���x�����O������
	void Mask(BYTE byVal,CRect &rect,BOOL isPaint); //�}�X�N�̈�̃\�[�X�o�b�t�@���N���A
	int	Exec(int nMinSize,int nMaxSize);	//���x�����O���s
	int Exec(int nMinSize,int nMaxSize,CRect &rect);//���x�����O�̈�w��	
	int ElementExec(int nEnum,int nMinSize,int nMaxSize);//���x�����O�̈�w��
	void Sort(int nSnum,BOOL isAscending); //���x�����O���ʃ\�[�g
	BOOL CreateSrcBuf(CDIB &dib,BYTE byTh);	//�r�b�g�}�b�v����̃��x�����O�\�[�X�o�b�t�@�̍쐬
	BOOL CreateSrcBuf(Dib256 &dib,BYTE byTh);

	void InvertSrcBuf();//���x�����O�\�[�X�o�b�t�@�̔������]
	void EraseDot();//�h�b�g�����폜

	BOOL FillDstBuf(Element *pElem,BYTE byVal); //���x�����O���ʃo�b�t�@�̍쐬
	BOOL FillDstBuf(int nEnum,BYTE byVal); //���x�����O���ʃo�b�t�@�̍쐬
	BOOL FillBitmapSrc(CDIB &dib); //���x�����OSrc�摜�����r�b�g�}�b�v�֓]��
	BOOL FillBitmapDst(CDIB &dib); //���x�����O���ʉ摜���r�b�g�}�b�v�֓]��
	BOOL FillBitmapDst(Dib256 &dib);
	//���x�����O���ʂ̎擾(Point)
	void FillFramePoint(Element *pElem,ArrayPoint &aryPoint);

	void ContractSrc();	//�k��
	void ExpandSrc();	//�g��
	void ContractDst();	//�k��
	void ExpandDst();	//�g��
	int  CountDst(BYTE byVal); //�w��l�̃J�E���g

	void ExpandSrcX(int nNum);//�w���X�����̂݊g�� Src
	void ExpandDstX(int nNum);//�w���x�����̂݊g�� Dst

	int FindBorder(int nEnum,ArrayPoint &aryPoint);//���E���ǐ�(8-�A��)
	int FindBorder(Element *pElem,ArrayPoint &aryPoint);
	int FindElement(CRect &rect,AryInt &aryElement); //�̈���v�f���W

	void FillElement(CDIB &dib,BYTE byVal);//Element�����w��l�œh��Ԃ�

	//���x�����O���ʂ̕`��
	void PaintElementColor(Element *pElem,CDIB &dib,BYTE byR,BYTE byG,BYTE byB);
	void PaintElementBitmap(Element *pElem, CDIB &dibDst, CDIB &dibSrc);

	//���x�����O�p�̃o�b�t�@���쐬
	BOOL CreateBuffer(int nSize);

	int LoadCheckImage(CString& strName); //��r�p�̉摜��ǂݍ���
	int MatchingImage(Dib256 &dib,int nTh,double &dbPer);//�}�b�`���O����

private:
	int ChaseBorder(ArrayPoint &aryPoint,int x,int y,BORDER nDirection,BYTE *pBin,BYTE byBnum);//���E���ǐ�(8-�A��)
};
