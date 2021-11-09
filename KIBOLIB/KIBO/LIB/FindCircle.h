#pragma once
#include <vector>
#include "DIB256.h"

#define THETA_MAX_C	128

////検出円
//struct CIRCLE
//{
//	int x; //中心x
//	int y; //中心y
//	int r; //半径
//	int val0;	//  0の画素数
//	int val255; //255の画素数
//};
//
////円検出クラス
//class FindCircle
//{
//private:
//	Dib256 m_dibGray; //画像ファイル
//	int m_nRmin;	//半径最小
//	int m_nRmax;	//半径最大
//	float m_sn[THETA_MAX_C];		//三角関数テーブル（サイン）	
//	float m_cs[THETA_MAX_C];		//三角関数テーブル（コサイン）
//	vector<short> m_diagonal;	//半径計算用斜線長テーブル  
//	vector<CIRCLE> m_aryCircle; //検出円
//	int m_nPixelsMin; //指定画素数黒
//	int m_nPixelsMax; //指定画素数白
//public:
//	FindCircle();
//	~FindCircle();
//
//	Dib256& GetDib(){return m_dibGray;} //画像取得
//	vector<CIRCLE>& GetCircle(){return m_aryCircle;} //検出円配列取得
//	BOOL Init(CDIB &dib,int nRmin,int nRmax,int nPixelsMin,int nPixelsMax); //初期化
//	void Find(); //円検出
//	void Output();//結果表示
//};
//
