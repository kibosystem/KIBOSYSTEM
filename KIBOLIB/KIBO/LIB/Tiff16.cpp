#include "../stdafx.h"
#include "Tiff16.h"


//コンストラクタ
Tiff16::Tiff16(int nW,int nH)
{
	m_nW = nW; //画像幅
	m_nH = nH; //画像高さ
	Create(nW, nH);
}


//デストラクタ
Tiff16::~Tiff16()
{
}
//画像領域作成
BOOL Tiff16::Create(int nW, int nH)
{
	m_nW = nW; //画像幅
	m_nH = nH; //画像高さ
	if (!m_header.Alloc(8)){ //ヘッダ8 Byte
		return FALSE;
	}
	if (!m_imageData.Alloc(m_nW * m_nH * 2)){//16bit Data
		return FALSE;
	}
	return TRUE;
}


//Data消去
void Tiff16::Clear()
{
	if (m_header.m_pBuf != NULL){
		ZeroMemory(m_header.m_pBuf, m_header.GetSize());
	}
	if (m_imageData.m_pBuf != NULL){
		ZeroMemory(m_imageData.m_pBuf, m_imageData.GetSize());
	}
}

//Tiffファイル書き込み
BOOL Tiff16::Save(LPCTSTR lpszPath)
{
	int nW = this->GetWidth();
	int nH = this->GetHeight();
	//Header
	static TIFHEAD header;
	header.wIdentifier = 0x4949;	/* Byte-order Identifier */
	header.wVersion = 0x2A;		/* TIFF version number (always 2Ah) */
	header.dwIFDOffset = sizeof(TIFHEAD)+(nW * nH * 2);  /* Offset of the first Image File Directory*/

	DWORD dwAddr = header.dwIFDOffset + sizeof(TIFIFD);
	//IFD
	static TIFIFD ifd;
	ZeroMemory(&ifd, sizeof(ifd));
	ifd.wNumDirEntries = 16; //tag16個
	ifd.tagList[0].wTagId = 254;
	ifd.tagList[0].wDataType = 0x04;
	ifd.tagList[0].dwDataCount = 1;
	ifd.tagList[0].dwDataOffset = 0;
	ifd.tagList[1].wTagId = 256;
	ifd.tagList[1].wDataType = 0x03;
	ifd.tagList[1].dwDataCount = 1;
	ifd.tagList[1].dwDataOffset = nW;
	ifd.tagList[2].wTagId = 257;
	ifd.tagList[2].wDataType = 0x03;
	ifd.tagList[2].dwDataCount = 1;
	ifd.tagList[2].dwDataOffset = nH;
	ifd.tagList[3].wTagId = 258; //BitsPerSample
	ifd.tagList[3].wDataType = 0x03;
	ifd.tagList[3].dwDataCount = 1;
	ifd.tagList[3].dwDataOffset = 16;
	ifd.tagList[4].wTagId = 259; //Compression
	ifd.tagList[4].wDataType = 0x03;
	ifd.tagList[4].dwDataCount = 1;
	ifd.tagList[4].dwDataOffset = 1; //圧縮なし
	ifd.tagList[5].wTagId = 262; //PhotometricInterpretation
	ifd.tagList[5].wDataType = 0x03;
	ifd.tagList[5].dwDataCount = 1;
	ifd.tagList[5].dwDataOffset = 1;
	ifd.tagList[6].wTagId = 273; //StripOffsets
	ifd.tagList[6].wDataType = 0x04;
	ifd.tagList[6].dwDataCount = nH;
	ifd.tagList[6].dwDataOffset = dwAddr;//画像データの開始位置
	dwAddr += sizeof(DWORD)* ifd.tagList[6].dwDataCount;
	ifd.tagList[7].wTagId = 274; //Orientation
	ifd.tagList[7].wDataType = 0x03;
	ifd.tagList[7].dwDataCount = 1;
	ifd.tagList[7].dwDataOffset = 1; //TOP LEFT
	ifd.tagList[8].wTagId = 277; //SaplesPerPixel
	ifd.tagList[8].wDataType = 0x03;
	ifd.tagList[8].dwDataCount = 1;
	ifd.tagList[8].dwDataOffset = 1;
	ifd.tagList[9].wTagId = 278; //RowsPerStrip
	ifd.tagList[9].wDataType = 0x03;
	ifd.tagList[9].dwDataCount = 1;
	ifd.tagList[9].dwDataOffset = 3;
	ifd.tagList[10].wTagId = 279; //StripByteCounts//////////////////////////
	ifd.tagList[10].wDataType = 0x04;
	ifd.tagList[10].dwDataCount = nH;
	ifd.tagList[10].dwDataOffset = dwAddr;
	dwAddr += sizeof(DWORD)* ifd.tagList[10].dwDataCount;
	ifd.tagList[11].wTagId = 284; //PlanarConfiguration
	ifd.tagList[11].wDataType = 0x03;
	ifd.tagList[11].dwDataCount = 1;
	ifd.tagList[11].dwDataOffset = 1;
	ifd.tagList[12].wTagId = 296; //ResolutionUnit
	ifd.tagList[12].wDataType = 0x03;
	ifd.tagList[12].dwDataCount = 1;
	ifd.tagList[12].dwDataOffset = 2;
	ifd.tagList[13].wTagId = 339; //SampleFormat
	ifd.tagList[13].wDataType = 0x03;
	ifd.tagList[13].dwDataCount = 1;
	ifd.tagList[13].dwDataOffset = 1;
	ifd.tagList[14].wTagId = 282; //XResolution
	ifd.tagList[14].wDataType = 0x05;
	ifd.tagList[14].dwDataCount = 1;
	ifd.tagList[14].dwDataOffset = dwAddr;
	dwAddr += sizeof(DWORD)*2;
	ifd.tagList[15].wTagId = 283; //YResolution
	ifd.tagList[15].wDataType = 0x05;
	ifd.tagList[15].dwDataCount = 1;
	ifd.tagList[15].dwDataOffset = dwAddr;
	ifd.dwNextIFDOffset = 0; //ifd終了	
	DWORD dwAddrStrip = 8;
	DWORD dwStripBC = nW * 2;
	DWORD dw1 = 0x48000000;
	DWORD dw2 = 0x1000000;

	CFile file;
	if (file.Open(lpszPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		//ヘッダエントリ
		file.Write(&header, sizeof(TIFHEAD)); //8Byte書き込み
		file.Write(m_imageData.m_pBuf, m_imageData.GetSize()); //データ
		file.Write(&ifd, sizeof(TIFIFD)); //IFD
		for (int i = 0; i < nH; i++){ //StripOffsets			
			file.Write(&dwAddrStrip, sizeof(DWORD));
			dwAddrStrip += dwStripBC;
		}
		for (int i = 0; i < nH; i++){ //StripByteCount
			file.Write(&dwStripBC, sizeof(DWORD));
		}
		file.Write(&dw1, sizeof(DWORD));
		file.Write(&dw2, sizeof(DWORD));
		file.Write(&dw1, sizeof(DWORD));
		file.Write(&dw2, sizeof(DWORD));
		return TRUE;
	}
	return FALSE;
}

//データ領域取得
WORD* Tiff16::GetDataPtr()
{
	return m_imageData.m_pBuf;
}


//指定座標データ取得
WORD Tiff16::GetData(CPoint &pt)
{
	WORD *pSrc = GetDataPtr();
	if (0 <= pt.x && pt.x < m_nW && 0 <= pt.y && pt.y < m_nH){
		return *(pSrc + m_nW*pt.y + pt.x);
	}
	return 0;
}

