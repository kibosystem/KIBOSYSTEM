#include "../stdafx.h"
#include "globalmgr.h"

GlobalMgr g_GlobalMgr; //実体

GlobalMgr::GlobalMgr(void)
{
}

GlobalMgr::~GlobalMgr(void)
{
	RemoveFontMap(); //Font Handleの削除
	RemovePenMap();
	RemoveBrushMap();
}

//Font Handleの全削除
void GlobalMgr::RemoveFontMap() 
{
	for(map<LOGFONT,HFONT>::iterator it=m_mapFont.begin();it!=m_mapFont.end();it++){
		DeleteObject(it->second);
	}
	m_mapFont.clear();
}
//Pen   Handleの全削除
void GlobalMgr::RemovePenMap()
{
	for(map<LOGPEN,HPEN>::iterator it=m_mapPen.begin();it!=m_mapPen.end();it++){
		DeleteObject(it->second);
	}
	m_mapPen.clear();
}
//Brush Handleの全削除
void GlobalMgr::RemoveBrushMap()
{
	for(map<LOGBRUSH,HBRUSH>::iterator it=m_mapBrush.begin();it!=m_mapBrush.end();it++){
		DeleteObject(it->second);
	}
	m_mapBrush.clear();
}
//フォントハンドルを取得
//登録されていれば取り出し、登録されていなければ作成して登録
HFONT GlobalMgr::GetFontHandle(LOGFONT &lf)
{
	HFONT hFont = m_mapFont[lf]; //Font Handle 取得
	if( hFont == NULL ){//未だ作成されていないとき
		if( MAP_FONT_MAX < m_mapFont.size() ){ //もし登録数最大を超えたら
			RemoveFontMap(); //一旦全削除
		}
		//Font Handle 作成
		hFont = ::CreateFontIndirect(&lf);
		//Font Handle登録
		m_mapFont[lf] = hFont;
	}
	return hFont;
}

//ペンハンドルを取得
HPEN GlobalMgr::GetPenHandle(LOGPEN &lp)
{
	HPEN hPen = m_mapPen[lp]; //Pen Handle 取得
	if( hPen == NULL ){//未だ作成されていないとき
		if( MAP_PEN_MAX < m_mapPen.size() ){ //もし登録数最大を超えたら
			RemovePenMap(); //一旦全削除
		}
		//Pen Handle 作成
		hPen = ::CreatePenIndirect(&lp);
		//Pen Handle登録
		m_mapPen[lp] = hPen;
	}
	return hPen;
}

//ブラシハンドルを取得
HBRUSH GlobalMgr::GetBrushHandle(LOGBRUSH &lb)
{
	HBRUSH hBrush = m_mapBrush[lb]; //Pen Handle 取得
	if( hBrush == NULL ){//未だ作成されていないとき
		if( MAP_BRUSH_MAX < m_mapBrush.size() ){ //もし登録数最大を超えたら
			RemoveBrushMap(); //一旦全削除
		}
		//Brush Handle 作成
		hBrush = ::CreateBrushIndirect(&lb);
		//Brush Handle登録
		m_mapBrush[lb] = hBrush;
	}
	return hBrush;
}

//フォントクラスを取得
CFont* GlobalMgr::GetFont(LOGFONT &lf)
{
	return CFont::FromHandle(GetFontHandle(lf));//CFnot Classに変換して返す
}

//ペンを取得
CPen* GlobalMgr::GetPen(LOGPEN &lp)
{
	return CPen::FromHandle(GetPenHandle(lp));//CPen Classに変換して返す
}
CPen* GlobalMgr::GetPen(UINT unStyle,int nWidth,COLORREF color)   
{
	LOGPEN lp; //線色
	lp.lopnStyle = unStyle;
	lp.lopnWidth.x = nWidth;
	lp.lopnColor = color; //色指定
	return GetPen(lp); //Pen取得
}
//ブラシを取得
CBrush* GlobalMgr::GetBrush(LOGBRUSH &lb)
{
	return CBrush::FromHandle(GetBrushHandle(lb));//CBrush Classに変換して返す
}

//FONT要素比較
bool operator < (const LOGFONT &lfL,const LOGFONT &lfR)
{
	if(lfL.lfHeight != lfR.lfHeight){
		return (lfL.lfHeight < lfR.lfHeight);
	}
	if(lfL.lfWidth != lfR.lfWidth){
		return (lfL.lfWidth < lfR.lfWidth);
	}
	if(lfL.lfEscapement	!= lfR.lfEscapement){
		return (lfL.lfEscapement < lfR.lfEscapement);
	}
	if(lfL.lfOrientation != lfR.lfOrientation){
		return (lfL.lfOrientation < lfR.lfOrientation);
	}
	if(lfL.lfWeight	!= lfR.lfWeight){
		return (lfL.lfWeight < lfR.lfWeight);
	}	
	if(lfL.lfItalic	!= lfR.lfItalic){
		return (lfL.lfItalic < lfR.lfItalic);
	}	
	if(lfL.lfUnderline != lfR.lfUnderline){
		return (lfL.lfUnderline < lfR.lfUnderline);
	}
	if(lfL.lfStrikeOut != lfR.lfStrikeOut){
		return (lfL.lfStrikeOut < lfR.lfStrikeOut);
	}
	if(lfL.lfCharSet != lfR.lfCharSet){
		return (lfL.lfCharSet < lfR.lfCharSet);
	}
	if(lfL.lfOutPrecision != lfR.lfOutPrecision){
		return (lfL.lfOutPrecision < lfR.lfOutPrecision);
	}
	if(lfL.lfClipPrecision != lfR.lfClipPrecision){
		return (lfL.lfClipPrecision < lfR.lfClipPrecision);
	}
	if(lfL.lfQuality != lfR.lfQuality){
		return (lfL.lfQuality < lfR.lfQuality);
	}
	if(lfL.lfPitchAndFamily	!= lfR.lfPitchAndFamily){
		return (lfL.lfPitchAndFamily < lfR.lfPitchAndFamily);
	}
	return wcscmp(lfL.lfFaceName,lfR.lfFaceName) < 0;
}
//Pen  要素比較
bool operator < (const LOGPEN &lpL,const LOGPEN &lpR)
{
	if(lpL.lopnStyle != lpR.lopnStyle){
		return (lpL.lopnStyle < lpR.lopnStyle);
	}
	if(lpL.lopnWidth.x != lpR.lopnWidth.x){
		return (lpL.lopnWidth.x < lpR.lopnWidth.x);
	}
	if(lpL.lopnWidth.y != lpR.lopnWidth.y){
		return (lpL.lopnWidth.y < lpR.lopnWidth.y);
	}
	return (lpL.lopnColor < lpR.lopnColor);
}
//Brush要素比較
bool operator < (const LOGBRUSH &lbL,const LOGBRUSH &lbR)
{
	if(lbL.lbStyle != lbR.lbStyle){
		return (lbL.lbStyle < lbR.lbStyle);
	}
    if(lbL.lbColor != lbR.lbColor){
		return (lbL.lbColor < lbR.lbColor);
	}
	return (lbL.lbHatch < lbR.lbHatch);
}