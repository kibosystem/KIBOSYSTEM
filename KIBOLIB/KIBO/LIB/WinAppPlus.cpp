// WinAppPlus.cpp : 実装ファイル
//

#include "../stdafx.h"
#include <afxsock.h>
#include <Iphlpapi.h>
#include "WinAppPlus.h"
#include "DataBuf.h"
#include "FilePlus.h"
#include "../MACADDRESS.h"
#include "functions.h"

#pragma comment(lib,"Iphlpapi.lib")

static DWORD GetVolumeID(TCHAR tcVolume)
{
	TCHAR szVolumeName[100];
	TCHAR szSystemName[100];
	DWORD dwSerialNumber = 0;
	DWORD dwFileNameLength = 0;
	DWORD dwFlags = 0;

	TCHAR szPath[4];
	szPath[0] = tcVolume;
	szPath[1] = ':';
	szPath[2] = '\\';
	szPath[3] = NULL;

	if( GetVolumeInformation(szPath,szVolumeName,100
							,&dwSerialNumber,&dwFileNameLength,&dwFlags
							,szSystemName,100) ){
		return dwSerialNumber;
	}
	return 0;
}


// WinAppPlus

IMPLEMENT_DYNCREATE(WinAppPlus, CWinAppEx)

WinAppPlus::WinAppPlus()
{
	m_hMutex = NULL;
	m_isEnd = false; //終了フラグ
}

WinAppPlus::~WinAppPlus()
{
}

//カレントディレクトリの絶対パスを取得
CString WinAppPlus::GetCurrentPath()
{
	TCHAR szBuf[MAX_PATH];
	GetCurrentDirectory(sizeof(szBuf),szBuf); //カレントディレクトリ取得
	CString sPath(szBuf); //返却値
	return sPath; //カレントディレクトリ返却
}

BOOL WinAppPlus::InitInstance()
{
	m_strAppPath = GetCurrentPath(); //アプリケーション起動時のパスセット
	::AfxSocketInit(&m_wsaData);	//ソケット通信使用可能
	::CoInitialize(NULL);			//COMコンポーネント使用可能

	srand( (unsigned)time( NULL ) ); //ランダム数値初期化	
	//多重起動検査
	HANDLE hPrevMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,::AfxGetAppName()); 
	if( hPrevMutex ){ //以前のアプリケーションがMutexを作成している
		CloseHandle(hPrevMutex);
		return -1;
	}
	//Mutexの作成
	m_hMutex = CreateMutex(FALSE,0,::AfxGetAppName());
	if (CWinAppEx::InitInstance()){
		return 1; //確実に１に変換して戻す
	}

	return 0;
}

int WinAppPlus::ExitInstance()
{
	::CoUninitialize();
	ReleaseMutex(m_hMutex);//Mutexの解放
	return CWinAppEx::ExitInstance();
}

//PCのIPアドレスを取得します
CString WinAppPlus::GetIPAddr()
{
	char szBuf[MAX_PATH];
	ZeroMemory(szBuf,sizeof(szBuf));
	gethostname(szBuf,sizeof(szBuf));
	hostent *pHostent = gethostbyname(szBuf);	
	CString sRet;
	if( pHostent->h_addr_list != NULL ){
		IN_ADDR inaddr;
		memcpy(&inaddr, pHostent->h_addr_list[0], 4);
		sRet = inet_ntoa(inaddr);
	}
	return sRet;
}

//Mac Addressを取得します。
BOOL WinAppPlus::SetMacAddress(CStringArray& astrMacAddress)
{
	astrMacAddress.RemoveAll();

	PMIB_IFTABLE	pIfTable;
	CString			strBuff;

	DWORD dwSize = 0;
	::GetIfTable(NULL,&dwSize,TRUE);			//必要バッファサイズ取得

	BYTE *pBuff = new BYTE[dwSize];
	if( pBuff != NULL ){
		pIfTable = (PMIB_IFTABLE) pBuff;

		DWORD ret = ::GetIfTable(pIfTable,&dwSize,TRUE);
		if(ret == NO_ERROR)
		{
			for(UINT i = 0; i < pIfTable->dwNumEntries; i++)
			{
				strBuff.Format(L"%02X%02X%02X%02X%02X%02X"
						,pIfTable->table[i].bPhysAddr[0]
						,pIfTable->table[i].bPhysAddr[1]
						,pIfTable->table[i].bPhysAddr[2]
						,pIfTable->table[i].bPhysAddr[3]
						,pIfTable->table[i].bPhysAddr[4]
						,pIfTable->table[i].bPhysAddr[5]);
				astrMacAddress.Add(strBuff);
			}
		}
		delete	pBuff;
	}
	if(astrMacAddress.GetSize() > 0){
		return	true;
	}
	return	false;	
}

//ビデオカードの拡大縮小の機能をチェックします。
BOOL WinAppPlus::CheckDevice()
{
	BOOL isOK = FALSE;
	HDC hdc = ::GetDC(NULL);
	isOK = ( ::GetDeviceCaps(hdc,RASTERCAPS) & RC_STRETCHBLT ); //拡大縮小能力チェック
	ReleaseDC(NULL,hdc);
	return isOK;
}

//USBメモリのチェックをします。
BOOL WinAppPlus::CheckUSBMemory()
{
	//if(IsEvaluateVersion()){
	//	::ErrorMsg(L"このアプリケーションは評価版です。\n評価目的以外で使用することは出来ません。\n\n(C) 2009 KIBO Corporation.\nAll rights reserved.");
	//}
	//for(TCHAR tc='A';tc<='Z';tc++){//全ドライブチェック
	//	DWORD dwID = GetVolumeID(tc);
	//	int imax = sizeof(USBMEMORYID) / sizeof(DWORD); //配列個数
	//	for(int i=0;i<imax;i++){
	//		if( dwID == USBMEMORYID[i]){ 
	//			return TRUE;//一致した。
	//		}
	//	}
	//}
	return TRUE;
	//return FALSE;
}

BEGIN_MESSAGE_MAP(WinAppPlus, CWinAppEx)
END_MESSAGE_MAP()


