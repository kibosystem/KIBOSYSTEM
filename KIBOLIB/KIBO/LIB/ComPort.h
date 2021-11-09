#pragma once

//Com Port 通信クラス

class ComPort
{
//メンバ
private:
	HANDLE	m_hCom;			//Com port ハンドル
	char	m_cTerminal;	//終端文字
	int		m_nReceiveNum;	//受信数
	bool	m_isEnd;		//スレッド終了
	CString m_strRecive;	//受信文字列
public:
//コンストラクタ
	ComPort();
	ComPort(LPCTSTR lpszPortName);
//デストラクタ
	~ComPort();

//アクセス関数
	void Init();//初期化
	HANDLE GetHandle(){return m_hCom;} //Com port ハンドル
	BOOL IsOpen(){return (m_hCom != NULL);}
	char GetTerminal(){return m_cTerminal;} //終端文字取得
	void SetTerminal(char c){m_cTerminal = c;}    //終端文字設定
	int	 GetReceiveNum(){return m_nReceiveNum;} //受信数
	void SetReceiveNum(int n){m_nReceiveNum=n;} //受信数
	bool IsEnd(){return m_isEnd;}		//スレッド終了ですか？
	void SetEnd(bool is){m_isEnd = is;} //スレッド状態設定
	void SetRecvString(CString &str){m_strRecive=str;} //受信文字列設定
	CString GetRecvString(){return m_strRecive;} //受信文字列取得
//メソッド
	virtual HANDLE Open(LPCTSTR lpszPortName,DWORD dwDesiredAccess=GENERIC_READ|GENERIC_WRITE); //Com Portを開く
	void Close();

	BOOL SetReadTimeOut(DWORD dwTime);//通信タイムアウト時間設定
	BOOL SetCommRate(DWORD dwBaudRate);//COM Port転送速度設定

	DWORD SendCommand(LPCTSTR lpszCommand,DWORD dwSleepTime = 0);	//文字列を送信します。
	DWORD SendChar(BYTE byCode); //制御コード送信
	DWORD SendSTX(){return SendChar(0x02);} //STX送信
	BOOL RecvString(CString &strRecv);	//終端文字まで受信します。
	void RecvString(int nNum);		//指定文字列受信します。
};
