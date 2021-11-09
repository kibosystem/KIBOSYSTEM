// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

#include "Databuf.h"
#include "gdiplus.h"

#include "../OpencvInc.h"

//CDIB�͒��ڏ������݉\�ȃf�o�C�X�Ɨ��̃r�b�g�}�b�v�iDIB�j���쐬����N���X�ł��B

class CDIB  
{
	//�������o
private:

	//�r�b�g�}�b�v�T�C�Y
	int  m_nBitsSize;		

	//�r�b�g�}�b�v�w�b�_���
	BITMAPINFOHEADER m_BMI; 

	//�r�b�g�}�b�v�f�[�^�𒼐ڑ��삷�邽�߂̃|�C���^
	BYTE *m_pBits;			

	//�쐬�����r�b�g�}�b�v�n���h��
	HBITMAP m_hBitmap;	

	CString m_strFilePath; //�r�b�g�}�b�v�t�@�C���p�X

	DataBuf m_data; //�摜�����p�e���|�����o�b�t�@
public:
	CDIB();				//�R���X�g���N�^
	virtual ~CDIB();	//�f�X�g���N�^

	enum{ //�摜�ϊ��^�C�v
		 TYPE_COLOR = 0 //RGB				NORMAL
		,TYPE_GRAY  //256�FGray				GRAY
		,TYPE_RGRAY //R�l�݂̂�Gray�\��		RGRAY
		,TYPE_GGRAY //G�l�݂̂�Gray�\��		GGRAY
		,TYPE_BGRAY //B�l�݂̂�Gray�\��		BGRAY
		,TYPE_R	//R�l�̂�					R
		,TYPE_G	//G�l�̂�					G		
		,TYPE_B	//B�l�̂�					B
		,TYPE_Y //�P�x						Y
		,TYPE_H //�F��						H
		,TYPE_S //�ʓx						S
		,TYPE_YHS //RGB->YSH				YHS
		,TYPE_BIN //Binary Mode				2
		,TYPE_ACCENT_R	//R����
		,TYPE_MEDIAN //���f�B�A��
		,TYPE_NUM
	};

	CString& GetFilePath(){return m_strFilePath;} //�r�b�g�}�b�v�t�@�C���p�X�擾
	void SetFilePath(CString& str){ m_strFilePath = str; }
	CString GetFileName(); //�t�@�C�����̂̂ݎ擾

	//�A�[�J�C�u�𗘗p����Bitmap�̃Z�[�u�E���[�h�����܂��B
	void Serialize(CArchive& ar); //�A�[�J�C�u�N���X

	// �w��DIB�t�@�C����ǂݍ��݁A�w��T�C�Y�ɕύX���܂��B
	BOOL Load(CString strFileName,int nCx,int nCy);

	// �w��DIB�t�@�C����ǂݍ��݂܂��B
	BOOL Load(CString sFileName);		

	// �w��DIB�t�@�C�������\�[�X����ǂݍ��݁A�w��T�C�Y�ɕύX���܂��B
	BOOL Load(int nID,int nCx,int nCy);

	// �w��DIB�t�@�C�������\�[�X����ǂݍ��݂܂��B
	BOOL Load(int nID);

	//���ڏ������݉\�ȃf�o�C�X�Ɨ��̃r�b�g�}�b�v�i24bpp DIB�j���쐬���܂��B
	BOOL Create(int iWidth, int iHeight);

	//�r�b�g�}�b�v�̉�������܂��B
	void DestroyBitmap();	

	//���œh��Ԃ�	
	void PaintWhite();		

	//���œh��Ԃ�
	void PaintBlack();
	
	//�w��F�œh��Ԃ�
	void PaintColor(COLORREF color);
	void PaintColor(COLORREF cr1,COLORREF cr2);

	void MaskRect(CRect &rcMask,COLORREF color);	//��`Mask��`�悵�܂��i�`��̂݁j	
	void MaskCircle(CRect &rcMask,COLORREF color);	//�~�`�}�X�N��`�悵�܂�	
	void PaintRgn(CRgn &rgn,COLORREF color);//�̈�̓h��Ԃ�

	//�w��f�o�C�X�R���e�L�X�g�Ƀr�b�g�}�b�v��`��		
	void Shift(CDIB &dib, double dbDx, double dbDy);//xy�����ړ�
	void Draw(CDC *pDC,int nX1,int nY1,int nW1,int nH1,int nX2,int nY2);
	void DrawLine(CPoint &ptS, CPoint &ptE, LOGPEN &lp); //���C���`��	
	void DrawRectangle(CRect &rect, LOGBRUSH &lb, LOGPEN &lp); //��`�`��
	void StretchDraw(CDC *pDc,int x,int y,int nW,int nH); //�g��k���@�\�t���`��
	void Line(int nX1,int nY1,int nX2,int nY2,COLORREF color);
	//DIB�̃f�[�^�擪�A�h���X�擾
	BYTE* GetBits(){return m_pBits;}

	//DIB�̕����擾
	int GetWidth (){return m_BMI.biWidth;}

	//DIB�̍������擾
	int GetHeight(){return abs(m_BMI.biHeight);}

	//DIB�̃r�b�g�}�b�v�T�C�Y���擾
	int	GetSize(){return m_nBitsSize;}

	int GetLineSize(){return ((GetWidth() * 3 + 3) & ~3);} //1 Line Size�擾���܂��B

