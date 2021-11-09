// functions.cpp : Global Function によるライブラリファイル
#include "../stdafx.h"
#include "functions.h"
#include <math.h>
static LPCWSTR s_lpszEmpty = L""; //空白文字列

//メッセージボックス表示　Type指定
int UIMessageBox(UINT nType,LPWSTR lpszText,...)
{
	//▼メッセージ作成
	CString strMsg;
	va_list args;
	va_start(args, lpszText);
	strMsg.FormatV(lpszText, args);
	va_end(args);
	//▼メッセージ表示
	return ::AfxMessageBox(strMsg,nType);
}

//メッセージボックス表示 IDOK専用
int UIMessageBox(LPWSTR lpszText,...)
{
	//▼メッセージ作成
	CString strMsg;
	va_list args;
	va_start(args, lpszText);
	strMsg.FormatV(lpszText, args);
	va_end(args);
	//▼メッセージ表示
	return ::AfxMessageBox(strMsg);
}

//エラーメッセージ専用メッセージボックス
void ErrorMsg(LPWSTR lpszText, ...)
{
	//▼メッセージ作成
	CString strMsg;
	va_list args;
	va_start(args, lpszText);
	strMsg.FormatV(lpszText, args);
	va_end(args);
	//▼メッセージ表示
	::AfxMessageBox(strMsg,MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
}

//指定値より小さい値のランダムな数値を返す
int GetRandom(int nMax)
{
	int nRand = rand();
	return (nRand % nMax); //0からnMax-1までの値を返す
}

//シャットダウン権限取得
void ShutDownNameSet()
{
	//Windows の終了処理
	HANDLE hToken;              // handle to process token  
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);//特権ハンドル取得
	// Get the LUID for the shutdown privilege. 

	TOKEN_PRIVILEGES tkp;       // pointer to token structure 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
	// Cannot test the return value of AdjustTokenPrivileges. 	
}

//区切り文字以前の文字列を取得します。
CString GetLeft(CString& str,LPWSTR lpszSplit)
{
	int nFind = str.Find(lpszSplit); 
	if( nFind >= 0 ){ //区切り文字が存在した。
		return str.Left(nFind);
	}
	return s_lpszEmpty; //空白文字列
}

//区切り文字以降の文字列を取得します。
CString GetRight(CString& str,LPWSTR lpszSplit)
{
	int nFind = str.Find(lpszSplit); 
	if( nFind >= 0 ){ //区切り文字が存在した。
		return str.Mid(nFind+(int)wcslen(lpszSplit));
	}
	return s_lpszEmpty; //空白文字列
}

//ファイル名称取得
CString GetFileName(CString &strPath)
{
	if (strPath.GetLength() > 0){
		CFile file(strPath, CFile::modeRead);		
		return file.GetFileName();
	}
	return s_lpszEmpty; //空白文字列
}
//ファイルサイズ取得
int GetFileSize(CString &strPath)
{
	if (::PathFileExists(strPath)) {
		CFile file(strPath, CFile::modeRead);
		CFileStatus rStatus;
		file.GetStatus(rStatus);
		return (int)rStatus.m_size; //ファイルサイズ
	}
	return 0;
}
//ファイルタイトル取得
CString GetFileTitle(CString &strPath)
{
	CFile file(strPath,0);
	CString strName = file.GetFileName();
	return GetLeft(strName,L"."); //拡張子を除く
}

//ビットマップファイル選択
CString SelectBitmapFile(BOOL isOpen,LPCWSTR lpstrInitialDir)
{
	//static TCHAR szFilter[] = L"Bitmap File (*.bmp)|*.bmp|Jpeg File (*.jpg)|*.jpg|Png File (*.png)|*.png|Tiff File (*.tif)|*.tif||";
	static TCHAR szFilter[] = L"Btimap File (*.bmp)|*.bmp|Png File (*.png)|*.png|Jpeg File (*.jpg)|*.jpg|Tiff File (*.tif)|*.tif||";

	CFileDialog dlg(isOpen,_T("bmp"),_T("*.bmp"),NULL,szFilter);
	dlg.m_ofn.lpstrInitialDir = lpstrInitialDir;
	if( dlg.DoModal() == IDOK ){
		return dlg.GetPathName();
	}
	return s_lpszEmpty;//空白文字列
}

