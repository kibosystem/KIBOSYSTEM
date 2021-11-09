#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// ���͗p �_�C�A���O

class InputDlg : public CDialog
{
	DECLARE_DYNAMIC(InputDlg)
private:
	int m_nInput; //���͒l
	int m_nMin;	//�ŏ��l
	int m_nMax;	//�ő�l
public:
	CSpinButtonCtrl m_spinInput;
	CEdit m_editInput;
public:
	InputDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~InputDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUT };

	int  GetInput(){return m_nInput;}	//���͒l�擾
	void SetInput(int n){m_nInput=n;}	//���͏����l�ݒ�

	int GetMin(){return m_nMin;}//�ŏ��l
	int GetMax(){return m_nMax;}//�ő�l
	void SetRange(int nMin,int nMax){//�����W�ݒ�
		m_nMin = nMin;	//�ŏ��l
		m_nMax = nMax;	//�ő�l	
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
