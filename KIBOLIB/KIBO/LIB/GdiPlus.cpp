#include "../stdafx.h"
#include "GdiPlus.h"

GdiPlus::GdiPlus(void)
{
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

GdiPlus::~GdiPlus(void)
{
	GdiplusShutdown(m_gdiplusToken); //GDI+�I�u�W�F�N�g�̌�n��
}

//�y���f�[�^�̎擾
Pen* GdiPlus::GetPen(Color color,int nWidth)
{
	PENINFO pInfo;
	pInfo.m_argb = color.GetValue();
	pInfo.m_nWidth = nWidth;
	Pen* pPen = m_mapPen[pInfo];//�y���N���X�擾
	if( pPen == NULL ){//���쐬�̎�����
		pPen = new Pen(color,(REAL)nWidth);//Pen�쐬
		m_mapPen[pInfo] = pPen; //�쐬����Pen��o�^
	}
	return pPen;
}
//�u���V�f�[�^�̎擾
SolidBrush* GdiPlus::GetBrush(Color color)
{
	SolidBrush *pBrush = m_mapBrush[color.GetValue()];
	if(pBrush == NULL){//���쐬�̎�����
		pBrush = new SolidBrush(color);
		m_mapBrush[color.GetValue()] = pBrush;
	}
	return pBrush;
}

void GdiPlus::FillRectangle(CDC *pDC,int x,int y,int width,int height,Color color)
{	
	if(pDC != NULL ){
		Graphics graphics( pDC->GetSafeHdc() );
		//�u���V�쐬
		SolidBrush *pBrush = GetBrush(color);
		if( pBrush != NULL ){
			graphics.FillRectangle(pBrush,x,y,width,height);//�`��
		}
	}
}

void GdiPlus::DrawRectangle(CDC *pDC,int x,int y,int width,int height,Color color)
{
	if(pDC != NULL ){
		Graphics graphics( pDC->GetSafeHdc() );
		Pen* pPen = GetPen(color,1);//Pen�N���X�擾
		if(pPen != NULL ){
			graphics.DrawRectangle(pPen,x,y,width,height);//�g�`��
		}
	}
}

//Pen  �v�f��r
bool operator < (const PENINFO &piL,const PENINFO &piR)
{
	if( piL.m_argb != piR.m_argb ){
		return (piL.m_argb < piR.m_argb);
	}
	return (piL.m_nWidth < piR.m_nWidth);
}