	//�ꎞ�I�ȃr�b�g�}�b�v�N���X�擾
	CBitmap* GetBitmap(){return CBitmap::FromHandle(m_hBitmap);}

	//�r�b�g�}�b�v�n���h�����擾
	HBITMAP GetHandle(){return m_hBitmap;}
	
	//�w��Bitmap Data ����	
	void Copy  (CDIB& dibDest);

	void ScreenCapture(const CRect &rect); //�w��̈�X�N���[���L���v�`��

	//�w�肵���r�b�g�}�b�v�������o�ɃR�s�[���܂��B
	void SetDib(int x,int y,CDIB& dibSrc );//�w��r�b�g�}�b�v���w����W�ɓ]�����܂��B

	//�w�肵���r�b�g�}�b�v���w��T�C�Y�ɕύX���ăR�s�[�]�����܂��B
	void CopyDib(CDIB& dibSrc	//�w��r�b�g�}�b�v
				,int nW			//�r�b�g�}�b�v��
				,int nH);		//�r�b�g�}�b�v����

	//�w��DIB�̍����̐F�𓧖��F�Ƃ��ăX�v���C�g�]�����s��
	void Sprite(CDIB& dibSrc //Bitmap Sprite �]��
				,int nX		//�X�v���C�g�]��x���W
				,int nY);	//�X�v���C�g�]��y���W

	//�r�b�g�}�b�v�؂���
	BOOL Cut(CDIB &dibSrc,CRect &rect);

	//CDIB����CBitmap* �ւ�cast
	operator CBitmap*(){return GetBitmap();} 

	COLORREF GetColor(int x,int y); //�w����W�̐F���擾

	int GetPixels(int nVal); //�w��l��f���擾

	//Bit����
	BYTE GetBitsB(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )  );}//pixel Blue
	BYTE GetBitsG(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+1);}//pixel Green
	BYTE GetBitsR(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+2);}//pixel Red
	BYTE* GetPBitsB(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )  );}//pixel Blue ptr
	BYTE* GetPBitsG(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+1);}//pixel Green ptr
	BYTE* GetPBitsR(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+2);}//pixel Red ptr

	CRect Scroll(CWnd *pWnd,int nDx,int nDy); //�r�b�g�}�b�v�X�N���[��
	
	BOOL WriteBitmap(LPCTSTR lpszFilePath);//�r�b�g�}�b�v�t�@�C���ۑ�
	BOOL WriteImage(LPCTSTR lpszPathName,LPCTSTR lpszExt);//�摜�t�@�C���ۑ�
	BOOL Save(CString strPathName,const WCHAR* wpszFormat);//�w��t�H�[�}�b�g��Save

	void And(CDIB *pDib); //And�l�Z�b�g
	void ChangeBinary(BYTE byTH);//2�l������			
	void ChangeBinary(BYTE byThMin,BYTE byThMax);//2�l������
	void ChangeColor(int nType);//�F�ϊ�����
	BOOL GetRange(BYTE &byMin,BYTE &byMax);//�ő�ŏ��l�̎擾
	BOOL GetBlockRange(BYTE *pBits,int nBw,int nBh,BYTE& byMin,BYTE& byMax); //�u���b�N�P�ʂł̍ő�ŏ��l�̎擾
	void ChangeRange(BYTE byMin,BYTE byMax);//�R���g���X�g�ő剻	
	void CollectHistgram(int nHistR[],int nHistG[],int nHistB[]);//�q�X�g�O�������W
	void CollectHistgramGray(int nGray[]); //�q�X�g�O�������W�@Gray
	int CollectHistgramRect(int nHistR[],int nHistG[],int nHistB[],const CRect &rect);
	void CollectHistgramRgn (int nHistR[],int nHistG[],int nHistB[],const CRgn  &rgn);
	void ColorLength(BYTE byR,BYTE byG,BYTE byB);//�w��F�Ƃ̋����摜�ɕϊ�
	void Invert();//�e�F���]
	BOOL IsEqual(CDIB &dib); //�摜��r
	void MedianFilter(); //���f�B�A���t�B���^
	void MedianFilter(CRect &rect);
	int CountColor(BYTE byR,BYTE byG,BYTE byB);//�w��F�̉�f�����J�E���g	
	double GetFocusValue(const CRect &rect);//�I�[�g�t�H�[�J�X�p�v�Z�l�擾	
	int DeltaY(const CRect &rect);//���x�̍����擾

	
	BOOL IsExistR(double dbAmp);//�Ȉ�R����
	BOOL IsExistG(double dbAmp);//�Ȉ�G����
	BOOL IsExistB(double dbAmp);//�Ȉ�B����	
	BOOL IsExistTh(int nTh, int nXmin, int nYmin, int nXmax, int nYmax);//�������l����

	BOOL ToMat(cv::Mat &matImg); //DIB -> Mat
	BOOL FromMat(cv::Mat &matImg); //Mat -> DIB

	//�ȉ��̃����o�֐��ɂ̓A�N�Z�X���Ȃ��ł��������B
private: //�����֐�
	HBITMAP LoadBitmapHandle(CString &strFileName);	//�w��r�b�g�}�b�v�ǂݍ���	
	BOOL CreateDibSectionHandle(HBITMAP hBmpTmp);	//DibSection Handle �쐬	
	BYTE Median(BYTE *pByte,int nLineSize);//���f�B�A���l�擾
};

