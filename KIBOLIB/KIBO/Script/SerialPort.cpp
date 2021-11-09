#include "../stdafx.h"
#include <AtlBase.h>
#include <AtlConv.h>
#include "../KIBOView.h"
#include "../KIBODoc.h"
#include "App.h"
#include "SerialPort.h"

static CommandTable s_table[] =
{
	 {SerialPort::FUNC_OPEN		,1,L"Open"}			//シリアルポートオープン
	,{SerialPort::FUNC_CLOSE	,1,L"Close"}		//シリアルポートCLOSE
	,{SerialPort::FUNC_FIND		,1,L"Find"}			//シリアルポート探索
	,{SerialPort::FUNC_SEND		,2,L"Send"}			//データ送信
	,{SerialPort::FUNC_SENDSTX	,1,L"SendSTX"}		//STX送信
	,{SerialPort::FUNC_SENDFORMAT,3,L"SendFormat"}	//データ送信位置指定
	,{SerialPort::FUNC_ISOK		,1,L"IsOK"}			//シリアルポート使用可能ですか？
	,{SerialPort::FUNC_TIMEOUT	,1,L"SetTimeOut"}	//タイムアウト時間設定(usec)
	,{SerialPort::FUNC_COMMRATE	,2,L"SetCommRate"}	//通信速度設定(bps)
	,{SerialPort::FUNC_RECEIVE	,-1,L"Receive"}		//データ受信
	,{SerialPort::FUNC_RECEIVEOUT,1,L"ReceiveOutput"} //データ受信＆受信文字列表示
	,{SerialPort::FUNC_ANSWERWAIT,2,L"AnswerWait"}	//受信待ち
	,{SerialPort::FUNC_MAX		,0,NULL}			//コマンド数
};

static SerialPort *s_pInstance = NULL; //唯一のクラス実体

SerialPort* SerialPort::GetInstance()//実体取得
{
	return s_pInstance;
}

SerialPort::SerialPort(void)
{
	m_isThreadEnd = false;//スレッドは終了しましたか？
	s_pInstance = this;
}

SerialPort::~SerialPort(void)
{
}

//関数名チェック
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

//指定ComポートOpen
BOOL SerialPort::Open(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Portクラス取得
	return (comPort.Open(strPortName) != NULL );
}

//指定ComポートClose
void SerialPort::Close(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Portクラス取得
	comPort.Close();
}

//文字列送信
DWORD SerialPort::Send(const CString &strPortName,const CString &strCommand)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Portクラス取得
	return comPort.SendCommand(strCommand);
}

//受信データ取得
CString SerialPort::Receive(const CString &strPortName)
{
	ComPort &comPort = GetComPort(strPortName); //Serial Portクラス取得
	CString strRecv;
	comPort.RecvString(strRecv); //文字列受信
	return strRecv;
}

//処理実行
STDMETHODIMP SerialPort::Invoke(DISPID dispIdMember	//Function ID 番号
					,REFIID			riid
					,LCID			lcid
					,WORD			wFlags
					,DISPPARAMS		FAR *pDispParams //Script文字列
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
		case FUNC_FIND:		//シリアルポート探索
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
		case FUNC_SENDSTX:	//STX送信
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				ComPort& port = GetComPort(strPortName);
				port.SendSTX();
				return S_OK;
			}
			break;
		case FUNC_SENDFORMAT: //データ送信位置指定
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[2].bstrVal); //Serial Portクラス取得				
				CString strFormat  = pDispParams->rgvarg[1].bstrVal;
				int nPos = pDispParams->rgvarg[0].intVal;
				CString strCommand;
				strCommand.Format(strFormat,nPos);
				comPort.SendCommand(strCommand);
				return S_OK;						
			}
			break;
		case FUNC_ISOK: //Serial Port は使用可能ですか？
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[0].bstrVal); //Serial Portクラス取得				
				ReturnBool(wFlags,pVarResult,comPort.IsOpen());
				return S_OK;
			}
			break;
		case FUNC_TIMEOUT:	//タイムアウト時間設定(usec)
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Portクラス取得				
				int nTime = pDispParams->rgvarg[0].lVal;
				comPort.SetReadTimeOut(nTime);
				return S_OK;
			}
			break;
		case FUNC_COMMRATE:	//通信速度設定(bps)
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Portクラス取得				
				int nBRate = pDispParams->rgvarg[0].lVal;
				comPort.SetCommRate(nBRate);				
				return S_OK;
			}
			break;
		case FUNC_RECEIVE: //データ受信
			if( pDispParams->cArgs == 1 ){ //受信文字列を戻す
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				CString strRecv = Receive(strPortName); //受信データ取得
				ReturnString(wFlags,pVarResult,strRecv); //戻り値セット
			}else if( pDispParams->cArgs == 2 ){ //受信文字列が指定されている
				CString strPortName = pDispParams->rgvarg[1].bstrVal;//
				CString strText = pDispParams->rgvarg[0].bstrVal;//受信予定文字列
				CString strRecv = Receive(strPortName); //受信データ取得
				BOOL isOK = FALSE;
				if( strRecv.Find(strText) >= 0 ){
					isOK = TRUE;
				}
				ReturnBool(wFlags,pVarResult,isOK); //戻り値セット
			}
			return S_OK;
		case FUNC_RECEIVEOUT:	//データ受信＆受信文字列表示
			{
				CString strPortName = pDispParams->rgvarg[0].bstrVal;
				CString strRecv = Receive(strPortName); //受信データ取得
			//	GetFrame()->OutputDlgBarText(strRecv);
			}
			return S_OK;
		case FUNC_ANSWERWAIT: //指定文字長待ち
			{
				ComPort &comPort = GetComPort(pDispParams->rgvarg[1].bstrVal); //Serial Portクラス取得
				int nNum = GetValientInt(&pDispParams->rgvarg[0]);
				comPort.RecvString(nNum); //文字列受信
				while( !comPort.IsEnd() ){
					Sleep(0);
				}
				CString strRecv = comPort.GetRecvString();
				if( strRecv.GetLength() > 0 ){
					ReturnString(wFlags,pVarResult,strRecv); //戻り値セット
				}
				return S_OK;
			}
			break;
	}
	return DISP_E_MEMBERNOTFOUND;
}

//指定ポート名称のCOM Port取得
ComPort& SerialPort::GetComPort(CString strPortName)
{
	return m_mapComPort[strPortName]; //Serial Portクラス取得	
}
ComPort& SerialPort::GetComPort(int nPnum)
{
	CString strPort;
	strPort.Format(L"COM%d",nPnum);
	return GetComPort(strPort);
}

//使用するComPortの決定
BOOL SerialPort::FindComPort(int nStart,int nEnd)
{
	CString strKey = L"COM";
	CString strPort;
	for(int i=nStart;i<=nEnd;i++){		
		strPort.Format(L"COM%d",i);
		if( Open(strPort) ){ //Open成功
			App::GetInstance()->SetNum(strKey,i);
			Close(strPort); //Close
			return TRUE;
		}
	}
	return FALSE;
}