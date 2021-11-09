#pragma once
#include "../LIB/gdiplus.h"
#include "../LIB/ArrayPoint.h"
#include "../LIB/typedef.h"

//文字特徴
class CharElement
{
public:
	enum{
		POINT_NUM = 12 //特徴点数
	};
public:
	//save data
	TCHAR	m_tChar;		;	//検査文字
	WORD	m_wRnum			;	//領域番号
	int		m_nAreaA		;	//面積All				（要素チェックメンバ）
	REAL	m_dbAreaT		;	//面積上
	REAL	m_dbAreaB		;	//面積下
	REAL	m_dbAreaL		;	//面積左
	REAL	m_dbAreaR		;	//面積右
	REAL	m_dbGx			;	//重心x
	REAL	m_dbGy			;	//重心y
	WORD	m_wContourNum	;	//境界線の数
	REAL	m_dbContourOut	;	//外部境界線の長さ
	REAL	m_dbContourIn	;	//内部境界線の長さ
	REAL	m_dbCircle		;	//円形度
	BYTE	m_byEdgeLR		;	//左から右方向のエッジ数
	BYTE	m_byEdgeRL		;	//右から左方向のエッジ数
	BYTE	m_byEdgeTB		;	//上から下方向のエッジ数
	BYTE	m_byEdgeBT		;	//下から上方向のエッジ数
	RECT	m_rect			;	 //文字領域　　　　　（要素チェックメンバ）
	REAL	m_rLeft	 [POINT_NUM];//左特徴点からの距離（要素チェックメンバ）
	REAL	m_rTop	 [POINT_NUM];//上特徴点からの距離（要素チェックメンバ）
	REAL	m_rRight [POINT_NUM];//右特徴点からの距離（要素チェックメンバ）
	REAL	m_rBottom[POINT_NUM];//下特徴点からの距離（要素チェックメンバ）
public:
	CharElement();  //コンストラクタ
	~CharElement(); //デストラクタ
	CharElement& operator=(IN const CharElement &e);

	//距離の総和
	REAL GetLengthLeft();	//左特徴点からの距離取得
	REAL GetLengthTop();	//上特徴点からの距離取得
	REAL GetLengthRight();	//右特徴点からの距離取得
	REAL GetLengthBottom(); //下特徴点からの距離取得
	CString GetLeftCsv();	//左特徴点からの距離
	CString GetTopCsv();	//上特徴点からの距離
	CString GetRightCsv();	//右特徴点からの距離
	CString GetBottomCsv(); //下特徴点からの距離

	void SetLeft  (ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptLB);//左特徴点からの距離設定
	void SetTop   (ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptRT);//上特徴点からの距離設定
	void SetRight (ArrayPoint &aryPoint,const CPoint &ptRT,const CPoint &ptRB);//右特徴点からの距離設定
	void SetBottom(ArrayPoint &aryPoint,const CPoint &ptLB,const CPoint &ptRB);//下特徴点からの距離設定
	
	BOOL IsLeftOK  (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//左距離はOKですか？
	BOOL IsTopOK   (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//上距離はOKですか？
	BOOL IsRightOK (const double dbLenMin[],const double dbLenMax[],CString &strErrType);//右距離はOKですか？
	BOOL IsBottomOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType);//下距離はOKですか？
};
