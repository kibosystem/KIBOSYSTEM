#pragma once

#include "DIB256.h"
#include "PointDbl.h"
#define THETA_MAX	300 //円の分割数(100以上)
#define RHO_MAX		2560 //sqrt(XMAX*XMAX+YMAX*YMAX) length
//
////#define CHECK_LEN 80 //検査長さ
////#define COUNT_ERR 100 //エラーカウント
//
////直線検出
//
//class FindLine
//{
//private:
//	int m_YMAX;
//	int m_XMAX;
//	CDIB m_dib; //画像ファイル
//	float m_sn[THETA_MAX];		//三角関数テーブル（サイン）	
//	float m_cs[THETA_MAX];		//三角関数テーブル（コサイン）
//public:
//	FindLine(void);
//	~FindLine(void);
//
//	CDIB& GetDib(){return m_dib;} //画像取得
//	BOOL Init(CString strPath); //初期化	
//	BOOL Init(CDIB &dib);
//	BOOL Find(BYTE byLine,int nPixels,int nCount,int nCheckLen);//直線検出
//	void Draw(CDC &dc);
//private:
//	//CHECK_LENごとに直線検出検査
//	void DrawLine(PointDbl ptS,PointDbl ptE,BYTE byLine,int nPixels,int nCheckLen);
//};
