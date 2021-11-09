#include "../stdafx.h"
#include "globalmgr.h"

GlobalMgr g_GlobalMgr; //����

GlobalMgr::GlobalMgr(void)
{
}

GlobalMgr::~GlobalMgr(void)
{
	RemoveFontMap(); //Font Handle�̍폜
	RemovePenMap();
	RemoveBrushMap();
}

//Font Handle�̑S�폜
void GlobalMgr::RemoveFontMap() 
{
	for(map<LOGFONT,HFONT>::iterator it=m_mapFont.begin();it!=m_mapFont.end();it++){
		DeleteObject(it->second);
	}
	m_mapFont.clear();
}
//Pen   Handle�̑S�폜
void GlobalMgr::RemovePenMap()
{
	for(map<LOGPEN,HPEN>::iterator it=m_mapPen.begin();it!=m_mapPen.end();it++){
		DeleteObject(it->second);
	}
	m_mapPen.clear();
}
//Brush Handle�̑S�폜
void GlobalMgr::RemoveBrushMap()
{
	for(map<LOGBRUSH,HBRUSH>::iterator it=m_mapBrush.begin();it!=m_mapBrush.end();it++){
		DeleteObject(it->second);
	}
	m_mapBrush.clear();
}
//�t�H���g�n���h�����擾
//�o�^����Ă���Ύ��o���A�o�^����Ă��Ȃ���΍쐬���ēo�^
HFONT GlobalMgr::GetFontHandle(LOGFONT &lf)
{
	HFONT hFont = m_mapFont[lf]; //Font Handle �擾
	if( hFont == NULL ){//�����쐬����Ă��Ȃ��Ƃ�
		if( MAP_FONT_MAX < m_mapFont.size() ){ //�����o�^���ő�𒴂�����
			RemoveFontMap(); //��U�S�폜
		}
		//Font Handle �쐬
		hFont = ::CreateFontIndirect(&lf);
		//Font Handle�o�^
		m_mapFont[lf] = hFont;
	}
	return hFont;
}

//�y���n���h�����擾
HPEN GlobalMgr::GetPenHandle(LOGPEN &lp)
{
	HPEN hPen = m_mapPen[lp]; //Pen Handle �擾
	if( hPen == NULL ){//�����쐬����Ă��Ȃ��Ƃ�
		if( MAP_PEN_MAX < m_mapPen.size() ){ //�����o�^���ő�𒴂�����
			RemovePenMap(); //��U�S�폜
		}
		//Pen Handle �쐬
		hPen = ::CreatePenIndirect(&lp);
		//Pen Handle�o�^
		m_mapPen[lp] = hPen;
	}
	return hPen;
}

//�u���V�n���h�����擾
HBRUSH GlobalMgr::GetBrushHandle(LOGBRUSH &lb)
{
	HBRUSH hBrush = m_mapBrush[lb]; //Pen Handle �擾
	if( hBrush == NULL ){//�����쐬����Ă��Ȃ��Ƃ�
		if( MAP_BRUSH_MAX < m_mapBrush.size() ){ //�����o�^���ő�𒴂�����
			RemoveBrushMap(); //��U�S�폜
		}
		//Brush Handle �쐬
		hBrush = ::CreateBrushIndirect(&lb);
		//Brush Handle�o�^
		m_mapBrush[lb] = hBrush;
	}
	return hBrush;
}

//�t�H���g�N���X���擾
CFont* GlobalMgr::GetFont(LOGFONT &lf)
{
	return CFont::FromHandle(GetFontHandle(lf));//CFnot Class�ɕϊ����ĕԂ�
}

//�y�����擾
CPen* GlobalMgr::GetPen(LOGPEN &lp)
{
	return CPen::FromHandle(GetPenHandle(lp));//CPen Class�ɕϊ����ĕԂ�
}
CPen* GlobalMgr::GetPen(UINT unStyle,int nWidth,COLORREF color)   
{
	LOGPEN lp; //���F
	lp.lopnStyle = unStyle;
	lp.lopnWidth.x = nWidth;
	lp.lopnColor = color; //�F�w��
	return GetPen(lp); //Pen�擾
}
//�u���V���擾
CBrush* GlobalMgr::GetBrush(LOGBRUSH &lb)
{
	return CBrush::FromHandle(GetBrushHandle(lb));//CBrush Class�ɕϊ����ĕԂ�
}

//FONT�v�f��r
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
//Pen  �v�f��r
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
//Brush�v�f��r
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