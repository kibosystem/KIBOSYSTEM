#include "../stdafx.h"
#include "GdiPlus.h"

GdiPlus::GdiPlus(void)
{
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

GdiPlus::~GdiPlus(void)
{
	GdiplusShutdown(m_gdiplusToken); //GDI+オブジェクトの後始末
}

//ペンデータの取得
Pen* GdiPlus::GetPen(Color color,int nWidth)
{
	PENINFO pInfo;
	pInfo.m_argb = color.GetValue();
	pInfo.m_nWidth = nWidth;
	Pen* pPen = m_mapPen[pInfo];//ペンクラス取得
	if( pPen == NULL ){//未作成の時だけ
		pPen = new Pen(color,(REAL)nWidth);//Pen作成
		m_mapPen[pInfo] = pPen; //作成したPenを登録
	}
	return pPen;
}
//ブラシデータの取得
SolidBrush* GdiPlus::GetBrush(Color color)
{
	SolidBrush *pBrush = m_mapBrush[color.GetValue()];
	if(pBrush == NULL){//未作成の時だけ
		pBrush = new SolidBrush(color);
		m_mapBrush[color.GetValue()] = pBrush;
	}
	return pBrush;
}

void GdiPlus::FillRectangle(CDC *pDC,int x,int y,int width,int height,Color color)
{	
	if(pDC != NULL ){
		Graphics graphics( pDC->GetSafeHdc() );
		//ブラシ作成
		SolidBrush *pBrush = GetBrush(color);
		if( pBrush != NULL ){
			graphics.FillRectangle(pBrush,x,y,width,height);//描画
		}
	}
}

void GdiPlus::DrawRectangle(CDC *pDC,int x,int y,int width,int height,Color color)
{
	if(pDC != NULL ){
		Graphics graphics( pDC->GetSafeHdc() );
		Pen* pPen = GetPen(color,1);//Penクラス取得
		if(pPen != NULL ){
			graphics.DrawRectangle(pPen,x,y,width,height);//枠描画
		}
	}
}

//Pen  要素比較
bool operator < (const PENINFO &piL,const PENINFO &piR)
{
	if( piL.m_argb != piR.m_argb ){
		return (piL.m_argb < piR.m_argb);
	}
	return (piL.m_nWidth < piR.m_nWidth);
}
