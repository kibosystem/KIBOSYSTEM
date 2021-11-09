#pragma once
#include "../LIB/gdiplus.h"
#include "../LIB/ArrayPoint.h"
#include "../LIB/typedef.h"

//��������
class CharElement
{
public:
	enum{
		POINT_NUM = 12 //�����_��
	};
public:
	//save data
	TCHAR	m_tChar;		;	//��������
	WORD	m_wRnum			;	//�̈�ԍ�
	int		m_nAreaA		;	//�ʐ�All				�i�v�f�`�F�b�N�����o�j
	REAL	m_dbAreaT		;	//�ʐϏ�
	REAL	m_dbAreaB		;	//�ʐω�
	REAL	m_dbAreaL		;	//�ʐύ�
	REAL	m_dbAreaR		;	//�ʐωE
	REAL	m_dbGx			;	//�d�Sx
	REAL	m_dbGy			;	//�d�Sy
	WORD	m_wContourNum	;	//���E���̐�
	REAL	m_dbContourOut	;	//�O�����E���̒���
	REAL	m_dbContourIn	;	//�������E���̒���
	REAL	m_dbCircle		;	//�~�`�x
	BYTE	m_byEdgeLR		;	//������E�����̃G�b�W��
	BYTE	m_byEdgeRL		;	//�E���獶�����̃G�b�W��
	BYTE	m_byEdgeTB		;	//�ォ�牺�����̃G�b�W��
	BYTE	m_byEdgeBT		;	//�����������̃G�b�W��
	RECT	m_rect			;	 //�����̈�@�@�@�@�@�i�v�f�`�F�b�N�����o�j
	REAL	m_rLeft	 [POINT_NUM];//�������_����̋����i�v�f�`�F�b�N�����o�j
	REAL	m_rTop	 [POINT_NUM];//������_����̋����i�v�f�`�F�b�N�����o�j
	REAL	m_rRight [POINT_NUM];//�E�����_����̋����i�v�f�`�F�b�N�����o�j
	REAL	m_rBottom[POINT_NUM];//�������_����̋����i�v�f�`�F�b�N�����o�j
public:
	CharElement();  //�R���X�g���N�^
	~CharElement(); //�f�X�g���N�^
	CharElement& operator=(IN const CharElement &e);

	//�����̑��a
	REAL GetLengthLeft();	//�������_����̋����擾
	REAL GetLengthTop();	//������_����̋����擾
	REAL GetLengthRight();	//�E�����_����̋����擾
	REAL GetLengthBottom(); //�������_����̋����擾
	CString GetLeftCsv();	//�������_����̋���
	CString GetTopCsv();	//������_����̋���
	CString GetRightCsv();	//�E�����_����̋���
	CString GetBottomCsv(); //�������_����̋���

	void SetLeft  (ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptLB);//�������_����̋����ݒ�
	void SetTop   (ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptRT);//������_����̋����ݒ�
	void SetRight (ArrayPoint &aryPoint,const CPoint &ptRT,const CPoint &ptRB);//�E�����_����̋����ݒ�
	void SetBottom(ArrayPoint &aryPoint,const CPoint &ptLB,const CPoint &ptRB);//�������_����̋����ݒ�
	
	BOOL IsLeftOK  (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//��������OK�ł����H
	BOOL IsTopOK   (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//�㋗����OK�ł����H
	BOOL IsRightOK (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//�E������OK�ł����H
	BOOL IsBottomOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType);//��������OK�ł����H
};
