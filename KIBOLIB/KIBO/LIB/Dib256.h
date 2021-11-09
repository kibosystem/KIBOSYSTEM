// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

#include "DataBuf.h"
#include "ArrayPoint.h"
#include "DIB.h"

//
// Dib256.h --- 256�F�r�b�g�}�b�v��p Dib �N���X
//
class Dib256
{
private:
	BITMAPFILEHEADER	m_bmFileHeader;
	BITMAPINFOHEADER	m_bmInfoHeader;
	RGBQUAD				m_aryRGB[256]; //256�F�p���b�g
public:
	DataBuf m_buf;
	enum{ //���`��Ԉʒu�擾���
		 BLACK_MIN = 0	//�ŏ��l�@��
		,BLACK_MAX		//�ő�l�@��
		,BLACK_AVG		//���ϒl�@��
		,WHITE_MIN		//�ŏ��l�@��
		,WHITE_MAX		//�ő�l�@��
		,WHITE_AVG		//���ϒl�@��	
	};
public:
	Dib256();
	Dib256( LPCWSTR filename );
	~Dib256();

	BYTE*	GetBits(){return m_buf.m_pBuf;}
	BOOL	LoadFile( LPCWSTR filename );
	BOOL	SaveFile( LPCWSTR filename );
	DWORD	ColorsUsed();
	int		GetWidth();
	int		GetHeight();
	int		GetSize(){return m_bmInfoHeader.biSizeImage;}
	int GetLineSize();
	int		Bpp();
	void	GetPaletteEntries(int first, int count, PALETTEENTRY * entries);
	BOOL	Create(int nW,int nH); //�w��T�C�Y�̉摜�쐬
	BOOL	Copy(CDIB &dibSrc);//�r�b�g�}�b�v�R�s�[�@�t���J���[->256�F
	BOOL	Copy(Dib256 &dibSrc);//�r�b�g�}�b�v�R�s�[�@256�F -> 256�F
	BOOL	Paste(CDIB &dibDst);//�r�b�g�}�b�v�R�s�[�@256�F->�t���J���[		
	BOOL	Cut(CDIB &dibSrc,CRect &rect);//�r�b�g�}�b�v�؂���@�t���J���[->256�F
	BOOL	Cut(CDIB &dibSrc,CRect &rect,int nCx,int nCy);	//�r�b�g�}�b�v�؂���@�t���J���[->256�F
	void	MinMax();//�R���g���X�g�ő剻
	void	Invert();//���]
	//Bit����
	BYTE  GetValue(const CPoint &pt) {return GetValue(pt.x,pt.y);}//pixel value
	BYTE  GetValue(int x,int y) {return *( GetBits() + x + (GetHeight()-1-y) * GetLineSize());}//pixel value
	double GetAprxVal(double dbX,double dbY); //�ߎ��l�擾
	BYTE* GetPtr  (int x,int y) {return  ( GetBits() + x + (GetHeight()-1-y) * GetLineSize());}//pixel ptr
	double GetLeftEdge  (int nType,int nTh,CRect &rect);//�w��̈���̍��[��x���W���擾	
	double GetRightEdge (int nType,int nTh,CRect &rect);//�w��̈���̉E�[��x���W���擾
	double GetTopEdge   (int nType,int nTh,CRect &rect);//�w��̈���̏�[��y���W���擾	
	double GetBottomEdge(int nType,int nTh,CRect &rect);//�w��̈���̉��[��y���W���擾	
	void PaintBlack();//���œh��Ԃ�
	void PaintWhite();//���œh��Ԃ�
	int CountPixels(int nTh);//�������l���傫����f�����擾
	CPoint GetMaxValuePoint(const CRect &rect);//�ő�n�_�擾
	void CollectHistgram(int nHist[]);//�q�X�g�O�����擾
	void ChangeBinary(BYTE byTh);//2�l������		
	int CalcThreshold();//�������l�v�Z
	void Shift(double dbX, double dbY); //�f�[�^�V�t�g
private:
	void MinMax(BYTE byMin,BYTE byMax);//�R���g���X�g�ő剻	
	void Init();
};