#include "../stdafx.h"
#include "../KIBOView.h"
#include "RgnMask.h"


RgnMask::RgnMask(void)
{
	m_pBar = NULL; //�_�C�A���O�o�[
	m_nID = 0; //Mask ID �ԍ�
	m_dlgLT.SetMask(this); //����
	m_dlgRT.SetMask(this); //�E��
	m_dlgLB.SetMask(this); //����
	m_dlgRB.SetMask(this); //�E��		

	m_dlgLT.SetType(SelectDlg::TYPE_LT); //����
	m_dlgRT.SetType(SelectDlg::TYPE_RT); //�E��
	m_dlgLB.SetType(SelectDlg::TYPE_LB); //����
	m_dlgRB.SetType(SelectDlg::TYPE_RB); //�E��

	m_isMoveOK = false;
}

RgnMask::~RgnMask(void)
{
}

//�N���C�A���g���W�擾
CRect RgnMask::GetClientRect()
{
	CKIBOView *pView = GetView();
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	CRect rect((int)(m_rcImage.left*dbRx),(int)(m_rcImage.top*dbRx),(int)(m_rcImage.right*dbRx),(int)(m_rcImage.bottom *dbRy)); //�N���C�A���g���W�ݒ�
	return rect;
}
//���W�ݒ�
void RgnMask::SetClientRect(CRect &rect)
{
	CKIBOView *pView = GetView();
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	m_rcImage.SetRect( (int)(rect.left/dbRx),(int)(rect.top/dbRy),(int)(rect.right/dbRx),(int)(rect.bottom/dbRy));
	m_pBar->PostMessage(m_nID); //�R���g���[���l�ύX
}
void RgnMask::SetClientRect(int nLeft,int nTop,int nRight,int nBottom)
{
	CKIBOView *pView = GetView();
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();

	int nXmin = min(nLeft,nRight);
	int nXmax = max(nLeft,nRight);
	int nYmin = min(nTop ,nBottom);
	int nYmax = max(nTop ,nBottom);
	m_rcImage.SetRect((int)(nXmin/dbRx),(int)(nYmin/dbRy),(int)(nXmax/dbRx),(int)(nYmax/dbRy));
	m_pBar->PostMessage(m_nID); //�R���g���[���l�ύX
}
void RgnMask::SetImageRect(int nLeft,int nTop,int nRight,int nBottom)
{
	int nXmin = min(nLeft,nRight);
	int nXmax = max(nLeft,nRight);
	int nYmin = min(nTop ,nBottom);
	int nYmax = max(nTop ,nBottom);
	m_rcImage.SetRect(nXmin,nYmin,nXmax,nYmax);
	m_pBar->PostMessage(m_nID); //�R���g���[���l�ύX
}
//�}�X�N�\��
void RgnMask::Show() 
{
	CKIBOView *pView = GetView(); //View�擾
	
	if( m_dlgLT.GetSafeHwnd() == NULL ){ 
		m_dlgLT.Create(SelectDlg::IDD); //����
		m_dlgRT.Create(SelectDlg::IDD); //�E��
		m_dlgLB.Create(SelectDlg::IDD); //����
		m_dlgRB.Create(SelectDlg::IDD); //�E��		
	}
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	CRect rect((int)(m_rcImage.left*dbRx),(int)(m_rcImage.top*dbRx),(int)(m_rcImage.right*dbRx),(int)(m_rcImage.bottom *dbRy)); //�N���C�A���g���W�ݒ�
	pView->ClientToScreen(rect);//Screen���W�ɕϊ�
	
	m_isMoveOK = false;	//�A���֎~
	//�ړ�
	m_dlgLT.Move(rect.left ,rect.top   );
	m_dlgRT.Move(rect.right,rect.top   );
	m_dlgLB.Move(rect.left ,rect.bottom);
	m_dlgRB.Move(rect.right,rect.bottom);
	m_dlgLT.ShowWindow(SW_SHOW);
	m_dlgRT.ShowWindow(SW_SHOW);
	m_dlgLB.ShowWindow(SW_SHOW);
	m_dlgRB.ShowWindow(SW_SHOW);
	m_isMoveOK = true; //�A��OK
}

//�}�X�N��\��
void RgnMask::Hide() 
{
	m_dlgLT.ShowWindow(SW_HIDE);
	m_dlgRT.ShowWindow(SW_HIDE);
	m_dlgLB.ShowWindow(SW_HIDE);
	m_dlgRB.ShowWindow(SW_HIDE);	
}
//�\�����ł����H
BOOL RgnMask::IsVisible()
{
	if( m_isMoveOK ){
		return m_dlgLT.IsWindowVisible();
	}
	return FALSE;
}
//�}�X�N�`��
void RgnMask::Draw(CDC* pDC) 
{
	if( IsVisible() ){
		CPoint ptLT = m_dlgLT.GetClientPoint();
		CPoint ptRB = m_dlgRB.GetClientPoint();

		//�O����
		LOGBRUSH lb; //�u���V
		lb.lbColor = 0; //�F�Ȃ�
		lb.lbHatch = 0;
		lb.lbStyle = BS_NULL;
		CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
		LOGPEN lp; //�w��F�̃y��
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = COLOR_GREEN; //�F�w��
		CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾

		CBrush *pBrushOld = pDC->SelectObject(pBrush);		
		CPen *pPenOld = pDC->SelectObject(pPen);		
		
		pDC->Ellipse(ptLT.x,ptLT.y,ptRB.x,ptRB.y);
		//��n��
		pDC->SelectObject(pBrushOld);
		pDC->SelectObject(pPenOld);
	}
}

//�I���_�C�A���O�̘A��
void RgnMask::MoveSelectDlg(int nType)
{
	if(m_isMoveOK){
		switch(nType){
			case SelectDlg::TYPE_LT: //����
				{
					CPoint ptLT = m_dlgLT.GetScreenPoint();//�X�N���[�����W�擾
					m_dlgLB.MoveX(ptLT.x);
					m_dlgRT.MoveY(ptLT.y);
				}
				break;
			case SelectDlg::TYPE_RT: //�E��
				{
					CPoint ptRT = m_dlgRT.GetScreenPoint();//�X�N���[�����W�擾
					m_dlgRB.MoveX(ptRT.x);
					m_dlgLT.MoveY(ptRT.y);
				}
				break;
			case SelectDlg::TYPE_LB: //����
				{
					CPoint ptLB = m_dlgLB.GetScreenPoint();//�X�N���[�����W�擾
					m_dlgLT.MoveX(ptLB.x);
					m_dlgRB.MoveY(ptLB.y);
				}
				break;
			case SelectDlg::TYPE_RB: //�E��
				{
					CPoint ptRB = m_dlgRB.GetScreenPoint();//�X�N���[�����W�擾
					m_dlgRT.MoveX(ptRB.x);
					m_dlgLB.MoveY(ptRB.y);
				}
				break;
		}
		CPoint ptLT = m_dlgLT.GetClientPoint();
		CPoint ptRB = m_dlgRB.GetClientPoint();
		SetClientRect(ptLT.x,ptLT.y,ptRB.x,ptRB.y);
		GetView()->Render(TRUE);
	}
}