//指定区切り文字で文字列配列作成
int SplitToken(TCHAR cSep,CStringArray &sAry,CString& sText) 
{
	int nStart = 0;
	int nCount = 0;
	sAry.RemoveAll();
	int nMax = sText.GetLength();
	for( int i=0;i<nMax;i++ ){
		if( sText[i] == cSep){ //セパレータ発見
			sAry.Add( sText.Mid(nStart,nCount) );
			//Counter Rest
			nStart = i+1;
			nCount = 0;  
			continue;
		}
		nCount++;
	}
	if( nStart < nMax ){ //末尾にトークンがついていない場合
		sAry.Add( sText.Mid(nStart,nCount) );
	}
	return (int)sAry.GetSize();
}

//指定区切り文字列で文字列配列作成
int SplitToken(CString strSep,CStringArray &sAry,CString sText) 
{
	sAry.RemoveAll();
	while( sText.GetLength() > 0){
		int nIndex = sText.Find( strSep ); //区切り文字列検索
		if( nIndex >= 0 ){ //Hit
			sAry.Add( sText.Left(nIndex) );
			sText = sText.Mid(nIndex+strSep.GetLength()); //残り取得
		}else{
			sAry.Add( sText ); //残り全て登録
			break;
		}
	}
	return (int)sAry.GetSize();
}

//二点間の距離を計算
double GetLength(CPoint &pt1,CPoint &pt2)
{
	double dbDx = pt1.x-pt2.x;
	double dbDy = pt1.y-pt2.y;
	return sqrt(dbDx*dbDx + dbDy*dbDy);
}
double GetLength(POINT pt1,POINT pt2)
{
	double dbDx = pt1.x-pt2.x;
	double dbDy = pt1.y-pt2.y;
	return sqrt(dbDx*dbDx + dbDy*dbDy);
}
//重心からX軸方向左端までの距離
double GetLength(double dbX1,double dbY1,double dbX2,double dbY2)	
{
	double dbDx = dbX1-dbX2;
	double dbDy = dbY1-dbY2;
	return sqrt(dbDx*dbDx + dbDy*dbDy);	
}
//角度取得 degree
double GetAngle(CSize &vec1,CSize &vec2)
{
	double dbLen1 = sqrt((double)(vec1.cx*vec1.cx + vec1.cy*vec1.cy));
	double dbLen2 = sqrt((double)(vec2.cx*vec2.cx + vec2.cy*vec2.cy));
	double dbCos = (vec1.cx*vec2.cx + vec1.cy*vec2.cy) / (dbLen1*dbLen2);
	return acos(dbCos) * 180 / PAI;
}
double GetAngle(PointDbl &vec1,PointDbl &vec2)
{
	double dbLen1 = sqrt((double)(vec1.x*vec1.x + vec1.y*vec1.y));
	double dbLen2 = sqrt((double)(vec2.x*vec2.x + vec2.y*vec2.y));
	double dbCos = (vec1.x*vec2.x + vec1.y*vec2.y) / (dbLen1*dbLen2);
	return acos(dbCos) * 180 / PAI;
}
//コサイン取得
double GetCos(CSize &vec1,CSize &vec2)
{
	double dbLen1 = sqrt((double)(vec1.cx*vec1.cx + vec1.cy*vec1.cy));
	double dbLen2 = sqrt((double)(vec2.cx*vec2.cx + vec2.cy*vec2.cy));
	return (vec1.cx*vec2.cx + vec1.cy*vec2.cy) / (dbLen1*dbLen2);
}
//サイン取得
double GetSin(CSize &vec1,CSize &vec2)
{
	double dbLen1 = sqrt((double)(vec1.cx*vec1.cx + vec1.cy*vec1.cy));
	double dbLen2 = sqrt((double)(vec2.cx*vec2.cx + vec2.cy*vec2.cy));	
	return (vec1.cx*vec2.cy - vec2.cx*vec1.cy) / (dbLen1*dbLen2);
}

