#include "../stdafx.h"
#include "color.h"
#include "globalmgr.h"
#include "Gdi.h"


//�w�肳�ꂽ��`���r�b�g�}�b�v�ɕ`��
void DrawRectangle(CDIB &dib,CRect &rect,COLORREF color)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	if( pDC != NULL ){
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = dcMem.SelectObject( dib.GetBitmap() );
		LOGPEN lp; //���F
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = color; //�F�w��
		CPen *pPen = g_GlobalMgr.GetPen(lp);
		CPen *pOldPen = dcMem.SelectObject(pPen ); //Pen�擾
		CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);

		//dcMem.Rectangle(&rect);
		dcMem.MoveTo(rect.left ,rect.top);   dcMem.LineTo(rect.right,rect.top);
		dcMem.MoveTo(rect.left ,rect.top);   dcMem.LineTo(rect.left ,rect.bottom);
		dcMem.MoveTo(rect.right,rect.top);   dcMem.LineTo(rect.right,rect.bottom);
		dcMem.MoveTo(rect.left ,rect.bottom);dcMem.LineTo(rect.right,rect.bottom);

		//��n��
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldPen );
		dcMem.SelectObject( pOldBrush);
	}
	pDC->DeleteDC();
}

//�w�肳�ꂽ��`�z����r�b�g�}�b�v�ɕ`��
void DrawRectangles(CDIB &dib,CArray<CRect> &aryRect,COLORREF color)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	if( pDC != NULL ){
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = dcMem.SelectObject( dib.GetBitmap() );
		LOGPEN lp; //���F
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = color; //�F�w��
		CPen *pPen = g_GlobalMgr.GetPen(lp);
		CPen *pOldPen = dcMem.SelectObject(pPen); //Pen�擾
		CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);
		for(int i=0;i<aryRect.GetSize();i++){
			dcMem.Rectangle(&aryRect[i]);
		}
		//��n��
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldPen );
		dcMem.SelectObject( pOldBrush);
	}
	pDC->DeleteDC();
}