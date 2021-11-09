// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once
#pragma warning(disable : 4995)

#include <map>

using namespace std; 

#define MAP_FONT_MAX	MAXWORD	//登録フォント数最大
#define MAP_PEN_MAX		MAXWORD	//登録ペン数最大
#define MAP_BRUSH_MAX	MAXWORD	//登録ブラシ数最大

class GlobalMgr
{
private:	
	map<LOGFONT,HFONT>	 m_mapFont; //フォント
	map<LOGPEN,HPEN>	 m_mapPen;	//ペン
	map<LOGBRUSH,HBRUSH> m_mapBrush;//ブラシ
public:
	GlobalMgr(void);
	~GlobalMgr(void);
	
	HFONT   GetFontHandle(LOGFONT &lf);		//フォントハンドルを取得
	HPEN    GetPenHandle(LOGPEN &lp);		//ペンハンドルを取得
	HBRUSH  GetBrushHandle(LOGBRUSH &lb);	//ブラシハンドルを取得

	CFont*  GetFont(LOGFONT &lf);	//フォントを取得
	CPen*   GetPen(LOGPEN &lp);		//ペンを取得
	CPen*	GetPen(UINT unStyle,int nWidth,COLORREF color);
	CBrush* GetBrush(LOGBRUSH &lb);	//ブラシを取得


	void RemoveFontMap();	//Font  Handleの全削除
	void RemovePenMap();	//Pen   Handleの全削除
	void RemoveBrushMap();	//Brush Handleの全削除
};
extern GlobalMgr g_GlobalMgr; //実体はGlobalMgr.cppに存在
extern bool operator < (const LOGFONT  &lfL,const LOGFONT  &lfR);//FONT 要素比較 
extern bool operator < (const LOGPEN   &lpL,const LOGPEN   &lpR);//Pen  要素比較
extern bool operator < (const LOGBRUSH &lbL,const LOGBRUSH &lbR);//Brush要素比較
