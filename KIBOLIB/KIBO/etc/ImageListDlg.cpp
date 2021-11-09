// ImageListDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOView.h"
#include "ImageListDlg.h"

// ImageListDlg ダイアログ

IMPLEMENT_DYNAMIC(ImageListDlg, CDialog)

ImageListDlg::ImageListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ImageListDlg::IDD, pParent)
{

}

ImageListDlg::~ImageListDlg()
{
}

void ImageListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTVIEW, m_ctrList);
}


BEGIN_MESSAGE_MAP(ImageListDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// ImageListDlg メッセージ ハンドラ

//初期化
BOOL ImageListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_dib.Create(DATA_SIZE_COL,DATA_SIZE_ROW); //画像データ格納領域作成
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	
	//▼初期データセット
	CString strItem;
	for(int i=0;i<nW;i++){//幅分のカラム作成
		m_ctrList.InsertColumn(i,_T(""),LVCFMT_RIGHT,30); 
	}
	for(int y=0;y<nH;y++){
		//x==0
		m_ctrList.InsertItem(0,strItem); //x==0の文字列セット		
		for(int x=1;x<nW;x++){
			m_ctrList.SetItemText(0,x,strItem);
		}		
	}
	MoveWindow(640,0,640,480);
	return TRUE;
}

//表示データの書き換え
void ImageListDlg::ResetBitmapData()
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	//▼データ処理
	BYTE *pBits = m_dib.GetBits();
	CString strItem;
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pBits++; //B
			BYTE byG = *pBits++; //G
			BYTE byR = *pBits++; //R
			BYTE byGray = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB);
			strItem.Format(_T("%u"),byGray);
			m_ctrList.SetItemText(nH-y-1,x,strItem);
		}		
	}
}

//サイズ変更
void ImageListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//コントロールのサイズ変更
	if( m_ctrList.GetSafeHwnd() != NULL ){ 
		m_ctrList.MoveWindow(0,0,cx,cy,TRUE);
	}
}

void ImageListDlg::OnClose()
{
	GetParent()->PostMessage(WM_CLOSE);
	CDialog::OnClose();
}