//ファイル更新日時取得
CTime GetLastWriteTime(const CString &strPath) 
{
	CTime time;//ファイル更新日時

	CFileFind finder;
	BOOL isWorking = finder.FindFile(strPath);
	while(isWorking){		
		isWorking = finder.FindNextFile();
		if( finder.GetLastWriteTime(time) ){ //更新日時取得
			break;
		}
	}
	return time;
}

CString GetStringInt(int n)
{
	CString str;
	str.Format(L"%d",n);
	return str;
}

//連番名称取得
CString Name(LPWSTR lpsz,int n)
{
	CString str;
	str.Format(L"%s%d",lpsz,n);
	return str;
}

//指定ファイル実行
int Exec(int nShow,LPWSTR lpszExec,LPWSTR lpszFile)
{
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = nShow;// SW_HIDE or SW_SHOWNORMAL;
	CString strProcess;
	strProcess.Format(L"%s %s",lpszExec,lpszFile);
	int nRet = CreateProcess( NULL
							,strProcess.GetBuffer()
							,NULL
							,NULL
							,FALSE
							,0
							,NULL
							,NULL
							,&sinfo
							,&pinfo );
	//編集が終わるまで制御を戻しません
	WaitForInputIdle( pinfo.hProcess, INFINITE );
	WaitForSingleObject( pinfo.hProcess, INFINITE );
	DWORD dwExitCode;
	GetExitCodeProcess(pinfo.hProcess,&dwExitCode);
	return (int)dwExitCode;
}

//.iniファイルから文字列取得
CString GetPrivateProfileText(LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));

	::GetPrivateProfileString(lpszSection, lpszKey, L"", szBuf, sizeof(szBuf), lpszPath);
	return CString(szBuf);
}

//.iniファイルから少数値取得
double GetPrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,double dbDefault,LPCWSTR lpszPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));

	::GetPrivateProfileString(lpszSection,lpszKey,L"",szBuf,sizeof(szBuf),lpszPath);
	CString strRet = szBuf;
	if( strRet.IsEmpty() ){
		return dbDefault;
	}
	double dbRet = _wtof(szBuf);
	return dbRet;
}
//.iniファイルに数値書き込み
void WritePrivateProfileInt(LPCWSTR lpszSection,LPCWSTR lpszKey,int nVal,LPCWSTR lpszPath)
{
	CString strBuf;
	strBuf.Format(L"%d",nVal);
	::WritePrivateProfileString(lpszSection,lpszKey,strBuf,lpszPath);
}
//.iniファイルに実数書き込み
void WritePrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,LPCTSTR lpszFormat,double dbVal,LPCWSTR lpszPath)
{
	CString strBuf;
	strBuf.Format(lpszFormat,dbVal);
	::WritePrivateProfileString(lpszSection,lpszKey,strBuf,lpszPath);
}

//RGB -> YSH
YSH GetYSH(BYTE byR, BYTE byG, BYTE byB)
{
	YSH ysh;
	ysh.m_dbY = (0.299*byR + 0.587*byG + 0.114*byB);
	double dbC1 = byR - ysh.m_dbY;
	double dbC2 = byB - ysh.m_dbY;
	double dbCH = atan2(dbC1, dbC2) + PAI; //0..2Π
	ysh.m_dbH  = (dbCH*MAXBYTE) / (2 * PAI);
	ysh .m_dbS = sqrt(dbC1*dbC1 + dbC2*dbC2);
	return ysh;
}

//キー入力
void KeyAction(WORD VirtualKey, BOOL bKeepPressing)
{
	INPUT input[1];
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VirtualKey;
	input[0].ki.wScan = ::MapVirtualKeyW(input[0].ki.wVk, 0);
	input[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	input[0].ki.time = 0;
	input[0].ki.dwExtraInfo = ::GetMessageExtraInfo();

	::SendInput(1, input, sizeof(INPUT));

	if (!bKeepPressing)
	{
		input[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
		::SendInput(1, input, sizeof(INPUT));
	}
}