// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

//動的なBuffer作成を支援するクラス

class DataBuf
{
private:
	BYTE m_byBuf[1024]; //１K Buffer 
public:
	int  m_nSize;	//Data Size
	int  m_nAllocSize; //Memory Alloc Size
	BYTE *m_pBuf; //Buffer Pointer
public:
	DataBuf();
	DataBuf(int nSize);
	~DataBuf(void);

	virtual BOOL Alloc(int nSize);	//ヒープ領域の確保
	int GetSize(){		//Bufferサイズ取得
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //無理やり指定サイズ以降をのアクセスを禁止する
private:
	BOOL Free();			//ヒープ領域の解放
};

class ShortBuf
{
public:
	int  m_nSize;	//Data Size
	int  m_nAllocSize; //Memory Alloc Size
	short *m_pBuf;	//Buffer Pointer
public:
	ShortBuf();
	ShortBuf(int nSize);
	~ShortBuf(void);

	virtual BOOL Alloc(int nSize);	//ヒープ領域の確保
	BOOL Free();			//ヒープ領域の解放
	int GetSize(){		//Bufferサイズ取得
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //無理やり指定サイズ以降をのアクセスを禁止する
};

class IntBuf
{
public:
	int  m_nSize;	//Data Size
	int  m_nAllocSize; //Memory Alloc Size
	int *m_pBuf;	//Buffer Pointer
public:
	IntBuf();
	IntBuf(int nSize);
	~IntBuf(void);

	virtual BOOL Alloc(int nSize);	//ヒープ領域の確保
	BOOL Free();			//ヒープ領域の解放
	int GetSize(){		//Bufferサイズ取得
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //無理やり指定サイズ以降をのアクセスを禁止する
};

class WordBuf
{
public:
	int  m_nSize;	//Data Size
	int  m_nAllocSize; //Memory Alloc Size
	WORD *m_pBuf;	//Buffer Pointer
public:
	WordBuf();
	WordBuf(int nSize);
	~WordBuf(void);

	virtual BOOL Alloc(int nSize);	//ヒープ領域の確保
	BOOL Free();			//ヒープ領域の解放
	int GetSize() {		//Bufferサイズ取得
		return m_nSize;
	}
	void SetSize(int n) { m_nSize = n; } //無理やり指定サイズ以降をのアクセスを禁止する
};