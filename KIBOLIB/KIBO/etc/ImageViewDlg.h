#pragma once
#include "../resource.h"
#include "../LIB/DIB.h"
#include "afxwin.h"
class CameraEx;

// ImageViewDlg �摜�\���_�C�A���O

class ImageViewDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageViewDlg)
private:
	CameraEx *m_pCamera;	//�J����
	CDIB m_dib;				//�\���摜
public:
	ImageViewDlg(CameraEx *pCamera,CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ImageViewDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_scrollV;
	CScrollBar m_scrollH;
};