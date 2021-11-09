#include "../stdafx.h"
#include "color.h"
#include "globalmgr.h"
#include "Gdi.h"


//指定された矩形をビットマップに描画
void DrawRectangle(CDIB &dib,CRect &rect,COLORREF color)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	if( pDC != NULL ){
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = dcMem.SelectObject( dib.GetBitmap() );
		LOGPEN lp; //線色
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = color; //色指定
		CPen *pPen = g_GlobalMgr.GetPen(lp);
		CPen *pOldPen = dcMem.SelectObject(pPen ); //Pen取得
		CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);

		//dcMem.Rectangle(&rect);
		dcMem.MoveTo(rect.left ,rect.top);   dcMem.LineTo(rect.right,rect.top);
		dcMem.MoveTo(rect.left ,rect.top);   dcMem.LineTo(rect.left ,rect.bottom);
		dcMem.MoveTo(rect.right,rect.top);   dcMem.LineTo(rect.right,rect.bottom);
		dcMem.MoveTo(rect.left ,rect.bottom);dcMem.LineTo(rect.right,rect.bottom);

		//後始末
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldPen );
		dcMem.SelectObject( pOldBrush);
	}
	pDC->DeleteDC();
}

//指定された矩形配列をビットマップに描画
void DrawRectangles(CDIB &dib,CArray<CRect> &aryRect,COLORREF color)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	if( pDC != NULL ){
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = dcMem.SelectObject( dib.GetBitmap() );
		LOGPEN lp; //線色
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = color; //色指定
		CPen *pPen = g_GlobalMgr.GetPen(lp);
		CPen *pOldPen = dcMem.SelectObject(pPen); //Pen取得
		CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);
		for(int i=0;i<aryRect.GetSize();i++){
			dcMem.Rectangle(&aryRect[i]);
		}
		//後始末
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldPen );
		dcMem.SelectObject( pOldBrush);
	}
	pDC->DeleteDC();
}