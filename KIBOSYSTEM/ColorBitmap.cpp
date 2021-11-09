#include "StdAfx.h"
#include "ColorBitmap.h"
//#include "BitmapDialog.h"

ColorBitmap::ColorBitmap(void)
{
}

ColorBitmap::~ColorBitmap(void)
{
}

//実体の作成
ColorBitmap* ColorBitmap::CreateInstance() 
{
	ColorBitmap *pColorBitmap = new ColorBitmap;
	return pColorBitmap;
}

////ビットマップ表示
//BOOL ColorBitmap::ShowBitmap()
//{
//	BitmapDialog dlg;
//	dlg.SetDIB(&m_dib);
//	dlg.DoModal();
//	return TRUE;
//}