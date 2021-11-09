#include "stdafx.h"
#include "KIBO.h"
#include "CaptureBuffer.h"


//コンストラクタ
CaptureBuffer::CaptureBuffer(void)
{
	m_isEnd = false; //終了フラグ
}
//デストラクタ
CaptureBuffer::~CaptureBuffer(void)
{
}

//ビットマップ追加
int CaptureBuffer::Add(CDIB &dib)
{
	//未使用のビットマップを探す
	for(int i=0,imax=(int)m_aryFlag.GetSize();i<imax;i++){
		if( m_aryFlag[i] == BITAMP_INVALID){ //無効なビットマップを変更
			m_aryBmp.Set(i,dib); //指定インデックスにコピー
			m_aryFlag.SetAt(i,BITMAP_VALID); //ビットマップフラグ変更
			return imax; //配列サイズ
		}
	}
	m_aryBmp.Add(dib); //ビットマップ配列追加
	m_aryFlag.Add(BITMAP_VALID);//ビットマップフラグ追加
	return (int)m_aryBmp.GetSize(); //配列サイズ
}

//使用可能なビットマップを取得
CDIB* CaptureBuffer::GetDibPtr()
{
	//使用中ののビットマップを探す
	for(int i=0;i<m_aryFlag.GetSize();i++){
		if( m_aryFlag[i] == BITMAP_VALID){ //フラグ配列
			m_aryFlag.SetAt(i,BITAMP_INVALID); //取り出したビットマップは無効にする
			return m_aryBmp.GetDibPtr(i); //取り出し
		}
	}
	return NULL;
}

//ビットマップ数取得
int CaptureBuffer::GetSize(int nType) 
{
	int nCnt = 0;
	for(int i=0,imax=(int)m_aryFlag.GetSize();i<imax;i++){
		if( m_aryFlag[i] == nType ){
			nCnt++;
		}
	}
	return nCnt; //指定フラグビットマップ数
}