// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

//Text File クラス

class FilePlus : public CStdioFile
{
public:
	//定数
	enum {
		modeUnicodeIn =(int) CFile::modeRead | CFile::typeBinary,
		modeUnicodeOut=(int) CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,
		modeTextIn  = (int) CFile::modeRead|CFile::typeText,
		modeTextOut = (int) CFile::modeCreate | CFile::modeWrite | CFile::typeText,// | CFile::typeUnicode ,	
		modeTextAdd = (int) CFile::modeWrite | CFile::typeText,// | CFile::typeUnicode ,	
		modeBinIn   = (int) CFile::modeRead | CFile::typeBinary,		
		modeBinOut  = (int) CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,
		modeBinAdd  = (int) CFile::modeWrite | CFile::typeBinary
	};

public:
	FilePlus(void);
	~FilePlus(void);

	//エラーメッセージ表示付きファイルオープン
	BOOL Open(LPCWSTR lpszFileName, UINT nOpenFlags);

	//書式化された文字列を1行書き込みます。
	void WriteFormat(LPWSTR lpsz,...);

	//行末に改行を付け足して文字列を1行書き込みます。
	void WriteStringln(LPWSTR lpsz);
	void WriteStringln(CString &strText);

	//行末に改行を付け足して書式化された文字列を1行書き込みます。
	void WriteFormatln(LPWSTR lpsz,...);

	//指定された複数行の文字列を書き込みます。
	void WriteStringArray(CStringArray &sAry); 
};
