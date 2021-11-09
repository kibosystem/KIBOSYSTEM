#pragma once
#include "DataBuf.h"

class Tiff16
{
private:
	int m_nW; //画像幅
	int m_nH; //画像高さ
public:
	DataBuf m_header; //ヘッダ2782 Byte
	WordBuf m_imageData; //16bitデータ部

public:
	Tiff16(int nW,int nH);
	~Tiff16();
	int GetWidth(){ return m_nW; }
	int GetHeight(){ return m_nH; }
	void SetWidth(int w){ m_nW = w; }
	void SetHeight(int h){ m_nH = h; }
	BOOL Create(int nW,int nH); //画像領域作成
	BOOL Save(LPCTSTR lpszPath); //Tiffファイル書き込み
	WORD* GetDataPtr(); //データ領域取得	
	double GetData(int nX,double dbY); //補完データ取得	
	WORD GetData(CPoint &pt);//指定座標データ取得
	CSize GetSize(LPCTSTR lpszPath); //画像サイズを取得	
	void ConvertData();//RGB24Bit Data作成	
	void ConvertData(Tiff16 &tifR, Tiff16 &tifG, Tiff16 &tifB);//RGB24Bit Data指定作成
	void Clear();
};


//ヘッダ
#pragma pack(push, 1) // アライメント1に設定

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