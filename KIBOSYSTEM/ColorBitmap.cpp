#include "StdAfx.h"
#include "ColorBitmap.h"
//#include "BitmapDialog.h"

ColorBitmap::ColorBitmap(void)
{
}

ColorBitmap::~ColorBitmap(void)
{
}

//���̂̍쐬
ColorBitmap* ColorBitmap::CreateInstance() 
{
	ColorBitmap *pColorBitmap = new ColorBitmap;
	return pColorBitmap;
}

////�r�b�g�}�b�v�\��
//BOOL ColorBitmap::ShowBitmap()
//{
//	BitmapDialog dlg;
//	dlg.SetDIB(&m_dib);
//	dlg.DoModal();
//	return TRUE;
//}