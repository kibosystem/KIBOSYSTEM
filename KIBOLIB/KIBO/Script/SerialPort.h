#pragma once

#include "ScriptBase.h"
#include "../LIB/ComPort.h"
#include <map>
using namespace std;
//
////CaptureWait引数
//class CaptureWait
//{
//public:
//	int m_nNum;			 //受信文字数
//	ComPort *m_pComPort; //Serial通信クラス
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
	bool m_isThreadEnd; //スレッドは終了しましたか？

	enum{
		 FUNC_OPEN  = 1	//シリアルポートオープン
		,FUNC_CLOSE		//シリアルポートCLOSE
		,FUNC_FIND		//シリアルポート探索
		,FUNC_SEND		//データ送信
		,FUNC_SENDSTX	//STX送信
		,FUNC_SENDFORMAT//データ送信位置指定
		,FUNC_ISOK		//シリアルポート使用可能ですか？
		,FUNC_TIMEOUT	//タイムアウト時間設定(usec)
		,FUNC_COMMRATE		//通信速度設定(bps)
		,FUNC_RECEIVE		//データ受信		
		,FUNC_RECEIVEOUT	//データ受信＆受信文字列表示
		,FUNC_ANSWERWAIT	//受信結果待ち
		,FUNC_MAX			//シリアルポートコマンド数
	};

public:
	SerialPort(void);
	virtual ~SerialPort(void);
	static SerialPort* GetInstance();	

	//関数名チェック
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//実行
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID 番号
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script文字列
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);

	BOOL Open (const CString &strPortName); //指定ComポートOpen	
	void Close(const CString &strPortName); //指定ComポートClose
	DWORD Send(const CString &strPortName,const CString &strCommand);//文字列送信	
	CString Receive(const CString &strPortName);//受信データ取得	
	ComPort& GetComPort(int nPnum);//指定ポート名称のCOM Port取得
	ComPort& GetComPort(CString strPortName);//指定ポート名称のCOM Port取得
	BOOL FindComPort(int nStart,int nEnd);//使用可能なComPortの探索
};
