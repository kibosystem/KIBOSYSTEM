#pragma once

//Com Port �ʐM�N���X

class ComPort
{
//�����o
private:
	HANDLE	m_hCom;			//Com port �n���h��
	char	m_cTerminal;	//�I�[����
	int		m_nReceiveNum;	//��M��
	bool	m_isEnd;		//�X���b�h�I��
	CString m_strRecive;	//��M������
public:
//�R���X�g���N�^
	ComPort();
	ComPort(LPCTSTR lpszPortName);
//�f�X�g���N�^
	~ComPort();

//�A�N�Z�X�֐�
	void Init();//������
	HANDLE GetHandle(){return m_hCom;} //Com port �n���h��
	BOOL IsOpen(){return (m_hCom != NULL);}
	char GetTerminal(){return m_cTerminal;} //�I�[�����擾
	void SetTerminal(char c){m_cTerminal = c;}    //�I�[�����ݒ�
	int	 GetReceiveNum(){return m_nReceiveNum;} //��M��
	void SetReceiveNum(int n){m_nReceiveNum=n;} //��M��
	bool IsEnd(){return m_isEnd;}		//�X���b�h�I���ł����H
	void SetEnd(bool is){m_isEnd = is;} //�X���b�h��Ԑݒ�
	void SetRecvString(CString &str){m_strRecive=str;} //��M������ݒ�
	CString GetRecvString(){return m_strRecive;} //��M������擾
//���\�b�h
	virtual HANDLE Open(LPCTSTR lpszPortName,DWORD dwDesiredAccess=GENERIC_READ|GENERIC_WRITE); //Com Port���J��
	void Close();

	BOOL SetReadTimeOut(DWORD dwTime);//�ʐM�^�C���A�E�g���Ԑݒ�
	BOOL SetCommRate(DWORD dwBaudRate);//COM Port�]�����x�ݒ�

	DWORD SendCommand(LPCTSTR lpszCommand,DWORD dwSleepTime = 0);	//������𑗐M���܂��B
	DWORD SendChar(BYTE byCode); //����R�[�h���M
	DWORD SendSTX(){return SendChar(0x02);} //STX���M
	BOOL RecvString(CString &strRecv);	//�I�[�����܂Ŏ�M���܂��B
	void RecvString(int nNum);		//�w�蕶�����M���܂��B
};
