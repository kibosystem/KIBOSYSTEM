#pragma once


// BinaryDlg �_�C�A���O

class BinaryDlg : public InputDlg
{
	DECLARE_DYNAMIC(BinaryDlg)
public:
	CDIB m_dibSrc; //2�l���摜���f�[�^
public:
	BinaryDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~BinaryDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeEdit();
public:
	virtual BOOL OnInitDialog();
};
