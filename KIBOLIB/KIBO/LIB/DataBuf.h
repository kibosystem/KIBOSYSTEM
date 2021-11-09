// DLib C++ library.
// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

//���I��Buffer�쐬���x������N���X

class DataBuf
{
private:
	BYTE m_byBuf[1024]; //�PK Buffer 
public:
	int  m_nSize;	//Data Size
	int  m_nAllocSize; //Memory Alloc Size
	BYTE *m_pBuf; //Buffer Pointer
public:
	DataBuf();
	DataBuf(int nSize);
	~DataBuf(void);

	virtual BOOL Alloc(int nSize);	//�q�[�v�̈�̊m��
	int GetSize(){		//Buffer�T�C�Y�擾
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //�������w��T�C�Y�ȍ~���̃A�N�Z�X���֎~����
private:
	BOOL Free();			//�q�[�v�̈�̉��
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

	virtual BOOL Alloc(int nSize);	//�q�[�v�̈�̊m��
	BOOL Free();			//�q�[�v�̈�̉��
	int GetSize(){		//Buffer�T�C�Y�擾
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //�������w��T�C�Y�ȍ~���̃A�N�Z�X���֎~����
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

	virtual BOOL Alloc(int nSize);	//�q�[�v�̈�̊m��
	BOOL Free();			//�q�[�v�̈�̉��
	int GetSize(){		//Buffer�T�C�Y�擾
		return m_nSize;
	}      
	void SetSize(int n){m_nSize=n;} //�������w��T�C�Y�ȍ~���̃A�N�Z�X���֎~����
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

	virtual BOOL Alloc(int nSize);	//�q�[�v�̈�̊m��
	BOOL Free();			//�q�[�v�̈�̉��
	int GetSize() {		//Buffer�T�C�Y�擾
		return m_nSize;
	}
	void SetSize(int n) { m_nSize = n; } //�������w��T�C�Y�ȍ~���̃A�N�Z�X���֎~����
};