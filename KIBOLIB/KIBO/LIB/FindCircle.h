#pragma once
#include <vector>
#include "DIB256.h"

#define THETA_MAX_C	128

////���o�~
//struct CIRCLE
//{
//	int x; //���Sx
//	int y; //���Sy
//	int r; //���a
//	int val0;	//  0�̉�f��
//	int val255; //255�̉�f��
//};
//
////�~���o�N���X
//class FindCircle
//{
//private:
//	Dib256 m_dibGray; //�摜�t�@�C��
//	int m_nRmin;	//���a�ŏ�
//	int m_nRmax;	//���a�ő�
//	float m_sn[THETA_MAX_C];		//�O�p�֐��e�[�u���i�T�C���j	
//	float m_cs[THETA_MAX_C];		//�O�p�֐��e�[�u���i�R�T�C���j
//	vector<short> m_diagonal;	//���a�v�Z�p�ΐ����e�[�u��  
//	vector<CIRCLE> m_aryCircle; //���o�~
//	int m_nPixelsMin; //�w���f����
//	int m_nPixelsMax; //�w���f����
//public:
//	FindCircle();
//	~FindCircle();
//
//	Dib256& GetDib(){return m_dibGray;} //�摜�擾
//	vector<CIRCLE>& GetCircle(){return m_aryCircle;} //���o�~�z��擾
//	BOOL Init(CDIB &dib,int nRmin,int nRmax,int nPixelsMin,int nPixelsMax); //������
//	void Find(); //�~���o
//	void Output();//���ʕ\��
//};
//
