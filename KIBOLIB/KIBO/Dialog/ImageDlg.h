#pragma once
#include "../LIB/dib.h"

// ImageDlg �_�C�A���O

class ImageDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageDlg)
public:
	bool m_isSizing; //�T�C�Y�ύX�\�ł����H
	int  m_nAlpha;   //�����x
	CDIB m_dib; //�`��r�b�g�}�b�v
	CScrollBar m_scrollV;
	CScrollBar m_scrollH;
public:
	ImageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ImageDlg();

	virtual void SetBitmap(CDIB &dib){
		dib.Copy(m_dib);
	}
// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
