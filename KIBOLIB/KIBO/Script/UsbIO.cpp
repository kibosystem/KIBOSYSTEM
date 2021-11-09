#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOFrame.h"
#include "../enum.h"
#include "UsbIO.h"
#include "App.h"

#ifdef USE_USBIO 

//旧型USBIO
//#define VENDOR_ID				0x1352
//#define PRODUCT_ID			0x0100

//テクノキット製USBIO
#define VENDOR_ID				0x12ED
#define PRODUCT_ID				0x1003
#define MAX_ENUM_INDEX			100

HANDLE OpenUsbIo();
BYTE InputUsbIo(HANDLE hHid, int iPort);
BOOL OutputUsbIo(HANDLE hHid,int iPort, int iData);

//USBIO入力待ちタイマー
static void CALLBACK EXPORT TimerProc(HWND hWnd,UINT nMsg,UINT_PTR unID,DWORD dwTime)
{
	UsbIO *pUsbIO = UsbIO::GetInstance();
	if(pUsbIO == NULL){
		return ;
	}
	CArray<IOScript> &aryIO = pUsbIO->m_aryIOScript;
	for(int i=0;i<aryIO.GetSize();i++){
		IOScript& io = aryIO[i];
		int nRet = pUsbIO->Read(io.m_nPort,io.m_nNo);
		if( nRet == io.m_nState ){ //入力が一致した
			//スクリプト実行
			//GetFrame()->ScriptExec(io.m_strFileName);
		}
	}
	//特殊処理///////////////////////////////////////////
	App *pApp = App::GetInstance();
	if( pUsbIO->Read(1,2) == 0){ //IN_RESET=2;  //USBIO入力によるリセット
		pApp->SetNum(L"NGFLAG",0);
	}else if( pUsbIO->Read(1,3) == 0 ){//扉STOP
		pApp->SetNum(L"STOPFLAG",1);
	}
}

static CommandTable s_table[] =
{
	 {UsbIO::FUNC_READ	,2,L"Read"}	//読み込み
	,{UsbIO::FUNC_WRITE	,3,L"Write"}//出力
	,{UsbIO::FUNC_INIT	,2,L"Init"}	//USBIOの初期化
	,{UsbIO::FUNC_WAIT	,3,L"Wait"}	//USBIO状態変化待ち
	,{UsbIO::FUNC_SETTIMER ,4,L"SetTimer"}//USBIOタイマー
	,{UsbIO::FUNC_STARTTIMER,0,L"StartTimer"} //USBIOタイマー開始
	,{UsbIO::FUNC_KILLTIMER,0,L"KillTimer"}//USBIOタイマー
	,{UsbIO::FUNC_MAX	,0,NULL}	//コマンド数
};

static UsbIO *s_pInstance = NULL; //唯一のクラス実体
UsbIO* UsbIO::GetInstance()//実体取得
{
	return s_pInstance;
}

UsbIO::UsbIO(void)
{
	s_pInstance = this;
	m_hHid = NULL;
}

UsbIO::~UsbIO(void)
{
	if( m_hHid != NULL ){
		CloseHandle(m_hHid); 
		m_hHid = NULL;
	}
}

//関数名チェック
STDMETHODIMP UsbIO::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
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

//実行
STDMETHODIMP UsbIO::Invoke(DISPID dispIdMember	//Function ID 番号
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script文字列
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr)
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //引数チェック
		return S_FALSE;
	}		

	switch(dispIdMember){
		case FUNC_READ:	//読み込み
			{
				int nPort = GetValientInt(&pDispParams->rgvarg[1]); //Port 番号
				int nNo = GetValientInt(&pDispParams->rgvarg[0]);	//入力番号
				int nRet = Read(nPort,nNo); //指定番号読み込み
				ReturnInt(wFlags,pVarResult,nRet); //Int
			}
			break;
		case FUNC_WRITE://出力
			{
				int nPort  = GetValientInt(&pDispParams->rgvarg[2]); //Port 番号
				int nNo	   = GetValientInt(&pDispParams->rgvarg[1]); //入力番号
				int nState = GetValientInt(&pDispParams->rgvarg[0]); //ON or OFF
				Write(nPort,nNo,nState); //指定出力番号に書き込み
			}
			break;
		case FUNC_INIT:	//USBIOの初期化
			{
				BYTE byData1 = (BYTE)pDispParams->rgvarg[1].ulVal; //データ１
				BYTE byData2 = (BYTE)pDispParams->rgvarg[0].ulVal; //データ２
				BOOL isOK = Init(byData1,byData2);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_WAIT:	//USBIO状態変化待ち
			{
				int nPort  = GetValientInt(&pDispParams->rgvarg[2]); //Port 番号
				int nNo	   = GetValientInt(&pDispParams->rgvarg[1]); //入力番号
				int nState = GetValientInt(&pDispParams->rgvarg[0]); //ON or OFF
				int nStatus = Wait(nPort,nNo,nState); //状態待ち
				ReturnInt(wFlags,pVarResult,nStatus);
			}
			break;
		case FUNC_SETTIMER: //USBIOタイマー開始
			{
				IOScript io;
				io.m_nPort  = GetValientInt(&pDispParams->rgvarg[3]); //Port 番号
				io.m_nNo	= GetValientInt(&pDispParams->rgvarg[2]); //入力番号
				io.m_nState = GetValientInt(&pDispParams->rgvarg[1]); //ON or OFF
				io.m_strFileName = pDispParams->rgvarg[0].bstrVal; //スクリプトファイル				
				m_aryIOScript.Add(io);				
			}
			break;		
		case FUNC_STARTTIMER: //USBIOタイマー再開
			StartTimer();
			break;
		case FUNC_KILLTIMER: //USBIOタイマー
			KillTimer(); //タイマー終了
			break;

	}
	return S_OK;
}

