#include "../stdafx.h"
#include "../MACADDRESS.h"
#include "AboutDlg.h"

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

AboutDlg::AboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL AboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strVersion;
	strVersion.Format(L"KIBO, Version %s",FILE_VERSION);
	SetDlgItemText(IDC_VERSION,strVersion);
	return TRUE;
}
