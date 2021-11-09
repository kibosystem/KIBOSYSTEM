// BinaryDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MainFrm.h"
#include "BinaryDlg.h"

// BinaryDlg �_�C�A���O

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


// BinaryDlg ���b�Z�[�W �n���h��


//������
BOOL BinaryDlg::OnInitDialog()
{
	//�����l�Z�b�g
	GetView()->GetDib().Copy(m_dibSrc); //View�摜���R�s�[
	InputDlg::OnInitDialog();	
	return TRUE;
}

//�l�ύX
void BinaryDlg::OnEnChangeEdit()
{
	BYTE byTh = (BYTE)GetDlgItemInt(IDC_EDIT); //�l�擾
	CKIBOView *pView = GetView();

	CDIB dibBin;
	dibBin.Create(m_dibSrc.GetWidth(), m_dibSrc.GetHeight());
	cv::Mat matSrc;
	matSrc.create(m_dibSrc.GetHeight(), m_dibSrc.GetWidth(), CV_8UC1); //GRAYSCALE
	m_dibSrc.ToMat(matSrc);
	// ��l��
	Mat matBin;
	cv::threshold(matSrc, matBin, byTh, 255, cv::THRESH_BINARY); //
	dibBin.FromMat(matBin); //�`��


	pView->GetDib().SetDib(0,0, dibBin);//���ɖ߂�
	//pView->DoBinary(byTh); //�Q�l��
	pView->RedrawWindow();

	int nBlack = pView->GetDib().GetPixels(0); //��
	int nWhite = pView->GetDib().GetPixels(MAXBYTE); //��
	CString strFormat;
	strFormat.Format(L"���F%d ��:%d",nBlack,nWhite);
	GetMainFrame()->m_wndStatusBar.SetPaneText(0, strFormat);
}