//タイマー終了
void UsbIO::KillTimer()
{
	//GetFrame()->KillTimer(FUNC_SETTIMER); //タイマー終了
}
//タイマー再開
void UsbIO::StartTimer()
{
	//GetFrame()->SetTimer(FUNC_SETTIMER,10,TimerProc); //タイマー開始
}

//USBIO 初期化
BOOL UsbIO::Init(BYTE byData1,BYTE byData2)
{
	m_hHid = OpenUsbIo(); //USB IO Open	
	if( m_hHid != NULL ){
		OutputUsbIo(m_hHid, 0, byData1); //Port0初期化
		OutputUsbIo(m_hHid, 1, byData2); //Port1初期化
		return TRUE;
	}
	return FALSE;
}

//指定番号読み込み
int UsbIO::Read(int nPort,int nNo) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		BYTE byInput = InputUsbIo(hHid, nPort);
		if( (byInput & (1 << nNo)) == USBIO_ON ){ //フラグチェック
			return USBIO_ON; //ONです
		}
	}		
	return USBIO_OFF; //OFFです
}

//指定出力番号に書き込み
void UsbIO::Write(int nPort,int nNo,int nState) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		BYTE byInput = InputUsbIo(hHid, nPort);
		if( nState == USBIO_ON ){
			byInput &= ~(1<<nNo); //指定出力番号のみ０にする
		}else{
			byInput |= (1<<nNo); //指定出力番号のみ1にする
		}
		OutputUsbIo(m_hHid, nPort, byInput);
	}
}
//USBIO状態変化待ち
int UsbIO::Wait(int nPort,int nNo,int nState) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		KIBOFrame *pFrame = GetFrame();
		MSG msg;
		while( IsWindowOK() && Read(nPort,nNo) != nState ){ //nStateになるまでWait
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				if(msg.message == WM_CLOSE){
					return FALSE; //強制終了
				}
				if( msg.message == WM_KEYUP ){
					CString strFilePath;
					if( pFrame->m_mapKeyFilePath.Lookup((int)msg.wParam,strFilePath) ){ //登録されたキー入力ならば
						return -1;
					}
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return TRUE;//正常終了
	}
	return FALSE;//異常終了
}

