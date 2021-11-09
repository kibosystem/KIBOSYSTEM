// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once
#pragma warning(disable : 4995)

#include <map>

using namespace std; 

#define MAP_FONT_MAX	MAXWORD	//�o�^�t�H���g���ő�
#define MAP_PEN_MAX		MAXWORD	//�o�^�y�����ő�
#define MAP_BRUSH_MAX	MAXWORD	//�o�^�u���V���ő�

class GlobalMgr
{
private:	
	map<LOGFONT,HFONT>	 m_mapFont; //�t�H���g
	map<LOGPEN,HPEN>	 m_mapPen;	//�y��
	map<LOGBRUSH,HBRUSH> m_mapBrush;//�u���V
public:
	GlobalMgr(void);
	~GlobalMgr(void);
	
	HFONT   GetFontHandle(LOGFONT &lf);		//�t�H���g�n���h�����擾
	HPEN    GetPenHandle(LOGPEN &lp);		//�y���n���h�����擾
	HBRUSH  GetBrushHandle(LOGBRUSH &lb);	//�u���V�n���h�����擾

	CFont*  GetFont(LOGFONT &lf);	//�t�H���g���擾
	CPen*   GetPen(LOGPEN &lp);		//�y�����擾
	CPen*	GetPen(UINT unStyle,int nWidth,COLORREF color);
	CBrush* GetBrush(LOGBRUSH &lb);	//�u���V���擾


	void RemoveFontMap();	//Font  Handle�̑S�폜
	void RemovePenMap();	//Pen   Handle�̑S�폜
	void RemoveBrushMap();	//Brush Handle�̑S�폜
};
extern GlobalMgr g_GlobalMgr; //���̂�GlobalMgr.cpp�ɑ���
extern bool operator < (const LOGFONT  &lfL,const LOGFONT  &lfR);//FONT �v�f��r 
extern bool operator < (const LOGPEN   &lpL,const LOGPEN   &lpR);//Pen  �v�f��r
extern bool operator < (const LOGBRUSH &lbL,const LOGBRUSH &lbR);//Brush�v�f��r
