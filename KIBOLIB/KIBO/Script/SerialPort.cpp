#include "../stdafx.h"
#include <AtlBase.h>
#include <AtlConv.h>
#include "../KIBOView.h"
#include "../KIBODoc.h"
#include "App.h"
#include "SerialPort.h"

static CommandTable s_table[] =
{
	 {SerialPort::FUNC_OPEN		,1,L"Open"}			//�V���A���|�[�g�I�[�v��
	,{SerialPort::FUNC_CLOSE	,1,L"Close"}		//�V���A���|�[�gCLOSE
	,{SerialPort::FUNC_FIND		,1,L"Find"}			//�V���A���|�[�g�T��
	,{SerialPort::FUNC_SEND		,2,L"Send"}			//�f�[�^���M
	,{SerialPort::FUNC_SENDSTX	,1,L"SendSTX"}		//STX���M
	,{SerialPort::FUNC_SENDFORMAT,3,L"SendFormat"}	//�f�[�^���M�ʒu�w��
	,{SerialPort::FUNC_ISOK		,1,L"IsOK"}			//�V���A���|�[�g�g�p�\�ł����H
	,{SerialPort::FUNC_TIMEOUT	,1,L"SetTimeOut"}	//�^�C���A�E�g���Ԑݒ�(usec)
	,{SerialPort::FUNC_COMMRATE	,2,L"SetCommRate"}	//�ʐM���x�ݒ�(bps)
	,{SerialPort::FUNC_RECEIVE	,-1,L"Receive"}		//�f�[�^��M
	,{SerialPort::FUNC_RECEIVEOUT,1,L"ReceiveOutput"} //�f�[�^��M����M������\��
	,{SerialPort::FUNC_ANSWERWAIT,2,L"AnswerWait"}	//��M�҂�
	,{SerialPort::FUNC_MAX		,0,NULL}			//�R�}���h��
};

static SerialPort *s_pInstance = NULL; //�B��̃N���X����

SerialPort* SerialPort::GetInstance()//���̎擾
{
	return s_pInstance;
}

SerialPort::SerialPort(void)
{
	m_isThreadEnd = false;//�X���b�h�͏I�����܂������H
	s_pInstance = this;
}

SerialPort::~SerialPort(void)
{
}

//�֐����`�F�b�N
STDMETHODIMP SerialPort::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId) 
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//�w��Com�|�[�gOpen
BOOL SerialPort::Open(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Port�N���X�擾
	return (comPort.Open(strPortName) != NULL );
}

//�w��Com�|�[�gClose
void SerialPort::Close(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Port�N���X�擾
	comPort.Close();
}

//�����񑗐M
DWORD SerialPort::Send(const CString &strPortName,const CString &strCommand)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Port�N���X�擾
	return comPort.SendCommand(strCommand);
}

//��M�f�[�^�擾
CString SerialPort::Receive(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Port�N���X�擾
	CString strRecv;
	comPort.RecvString(strRecv); //�������M
	return strRecv;
}