//---------------------------------------------------------
//関数		：USB-IOオープン
//概要		：接続中のUSB-IOを全てオープン
//戻り値	：USB-IOのファイルハンドル
//---------------------------------------------------------
static HANDLE OpenUsbIo()
{
	// Get hid GUID
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid); //HID デバイスのGUID コードを取得する。

	// Open PnP Handle
	// デバイスが列挙されたリストへの、ハンドルを取得する。
	HDEVINFO hdevinfo = SetupDiGetClassDevs( &hidGuid,NULL,NULL,(DIGCF_PRESENT|DIGCF_INTERFACEDEVICE) );
	if( hdevinfo == INVALID_HANDLE_VALUE ){
		_tprintf( _T("Error : SetupDiGetClassDevs() [%d]\n"), GetLastError() );
		return FALSE;
	}
	
	for(DWORD dwIndex=0; dwIndex<MAX_ENUM_INDEX; dwIndex++ ){ //全デバイス検索
		// Enum hid devices
		SP_INTERFACE_DEVICE_DATA device_data;
		ZeroMemory( &device_data, sizeof(device_data) );
		device_data.cbSize = sizeof(device_data);
		//リストの先頭から順にHIDデバイスの情報を取得する。
		if( !SetupDiEnumDeviceInterfaces( hdevinfo, NULL, &hidGuid, dwIndex, &device_data ) ){
			break ; //これ以上デバイスなし
		}

		// Request SP_DEVICE_INTERFACE_DETAIL_DATA size		
		DWORD dwNeeded = 0;
		//デバイスの詳細情報サイズを求める
		SetupDiGetDeviceInterfaceDetail( hdevinfo, &device_data, NULL, 0, &dwNeeded, NULL );
		DWORD dwSize = dwNeeded;
		if( dwSize > 1024 ){ //デバイスサイズエラー
			continue ;
		}
		BYTE byData[1024]; //一時的なデータ格納領域
		PSP_DEVICE_INTERFACE_DETAIL_DATA pdevice_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)byData;
		pdevice_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//デバイスの詳細情報からWindows 内部のデバイス名を取得する。
		// Get SP_DEVICE_INTERFACE_DETAIL_DATA
		if( SetupDiGetDeviceInterfaceDetail( hdevinfo, &device_data, pdevice_detail, dwSize, &dwNeeded, NULL ) ){
			// for w2k patch
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = FALSE;

			//HIDデバイスをオープンし、ファイルハンドルを取得する。
			HANDLE hHid = CreateFile(pdevice_detail->DevicePath
									,GENERIC_READ|GENERIC_WRITE
									,FILE_SHARE_READ|FILE_SHARE_WRITE
									,&sa
									, OPEN_EXISTING, 0, NULL );

			if( hHid != INVALID_HANDLE_VALUE ){
				HIDD_ATTRIBUTES hidattr;
				BOOL bRet = HidD_GetAttributes( hHid, &hidattr ); //HIDデバイス属性の取得

				if( hidattr.VendorID == VENDOR_ID && hidattr.ProductID == PRODUCT_ID){
					return hHid; //ハンドル取得
				}else{
					CloseHandle(hHid); //対称外のハンドルクローズ
				}
			}
		}
	}
	return NULL;
}

static PHIDP_PREPARSED_DATA GetHidData(HANDLE hHid, HIDP_CAPS &hidpCaps)
{
	PHIDP_PREPARSED_DATA phidpData;
	if( HidD_GetPreparsedData(hHid, &phidpData) ){ //HID デバイスの能力を取得
		NTSTATUS ntstat = HidP_GetCaps(phidpData, &hidpCaps);
		if( ntstat == HIDP_STATUS_SUCCESS ){
			_tprintf(_T("Info : Success HidP_GetCaps()\n"));
			_tprintf(_T("-----\n"));
			_tprintf(_T("  Usage     : [%d]\n"), hidpCaps.Usage );
			_tprintf(_T("  UsagePage : [%d]\n"), hidpCaps.UsagePage);
			_tprintf(_T("  InputReportByteLength    : [%d]\n"), hidpCaps.InputReportByteLength);
			_tprintf(_T("  OutputReportByteLength   : [%d]\n"), hidpCaps.OutputReportByteLength);
			_tprintf(_T("  InputReportByteLength    : [%d]\n"), hidpCaps.InputReportByteLength);
			_tprintf(_T("  OutputReportByteLength   : [%d]\n"), hidpCaps.OutputReportByteLength);
			_tprintf(_T("  FeatureReportByteLength  : [%d]\n"), hidpCaps.FeatureReportByteLength);
			_tprintf(_T("  NumberLinkCollectionNodes: [%d]\n"), hidpCaps.NumberLinkCollectionNodes);
			_tprintf(_T("  NumberInputButtonCaps    : [%d]\n"), hidpCaps.NumberInputButtonCaps);
			_tprintf(_T("  NumberInputValueCaps     : [%d]\n"), hidpCaps.NumberInputValueCaps);
			_tprintf(_T("  NumberInputDataIndices   : [%d]\n"), hidpCaps.NumberInputDataIndices);
			_tprintf(_T("  NumberOutputButtonCaps   : [%d]\n"), hidpCaps.NumberOutputButtonCaps);
			_tprintf(_T("  NumberOutputValueCaps    : [%d]\n"), hidpCaps.NumberOutputValueCaps);
			_tprintf(_T("  NumberOutputDataIndices  : [%d]\n"), hidpCaps.NumberOutputDataIndices);
			_tprintf(_T("  NumberFeatureButtonCaps  : [%d]\n"), hidpCaps.NumberFeatureButtonCaps);
			_tprintf(_T("  NumberFeatureValueCaps   : [%d]\n"), hidpCaps.NumberFeatureValueCaps);
			_tprintf(_T("  NumberFeatureDataIndices : [%d]\n"), hidpCaps.NumberFeatureDataIndices);
			_tprintf(_T("-----\n"));
			return phidpData;
		}
	}
	return NULL;
}


