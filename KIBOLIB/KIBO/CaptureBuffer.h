#pragma once

#include "./LIB/typedef.h"
#include "./LIB/ArrayBitmap.h"

//マルチスレッドキャプチャバッファ
class CaptureBuffer
{
public:
	enum{
		 BITMAP_VALID = 0	//有効
		,BITAMP_INVALID		//無効
	};
private:
	bool m_isEnd; //終了フラグ
public:
	ArrayBitmap m_aryBmp; //ビットマップ配列
	AryByte		m_aryFlag; //フラグ配列
public:
	CaptureBuffer(void);
	~CaptureBuffer(void);

	int  Add(CDIB &dib);//ビットマップ追加
	bool IsEnd(){return m_isEnd;}	//終了ですか？
	void SetEnd(bool b){m_isEnd=b;}	//終了条件設定
	CDIB* GetDibPtr(); //使用可能なビットマップを取得

	int GetSize(int nType); //ビットマップ数取得
};
