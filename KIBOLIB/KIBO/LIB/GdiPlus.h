// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once
#pragma warning(disable : 4995)
#include <map>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

using namespace std; 
using namespace Gdiplus;

//ペン構成データ
struct PENINFO
{
	ARGB   m_argb;	//色
	int	 m_nWidth;	//線幅
};

class GdiPlus
{
private://GDI+使用
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR           m_gdiplusToken;
	map<PENINFO,Pen*> m_mapPen;//ペン管理
	map<ARGB,SolidBrush*>  m_mapBrush; //ブラシ管理
public:
	GdiPlus(void);
	~GdiPlus(void);

	SolidBrush* GetBrush(Color color);   //ブラシデータの取得
	Pen* GetPen(Color color,int nWidth); //ペンデータの取得
	void FillRectangle(CDC *pDC,int x,int y,int width,int height,Color color); //塗りつぶし
	void DrawRectangle(CDC *pDC,int x,int y,int width,int height,Color color); //枠線描画
};

extern GdiPlus g_GdiPlus;
extern bool operator < (const PENINFO &piL,const PENINFO &piR);