//---------------------------------------------------------
//関数		：USB-IOへ送信
//引数		：int ハンドル
//			：int コマンド
//			：int データ
//			：int カウンタ
//概要		：USB-IOへコマンドを送信
//戻り値	：FALSE=NG	0以外 OK
//---------------------------------------------------------
static BOOL SendUsbIoCmd(HANDLE hHid, int iCmd, int iData, int iCounter)
{
	BYTE byData[9]; //9Byte 送信
	byData[0] = 0x00;
	byData[1] = iCmd & 0xff;
	byData[2] = iData & 0xff;
	byData[3] = 0xF3;
	byData[4] = 0xF4;
	byData[5] = 0xF5;
	byData[6] = 0xF6;
	byData[7] = iCounter & 0xff;
	byData[8] = 0xF8;

	DWORD dwWritten = 9;
	if( !WriteFile( hHid, byData, dwWritten, &dwWritten, NULL) ){
		char szMsgBuf[1024];
		ZeroMemory(szMsgBuf,sizeof(szMsgBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS
						,NULL
						,GetLastError()
						,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // 既定の言語
						,(LPTSTR) szMsgBuf
						,0
						,NULL
						);
		_tprintf(_T("Error: %hs\n"),szMsgBuf);
	}
	return (dwWritten==sizeof(byData));
}

//---------------------------------------------------------
//関数		：USB-IO 出力
//引数		：HANDLE hHid オープンしたUsbIo Handle
//			：int iPort		出力ポート 0 or 1
//			：int iData		出力データ
//概要		：USB-IO の指定ポートを出力
//戻り値	：TRUE 出力成功 その他エラー
//---------------------------------------------------------
static BOOL OutputUsbIo(HANDLE hHid,int iPort, int iData)
{
	if( iPort == 0 || iPort == 1 ){
		if( (iData > 0xff) || (iData < 0) ){ //Data Size Check
			return FALSE;
		}
		return SendUsbIoCmd( hHid,iPort+1,iData,0 ); //Outputの場合はPort番号+1 == コマンド番号
	}
	return FALSE;
}


//---------------------------------------------------------
//関数		：USB-IOデータ読み込み
//引数		：int ハンドル
//			：int データ識別カウンタ
//概要		：USB-IOへコマンドを送信
//戻り値	：0 以上 入力値		0 未満 エラー
//---------------------------------------------------------
static BYTE ReadUsbIoData(HANDLE hHid, int iCounter)
{
	BYTE byRData[9]; //9Byte Data
	DWORD dwRead = 0;
	ReadFile(hHid, byRData, 9, &dwRead , 0); //Data読み込み

	for(int iWaitRetry=0;iWaitRetry<15;iWaitRetry++){
		for(int iImmediateRetry=0;iImmediateRetry<10;iImmediateRetry++){
			ReadFile(hHid, byRData, 9, &dwRead , 0); //Data読み込み
			int iRetCounter = byRData[7];
			int iRetCmd		= byRData[1];
			if( iRetCounter == iCounter && (iRetCmd == 3 || iRetCmd == 4) ){
				ReadFile(hHid, byRData, 9, &dwRead , 0); //Data読み込み
				return byRData[2];
			}
		}
		Sleep(1);
	}
	
	return 0;
}


//---------------------------------------------------------
//関数		：USB-IO リード
//引数		：int iUsbioNo	オープンしたUsbIo 0～
//			：int iPort		入力ポート 0 or 1
//概要		：指定したUSB-IO番号接続中のUSB-IOを全てオープン
//戻り値	：0 以上 入力値		0 未満 エラー
//---------------------------------------------------------
static BYTE InputUsbIo(HANDLE hHid, int iPort)
{
	static int s_iUsbIoSendCnt = 0;

	int iCmd = 0;
	switch(iPort){ //Input コマンドに変更
		case 0:
			iCmd = 3;
			break;
		case 1:
			iCmd = 4;
			break;
		default:
			return 0;
	}

	s_iUsbIoSendCnt++;
	if (s_iUsbIoSendCnt > 0xff) {
		s_iUsbIoSendCnt = 1;
	}

	if( !SendUsbIoCmd( hHid, iCmd, 0, s_iUsbIoSendCnt) ){
		return 0;
	}
	return ReadUsbIoData(hHid, s_iUsbIoSendCnt);
}

#endif