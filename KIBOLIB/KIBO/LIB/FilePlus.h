// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

//Text File �N���X

class FilePlus : public CStdioFile
{
public:
	//�萔
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

	//�G���[���b�Z�[�W�\���t���t�@�C���I�[�v��
	BOOL Open(LPCWSTR lpszFileName, UINT nOpenFlags);

	//���������ꂽ�������1�s�������݂܂��B
	void WriteFormat(LPWSTR lpsz,...);

	//�s���ɉ��s��t�������ĕ������1�s�������݂܂��B
	void WriteStringln(LPWSTR lpsz);
	void WriteStringln(CString &strText);

	//�s���ɉ��s��t�������ď��������ꂽ�������1�s�������݂܂��B
	void WriteFormatln(LPWSTR lpsz,...);

	//�w�肳�ꂽ�����s�̕�������������݂܂��B
	void WriteStringArray(CStringArray &sAry); 
};
