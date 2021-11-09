// TextCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KIBO.h"
#include "TextCtrl.h"


// TextCtrl

IMPLEMENT_DYNAMIC(TextCtrl, CStatic)

TextCtrl::TextCtrl()
{

}

TextCtrl::~TextCtrl()
{
}

//文字列表示
void TextCtrl::SetText(LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	SetWindowText(strFormat);
}

BEGIN_MESSAGE_MAP(TextCtrl, CStatic)
END_MESSAGE_MAP()



// TextCtrl メッセージ ハンドラ


