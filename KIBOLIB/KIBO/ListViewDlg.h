#pragma once
#include "afxcmn.h"

// ListViewDlg �_�C�A���O

class ListViewDlg : public CDialog
{
	DECLARE_DYNAMIC(ListViewDlg)

protected:
	CListCtrl m_ctrlList;
	CStringArray m_strAry;
public:
	ListViewDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ListViewDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_LISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

////List�ɕ����摜��\������_�C�A���O
//class CharPictureDlg : public ListViewDlg
//{
//	CharPictureDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
//	virtual ~CharPictureDlg();	
//};