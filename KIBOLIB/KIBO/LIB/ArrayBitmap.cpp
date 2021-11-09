#include "../stdafx.h"
#include "ArrayBitmap.h"

//�R���X�g���N�^
ArrayBitmap::ArrayBitmap()
{
}

//�f�X�g���N�^
ArrayBitmap::~ArrayBitmap()
{
	Clear();
}

//�S�r�b�g�}�b�v�폜
void ArrayBitmap::Clear()
{
	for(int i=0;i<GetSize();i++){
		CDIB *pPtr = GetAt(i);
		if( pPtr != NULL ){
			delete pPtr;
		}		
	}
	RemoveAll();
}

//�r�b�g�}�b�v�N���X�擾
CDIB* ArrayBitmap::GetDibPtr(int nIndex) 
{
	if( nIndex < GetSize() ){
		return GetAt(nIndex);
	}
	return NULL;
}
//�r�b�g�}�b�v�ǉ�
BOOL ArrayBitmap::Add(CDIB &dib)
{
	CDIB *pDib = new CDIB;
	if(pDib != NULL){
		dib.Copy(*pDib); //Copy
		CArray<CDIB*>::Add(pDib); //�ǉ�
		return TRUE;
	}
	return FALSE;
}
BOOL ArrayBitmap::Add(CString &strPath)
{
	CDIB *pDib = new CDIB;
	if(pDib != NULL){
		pDib->Load(strPath);
		CArray<CDIB*>::Add(pDib); //�ǉ�
		return TRUE;
	}
	return FALSE;
}
//�w��C���f�b�N�X�ɃR�s�[
BOOL ArrayBitmap::Set(int nIndex,CDIB &dib) 
{
	CDIB *pDib = GetDibPtr(nIndex);
	if(pDib != NULL){
		pDib->SetDib(0,0,dib);
		return TRUE;
	}
	return FALSE;
}

