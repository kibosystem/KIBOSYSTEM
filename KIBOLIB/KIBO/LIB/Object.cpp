#include "../stdafx.h"
#include "Object.h"
#include "../KIBOView.h"

/////////////////////////////////////////////////
///ライン描画オブジェクト///

LineObject::LineObject(void)
{
	m_ptE.x	= 0; //終点
	m_ptE.y	= 0;
	m_color = Color::Red; //塗りつぶし色
	SetType(TYPE_LINE);
}
LineObject::~LineObject(void)
{
}

//矩形描画	
void LineObject::Draw(CDC *pDC)
{
	//Penの選択
	LOGPEN lp; //線色
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = GetColor().ToCOLORREF(); //色指定
	CPen *pPenG = g_GlobalMgr.GetPen(lp); //Pen取得
	CPen *pPenOld = pDC->SelectObject(pPenG); //Pen選択
	
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CPoint ptS = GetPointS();
	CPoint ptE = GetPointE();
	
	//描画
	pDC->MoveTo((int)(ptS.x*dbRateCx+0.5),(int)(ptS.y*dbRateCy+0.5));
	pDC->LineTo((int)(ptE.x*dbRateCx+0.5),(int)(ptE.y*dbRateCy+0.5));
	//後始末
	pDC->SelectObject( pPenOld );	
}

///ライン描画オブジェクト///
/////////////////////////////////////////////////


/////////////////////////////////////////////////
///矩形描画オブジェクト///
RectObject::RectObject(void)
{
	m_isPaint = false; //塗りつぶしますか？
	m_nWidth  = 0;	//幅
	m_nHeight = 0;	//高さ	
	m_color = Color::Red; //Default:赤
	SetType(TYPE_RECT);
}

RectObject::~RectObject(void)
{
}

//矩形描画
void RectObject::Draw(CDC *pDC)
{
	CRect rect(GetX(),GetY(),GetX()+GetWidth()+1,GetY()+GetHeight()+1);
	//描画領域
	if( IsPaint() ){
		LOGBRUSH lb; //ブラシ
		lb.lbColor = m_color.ToCOLORREF(); //色指定
		lb.lbHatch = 0;
		lb.lbStyle = BS_SOLID; //NULLブラシ
		CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
		pDC->FillRect(&rect,pBrush);
	}else{ //線のみ		
		//色選択
		LOGPEN lp;
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = m_color.ToCOLORREF(); //色指定
		CPen *pOldPen = pDC->SelectObject(g_GlobalMgr.GetPen(lp));
		CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&rect);//矩形描画
		//後始末
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
	//TRACE(_T("%s\n"),GetName());
}

//表示領域設定
void RectObject::SetRect(CRect &rect)
{
	SetX(min(rect.left,rect.right));
	SetY(min(rect.top,rect.bottom));
	SetWidth(rect.Width());
	SetHeight(rect.Height());
}

///矩形描画オブジェクト///
/////////////////////////////////////////////////

/////////////////////////////////////////////////
///円描画オブジェクト///
CircleObject::CircleObject(void)
{
	SetType(TYPE_CIRCLE);
}
CircleObject::~CircleObject(void)
{
}
void CircleObject::Draw(CDC *pDC)
{
	CRect rect(GetX(),GetY(),GetX()+GetWidth()+1,GetY()+GetHeight()+1);
	//描画領域
	LOGPEN lp;
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = m_color.ToCOLORREF(); //色指定
	CPen *pOldPen = pDC->SelectObject(g_GlobalMgr.GetPen(lp));
	LOGBRUSH lb; //ブラシ
	lb.lbColor = m_color.ToCOLORREF(); //色指定
	lb.lbHatch = 0;
	lb.lbStyle = BS_NULL; //NULLブラシ
	if( IsPaint() ){//塗りつぶしの時
		lb.lbStyle = BS_SOLID; //塗りつぶし
	}
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
	CBrush *pOldBrush = pDC->SelectObject(pBrush);
	pDC->Ellipse(&rect);//楕円描画
	//後始末
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
///円描画オブジェクト///
/////////////////////////////////////////////////


/////////////////////////////////////////////////
//PolylineObject

PolylineObject::PolylineObject()
{
	m_isClosed = true;	//閉じますか？
	SetType(TYPE_POLYLINE);
}

PolylineObject::~PolylineObject()
{

}

//ポリゴン枠線描画
void PolylineObject::Draw(CDC *pDC)
{
	LOGPEN lp; //線色
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = GetColor(); //色指定
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得
	CPen *pPenOld = pDC->SelectObject(pPen); //Pen選択

	CPoint pt = m_ptAry[0];
	pDC->MoveTo(pt.x,pt.y); //Start Point
	for(int i=1,imax=(int)m_ptAry.GetSize();i<imax;i++){
		pt = m_ptAry[i];	
		pDC->LineTo(pt.x,pt.y);
	}
	if( IsClosed() ){ //閉じますか？
		pt = m_ptAry[0]; //Start Pointまで線描画
		pDC->LineTo(pt.x,pt.y);
	}

	//後始末
	pDC->SelectObject( pPenOld );
}

//PolylineObject
/////////////////////////////////////////////////




/////////////////////////////////////////////////
//StringObject

StringObject::StringObject(void)
{
	m_unFormat = 0;
	SetType(TYPE_STRING);
}
StringObject::~StringObject(void)
{
	
}
//文字列描画領域初期化
void StringObject::InitRect()
{
	m_unFormat = 0;
	m_rect.SetRectEmpty();
}

//文字列描画領域指定
void StringObject::SetRect(CString &strH,CString &strV,int nLeft,int nTop,int nRight,int nBottom)
{
	if(strH.CompareNoCase(L"center")==0 ){
		m_unFormat = DT_CENTER;
	}else if(strH.CompareNoCase(L"right")==0){
		m_unFormat = DT_RIGHT;
	}else{
		m_unFormat = DT_LEFT;
	}
	if(strV.CompareNoCase(L"center")==0){
		m_unFormat |= (DT_VCENTER | DT_SINGLELINE);
	}else if(strV.CompareNoCase(L"bottom")==0){
		m_unFormat |= DT_BOTTOM;
	}else{
		m_unFormat |= DT_TOP;
	}
	m_rect.left = nLeft;
	m_rect.top  = nTop;
	m_rect.right = nRight;
	m_rect.bottom = nBottom;
}
//文字列描画
void StringObject::Draw(CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT); //背景なし
	pDC->SetTextColor( GetColor() );
	CFont *pOld = pDC->SelectObject( GetFont() );
	//文字列描画	
	if( m_rect.IsRectEmpty() ){
		pDC->TextOut(GetX(),GetY(),GetText()); 
	}else{
		pDC->DrawText(GetText(),m_rect,m_unFormat); 		
	}	
	pDC->SelectObject(pOld);
}
