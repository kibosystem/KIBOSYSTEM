// TextCtrl.cpp : �����t�@�C��
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

//������\��
void TextCtrl::SetText(LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	SetWindowText(strFormat);
}

BEGIN_MESSAGE_MAP(TextCtrl, CStatic)
END_MESSAGE_MAP()



// TextCtrl ���b�Z�[�W �n���h��


