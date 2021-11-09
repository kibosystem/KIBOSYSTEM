#include "../stdafx.h"
#include "databuf.h"

///////////////////////////////////////
// BYTE Buffer class

//�R���X�g���N�^
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
	Alloc(nSize); //�R���X�g���N�^�Ŋm��
}

//�f�X�g���N�^
DataBuf::~DataBuf(void)
{
	Free(); //�f�X�g���N�^�Ŋm���ɉ��
}

//�q�[�v�̈�̊m��
BOOL DataBuf::Alloc(int nSize)
{
	if( m_nAllocSize >= nSize ){ //���ɕK�v�ȗ̈���m�ۂ��Ă���Ƃ�
		//�������Ȃ�
	}else if( nSize > sizeof(m_byBuf) ){
		Free(); //�O��̃f�[�^�����
		//���I�ɗ̈�m��
		m_pBuf = new(std::nothrow) BYTE[nSize]; 		
		m_nAllocSize = nSize;
	}else{//�V�K�Ƀo�b�t�@���m�ۂ���K�v�͖��� (�X�^�b�N�̈�ŏ\���ȏꍇ)
		Free(); //�O��̃f�[�^�����
		m_pBuf = m_byBuf;
	}
	m_nSize = nSize; //�f�[�^�T�C�Y�L��
	ZeroMemory(m_pBuf,nSize);
	return (m_pBuf != NULL);
}

//�q�[�v�̈�̉��
BOOL DataBuf::Free()
{
	//�f�X�g���N�^�ŊJ��
	if( m_pBuf != NULL ){
		m_nSize = 0;
		if( m_pBuf != m_byBuf ){ //�q�[�v���������g�p���Ă����Ƃ�����
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
	Free(); //�f�X�g���N�^�Ŋm���ɉ��
}

//�q�[�v�̈�̊m��
BOOL ShortBuf::Alloc(int nSize)	
{
	if( m_nAllocSize >= nSize ){ //���ɕK�v�ȗ̈���m�ۂ��Ă���Ƃ�
		m_nSize = nSize; //�d�l�T�C�Y�ύX
	}else{
		Free(); //�O��̃f�[�^�����
		//���I�ɗ̈�m��
		m_pBuf = new(std::nothrow) short[nSize]; 		
		m_nSize = nSize; //�T�C�Y�L��
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf,nSize);
	return (m_pBuf != NULL);
}

//�q�[�v�̈�̉��
BOOL ShortBuf::Free()
{
	//�f�X�g���N�^�ŊJ��
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
	Free(); //�f�X�g���N�^�Ŋm���ɉ��
}

//�q�[�v�̈�̊m��
BOOL IntBuf::Alloc(int nSize)	
{
	if( m_nAllocSize >= nSize ){ //���ɕK�v�ȗ̈���m�ۂ��Ă���Ƃ�
		m_nSize = nSize; //�d�l�T�C�Y�ύX
	}else{
		Free(); //�O��̃f�[�^�����
		//���I�ɗ̈�m��
		m_pBuf = new(std::nothrow) int[nSize]; 		
		m_nSize = nSize; //�T�C�Y�L��
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf,nSize*sizeof(int));
	return (m_pBuf != NULL);
}

//�q�[�v�̈�̉��
BOOL IntBuf::Free()
{
	//�f�X�g���N�^�ŊJ��
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
	Free(); //�f�X�g���N�^�Ŋm���ɉ��
}

//�q�[�v�̈�̊m��
BOOL WordBuf::Alloc(int nSize)
{
	if (m_nAllocSize >= nSize) { //���ɕK�v�ȗ̈���m�ۂ��Ă���Ƃ�
		m_nSize = nSize; //�d�l�T�C�Y�ύX
	}
	else {
		Free(); //�O��̃f�[�^�����
				//���I�ɗ̈�m��
		m_pBuf = new(std::nothrow) WORD[nSize];
		m_nSize = nSize; //�T�C�Y�L��
		m_nAllocSize = nSize;
	}
	ZeroMemory(m_pBuf, nSize);
	return (m_pBuf != NULL);
}

//�q�[�v�̈�̉��
BOOL WordBuf::Free()
{
	//�f�X�g���N�^�ŊJ��
	if (m_pBuf != NULL) {
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
	m_nAllocSize = 0;
	return (m_pBuf == NULL);
}
