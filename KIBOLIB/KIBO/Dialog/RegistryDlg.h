#pragma once
#include "afxwin.h"

// RegistryDlg �_�C�A���O

class RegistryDlg : public CDialog
{
	DECLARE_DYNAMIC(RegistryDlg)

private:
	CListCtrl	m_ctrlList;
public:
	RegistryDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~RegistryDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_REGISTRY };

	
	void CollectItemInt(CStringArray &strItem);//���W�X�g���l���ڈꗗ���W
	void CollectItemString(CStringArray &strItem);//���W�X�g�������񍀖ڈꗗ���W	
	int GetHitCell(int& nRow,int& nCol);//Click���W���擾
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	CEdit m_edit;
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
