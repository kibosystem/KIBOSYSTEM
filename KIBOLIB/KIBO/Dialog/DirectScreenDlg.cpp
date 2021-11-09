// DirectScreenDlg.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "DirectScreenDlg.h"

// DirectScreenDlg �_�C�A���O

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


// DirectScreenDlg ���b�Z�[�W �n���h��

//������
BOOL DirectScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_DXGraphic.InitDXGraphics(this->GetSafeHwnd(),m_size.cx,m_size.cy); //�t���X�N���[�����[�h�ݒ�	
	return TRUE;
}

//�`��
void DirectScreenDlg::Render()
{
	m_DXGraphic.Render();
}