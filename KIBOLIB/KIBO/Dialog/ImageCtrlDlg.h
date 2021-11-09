#pragma once

#include "../LIB/dib.h"

// ImageCtrlDlg �_�C�A���O

class ImageCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageCtrlDlg)
private:
	CDIB *m_pDib; //�\���r�b�g�}�b�v�|�C���^
	CListCtrl m_ctrlList;
public:
	ImageCtrlDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ImageCtrlDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGECTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);	
};
