#pragma once

#include <afxcmn.h>
#include "../LIB/Dib.h"

//�摜�����X�g�r���[�ŕ\�����A���̒l��\�����܂��B
// ImageListDlg �_�C�A���O

#define DATA_SIZE_COL 100 //��ő�
#define DATA_SIZE_ROW 100 //�s�ő�

class ImageListDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageListDlg)
private:
	CListCtrl m_ctrList;
public:
	CDIB m_dib; //�f�[�^�摜

public:
	ImageListDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ImageListDlg();

	//�\���f�[�^�̏�������
	void ResetBitmapData();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
