#include "../stdafx.h"
#include "Object.h"
#include "../KIBOView.h"

/////////////////////////////////////////////////
///���C���`��I�u�W�F�N�g///

LineObject::LineObject(void)
{
	m_ptE.x	= 0; //�I�_
	m_ptE.y	= 0;
	m_color = Color::Red; //�h��Ԃ��F
	SetType(TYPE_LINE);
}
LineObject::~LineObject(void)
{
}

//��`�`��	
void LineObject::Draw(CDC *pDC)
{
	//Pen�̑I��
	LOGPEN lp; //���F
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = GetColor().ToCOLORREF(); //�F�w��
	CPen *pPenG = g_GlobalMgr.GetPen(lp); //Pen�擾
	CPen *pPenOld = pDC->SelectObject(pPenG); //Pen�I��
	
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CPoint ptS = GetPointS();
	CPoint ptE = GetPointE();
	
	//�`��
	pDC->MoveTo((int)(ptS.x*dbRateCx+0.5),(int)(ptS.y*dbRateCy+0.5));
	pDC->LineTo((int)(ptE.x*dbRateCx+0.5),(int)(ptE.y*dbRateCy+0.5));
	//��n��
	pDC->SelectObject( pPenOld );	
}

///���C���`��I�u�W�F�N�g///
/////////////////////////////////////////////////


/////////////////////////////////////////////////
///��`�`��I�u�W�F�N�g///
RectObject::RectObject(void)
{
	m_isPaint = false; //�h��Ԃ��܂����H
	m_nWidth  = 0;	//��
	m_nHeight = 0;	//����	
	m_color = Color::Red; //Default:��
	SetType(TYPE_RECT);
}

RectObject::~RectObject(void)
{
}

//��`�`��
void RectObject::Draw(CDC *pDC)
{
	CRect rect(GetX(),GetY(),GetX()+GetWidth()+1,GetY()+GetHeight()+1);
	//�`��̈�
	if( IsPaint() ){
		LOGBRUSH lb; //�u���V
		lb.lbColor = m_color.ToCOLORREF(); //�F�w��
		lb.lbHatch = 0;
		lb.lbStyle = BS_SOLID; //NULL�u���V
		CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
		pDC->FillRect(&rect,pBrush);
	}else{ //���̂�		
		//�F�I��
		LOGPEN lp;
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = m_color.ToCOLORREF(); //�F�w��
		CPen *pOldPen = pDC->SelectObject(g_GlobalMgr.GetPen(lp));
		CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&rect);//��`�`��
		//��n��
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
	//TRACE(_T("%s\n"),GetName());
}

//�\���̈�ݒ�
void RectObject::SetRect(CRect &rect)
{
	SetX(min(rect.left,rect.right));
	SetY(min(rect.top,rect.bottom));
	SetWidth(rect.Width());
	SetHeight(rect.Height());
}

///��`�`��I�u�W�F�N�g///
/////////////////////////////////////////////////

/////////////////////////////////////////////////
///�~�`��I�u�W�F�N�g///
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
	//�`��̈�
	LOGPEN lp;
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = m_color.ToCOLORREF(); //�F�w��
	CPen *pOldPen = pDC->SelectObject(g_GlobalMgr.GetPen(lp));
	LOGBRUSH lb; //�u���V
	lb.lbColor = m_color.ToCOLORREF(); //�F�w��
	lb.lbHatch = 0;
	lb.lbStyle = BS_NULL; //NULL�u���V
	if( IsPaint() ){//�h��Ԃ��̎�
		lb.lbStyle = BS_SOLID; //�h��Ԃ�
	}
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
	CBrush *pOldBrush = pDC->SelectObject(pBrush);
	pDC->Ellipse(&rect);//�ȉ~�`��
	//��n��
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
///�~�`��I�u�W�F�N�g///
/////////////////////////////////////////////////


/////////////////////////////////////////////////
//PolylineObject

PolylineObject::PolylineObject()
{
	m_isClosed = true;	//���܂����H
	SetType(TYPE_POLYLINE);
}

PolylineObject::~PolylineObject()
{

}

//�|���S���g���`��
void PolylineObject::Draw(CDC *pDC)
{
	LOGPEN lp; //���F
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = GetColor(); //�F�w��
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾
	CPen *pPenOld = pDC->SelectObject(pPen); //Pen�I��

	CPoint pt = m_ptAry[0];
	pDC->MoveTo(pt.x,pt.y); //Start Point
	for(int i=1,imax=(int)m_ptAry.GetSize();i<imax;i++){
		pt = m_ptAry[i];	
		pDC->LineTo(pt.x,pt.y);
	}
	if( IsClosed() ){ //���܂����H
		pt = m_ptAry[0]; //Start Point�܂Ő��`��
		pDC->LineTo(pt.x,pt.y);
	}

	//��n��
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
//������`��̈揉����
void StringObject::InitRect()
{
	m_unFormat = 0;
	m_rect.SetRectEmpty();
}

//������`��̈�w��
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
//������`��
void StringObject::Draw(CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT); //�w�i�Ȃ�
	pDC->SetTextColor( GetColor() );
	CFont *pOld = pDC->SelectObject( GetFont() );
	//������`��	
	if( m_rect.IsRectEmpty() ){
		pDC->TextOut(GetX(),GetY(),GetText()); 
	}else{
		pDC->DrawText(GetText(),m_rect,m_unFormat); 		
	}	
	pDC->SelectObject(pOld);
}
