// InputDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../enum.h"
#include "../KIBO.h"
#include "InputDlg.h"

// InputDlg ダイアログ

IMPLEMENT_DYNAMIC(InputDlg, CDialog)

InputDlg::InputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InputDlg::IDD, pParent)
{

}

InputDlg::~InputDlg()
{
}

void InputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN, m_spinInput);
	DDX_Control(pDX, IDC_EDIT, m_editInput);
}

BEGIN_MESSAGE_MAP(InputDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, &InputDlg::OnDeltaposSpin)
END_MESSAGE_MAP()

// InputDlg メッセージ ハンドラ

//初期化
BOOL InputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strInput;
	strInput.Format(_T("%d"),GetInput());
	m_editInput.SetWindowTextW(strInput);
	m_spinInput.SetPos32( GetInput() ); //入力値セット
	m_spinInput.SetRange32(GetMin(),GetMax()); //レンジ設定
	return TRUE;
}

void InputDlg::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strInput; //現在値取得
	m_editInput.GetWindowText(strInput);
	int nNow = _wtoi(strInput); 
	if( pNMUpDown->iDelta > 0 ){
		nNow += 1;
	}else if( pNMUpDown->iDelta < 0 ){
		nNow -= 1;
	}
	strInput.Format(_T("%d"),nNow);
	m_editInput.SetWindowText(strInput);
	*pResult = 0;
}

