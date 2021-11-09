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

/// STL コレクション

typedef vector<BYTE>  vBYTE;
typedef vector<WORD>  vWORD;
typedef vector<DWORD> vDWORD;
typedef vector<LONGLONG> vLONGLONG;
typedef vector<double>  vDOUBLE;

typedef map<WORD,LPARAM>	mapWordParam;
typedef map<int ,int>		mapIntInt;


/// MFC コレクション 

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
	double GetAverage(); //平均
	void RemoveLast(); //末尾削除
};
class AryDouble : public CArray<double>
{
public:
	AryDouble(void){}
	~AryDouble(void){}

	AryDouble& operator=(const AryDouble &t);

	int Find(double by);
	double GetAverage(); //平均
	double GetMin(int *pIndex=NULL); //最小値取得
	double GetMax(int *pIndex=NULL); //最大値取得
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

//Lineクラス
class CLine
{
public:
	CPoint m_ptS; //開始点
	CPoint m_ptE; //終了点
public:
	CLine():m_ptS(-1,-1),m_ptE(-1,-1){} //コンストラクタ
	CLine(CPoint &ptS,CPoint &ptE){m_ptS=ptS;m_ptE=ptE;} //コンストラクタ
	CLine(int nXS,int nYS,int nXE,int nYE){SetPointS(nXS,nYS);SetPointE(nXE,nYE);} //コンストラクタ
	~CLine(){}
	void SetPointS(CPoint& pt){m_ptS=pt;}
	void SetPointS(int x,int y){m_ptS.x=x;m_ptS.y=y;}
	void SetPointE(CPoint& pt){m_ptE=pt;}
	void SetPointE(int x,int y){m_ptE.x=x;m_ptE.y=y;}	
	double GetLength(); //線の長さ	
	double GetLength(double dbCx,double dbCy) ;//単位付き指定線の長さ
	double GetLength(CLine &line);//線分間の長さ	
	double CalcLength(double dbX,double dbY);//線分と点間の長さ
	BOOL IsIn(CPoint &pt); //指定座標を含みますか？
	BOOL IsVLine(){return (m_ptS.x==m_ptE.x);} //垂直ラインですか？
	BOOL IsHLine(){return (m_ptS.y==m_ptE.y);} //水平ラインですか？	
	BOOL IsValid(){ return (m_ptS.x >= 0 && m_ptE.x >=0);} //座標値はセットされていますか？
	BOOL IsPtSOK(){return (m_ptS.x >= 0);} //開始地点はOKですか？
	BOOL IsPtEOK(){return (m_ptE.x >= 0);} //終了地点はOKですか？
	void Clear(){m_ptS.x=m_ptS.y=-1;m_ptE.x=m_ptE.y=-1;} //クリア
	CLine& operator=(const CLine &l){
		m_ptS = l.m_ptS;
		m_ptE = l.m_ptE;
		return *this;
	}
	BOOL operator==(const CLine &l){
		return (m_ptS==l.m_ptS && m_ptE==l.m_ptE);
	}
};
//Line配列
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
	void RemoveLast(); //末尾削除
	void RemoveX(int x); //指定x座標要素削除
	void RemoveY(int y); //指定x座標要素削除
	int  Find(CLine &line); //指定ライン検索
	int GetMinX(); //領域最小点取得
	int GetMaxX(); //領域最大点取得
	int GetMinY(); //領域最小点取得
	int GetMaxY(); //領域最大点取得
	BOOL GetLine(CLine& line,CPoint &pt); //指定座標を含むラインを取得
};

//並び替えクラス
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

//ソート機能付き文字列配列
class CSortStringArray : public CStringArray 
{
public:
   void Sort();
private:
   BOOL CompareAndSwap(int pos);
};
