// DirectScreenDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "DirectScreenDlg.h"

// DirectScreenDlg ダイアログ

IMPLEMENT_DYNAMIC(DirectScreenDlg, CDialog)

DirectScreenDlg::DirectScreenDlg(CSize size,CWnd* pParent /*=NULL*/)
	: CDialog(DirectScreenDlg::IDD, pParent)
{
	m_size = size;
}

DirectScreenDlg::~DirectScreenDlg()
{
}

void DirectScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DirectScreenDlg, CDialog)
END_MESSAGE_MAP()


// DirectScreenDlg メッセージ ハンドラ

//初期化
BOOL DirectScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_DXGraphic.InitDXGraphics(this->GetSafeHwnd(),m_size.cx,m_size.cy); //フルスクリーンモード設定	
	return TRUE;
}

//描画
void DirectScreenDlg::Render()
{
	m_DXGraphic.Render();
}