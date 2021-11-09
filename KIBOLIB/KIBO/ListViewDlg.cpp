// ListViewDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KIBO.h"
#include "./LIB/globalmgr.h"
#include "./LIB/color.h"
#include "ListViewDlg.h"


// ListViewDlg ダイアログ

IMPLEMENT_DYNAMIC(ListViewDlg, CDialog)

ListViewDlg::ListViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ListViewDlg::IDD, pParent)
{

}

ListViewDlg::~ListViewDlg()
{
}

void ListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
}


BEGIN_MESSAGE_MAP(ListViewDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// ListViewDlg メッセージ ハンドラ

//サイズ変更
void ListViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	
}

//初期化
BOOL ListViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctrlList.InsertColumn(0,_T("項目"),LVCFMT_RIGHT,2048); 
	m_ctrlList.InsertItem(0,L""); //メッセージ表示
	m_ctrlList.ShowScrollBar(SB_BOTH);
	return TRUE; 
}

//オーナードロー描画
void ListViewDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;
	LOGBRUSH lb; //ブラシ
	lb.lbColor = COLOR_GREEN; //色指定
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID;
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);
	CBrush *pBrOld  = pDC->SelectObject(pBrush);
	pDC->Rectangle(rcItem);
	//後始末
	pDC->SelectObject(pBrOld);	
}

//設定
void ListViewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth  = 2048;
	lpMeasureItemStruct->itemHeight = 1536;
	//CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

