#pragma once

#include "../LIB/DxGraphic.h"

//DirectX FullScreen Mode��p�_�C�A���O
// DirectScreenDlg �_�C�A���O

class DirectScreenDlg : public CDialog
{
	DECLARE_DYNAMIC(DirectScreenDlg)
private:
	CSize	  m_size; //Screen Size
public:
	DXGraphic m_DXGraphic; //DirectX FullScreen Mode
public:
	DirectScreenDlg(CSize size,CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~DirectScreenDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FULLSCREEN };

	void Render(); //�`��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
