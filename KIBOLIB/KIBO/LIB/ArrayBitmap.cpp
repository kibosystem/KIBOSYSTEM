#include "../stdafx.h"
#include "ArrayBitmap.h"

//コンストラクタ
ArrayBitmap::ArrayBitmap()
{
}

//デストラクタ
ArrayBitmap::~ArrayBitmap()
{
	Clear();
}

//全ビットマップ削除
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

//ビットマップクラス取得
CDIB* ArrayBitmap::GetDibPtr(int nIndex) 
{
	if( nIndex < GetSize() ){
		return GetAt(nIndex);
	}
	return NULL;
}
//ビットマップ追加
BOOL ArrayBitmap::Add(CDIB &dib)
{
	CDIB *pDib = new CDIB;
	if(pDib != NULL){
		dib.Copy(*pDib); //Copy
		CArray<CDIB*>::Add(pDib); //追加
		return TRUE;
	}
	return FALSE;
}
BOOL ArrayBitmap::Add(CString &strPath)
{
	CDIB *pDib = new CDIB;
	if(pDib != NULL){
		pDib->Load(strPath);
		CArray<CDIB*>::Add(pDib); //追加
		return TRUE;
	}
	return FALSE;
}
//指定インデックスにコピー
BOOL ArrayBitmap::Set(int nIndex,CDIB &dib) 
{
	CDIB *pDib = GetDibPtr(nIndex);
	if(pDib != NULL){
		pDib->SetDib(0,0,dib);
		return TRUE;
	}
	return FALSE;
}

