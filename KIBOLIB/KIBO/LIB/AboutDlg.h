#pragma once

class AboutDlg : public CDialog
{
public:
	AboutDlg();

// �_�C�A���O �f�[�^
//	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
