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

//�y���\���f�[�^
struct PENINFO
{
	ARGB   m_argb;	//�F
	int	 m_nWidth;	//����
};

class GdiPlus
{
private://GDI+�g�p
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR           m_gdiplusToken;
	map<PENINFO,Pen*> m_mapPen;//�y���Ǘ�
	map<ARGB,SolidBrush*>  m_mapBrush; //�u���V�Ǘ�
public:
	GdiPlus(void);
	~GdiPlus(void);

	SolidBrush* GetBrush(Color color);   //�u���V�f�[�^�̎擾
	Pen* GetPen(Color color,int nWidth); //�y���f�[�^�̎擾
	void FillRectangle(CDC *pDC,int x,int y,int width,int height,Color color); //�h��Ԃ�
	void DrawRectangle(CDC *pDC,int x,int y,int width,int height,Color color); //�g���`��
};

extern GdiPlus g_GdiPlus;
extern bool operator < (const PENINFO &piL,const PENINFO &piR);