#pragma once

#include "dib.h"

//ビットマップ配列
class ArrayBitmap : public CArray<CDIB*>
{
public:
	ArrayBitmap();
	~ArrayBitmap();

	BOOL Add(CDIB &dib);//ビットマップ追加	
	BOOL Add(CString &strPath);
	BOOL Set(int nIndex,CDIB &dib);//指定インデックスにコピー
	CDIB* GetDibPtr(int nIndex); //ビットマップクラス取得	
	void Clear();//全ビットマップ削除
};


