#include "../stdafx.h"
#include "../KIBOView.h"
#include "RgnMask.h"


RgnMask::RgnMask(void)
{
	m_pBar = NULL; //ダイアログバー
	m_nID = 0; //Mask ID 番号
	m_dlgLT.SetMask(this); //左上
	m_dlgRT.SetMask(this); //右上
	m_dlgLB.SetMask(this); //左下
	m_dlgRB.SetMask(this); //右下		

	m_dlgLT.SetType(SelectDlg::TYPE_LT); //左上
	m_dlgRT.SetType(SelectDlg::TYPE_RT); //右上
	m_dlgLB.SetType(SelectDlg::TYPE_LB); //左下
	m_dlgRB.SetType(SelectDlg::TYPE_RB); //右下

	m_isMoveOK = false;
}

RgnMask::~RgnMask(void)
{
}

//クライアント座標取得
CRect RgnMask::GetClientRect()
{
	CKIBOView *pView = GetView();
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	CRect rect((int)(m_rcImage.left*dbRx),(int)(m_rcImage.top*dbRx),(int)(m_rcImage.right*dbRx),(int)(m_rcImage.bottom *dbRy)); //クライアント座標設定
	return rect;
}
//座標設定
void RgnMask::SetClientRect(CRect &rect)
{
	CKIBOView *pView = GetView();
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	m_rcImage.SetRect( (int)(rect.left/dbRx),(int)(rect.top/dbRy),(int)(rect.right/dbRx),(int)(rect.bottom/dbRy));
	m_pBar->PostMessage(m_nID); //コントロール値変更
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
	m_pBar->PostMessage(m_nID); //コントロール値変更
}
void RgnMask::SetImageRect(int nLeft,int nTop,int nRight,int nBottom)
{
	int nXmin = min(nLeft,nRight);
	int nXmax = max(nLeft,nRight);
	int nYmin = min(nTop ,nBottom);
	int nYmax = max(nTop ,nBottom);
	m_rcImage.SetRect(nXmin,nYmin,nXmax,nYmax);
	m_pBar->PostMessage(m_nID); //コントロール値変更
}
//マスク表示
void RgnMask::Show() 
{
	CKIBOView *pView = GetView(); //View取得
	
	if( m_dlgLT.GetSafeHwnd() == NULL ){ 
		m_dlgLT.Create(SelectDlg::IDD); //左上
		m_dlgRT.Create(SelectDlg::IDD); //右上
		m_dlgLB.Create(SelectDlg::IDD); //左下
		m_dlgRB.Create(SelectDlg::IDD); //右下		
	}
	double dbRx = pView->GetRateCx();
	double dbRy = pView->GetRateCy();
	CRect rect((int)(m_rcImage.left*dbRx),(int)(m_rcImage.top*dbRx),(int)(m_rcImage.right*dbRx),(int)(m_rcImage.bottom *dbRy)); //クライアント座標設定
	pView->ClientToScreen(rect);//Screen座標に変換
	
	m_isMoveOK = false;	//連動禁止
	//移動
	m_dlgLT.Move(rect.left ,rect.top   );
	m_dlgRT.Move(rect.right,rect.top   );
	m_dlgLB.Move(rect.left ,rect.bottom);
	m_dlgRB.Move(rect.right,rect.bottom);
	m_dlgLT.ShowWindow(SW_SHOW);
	m_dlgRT.ShowWindow(SW_SHOW);
	m_dlgLB.ShowWindow(SW_SHOW);
	m_dlgRB.ShowWindow(SW_SHOW);
	m_isMoveOK = true; //連動OK
}

//マスク非表示
void RgnMask::Hide() 
{
	m_dlgLT.ShowWindow(SW_HIDE);
	m_dlgRT.ShowWindow(SW_HIDE);
	m_dlgLB.ShowWindow(SW_HIDE);
	m_dlgRB.ShowWindow(SW_HIDE);	
}
//表示中ですか？
BOOL RgnMask::IsVisible()
{
	if( m_isMoveOK ){
		return m_dlgLT.IsWindowVisible();
	}
	return FALSE;
}
//マスク描画
void RgnMask::Draw(CDC* pDC) 
{
	if( IsVisible() ){
		CPoint ptLT = m_dlgLT.GetClientPoint();
		CPoint ptRB = m_dlgRB.GetClientPoint();

		//前準備
		LOGBRUSH lb; //ブラシ
		lb.lbColor = 0; //色なし
		lb.lbHatch = 0;
		lb.lbStyle = BS_NULL;
		CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
		LOGPEN lp; //指定色のペン
		lp.lopnStyle = PS_SOLID;
		lp.lopnWidth.x = 1;
		lp.lopnColor = COLOR_GREEN; //色指定
		CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得

		CBrush *pBrushOld = pDC->SelectObject(pBrush);		
		CPen *pPenOld = pDC->SelectObject(pPen);		
		
		pDC->Ellipse(ptLT.x,ptLT.y,ptRB.x,ptRB.y);
		//後始末
		pDC->SelectObject(pBrushOld);
		pDC->SelectObject(pPenOld);
	}
}

//選択ダイアログの連動
void RgnMask::MoveSelectDlg(int nType)
{
	if(m_isMoveOK){
		switch(nType){
			case SelectDlg::TYPE_LT: //左上
				{
					CPoint ptLT = m_dlgLT.GetScreenPoint();//スクリーン座標取得
					m_dlgLB.MoveX(ptLT.x);
					m_dlgRT.MoveY(ptLT.y);
				}
				break;
			case SelectDlg::TYPE_RT: //右上
				{
					CPoint ptRT = m_dlgRT.GetScreenPoint();//スクリーン座標取得
					m_dlgRB.MoveX(ptRT.x);
					m_dlgLT.MoveY(ptRT.y);
				}
				break;
			case SelectDlg::TYPE_LB: //左下
				{
					CPoint ptLB = m_dlgLB.GetScreenPoint();//スクリーン座標取得
					m_dlgLT.MoveX(ptLB.x);
					m_dlgRB.MoveY(ptLB.y);
				}
				break;
			case SelectDlg::TYPE_RB: //右下
				{
					CPoint ptRB = m_dlgRB.GetScreenPoint();//スクリーン座標取得
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

