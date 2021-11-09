#pragma once
#include "DataBuf.h"

class Tiff16
{
private:
	int m_nW; //�摜��
	int m_nH; //�摜����
public:
	DataBuf m_header; //�w�b�_2782 Byte
	WordBuf m_imageData; //16bit�f�[�^��

public:
	Tiff16(int nW,int nH);
	~Tiff16();
	int GetWidth(){ return m_nW; }
	int GetHeight(){ return m_nH; }
	void SetWidth(int w){ m_nW = w; }
	void SetHeight(int h){ m_nH = h; }
	BOOL Create(int nW,int nH); //�摜�̈�쐬
	BOOL Save(LPCTSTR lpszPath); //Tiff�t�@�C����������
	WORD* GetDataPtr(); //�f�[�^�̈�擾	
	double GetData(int nX,double dbY); //�⊮�f�[�^�擾	
	WORD GetData(CPoint &pt);//�w����W�f�[�^�擾
	CSize GetSize(LPCTSTR lpszPath); //�摜�T�C�Y���擾	
	void ConvertData();//RGB24Bit Data�쐬	
	void ConvertData(Tiff16 &tifR, Tiff16 &tifG, Tiff16 &tifB);//RGB24Bit Data�w��쐬
	void Clear();
};


//�w�b�_
#pragma pack(push, 1) // �A���C�����g1�ɐݒ�

typedef struct _TiffHeader
{
	WORD wIdentifier;	/* Byte-order Identifier */
	WORD wVersion;		/* TIFF version number (always 2Ah) */
	DWORD dwIFDOffset;  /* Offset of the first Image File Directory*/	
} TIFHEAD;

typedef struct _TifTag
{
	WORD wTagId; /* The tag identifier */
	WORD wDataType; /* The scalar type of the data items */
	DWORD dwDataCount; /* The number of items in the tag data */
	DWORD dwDataOffset; /* The byte offset to the data items */
} TIFTAG;

typedef struct _TifIfd
{

	WORD wNumDirEntries; /* Number of Tags in IFD */
	TIFTAG tagList[16]; /* Array of Tags */
	DWORD dwNextIFDOffset; /* Offset to next IFD */
} TIFIFD;

#pragma pack(pop)