//�������s
STDMETHODIMP SerialPort::Invoke(DISPID dispIdMember	//Function ID �ԍ�
					,REFIID			riid
					,LCID			lcid
					,WORD			wFlags
					,DISPPARAMS		FAR *pDispParams //Script������
					,VARIANT		FAR *pVarResult
					,EXCEPINFO		FAR *pExcepInfo
					,unsigned int	FAR *puArgErr) 
{
	switch(dispIdMember){
		case FUNC_OPEN: //Serial Port Open
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				BOOL isOpen = Open(strPortName);
				ReturnBool( wFlags , pVarResult,isOpen);
				return S_OK;
			}
			break;
		case FUNC_CLOSE:
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				Close(strPortName);
				return S_OK;				
			}
			break;	
		case FUNC_FIND:		//�V���A���|�[�g�T��
			{
				int nStart = pDispParams->rgvarg[1].lVal;
				int nEnd = pDispParams->rgvarg[0].lVal;
				BOOL isOpen = FindComPort(nStart,nEnd);
				ReturnBool( wFlags , pVarResult,isOpen);
				return S_OK;
			}
			break;
		case FUNC_SEND:
			{
				CString strPortName = pDispParams->rgvarg[1].bstrVal;
				CString strCommand = pDispParams->rgvarg[0].bstrVal;
				Send(strPortName,strCommand);
				return S_OK;
			}
			break;
		case FUNC_SENDSTX:	//STX���M
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				ComPort& port = GetComPort(strPortName);
				port.SendSTX();
				return S_OK;
			}
			break;
		case FUNC_SENDFORMAT: //�f�[�^���M�ʒu�w��
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[2].bstrVal); //Serial Port�N���X�擾				
				CString strFormat  = pDispParams->rgvarg[1].bstrVal;
				int nPos = pDispParams->rgvarg[0].intVal;
				CString strCommand;
				strCommand.Format(strFormat,nPos);
				comPort.SendCommand(strCommand);
				return S_OK;						
			}
			break;
		case FUNC_ISOK: //Serial Port �͎g�p�\�ł����H
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[0].bstrVal); //Serial Port�N���X�擾				
				ReturnBool(wFlags,pVarResult,comPort.IsOpen());
				return S_OK;
			}
			break;
		case FUNC_TIMEOUT:	//�^�C���A�E�g���Ԑݒ�(usec)
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Port�N���X�擾				
				int nTime = pDispParams->rgvarg[0].lVal;
				comPort.SetReadTimeOut(nTime);
				return S_OK;
			}
			break;
		case FUNC_COMMRATE:	//�ʐM���x�ݒ�(bps)
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Port�N���X�擾				
				int nBRate = pDispParams->rgvarg[0].lVal;
				comPort.SetCommRate(nBRate);				
				return S_OK;
			}
			break;
		case FUNC_RECEIVE: //�f�[�^��M
			if( pDispParams->cArgs == 1 ){ //��M�������߂�
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				CString strRecv = Receive(strPortName); //��M�f�[�^�擾
				ReturnString(wFlags,pVarResult,strRecv); //�߂�l�Z�b�g
			}else if( pDispParams->cArgs == 2 ){ //��M�����񂪎w�肳��Ă���
				CString strPortName = pDispParams->rgvarg[1].bstrVal;//
				CString strText = pDispParams->rgvarg[0].bstrVal;//��M�\�蕶����
				CString strRecv = Receive(strPortName); //��M�f�[�^�擾
				BOOL isOK = FALSE;
				if( strRecv.Find(strText) >= 0 ){
					isOK = TRUE;
				}
				ReturnBool(wFlags,pVarResult,isOK); //�߂�l�Z�b�g
			}
			return S_OK;
		case FUNC_RECEIVEOUT:	//�f�[�^��M����M������\��
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				CString strRecv = Receive(strPortName); //��M�f�[�^�擾
			//	GetFrame()->OutputDlgBarText(strRecv);
			}
			return S_OK;
		case FUNC_ANSWERWAIT: //�w�蕶�����҂�
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Port�N���X�擾
				int nNum = GetValientInt(&pDispParams->rgvarg[0]);
				comPort.RecvString(nNum); //�������M
				while( !comPort.IsEnd() ){
					Sleep(0);
				}
				CString strRecv = comPort.GetRecvString();
				if( strRecv.GetLength() > 0 ){
					ReturnString(wFlags,pVarResult,strRecv); //�߂�l�Z�b�g
				}
				return S_OK;
			}
			break;
	}
	return DISP_E_MEMBERNOTFOUND;
}

//�w��|�[�g���̂�COM Port�擾
ComPort& SerialPort::GetComPort(CString strPortName)
{
	return m_mapComPort[strPortName]; //Serial Port�N���X�擾	
}
ComPort& SerialPort::GetComPort(int nPnum)
{
	CString strPort;
	strPort.Format(L"COM%d",nPnum);
	return GetComPort(strPort);
}

//�g�p����ComPort�̌���
BOOL SerialPort::FindComPort(int nStart,int nEnd)
{
	CString strKey = L"COM";
	CString strPort;
	for(int i=nStart;i<=nEnd;i++){		
		strPort.Format(L"COM%d",i);
		if( Open(strPort) ){ //Open����
			App::GetInstance()->SetNum(strKey,i);
			Close(strPort); //Close
			return TRUE;
		}
	}
	return FALSE;
}