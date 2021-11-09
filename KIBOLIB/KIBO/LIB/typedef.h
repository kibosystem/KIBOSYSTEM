// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include "PointDbl.h"

using namespace std;

/// STL �R���N�V����

typedef vector<BYTE>  vBYTE;
typedef vector<WORD>  vWORD;
typedef vector<DWORD> vDWORD;
typedef vector<LONGLONG> vLONGLONG;
typedef vector<double>  vDOUBLE;

typedef map<WORD,LPARAM>	mapWordParam;
typedef map<int ,int>		mapIntInt;


/// MFC �R���N�V���� 

#include <afxtempl.h>

class AryByte : public CArray<BYTE>
{
public:
	AryByte(void){}
	~AryByte(void){}

	AryByte& operator=(const AryByte &t);
	int Find(BYTE by);
};
class AryInt : public CArray<int>
{
public:
	AryInt(void){}
	~AryInt(void){}

	AryInt& operator=(const AryInt &t);
	int Find(int n);
	double GetAverage(); //����
	void RemoveLast(); //�����폜
};
class AryDouble : public CArray<double>
{
public:
	AryDouble(void){}
	~AryDouble(void){}

	AryDouble& operator=(const AryDouble &t);

	int Find(double by);
	double GetAverage(); //����
	double GetMin(int *pIndex=NULL); //�ŏ��l�擾
	double GetMax(int *pIndex=NULL); //�ő�l�擾
};
class AryWord : public CArray<WORD>
{
public:
	AryWord(void){}
	~AryWord(void){}

	AryWord& operator=(const AryWord &t);
	int Find(WORD w);
};

class AryDWord : public CArray<DWORD>
{
public:
	AryDWord(void){}
	~AryDWord(void){}

	AryDWord& operator=(const AryDWord &t);
	int Find(DWORD dw);
};

class AryParam : public CArray<LPARAM>
{
public:
	AryParam(void){}
	~AryParam(void){}

	AryParam& operator=(const AryParam &t);
	int Find(LPARAM lParam);
};

class AryLongLong : public CArray<LONGLONG>
{
public:
	AryLongLong(void){}
	~AryLongLong(void){}

	AryLongLong& operator=(const AryLongLong &t);
	int Find(LONGLONG ll);
};

class StringArrayPlus : public CStringArray
{
public:
	StringArrayPlus(void){}
	~StringArrayPlus(void){}

	StringArrayPlus& operator=(const StringArrayPlus &t);
	int Find(LPCTSTR lpsz);
};

//Line�N���X
class CLine
{
public:
	CPoint m_ptS; //�J�n�_
	CPoint m_ptE; //�I���_
public:
	CLine():m_ptS(-1,-1),m_ptE(-1,-1){} //�R���X�g���N�^
	CLine(CPoint &ptS,CPoint &ptE){m_ptS=ptS;m_ptE=ptE;} //�R���X�g���N�^
	CLine(int nXS,int nYS,int nXE,int nYE){SetPointS(nXS,nYS);SetPointE(nXE,nYE);} //�R���X�g���N�^
	~CLine(){}
	void SetPointS(CPoint& pt){m_ptS=pt;}
	void SetPointS(int x,int y){m_ptS.x=x;m_ptS.y=y;}
	void SetPointE(CPoint& pt){m_ptE=pt;}
	void SetPointE(int x,int y){m_ptE.x=x;m_ptE.y=y;}	
	double GetLength(); //���̒���	
	double GetLength(double dbCx,double dbCy) ;//�P�ʕt���w����̒���
	double GetLength(CLine &line);//�����Ԃ̒���	
	double CalcLength(double dbX,double dbY);//�����Ɠ_�Ԃ̒���
	BOOL IsIn(CPoint &pt); //�w����W���܂݂܂����H
	BOOL IsVLine(){return (m_ptS.x==m_ptE.x);} //�������C���ł����H
	BOOL IsHLine(){return (m_ptS.y==m_ptE.y);} //�������C���ł����H	
	BOOL IsValid(){ return (m_ptS.x >= 0 && m_ptE.x >=0);} //���W�l�̓Z�b�g����Ă��܂����H
	BOOL IsPtSOK(){return (m_ptS.x >= 0);} //�J�n�n�_��OK�ł����H
	BOOL IsPtEOK(){return (m_ptE.x >= 0);} //�I���n�_��OK�ł����H
	void Clear(){m_ptS.x=m_ptS.y=-1;m_ptE.x=m_ptE.y=-1;} //�N���A
	CLine& operator=(const CLine &l){
		m_ptS = l.m_ptS;
		m_ptE = l.m_ptE;
		return *this;
	}
	BOOL operator==(const CLine &l){
		return (m_ptS==l.m_ptS && m_ptE==l.m_ptE);
	}
};
//Line�z��
class AryLine : public CArray<CLine>
{
public:
	AryLine(void){}
	~AryLine(void){}

	enum{
		 XMIN = 0
		,XMAX
		,YMIN
		,YMAX
	};

	AryLine& operator=(const AryLine &t);
	void RemoveLast(); //�����폜
	void RemoveX(int x); //�w��x���W�v�f�폜
	void RemoveY(int y); //�w��x���W�v�f�폜
	int  Find(CLine &line); //�w�胉�C������
	int GetMinX(); //�̈�ŏ��_�擾
	int GetMaxX(); //�̈�ő�_�擾
	int GetMinY(); //�̈�ŏ��_�擾
	int GetMaxY(); //�̈�ő�_�擾
	BOOL GetLine(CLine& line,CPoint &pt); //�w����W���܂ރ��C�����擾
};

//���ёւ��N���X
class SortPointX
{
public:
    bool operator()(CPoint pt1,CPoint pt2) const 
	{
		return (pt1.x < pt2.x);
    }
};
class SortPointY
{
public:
    bool operator()(CPoint pt1,CPoint pt2) const 
	{
		return (pt1.y < pt2.y);
    }
};

//�\�[�g�@�\�t��������z��
class CSortStringArray : public CStringArray 
{
public:
   void Sort();
private:
   BOOL CompareAndSwap(int pos);
};
