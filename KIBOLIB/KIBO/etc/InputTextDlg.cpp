// InputTextDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "InputTextDlg.h"


// InputTextDlg ダイアログ

IMPLEMENT_DYNAMIC(InputTextDlg, CDialog)

InputTextDlg::InputTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InputTextDlg::IDD, pParent)
{
}

InputTextDlg::~InputTextDlg()
{
}

void InputTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_strInput);
}


BEGIN_MESSAGE_MAP(InputTextDlg, CDialog)
END_MESSAGE_MAP()


// InputTextDlg メッセージ ハンドラ

//初期化
BOOL InputTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText( GetCaption() ); //Windowタイトル設定
	return TRUE;
}

//複数行文字列入力
void InputTextDlg::SetInputStringArray(CStringArray &sAry)
{
	if( sAry.GetSize() > 0){
		m_strInput = sAry[0];
		for(int i=1;i<sAry.GetSize();i++){
			m_strInput += "\r\n";
			m_strInput += sAry[i];
		}
	}
}
