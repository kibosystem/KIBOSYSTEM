// Static.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "Static.h"


// Static

IMPLEMENT_DYNAMIC(Static, CStatic)
Static::Static()
{
}

Static::~Static()
{
}

void Static::SetText(LPCTSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strText;
	va_list args;
	va_start(args, lpsz);
	strText.FormatV(lpsz, args);
	va_end(args);
	SetWindowText(strText);
}

BEGIN_MESSAGE_MAP(Static, CStatic)
END_MESSAGE_MAP()



// Static メッセージ ハンドラ

