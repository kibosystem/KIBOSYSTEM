#pragma once

#include "DIB256.h"
#include "PointDbl.h"
#define THETA_MAX	300 //�~�̕�����(100�ȏ�)
#define RHO_MAX		2560 //sqrt(XMAX*XMAX+YMAX*YMAX) length
//
////#define CHECK_LEN 80 //��������
////#define COUNT_ERR 100 //�G���[�J�E���g
//
////�������o
//
//class FindLine
//{
//private:
//	int m_YMAX;
//	int m_XMAX;
//	CDIB m_dib; //�摜�t�@�C��
//	float m_sn[THETA_MAX];		//�O�p�֐��e�[�u���i�T�C���j	
//	float m_cs[THETA_MAX];		//�O�p�֐��e�[�u���i�R�T�C���j
//public:
//	FindLine(void);
//	~FindLine(void);
//
//	CDIB& GetDib(){return m_dib;} //�摜�擾
//	BOOL Init(CString strPath); //������	
//	BOOL Init(CDIB &dib);
//	BOOL Find(BYTE byLine,int nPixels,int nCount,int nCheckLen);//�������o
//	void Draw(CDC &dc);
//private:
//	//CHECK_LEN���Ƃɒ������o����
//	void DrawLine(PointDbl ptS,PointDbl ptE,BYTE byLine,int nPixels,int nCheckLen);
//};
