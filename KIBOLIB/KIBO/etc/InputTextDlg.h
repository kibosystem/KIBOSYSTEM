#pragma once

// InputTextDlg �_�C�A���O

class InputTextDlg : public CDialog
{
	DECLARE_DYNAMIC(InputTextDlg)
private:
	CString m_strInput; //���͕�����
	CString m_strCaption; //Window�^�C�g��
public:
	InputTextDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~InputTextDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUTTEXTDLG };
	CString& GetInputString(){return m_strInput;} //���͕�����擾
	void SetInputStringArray(CStringArray &sAry); //�����s���������
	void SetCaption(CString s){m_strCaption = s;} //Window�^�C�g���ݒ�
	CString& GetCaption(){return m_strCaption;} //Window�^�C�g���擾
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
