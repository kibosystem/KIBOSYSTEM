// BinaryDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MainFrm.h"
#include "BinaryDlg.h"

// BinaryDlg ダイアログ

IMPLEMENT_DYNAMIC(BinaryDlg, InputDlg)

BinaryDlg::BinaryDlg(CWnd* pParent /*=NULL*/)
: InputDlg(pParent)
{
}

BinaryDlg::~BinaryDlg()
{
}

void BinaryDlg::DoDataExchange(CDataExchange* pDX)
{
	InputDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BinaryDlg, InputDlg)
	ON_EN_CHANGE(IDC_EDIT, OnEnChangeEdit)
END_MESSAGE_MAP()


// BinaryDlg メッセージ ハンドラ


//初期化
BOOL BinaryDlg::OnInitDialog()
{
	//初期値セット
	GetView()->GetDib().Copy(m_dibSrc); //View画像をコピー
	InputDlg::OnInitDialog();	
	return TRUE;
}

//値変更
void BinaryDlg::OnEnChangeEdit()
{
	BYTE byTh = (BYTE)GetDlgItemInt(IDC_EDIT); //値取得
	CKIBOView *pView = GetView();

	CDIB dibBin;
	dibBin.Create(m_dibSrc.GetWidth(), m_dibSrc.GetHeight());
	cv::Mat matSrc;
	matSrc.create(m_dibSrc.GetHeight(), m_dibSrc.GetWidth(), CV_8UC1); //GRAYSCALE
	m_dibSrc.ToMat(matSrc);
	// 二値化
	Mat matBin;
	cv::threshold(matSrc, matBin, byTh, 255, cv::THRESH_BINARY); //
	dibBin.FromMat(matBin); //描画


	pView->GetDib().SetDib(0,0, dibBin);//元に戻す
	//pView->DoBinary(byTh); //２値化
	pView->RedrawWindow();

	int nBlack = pView->GetDib().GetPixels(0); //黒
	int nWhite = pView->GetDib().GetPixels(MAXBYTE); //白
	CString strFormat;
	strFormat.Format(L"黒：%d 白:%d",nBlack,nWhite);
	GetMainFrame()->m_wndStatusBar.SetPaneText(0, strFormat);
}
