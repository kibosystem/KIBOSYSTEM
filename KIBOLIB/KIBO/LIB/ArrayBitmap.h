#pragma once

#include "dib.h"

//�r�b�g�}�b�v�z��
class ArrayBitmap : public CArray<CDIB*>
{
public:
	ArrayBitmap();
	~ArrayBitmap();

	BOOL Add(CDIB &dib);//�r�b�g�}�b�v�ǉ�	
	BOOL Add(CString &strPath);
	BOOL Set(int nIndex,CDIB &dib);//�w��C���f�b�N�X�ɃR�s�[
	CDIB* GetDibPtr(int nIndex); //�r�b�g�}�b�v�N���X�擾	
	void Clear();//�S�r�b�g�}�b�v�폜
};


