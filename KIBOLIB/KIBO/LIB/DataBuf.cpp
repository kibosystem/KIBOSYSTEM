#include "../stdafx.h"
#include "databuf.h"

///////////////////////////////////////
// BYTE Buffer class

//コンストラクタ
DataBuf::DataBuf()
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = 0;	//Data Size
	m_pBuf  = NULL;	//Buffer Pointer
	ZeroMemory(m_byBuf,sizeof(m_byBuf));
}
DataBuf::DataBuf(int nSize)
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = 0;	//Data Size
	m_pBuf  = NULL;	//Buffer Pointer
	ZeroMemory(m_byBuf,sizeof(m_byBuf));
	Alloc(nSize); //コンストラクタで確保
}

//デストラクタ
DataBuf::~DataBuf(void)
{
	Free(); //デストラクタで確実に解放
}

//ヒープ領域の確保
BOOL DataBuf::Alloc(int nSize)
{
	if( m_nAllocSize >= nSize ){ //既に必要な領域を確保しているとき
		//何もしない
	}else if( nSize > sizeof(m_byBuf) ){
		Free(); //前回のデータを解放
		//動的に領域確保
		m_pBuf = new(std::nothrow) BYTE[nSize]; 		
		m_nAllocSize = nSize;
	}else{//新規にバッファを確保する必要は無い (スタック領域で十分な場合)
		Free(); //前回のデータを解放
		m_pBuf = m_byBuf;
	}
	m_nSize = nSize; //データサイズ記憶
	ZeroMemory(m_pBuf,nSize);
	return (m_pBuf != NULL);
}

//ヒープ領域の解放
BOOL DataBuf::Free()
{
	//デストラクタで開放
	if( m_pBuf != NULL ){
		m_nSize = 0;
		if( m_pBuf != m_byBuf ){ //ヒープメモリを使用していたときだけ
			delete [] m_pBuf; 
		}
		m_pBuf = NULL;
	}
	m_nAllocSize = 0;
	return (m_pBuf == NULL);
}



///////////////////////////////////////
// short Buffer class

ShortBuf::ShortBuf()
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = 0;	//Data Size
	m_pBuf  = NULL;	//Buffer Pointer	
}
ShortBuf::ShortBuf(int nSize)
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = nSize;		//Data Size
	m_pBuf  = NULL;	//Buffer Pointer	
	Alloc(nSize);	//Buffer Pointer
}
ShortBuf::~ShortBuf(void)
{
	Free(); //デストラクタで確実に解放
}

//ヒープ領域の確保
BOOL ShortBuf::Alloc(int nSize)	
{
	if( m_nAllocSize >= nSize ){ //既に必要な領域を確保しているとき
		m_nSize = nSize; //仕様サイズ変更
	}else{
		Free(); //前回のデータを解放
		//動的に領域確保
		m_pBuf = new(std::nothrow) short[nSize]; 		
		m_nSize = nSize; //サイズ記憶
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf,nSize);
	return (m_pBuf != NULL);
}

//ヒープ領域の解放
BOOL ShortBuf::Free()
{
	//デストラクタで開放
	if( m_pBuf != NULL ){
		delete [] m_pBuf; 
		m_pBuf = NULL;
	}
	m_nAllocSize = 0;
	return (m_pBuf == NULL);
}

///////////////////////////////////////
// int Buffer class

IntBuf::IntBuf()
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = 0;	//Data Size
	m_pBuf  = NULL;	//Buffer Pointer	
}
IntBuf::IntBuf(int nSize)
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = nSize;		//Data Size
	m_pBuf  = NULL;	//Buffer Pointer	
	Alloc(nSize);	//Buffer Pointer
}
IntBuf::~IntBuf(void)
{
	Free(); //デストラクタで確実に解放
}

//ヒープ領域の確保
BOOL IntBuf::Alloc(int nSize)	
{
	if( m_nAllocSize >= nSize ){ //既に必要な領域を確保しているとき
		m_nSize = nSize; //仕様サイズ変更
	}else{
		Free(); //前回のデータを解放
		//動的に領域確保
		m_pBuf = new(std::nothrow) int[nSize]; 		
		m_nSize = nSize; //サイズ記憶
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf,nSize*sizeof(int));
	return (m_pBuf != NULL);
}

//ヒープ領域の解放
BOOL IntBuf::Free()
{
	//デストラクタで開放
	if( m_pBuf != NULL ){
		delete [] m_pBuf; 
		m_pBuf = NULL;
	}
	m_nAllocSize = 0;
	return (m_pBuf == NULL);
}



///////////////////////////////////////
// WORDt Buffer class
WordBuf::WordBuf()
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = 0;	//Data Size
	m_pBuf = NULL;	//Buffer Pointer	
}
WordBuf::WordBuf(int nSize)
{
	m_nAllocSize = 0; //Memory Alloc Size
	m_nSize = nSize;		//Data Size
	m_pBuf = NULL;	//Buffer Pointer	
	Alloc(nSize);	//Buffer Pointer
}
WordBuf::~WordBuf(void)
{
	Free(); //デストラクタで確実に解放
}

//ヒープ領域の確保
BOOL WordBuf::Alloc(int nSize)
{
	if (m_nAllocSize >= nSize) { //既に必要な領域を確保しているとき
		m_nSize = nSize; //仕様サイズ変更
	}
	else {
		Free(); //前回のデータを解放
				//動的に領域確保
		m_pBuf = new(std::nothrow) WORD[nSize];
		m_nSize = nSize; //サイズ記憶
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf, nSize);
	return (m_pBuf != NULL);
}

//ヒープ領域の解放
BOOL WordBuf::Free()
{
	//デストラクタで開放
	if (m_pBuf != NULL) {
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
	m_nAllocSize = 0;
	return (m_pBuf == NULL);
}
