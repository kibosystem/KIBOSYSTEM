#pragma once

#ifdef USE_USBIO 

#include <setupapi.h>
extern "C"{
	#include <hidsdi.h>
}
#include "ScriptBase.h"

#pragma comment(lib,"setupapi.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"hid.lib")

//USBIO�̃X�N���v�g�t�@�C��
class IOScript
{
public:
	int m_nPort;			//Port �ԍ�
	int m_nNo;				//���͔ԍ�
	int m_nState;			//ON or OFF
	CString m_strFileName;	//�X�N���v�g�t�@�C��	
public:
	IOScript()
	{
		m_nPort = 0;			//Port �ԍ�
		m_nNo = 0;				//���͔ԍ�
		m_nState = 0;			//ON or OFF
		m_strFileName = L"";	//�X�N���v�g�t�@�C��	
	}
	~IOScript(){}
};

//UsbIO�A�N�Z�X�N���X
class UsbIO : public ScriptBase 
{
private:
	HANDLE		m_hHid;			//USBIO�n���h��
public:
	CArray<IOScript> m_aryIOScript; //USBIO�A���X�N���v�g�t�@�C��
public:
	enum{
		FUNC_READ = 1		//����
		,FUNC_WRITE			//�o��
		,FUNC_INIT			//USBIO�̏�����
		,FUNC_WAIT			//USBIO��ԕω��҂�
		,FUNC_SETTIMER		//USBIO�^�C�}�[�ݒ�
		,FUNC_STARTTIMER	//USBIO�^�C�}�[�J�n
		,FUNC_KILLTIMER		//USBIO�^�C�}�[��~
		,FUNC_MAX			//�R�}���h��
	};
	enum{
		 USBIO_ON  = 0 //ON
		,USBIO_OFF = 1 //OFF
	};
public:
	UsbIO(void);
	virtual ~UsbIO(void);

	HANDLE GetHandle(){return m_hHid;} //USBIO�n���h��
	BOOL IsOK(){return (m_hHid!=NULL);} //USBIO��OK�ł����H

	static UsbIO* GetInstance();
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

	//USBIO��ԕω��҂�
	int Wait(int nPort,int nNo,int nState);	
	void KillTimer();	//�^�C�}�[�I��
	void StartTimer();//�^�C�}�[�ĊJ
private:
	BOOL Init(BYTE byData1,BYTE byData2); //������	
	void Write(int nPort,int nNo,int nState);//�w��o�͔ԍ��ɏ�������
public:
	int  Read(int nPort,int nNo);//�w��ԍ��ǂݍ���	
};

#endif