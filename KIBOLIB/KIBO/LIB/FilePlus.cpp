#include "../stdafx.h"
#include "FilePlus.h"
#include "functions.h"

FilePlus::FilePlus(void)
{
}

FilePlus::~FilePlus(void)
{
}

//�G���[���b�Z�[�W�\���t���t�@�C���I�[�v��
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

//���������ꂽ�������1�s�������݂܂��B
void FilePlus::WriteFormat(LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//�����񏑂�����
	WriteString(strFormat);
}

//�Ō�ɉ��s��t�������ĕ������1�s�������݂܂��B
void FilePlus::WriteStringln(LPWSTR lpsz)
{
	//��������N���X�쐬
	CString strFormat(lpsz);
	//���s�t�������񏑂�����
	WriteStringln(strFormat);
}
void FilePlus::WriteStringln(CString &strText)
{
	WriteString(strText+_T("\n"));	
}

//�Ō�ɉ��s��t�������ď��������ꂽ�������1�s�������݂܂��B
void FilePlus::WriteFormatln(LPWSTR lpsz,...)
{
	//��������N���X�쐬
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//���s�t�������񏑂�����
	WriteStringln(strFormat.GetBuffer());
}

//�w�肳�ꂽ�����s�̕�������������݂܂��B
void FilePlus::WriteStringArray(CStringArray &sAry)
{
	for(int i=0;i<sAry.GetSize();i++){
		WriteStringln(sAry[i].GetBuffer());
	}
}
