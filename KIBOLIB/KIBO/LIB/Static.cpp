// Static.cpp : �����t�@�C��
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
	//��������N���X�쐬
	CString strText;
	va_list args;
	va_start(args, lpsz);
	strText.FormatV(lpsz, args);
	va_end(args);
	SetWindowText(strText);
}

BEGIN_MESSAGE_MAP(Static, CStatic)
END_MESSAGE_MAP()



// Static ���b�Z�[�W �n���h��

