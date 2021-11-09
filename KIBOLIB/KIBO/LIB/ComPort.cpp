//Com Port通信クラス

#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOFrame.h"
#include "comport.h"

ComPort::ComPort(void)
{
	Init(); //初期化
}

ComPort::ComPort(LPCTSTR lpszPortName)
{
	Init(); //初期化
	Open(lpszPortName,GENERIC_ALL);
}

ComPort::~ComPort(void)
{
	Close();
}

//初期化
void ComPort::Init()
{
	m_hCom = NULL;
	m_cTerminal = '\n';
	m_nReceiveNum=0; //受信数
}
//Com Portを開く
HANDLE ComPort::Open(LPCTSTR lpszPortName,DWORD dwDesiredAccess/* = GENERIC_READ|GENERIC_WRITE*/)
{
	m_hCom = ::CreateFile(lpszPortName		// ファイル名
						,dwDesiredAccess	// アクセスモード
						,0					// 共有モード
						,NULL				// セキュリティ記述子
						,OPEN_EXISTING		// 作成方法
						,0					// ファイル属性
						,NULL);				// テンプレートファイルのハンドル

	if( m_hCom == INVALID_HANDLE_VALUE ){
		return NULL;
	}
	return m_hCom;
}

void ComPort::Close()
{
	if( m_hCom != NULL ){
		::CloseHandle(m_hCom);
		m_hCom = NULL;
	}
}

//通信タイムアウト時間設定
BOOL ComPort::SetReadTimeOut(DWORD dwTime)
{
	if( m_hCom != NULL ){
		COMMTIMEOUTS commTimeouts;
		ZeroMemory(&commTimeouts,sizeof(COMMTIMEOUTS));
		commTimeouts.ReadTotalTimeoutConstant = dwTime; //時間切れ指定
		return ::SetCommTimeouts(m_hCom,&commTimeouts);	
	}
	return FALSE;
}

//COM Port転送速度設定
BOOL ComPort::SetCommRate(DWORD dwBaudRate)
{
	if( m_hCom != NULL ){
		DCB dcb;
		GetCommState(m_hCom, &dcb); /* DCB を取得 */
		dcb.DCBlength = sizeof(DCB);
		dcb.BaudRate = dwBaudRate;
		dcb.fBinary = TRUE;
		dcb.ByteSize = 8;
		dcb.fParity = NOPARITY;		//  パリティ有無種類　：　パリティなしの場合はNOPARITYを指定
									//                                     奇数パリティの場合は ODDPARITY 　他
		dcb.StopBits = ONESTOPBIT;	// ストップビットの数：　通常１ビット→ ONESTOPBIT;
		//ハードウェアフロー制御
		dcb.fOutxCtsFlow = FALSE;       // 　CTSハードウェアフロー制御：CTS制御を使用しない場合はFLASEを指定
										//   　　　　　　CTS 制御をする場合はTRUEを指定してCTS信号を監視します。                     
		dcb.fOutxDsrFlow = FALSE;       //  DSRハードウェアフロー制御：使用しない場合はFALSEを指定
		dcb.fDtrControl = DTR_CONTROL_DISABLE;	// DTR有効/無効：　無効なら　DTR_CONTROL_DISABLE;ISABLE
		dcb.fRtsControl = RTS_CONTROL_DISABLE;	// RTS制御：RTS制御をしない場合はRTS_CONTROL_DISABLEを指定
												//　　　　　RTS制御をする場合はRTS_CONTROL_ENABLE　等を指定

		// ソフトウェアフロー制御
		dcb.fOutX = FALSE;					// 送信時XON/OFF制御の有無：　なし→FLALSE  
		dcb.fInX = FALSE;                   // 受信時XON/XOFF制御の有無：なし→FALSE  
		dcb.fTXContinueOnXoff = TRUE;		// 受信バッファー満杯＆XOFF受信後の継続送信可否：送信可→TRUE
		dcb.XonLim = 512;					// XONが送られるまでに格納できる最小バイト数：
		dcb.XoffLim = 512;					// XOFFが送られるまでに格納できる最小バイト数：
		dcb.XonChar = 0x11;                 // 送信時XON文字 ( 送信可：ビジィ解除 ) の指定：
											//　一般に、XON文字として11H ( デバイス制御１：DC1 )がよく使われます
		dcb.XoffChar = 0x13;				// XOFF文字（送信不可：ビジー通告）の指定：なし→FALSE
											//　一般に、XOFF文字として13H ( デバイス制御3：DC3 )がよく使われます
		//その他		 
		dcb.fNull = FALSE;          // NULLバイトの破棄： 破棄する→TRUE
		dcb.fAbortOnError = TRUE;  // エラー時の読み書き操作終了：　終了する→TRUE
		dcb.fErrorChar = FALSE;    // パリティエラー発生時のキャラクタ（ErrorChar）置換：　なし→FLALSE
		dcb.ErrorChar = 0x00;      // パリティエラー発生時の置換キャラクタ
		dcb.EofChar = 0x03;        // データ終了通知キャラクタ　：　一般に0x03(ETX)がよく使われます。
		dcb.EvtChar = 0x02;        // イベント通知キャラクタ ：　一般に0x02(STX)がよく使われます		
		if( ::SetCommState(m_hCom,&dcb) ){
			EscapeCommFunction( GetHandle() , SETRTS | SETDTR );
			return TRUE;
		}
	}
	return FALSE;
}

//文字列を送信します。
DWORD ComPort::SendCommand(LPCTSTR lpszCommand,DWORD dwSleepTime/*=0*/)
{
	CW2A pszA(lpszCommand); //Unicode To Ascii

	DWORD dwWrite = 0;
	WriteFile(m_hCom,pszA,(DWORD)strlen(pszA),&dwWrite,NULL);//ｺﾏﾝﾄﾞ送信
	if( dwSleepTime > 0 ){
		::Sleep(dwSleepTime); //コマンド実行後待機
	}
	return dwWrite;
}
//制御コード送信
DWORD ComPort::SendChar(BYTE byCode)
{
	DWORD dwWrite = 0;
	WriteFile(m_hCom,&byCode,1,&dwWrite,NULL);//ｺﾏﾝﾄﾞ送信
	return dwWrite;	
}
//終端文字まで受信します。
BOOL ComPort::RecvString(CString &strRecv)
{
	strRecv.Empty();

	//受信
	// フロー制御関係の信号を送信（フロー制御をおこなう場合）
	BOOL isOK = EscapeCommFunction(
					GetHandle() // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
					,SETRTS);	// 受信可能であることを相手側に示す：RTSをアクティブにする→SETRTS
								//（参考）   RTSを非アクティブにする→CLRRTS　
	DWORD dwErrors;  /* エラー情報 */
	COMSTAT ComStat; /* デバイスの状態 */	
	ClearCommError(m_hCom, &dwErrors, &ComStat);

	char cData = 1;
	DWORD dwRead = 0;
	while(1){ //終端文字列が来るまで読込
		if( !ReadFile(m_hCom,&cData,1,&dwRead ,NULL) ){ 
			//読み込み失敗
			break; 
		}
		if( cData == 0 || cData == GetTerminal()){ //終端文字の場合
			return TRUE;
		}
		if(cData == 0x09 || cData == 0x0b){
			cData = ' ';
		}
		strRecv += cData;
		cData = 0;
	}
	return FALSE;
}

//受信待ちスレッド
static UINT WaitThreadProc( LPVOID pParam )
{
	ComPort *pComPort = (ComPort *)pParam;

	CString strRecv;
	//受信
	// フロー制御関係の信号を送信（フロー制御をおこなう場合）
	BOOL isOK = EscapeCommFunction(
					pComPort->GetHandle() // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
					,SETRTS);	// 受信可能であることを相手側に示す：RTSをアクティブにする→SETRTS
								//（参考）   RTSを非アクティブにする→CLRRTS　
	DWORD dwErrors;  /* エラー情報 */
	COMSTAT ComStat; /* デバイスの状態 */	
	ClearCommError(pComPort->GetHandle(), &dwErrors, &ComStat);

	char cData = 1;
	DWORD dwRead = 0;
	while(IsWindowOK()){ //終端文字列が来るまで読込
		if( !ReadFile(pComPort->GetHandle(),&cData,1,&dwRead ,NULL) ){ 
			//読み込み失敗
			break; 
		}
		if( cData != 0 ){ //終端文字の場合
			strRecv += cData;
		}
		if( strRecv.GetLength() >= pComPort->GetReceiveNum()){
			break;
		}
		cData = 0;
	}
	pComPort->SetRecvString(strRecv); //受信文字列設定
	pComPort->SetEnd(true); //終了
	return 0;
}
//指定文字数受信します。
void ComPort::RecvString(int nNum)
{
	SetEnd(false);
	SetReceiveNum(nNum); //受信文字数登録
	::AfxBeginThread(WaitThreadProc,this); //スレッド実行
}

