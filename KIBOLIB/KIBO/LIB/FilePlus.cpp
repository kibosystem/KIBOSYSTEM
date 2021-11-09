#include "../stdafx.h"
#include "FilePlus.h"
#include "functions.h"

FilePlus::FilePlus(void)
{
}

FilePlus::~FilePlus(void)
{
}

//エラーメッセージ表示付きファイルオープン
BOOL FilePlus::Open(LPCWSTR lpszFileName, UINT nOpenFlags)
{
	CFileException ex;
	if( !CStdioFile::Open(lpszFileName,nOpenFlags,&ex) ){ //File Open
		//File Open Error
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, sizeof(szError));
		::ErrorMsg(_T("Couldn't open file: %s"),szError);	
		return FALSE;
	}	
	return TRUE;
}

//書式化された文字列を1行書き込みます。
void FilePlus::WriteFormat(LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//文字列書き込み
	WriteString(strFormat);
}

//最後に改行を付け足して文字列を1行書き込みます。
void FilePlus::WriteStringln(LPWSTR lpsz)
{
	//▼文字列クラス作成
	CString strFormat(lpsz);
	//改行付き文字列書き込み
	WriteStringln(strFormat);
}
void FilePlus::WriteStringln(CString &strText)
{
	WriteString(strText+_T("\n"));	
}

//最後に改行を付け足して書式化された文字列を1行書き込みます。
void FilePlus::WriteFormatln(LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//改行付き文字列書き込み
	WriteStringln(strFormat.GetBuffer());
}

//指定された複数行の文字列を書き込みます。
void FilePlus::WriteStringArray(CStringArray &sAry)
{
	for(int i=0;i<sAry.GetSize();i++){
		WriteStringln(sAry[i].GetBuffer());
	}
}
