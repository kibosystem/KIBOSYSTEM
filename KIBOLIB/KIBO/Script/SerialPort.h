#pragma once

#include "ScriptBase.h"
#include "../LIB/ComPort.h"
#include <map>
using namespace std;
//
////CaptureWait����
//class CaptureWait
//{
//public:
//	int m_nNum;			 //��M������
//	ComPort *m_pComPort; //Serial�ʐM�N���X
//public:
//	CaptureWait(){
//		m_nNum = 0;
//		m_pComPort = NULL;
//	}
//	~CaptureWait(){}
//};

class SerialPort : public ScriptBase 
{
private:
	map<CString,ComPort> m_mapComPort;

public:
	bool m_isThreadEnd; //�X���b�h�͏I�����܂������H

	enum{
		 FUNC_OPEN  = 1	//�V���A���|�[�g�I�[�v��
		,FUNC_CLOSE		//�V���A���|�[�gCLOSE
		,FUNC_FIND		//�V���A���|�[�g�T��
		,FUNC_SEND		//�f�[�^���M
		,FUNC_SENDSTX	//STX���M
		,FUNC_SENDFORMAT//�f�[�^���M�ʒu�w��
		,FUNC_ISOK		//�V���A���|�[�g�g�p�\�ł����H
		,FUNC_TIMEOUT	//�^�C���A�E�g���Ԑݒ�(usec)
		,FUNC_COMMRATE		//�ʐM���x�ݒ�(bps)
		,FUNC_RECEIVE		//�f�[�^��M		
		,FUNC_RECEIVEOUT	//�f�[�^��M����M������\��
		,FUNC_ANSWERWAIT	//��M���ʑ҂�
		,FUNC_MAX			//�V���A���|�[�g�R�}���h��
	};

public:
	SerialPort(void);
	virtual ~SerialPort(void);
	static SerialPort* GetInstance();	

	//�֐����`�F�b�N
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//���s
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID �ԍ�
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script������
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);

	BOOL Open (const CString &strPortName); //�w��Com�|�[�gOpen	
	void Close(const CString &strPortName); //�w��Com�|�[�gClose
	DWORD Send(const CString &strPortName,const CString &strCommand);//�����񑗐M	
	CString Receive(const CString &strPortName);//��M�f�[�^�擾	
	ComPort& GetComPort(int nPnum);//�w��|�[�g���̂�COM Port�擾
	ComPort& GetComPort(CString strPortName);//�w��|�[�g���̂�COM Port�擾
	BOOL FindComPort(int nStart,int nEnd);//�g�p�\��ComPort�̒T��
};
