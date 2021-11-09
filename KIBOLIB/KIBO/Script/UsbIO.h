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

//USBIOのスクリプトファイル
class IOScript
{
public:
	int m_nPort;			//Port 番号
	int m_nNo;				//入力番号
	int m_nState;			//ON or OFF
	CString m_strFileName;	//スクリプトファイル	
public:
	IOScript()
	{
		m_nPort = 0;			//Port 番号
		m_nNo = 0;				//入力番号
		m_nState = 0;			//ON or OFF
		m_strFileName = L"";	//スクリプトファイル	
	}
	~IOScript(){}
};

//UsbIOアクセスクラス
class UsbIO : public ScriptBase 
{
private:
	HANDLE		m_hHid;			//USBIOハンドル
public:
	CArray<IOScript> m_aryIOScript; //USBIO連動スクリプトファイル
public:
	enum{
		FUNC_READ = 1		//入力
		,FUNC_WRITE			//出力
		,FUNC_INIT			//USBIOの初期化
		,FUNC_WAIT			//USBIO状態変化待ち
		,FUNC_SETTIMER		//USBIOタイマー設定
		,FUNC_STARTTIMER	//USBIOタイマー開始
		,FUNC_KILLTIMER		//USBIOタイマー停止
		,FUNC_MAX			//コマンド数
	};
	enum{
		 USBIO_ON  = 0 //ON
		,USBIO_OFF = 1 //OFF
	};
public:
	UsbIO(void);
	virtual ~UsbIO(void);

	HANDLE GetHandle(){return m_hHid;} //USBIOハンドル
	BOOL IsOK(){return (m_hHid!=NULL);} //USBIOはOKですか？

	static UsbIO* GetInstance();
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

	//USBIO状態変化待ち
	int Wait(int nPort,int nNo,int nState);	
	void KillTimer();	//タイマー終了
	void StartTimer();//タイマー再開
private:
	BOOL Init(BYTE byData1,BYTE byData2); //初期化	
	void Write(int nPort,int nNo,int nState);//指定出力番号に書き込み
public:
	int  Read(int nPort,int nNo);//指定番号読み込み	
};

#endif