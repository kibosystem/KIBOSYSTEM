// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

#include "DataBuf.h"
#include "ArrayPoint.h"
#include "DIB.h"

//
// Dib256.h --- 256色ビットマップ専用 Dib クラス
//
class Dib256
{
private:
	BITMAPFILEHEADER	m_bmFileHeader;
	BITMAPINFOHEADER	m_bmInfoHeader;
	RGBQUAD				m_aryRGB[256]; //256色パレット
public:
	DataBuf m_buf;
	enum{ //線形補間位置取得種別
		 BLACK_MIN = 0	//最小値　黒
		,BLACK_MAX		//最大値　黒
		,BLACK_AVG		//平均値　黒
		,WHITE_MIN		//最小値　白
		,WHITE_MAX		//最大値　白
		,WHITE_AVG		//平均値　白	
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
	BOOL	Create(int nW,int nH); //指定サイズの画像作成
	BOOL	Copy(CDIB &dibSrc);//ビットマップコピー　フルカラー->256色
	BOOL	Copy(Dib256 &dibSrc);//ビットマップコピー　256色 -> 256色
	BOOL	Paste(CDIB &dibDst);//ビットマップコピー　256色->フルカラー		
	BOOL	Cut(CDIB &dibSrc,CRect &rect);//ビットマップ切り取り　フルカラー->256色
	BOOL	Cut(CDIB &dibSrc,CRect &rect,int nCx,int nCy);	//ビットマップ切り取り　フルカラー->256色
	void	MinMax();//コントラスト最大化
	void	Invert();//反転
	//Bit操作
	BYTE  GetValue(const CPoint &pt) {return GetValue(pt.x,pt.y);}//pixel value
	BYTE  GetValue(int x,int y) {return *( GetBits() + x + (GetHeight()-1-y) * GetLineSize());}//pixel value
	double GetAprxVal(double dbX,double dbY); //近似値取得
	BYTE* GetPtr  (int x,int y) {return  ( GetBits() + x + (GetHeight()-1-y) * GetLineSize());}//pixel ptr
	double GetLeftEdge  (int nType,int nTh,CRect &rect);//指定領域内の左端のx座標を取得	
	double GetRightEdge (int nType,int nTh,CRect &rect);//指定領域内の右端のx座標を取得
	double GetTopEdge   (int nType,int nTh,CRect &rect);//指定領域内の上端のy座標を取得	
	double GetBottomEdge(int nType,int nTh,CRect &rect);//指定領域内の下端のy座標を取得	
	void PaintBlack();//黒で塗りつぶし
	void PaintWhite();//白で塗りつぶし
	int CountPixels(int nTh);//しきい値より大きい画素数を取得
	CPoint GetMaxValuePoint(const CRect &rect);//最大地点取得
	void CollectHistgram(int nHist[]);//ヒストグラム取得
	void ChangeBinary(BYTE byTh);//2値化処理		
	int CalcThreshold();//しきい値計算
	void Shift(double dbX, double dbY); //データシフト
private:
	void MinMax(BYTE byMin,BYTE byMax);//コントラスト最大化	
	void Init();
};