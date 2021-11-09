//
// Dib256.cpp --- Direct Draw CDib class
//
#include "../stdafx.h"
#include "dib256.h"

//
// Init --- メンバーの初期化
//
void Dib256::Init()
{
	ZeroMemory(&m_bmFileHeader,sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_bmInfoHeader,sizeof(BITMAPINFOHEADER));
	ZeroMemory(m_aryRGB,sizeof(m_aryRGB)); //256色パレット	
}
//
// コンストラクタ
//
Dib256::Dib256()
{
	Init();
}
//
// 生成とファイルのロード
//
Dib256::Dib256( LPCWSTR pszFilename )
{
	Init();
	LoadFile(pszFilename);
}
//
// デストラクタ(メモリの解放）
//
Dib256::~Dib256()
{	
}

//
// ColorsUsed --- DIBで使われている色の数を返す
//
inline DWORD Dib256::ColorsUsed()
{
	//
	// 色数の情報があれば、それを使う
	//
	if( m_bmInfoHeader.biClrUsed != 0 )
		return m_bmInfoHeader.biClrUsed;
	//
	// そうでなければ、ピクセル当りのビット数から決める
	//
	switch( m_bmInfoHeader.biBitCount )
	{
		case 1: 
			return 2;
		case 4: 
			return 16;
		case 8: 
			return 256;
		default: 
			return 0;
	}
}
//
// ビットマップの幅を返す(ピクセル単位)
//
int Dib256::GetWidth()
{
	return m_bmInfoHeader.biWidth;
}
//
// ビットマップの幅を返す(ピクセル単位)
//
int	Dib256::GetHeight()
{
	return m_bmInfoHeader.biHeight;
}
//
// RowWidth --- ビットマップの1行(横方向)のバイト数を返す
//
// 注意:
//   常に4の倍数です。
//
int Dib256::GetLineSize()
{
	return ((GetWidth() + 3) & ~3);
} 

//
// Bpp --- ピクセル当りのビット数を返します
//
int	Dib256::Bpp()
{
	return m_bmInfoHeader.biBitCount;
}
//
// GetPaletteEntries --- 指定された範囲のパレットエントリを返します
//
void Dib256::GetPaletteEntries(int first, int count, PALETTEENTRY * entries)
{
	while(count--)
	{
		entries->peRed = m_aryRGB[first].rgbRed;
		entries->peGreen = m_aryRGB[first].rgbGreen;
		entries->peBlue = m_aryRGB[first].rgbBlue;

		entries++;
		first++;
	}
}

//
// LoadFile --- DIBファイルのロード
//
BOOL Dib256::LoadFile( LPCWSTR pszFilename )
{
	CFile file;
	file.Open((LPCTSTR)pszFilename,CFile::modeRead | CFile::typeBinary);
	//
	// ファイルヘッダーを読み取ります。
	//	

	if( file.Read(&m_bmFileHeader,sizeof(m_bmFileHeader)) ){
		DWORD colorTableSize;
		//
		// ファイルの種類をチェックして、これがビットマップの場合は
		// 情報を読み取ります。
		//
		if( m_bmFileHeader.bfType != 0x4d42 ||
			file.Read(&m_bmInfoHeader, sizeof(BITMAPINFOHEADER)) <= 0){
			return FALSE;
		}
		//
		// ヘッダーをチェックして、ビットマップとして
		// 有効かどうかを確認します。
		//
		BITMAPINFOHEADER *p = &m_bmInfoHeader;

		if( !((p->biSize == sizeof(BITMAPINFOHEADER)) &&
			 (p->biPlanes == 1) &&
			 ((p->biBitCount == 1) ||
			  (p->biBitCount == 4) ||
			  (p->biBitCount == 8) ||
			  (p->biBitCount == 24)
			 ) &&
			 (p->biCompression == BI_RGB))){
			return FALSE;
		}
		//
		// カラーテーブルを読み取ります。
		//
		colorTableSize = ColorsUsed() * sizeof(long);
		if( p->biBitCount <= 8 ){
			if( file.Read(m_aryRGB, colorTableSize) <= 0 ){
				return FALSE;
			}
		}
	} else {
		return FALSE;
	}
	//
	// ビットマップビットを読み取ります。
	//
	int nSize = m_bmInfoHeader.biSizeImage;
	if( nSize == 0 ){
		nSize = GetLineSize() * m_bmInfoHeader.biHeight;
		m_bmInfoHeader.biSizeImage = nSize; //画像データサイズを必ずヘッダに記録する
	}
	if( m_buf.Alloc(nSize) ){
		return (file.Read(GetBits(), m_bmInfoHeader.biSizeImage) > 0 );	
	}	
	return FALSE;
}
//
// LoadFile --- DIBファイルのロード
//
BOOL Dib256::SaveFile( LPCWSTR pszFilename )
{
	BYTE *pBits = GetBits();
	if( pBits != NULL ){
		CFile file;
		if (file.Open((LPCTSTR)pszFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
			file.Write(&m_bmFileHeader, sizeof(BITMAPFILEHEADER)); // ビットマップファイルヘッダーを出力する
			file.Write(&m_bmInfoHeader, sizeof(BITMAPINFOHEADER));	// ビットマップインフォヘッダーを出力する
			for (int i = 0; i < 256; i++){// ビットマップカラーパレットを出力する
				file.Write(&m_aryRGB[i], sizeof(RGBQUAD));
			}
			// ビットマップ画像を出力する
			int nW = GetLineSize();
			int nH = GetHeight();
			file.Write(GetBits(), nW*nH);
			return TRUE;
		}
	}
	return FALSE;
}

//256色ビットマップの作成
BOOL Dib256::Create(int nW,int nH)
{
	if( GetWidth() != nW || GetHeight() != nH ){
		int nLineSize  = (nW + 3) & ~3;
		int nImageSize = nLineSize * nH;
		 // ビットマップファイルヘッダーを作成する
		ZeroMemory(&m_bmFileHeader,sizeof(BITMAPFILEHEADER));
		m_bmFileHeader.bfType	 = 0x4d42;
		m_bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;	//offset
		m_bmFileHeader.bfSize	 = m_bmFileHeader.bfOffBits + nImageSize; //File Size
		// ビットマップインフォヘッダーを作成する
		ZeroMemory(&m_bmInfoHeader,sizeof(BITMAPINFOHEADER));
		m_bmInfoHeader.biSize		= sizeof(BITMAPINFOHEADER);
		m_bmInfoHeader.biWidth		= nW;
		m_bmInfoHeader.biHeight		= nH;
		m_bmInfoHeader.biPlanes		= 1;
		m_bmInfoHeader.biBitCount	= 8;
		m_bmInfoHeader.biCompression= BI_RGB;
		m_bmInfoHeader.biSizeImage	= nImageSize;
		m_bmInfoHeader.biClrUsed	= 256;
		// ビットマップカラーパレットを作成する
		for(int i=0;i<256;i++){
			m_aryRGB[i].rgbBlue = m_aryRGB[i].rgbGreen = m_aryRGB[i].rgbRed = i;
		}
		// ビットマップ画像を作成する
		if( !m_buf.Alloc(nImageSize) ){
			return FALSE;
		}		
	}
	return TRUE;
}

//ビットマップコピー　フルカラー->256色
BOOL Dib256::Copy(CDIB &dibSrc)
{
	BYTE *pSrc = dibSrc.GetBits(); //コピー元取得
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();
	if( GetWidth() != nW || GetHeight() != nH ){
		if( !Create(nW,nH) ){ //サイズ変更
			return FALSE;
		}
	}
	BYTE *pDst = GetBits();	//コピー先取得
	int nPadSrc = dibSrc.GetLineSize() - nW * 3; //パディングサイズ
	int nPadDst = GetLineSize() - nW;	
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pSrc++;//B
			BYTE byG = *pSrc++;//G
			BYTE byR = *pSrc++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
			*pDst++ = by256;
		}
		pSrc += nPadSrc; //1Line終了
		pDst += nPadDst;
	}	
	return TRUE;
}
//ビットマップコピー　256色 -> 256色
BOOL Dib256::Copy(Dib256 &dibSrc)
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();
	if( GetWidth() != nW || GetHeight() != nH ){
		if( !Create(nW,nH) ){ //サイズ変更
			return FALSE;
		}
	}
	BYTE *pSrc = dibSrc.GetBits(); //コピー元取得
	BYTE *pDst = GetBits();	//コピー先取得
	memcpy(pDst,pSrc,m_bmInfoHeader.biSizeImage); //データコピー
	return TRUE;	
}

//ビットマップコピー　256色->フルカラー
BOOL Dib256::Paste(CDIB &dibDst)
{
	int nW = GetWidth();
	int nH = GetHeight();
	if( dibDst.GetWidth() != nW || dibDst.GetHeight() != nH ){
		if( !dibDst.Create(nW,nH) ){ //サイズ変更
			return FALSE;
		}
	}
	BYTE *pSrc = GetBits(); //コピー元取得
	int nPadSrc = GetLineSize() - nW;
	BYTE *pDst = dibDst.GetBits(); //コピー先取得
	int nPadDst = dibDst.GetLineSize() - nW * 3; //パディングサイズ
	
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pSrc++){
			*pDst++ = *pSrc;//B
			*pDst++ = *pSrc;//G
			*pDst++ = *pSrc;//R
		}
		pSrc += nPadSrc; //1Line終了
		pDst += nPadDst;
	}	
	return TRUE;
}

//ビットマップ切り取り　フルカラー->256色
BOOL Dib256::Cut(CDIB &dibSrc,CRect &rect)
{
	int nSrcW = dibSrc.GetWidth(); //Src画像幅
	int nSrcH = dibSrc.GetHeight();//Src画像高さ
	//サイズ調整
	if(rect.left < 0 ){
		rect.left = 0;
	}
	if( rect.top < 0 ){
		rect.top = 0;
	}
	if( rect.right >= nSrcW ){
		rect.right = nSrcW-1;
	}
	if( rect.bottom >= nSrcH ){
		rect.bottom = nSrcH-1;
	}
	
	int nW = rect.Width()+1; //探索画像のサイズ取得
	int nH = rect.Height()+1;	
	if( Create(nW,nH) ){
		BYTE *pSrcTop = dibSrc.GetBits(); //コピー元取得
		BYTE *pDstTop = GetBits();		//コピー先取得
		int nLineSrc = dibSrc.GetLineSize();//コピー元ラインサイズ
		int nLineDst = GetLineSize();		//コピー先ラインサイズ
		for(int y=0;y<nH;y++){
			BYTE *pSrc = pSrcTop + (nSrcH-(rect.top+y)-1)*nLineSrc + rect.left * 3;
			BYTE *pDst = pDstTop + (nH-y-1)*nLineDst;
			for(int x=0;x<nW;x++){
				BYTE byB = *pSrc++;//B
				BYTE byG = *pSrc++;//G
				BYTE byR = *pSrc++;//R
				BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
				*pDst++ = by256;
			}
		}	
		return TRUE;		
	}
	return FALSE;
}
//ビットマップ切り取り　フルカラー->256色
BOOL Dib256::Cut(CDIB &dibSrc,CRect &rect,int nCx,int nCy)
{
	int nSrcW = dibSrc.GetWidth(); //Src画像幅
	int nSrcH = dibSrc.GetHeight();//Src画像高さ
	//サイズ調整
	if(rect.left < 0 ){
		rect.left = 0;
	}
	if( rect.top < 0 ){
		rect.top = 0;
	}
	if( rect.right >= nSrcW ){
		rect.right = nSrcW-1;
	}
	if( rect.bottom >= nSrcH ){
		rect.bottom = nSrcH-1;
	}
	if( nCx <= 0 || nCy <= 0 ){
		nCx = rect.Width();
		nCy = rect.Height();
	}
	CDIB dib;
	if( dib.Create(nCx,nCy) ){ //１文字分のサイズ
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL); //Display互換メモリDC
		CBitmap *pSrcOld = dcSrc.SelectObject( dibSrc.GetBitmap() );

		CDC dcDst;
		dcDst.CreateCompatibleDC(NULL); //Display互換メモリDC
		CBitmap *pDstOld = dcDst.SelectObject( dib.GetBitmap() );

		dcDst.SetStretchBltMode(STRETCH_DELETESCANS);//平均化はしない
		dcDst.StretchBlt(0,0,nCx,nCy,&dcSrc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);

		//後始末
		dcSrc.SelectObject( pSrcOld );
		dcDst.SelectObject( pDstOld );

		if( Create(nCx,nCy) ){
			this->Copy( dib ); //フルカラ- -> 256色
			return TRUE;
		}
	}
	return FALSE;
}

//コントラスト最大化
void Dib256::MinMax(BYTE byMin,BYTE byMax)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nH = GetHeight();
	int xmax = GetLineSize(); //ラインサイズ
	BYTE byRange = byMax-byMin;
	BYTE byVal;
	for(int y=0;y<nH;y++){
		for(int x=0;x<xmax;x++){
			if(*pBits <= byMin){
				byVal = 0;
			}else if(*pBits >= byMax){
				byVal = MAXBYTE;
			}else{
				byVal = (*pBits - byMin) * MAXBYTE / byRange; //Rangeを広げる
			}
			*pBits++ = byVal; //値変更
		}
	}
}

//コントラスト最大化
void Dib256::MinMax()
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nLine = GetLineSize(); //ラインサイズ
	int nH = GetHeight();
	int nW = GetWidth();
	int nPad = nLine-nW;

	BYTE byMin = *pBits;
	BYTE byMax = *pBits;
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pBits++){
			if( *pBits < byMin ){
				byMin = *pBits;
			}else if(*pBits > byMax ){
				byMax = *pBits;
			}
		}
		pBits += nPad;
	}
	MinMax(byMin,byMax);
}

//輝度反転
void Dib256::Invert()
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nSize = GetSize();
	for(int i=0;i<nSize;i++){
		*pBits++ = MAXBYTE - *pBits; //反転
	}	
}

//近似値取得
double Dib256::GetAprxVal(double dbX,double dbY)
{
	int nX = (int)dbX;
	int nY = (int)dbY;
	if( nX < GetWidth()-1 && nY < GetHeight()-1){
		double dbVal0 = GetValue(nX  ,nY);
		double dbVal1 = GetValue(nX+1,nY);
		double dbVal2 = GetValue(nX  ,nY+1);
		double dbVal3 = GetValue(nX+1,nY+1);
		//x方向近似
		double dbVal01 = dbVal0*(nX+1-dbX) + dbVal1*(dbX-nX);
		double dbVal23 = dbVal2*(nX+1-dbX) + dbVal3*(dbX-nX);
		//y方向近似
		return (dbVal01*(nY+1-dbY)+dbVal23*(dbY-nY));
	}
	return GetValue(nX,nY);	
}

//指定領域内の左端黒位置を収集
static double CollectLeftEdgeBlack(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	BOOL isMin = (nType==Dib256::BLACK_MIN);
	BOOL isMax = (nType==Dib256::BLACK_MAX);
	BOOL isAvg = (nType==Dib256::BLACK_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	int nW = pSize->cx;
	int nH = pSize->cy;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pBits + nW*(nH-y-1)+nLeft; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData++; //nLeft+1から
		for(int x=nLeft+1;x<=nRight;x++,pData++){
			if( *pData < nTh ){ //しきい値を下回った
				//   ●x-1       ●x
				//  byPrev    *pData
				//    x - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //しきい値との差分
				double dbD2 = byPrev - *pData; //以前のデータとの差分
				double dbLeft = x;
				if( dbD2 > 0 && dbD1 < dbD2 ){
					dbLeft -= dbD1/dbD2; //左座標の計算
				}
				if( isMin && dbLeft < dbMin){//最小値　黒
					dbMin = dbLeft;
				}else if(isMax && dbMax < dbLeft){//最大値　黒
					dbMax = dbLeft;
				}else{
					dbSum += dbLeft;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値　黒
		return dbMin;
	}else if(isMax){//最大値　黒
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}	
	return -1;
}

//指定領域内の左端白位置を収集
static double CollectLeftEdgeWhite(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	BOOL isMin = (nType==Dib256::WHITE_MIN);
	BOOL isMax = (nType==Dib256::WHITE_MAX);
	BOOL isAvg = (nType==Dib256::WHITE_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	int nW = pSize->cx;
	int nH = pSize->cy;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pBits + nW*(nH-y-1)+nLeft; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData++; //nLeft+1から
		for(int x=nLeft+1;x<=nRight;x++,pData++){
			if( *pData >= nTh ){ //しきい値を上回った
				//   ●x-1    <  ●x
				//  byPrev    *pData
				//    x - (*pData-nTh)/(*pData-byPrev)
				double dbD1 = *pData - nTh;    //しきい値との差分
				double dbD2 = *pData - byPrev; //以前のデータとの差分
				double dbLeft = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbLeft -= dbD1/dbD2; //左座標の計算
				}
				if( isMin && dbLeft < dbMin){//最小値　黒
					dbMin = dbLeft;
				}else if(isMax && dbMax < dbLeft){//最大値　黒
					dbMax = dbLeft;
				}else{
					dbSum += dbLeft;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値　白
		return dbMin;
	}else if(isMax){//最大値　白
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}	
	return -1;
}

//指定領域内の上端黒位置を収集
static double CollectTopEdgeBlack(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::BLACK_MIN);
	BOOL isMax = (nType==Dib256::BLACK_MAX);
	BOOL isAvg = (nType==Dib256::BLACK_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int x=nLeft;x<=nRight;x++){
		BYTE *pData = pBits + nW*(nH-nTop-1) + x; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData -= nW; //nTop+1から
		for(int y=nTop+1;y<=nBtm;y++,pData-=nW){
			if( *pData < nTh){ //しきい値を下回った
				//   ●y-1       ●y
				//  byPrev    *pData
				//    y - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //しきい値との差分
				double dbD2 = byPrev - *pData; //以前のデータとの差分
				double dbTop = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbTop -= dbD1/dbD2; //左座標の計算
				}
				if( isMin && dbTop < dbMin){//最小値　黒
					dbMin = dbTop;
				}else if(isMax && dbMax < dbTop){//最大値　黒
					dbMax = dbTop;
				}else{
					dbSum += dbTop;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値　黒
		return dbMin;
	}else if(isMax){//最大値　黒
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}	
	return -1;	
}

//指定領域内の上端黒位置を収集
static double CollectTopEdgeWhite(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::WHITE_MIN);
	BOOL isMax = (nType==Dib256::WHITE_MAX);
	BOOL isAvg = (nType==Dib256::WHITE_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int x=nLeft;x<=nRight;x++){
		BYTE *pData = pBits + nW*(nH-nTop-1) + x; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData -= nW; //nTop+1から
		for(int y=nTop+1;y<=nBtm;y++,pData-=nW){
			if( *pData >= nTh ){ //しきい値を上回った
				//   ●y-1       ●y
				//  byPrev    *pData
				//    y - (*pData-nTh)/(*pData-byPrev)
				double dbD1 = *pData - nTh; //しきい値との差分
				double dbD2 = *pData - byPrev; //以前のデータとの差分
				double dbTop = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbTop -= dbD1/dbD2; //左座標の計算
				}
				if( isMin ){
					if( dbTop < dbMin){//最小値　
						dbMin = dbTop;
					}
				}else if(isMax ){
					if(dbMax < dbTop){//最大値
						dbMax = dbTop;
					}
				}else{
					dbSum += dbTop;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値
		return dbMin;
	}else if(isMax){//最大値
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}
	return -1;
}

//指定領域内の右端黒位置を収集
static double CollectRightEdgeBlack(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::BLACK_MIN);
	BOOL isMax = (nType==Dib256::BLACK_MAX);
	BOOL isAvg = (nType==Dib256::BLACK_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pBits + nW*(nH-y-1) + nRight; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData--; //nRight-1から
		for(int x=nRight;x>nLeft;x--,pData--){
			if( *pData < nTh ){ //しきい値を下回った
				//   ●x    <    ●x+1
				//*(pData)   byPrev
				//    x +  (nTh-*pData) / (byPrev-*pData)
				double dbD1 = nTh-*pData;	//しきい値との差分
				double dbD2 = byPrev-*pData; //以前のデータとの差分
				double dbRight = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbRight -= dbD1/dbD2; //右座標の計算
				}
				if( isMin && dbRight < dbMin){//最小値　黒
					dbMin = dbRight;
				}else if(isMax && dbMax < dbRight){//最大値　黒
					dbMax = dbRight;
				}else{
					dbSum += dbRight;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値　黒
		return dbMin;
	}else if(isMax){//最大値　黒
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}
	return -1;
}

//指定領域内の右端黒位置を収集
static double CollectRightEdgeWhite(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::WHITE_MIN);
	BOOL isMax = (nType==Dib256::WHITE_MAX);
	BOOL isAvg = (nType==Dib256::WHITE_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pBits + nW*(nH-y-1) + nRight; //Start位置
		BYTE byPrev = *pData; //以前の値
		pData--; //nRight-1から
		for(int x=nRight;x>nLeft;x--,pData--){
			if( *pData >= nTh ){ //しきい値を下回った
				//   ●x    >    ●x+1
				//*(pData)    *(pData+1)
				//    x + (*pData - nTh) / (*pData-byPrev)
				double dbD1 = *pData - nTh; //しきい値との差分
				double dbD2 = *pData - byPrev; //以前のデータとの差分
				double dbRight = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbRight += dbD1/dbD2; //右座標の計算
				}
				if( isMin && dbRight < dbMin){//最小値
					dbMin = dbRight;
				}else if(isMax && dbMax < dbRight){//最大値
					dbMax = dbRight;
				}else{
					dbSum += dbRight;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値
		return dbMin;
	}else if(isMax){//最大値
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}
	return -1;
}

//指定領域内の下端黒位置を収集
static double CollectBottomEdgeBlack(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::BLACK_MIN);
	BOOL isMax = (nType==Dib256::BLACK_MAX);
	BOOL isAvg = (nType==Dib256::BLACK_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int x=nLeft;x<=nRight;x++){
		BYTE *pData = pBits + nW*(nH-nBtm) + x; //Start位置 y==nBtm+1
		BYTE byPrev = *pData; //以前の値
		pData += nW; //nTop+1から
		for(int y=nBtm;y>nTop;y--,pData+=nW){
			if( *pData < nTh ){ //しきい値を下回った
				//   ●y       ●y+1
				//  *pData    byPrev
				//    y+1 - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //しきい値との差分
				double dbD2 = byPrev - *pData; //以前のデータとの差分
				double dbBtm = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbBtm += dbD1/dbD2; //左座標の計算
				}
				if( isMin && dbBtm < dbMin){//最小値
					dbMin = dbBtm;
				}else if(isMax && dbMax < dbBtm){//最大値
					dbMax = dbBtm;
				}else{
					dbSum += dbBtm;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値
		return dbMin;
	}else if(isMax){//最大値
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}
	return -1;
}


//指定領域内の下端白位置を収集
static double CollectBottomEdgeWhite(int nType,int nTh,BYTE *pBits,SIZE *pSize,RECT *pRect)
{
	int nLeft = pRect->left;
	int nTop  = pRect->top;
	int nRight= pRect->right;
	int nBtm  = pRect->bottom;
	int nW = pSize->cx;
	int nH = pSize->cy;
	BOOL isMin = (nType==Dib256::WHITE_MIN);
	BOOL isMax = (nType==Dib256::WHITE_MAX);
	BOOL isAvg = (nType==Dib256::WHITE_AVG);
	double dbMin = INT_MAX;
	double dbMax = 0;
	double dbSum = 0;
	int nSum = 0;
	for(int x=nLeft;x<=nRight;x++){
		BYTE *pData = pBits + nW*(nH-nBtm) + x; //Start位置 y==nBtm+1
		BYTE byPrev = *pData; //以前の値
		pData += nW; //nBtm+1から
		for(int y=nBtm;y>nTop;y--,pData+=nW){
			if( *pData >= nTh ){ //しきい値を上回った
				//   ●y       ●y+1
				//  *pData    byPrev
				//    y + (nTh-*pData)/(byPrev-*pData)
				double dbD1 = *pData - nTh; //しきい値との差分
				double dbD2 = *pData - byPrev; //以前のデータとの差分
				double dbBtm = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbBtm += dbD1/dbD2; //左座標の計算
				}
				if( isMin && dbBtm < dbMin){//最小値
					dbMin = dbBtm;
				}else if(isMax && dbMax < dbBtm){//最大値
					dbMax = dbBtm;
				}else{
					dbSum += dbBtm;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //以前の値を記憶
		}
	}
	if(isMin){//最小値
		return dbMin;
	}else if(isMax){//最大値
		return dbMax;
	}else if(isAvg && nSum > 0){//平均値
		return (dbSum / nSum);
	}
	return -1;
}

static double GetLeftEdge(int nType,int nTh,BYTE *pGray,SIZE *pSize,RECT *pRect)
{
	if( pGray != NULL && pSize != NULL && pRect != NULL){
		//Size Check
		pRect->top    = MAX(          0,pRect->top);
		pRect->bottom = MIN(pSize->cy-1,pRect->bottom);
		pRect->left   = MAX(          0,pRect->left);
		pRect->right  = MIN(pSize->cx-1,pRect->right);		
		//タイプ別データ収集
		switch(nType){
			case Dib256::BLACK_MIN:		//最小値　黒
			case Dib256::BLACK_MAX:		//最大値　黒
			case Dib256::BLACK_AVG:		//平均値　黒
				return CollectLeftEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//最小値　白
			case Dib256::WHITE_MAX:		//最大値　白
			case Dib256::WHITE_AVG:		//平均値　白	
				return CollectLeftEdgeWhite(nType,nTh,pGray,pSize,pRect);
		}
	}
	return -1;
}
static double GetTopEdge(int nType,int nTh,BYTE *pGray,SIZE *pSize,RECT *pRect)
{
	if( pGray != NULL && pSize != NULL && pRect != NULL){
		//Size Check
		pRect->top    = MAX(          0,pRect->top);
		pRect->bottom = min(pSize->cy-1,pRect->bottom);
		pRect->left   = MAX(          0,pRect->left);
		pRect->right  = min(pSize->cx-1,pRect->right);		
		//タイプ別データ収集
		switch(nType){
			case Dib256::BLACK_MIN:		//最小値　黒
			case Dib256::BLACK_MAX:		//最大値　黒
			case Dib256::BLACK_AVG:		//平均値　黒
				return CollectTopEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//最小値　白
			case Dib256::WHITE_MAX:		//最大値　白
			case Dib256::WHITE_AVG:		//平均値　白	
				return CollectTopEdgeWhite(nType,nTh,pGray,pSize,pRect);
		}
	}
	return -1;
}
static double GetRightEdge(int nType,int nTh,BYTE *pGray,SIZE *pSize,RECT *pRect)
{
	if( pGray != NULL && pSize != NULL && pRect != NULL){
		//Size Check
		pRect->top    = MAX(          0,pRect->top);
		pRect->bottom = min(pSize->cy-1,pRect->bottom);
		pRect->left   = MAX(          0,pRect->left);
		pRect->right  = min(pSize->cx-1,pRect->right);		
		//タイプ別データ収集
		switch(nType){
			case Dib256::BLACK_MIN:		//最小値　黒
			case Dib256::BLACK_MAX:		//最大値　黒
			case Dib256::BLACK_AVG:		//平均値　黒
				return CollectRightEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//最小値　白
			case Dib256::WHITE_MAX:		//最大値　白
			case Dib256::WHITE_AVG:		//平均値　白	
				return CollectRightEdgeWhite(nType,nTh,pGray,pSize,pRect);
		}
	}
	return -1;
}
static double GetBottomEdge(int nType,int nTh,BYTE *pGray,SIZE *pSize,RECT *pRect)
{
	if( pGray != NULL && pSize != NULL && pRect != NULL){
		//Size Check
		pRect->top    = MAX(          0,pRect->top);
		pRect->bottom = min(pSize->cy-1,pRect->bottom);
		pRect->left   = MAX(          0,pRect->left);
		pRect->right  = min(pSize->cx-1,pRect->right);		
		//タイプ別データ収集
		switch(nType){
			case Dib256::BLACK_MIN:		//最小値　黒
			case Dib256::BLACK_MAX:		//最大値　黒
			case Dib256::BLACK_AVG:		//平均値　黒
				return CollectBottomEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//最小値　白
			case Dib256::WHITE_MAX:		//最大値　白
			case Dib256::WHITE_AVG:		//平均値　白	
				return CollectBottomEdgeWhite(nType,nTh,pGray,pSize,pRect);
		}
	}
	return -1;
}

//指定領域内の左端のx座標を取得
double Dib256::GetLeftEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetLeftEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//指定領域内の右端のx座標を取得
double Dib256::GetRightEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetRightEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//指定領域内の上端のy座標を取得
double Dib256::GetTopEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetTopEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//指定領域内の下端のy座標を取得
double Dib256::GetBottomEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetBottomEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}

//しきい値以上の画素数を取得
int Dib256::CountPixels(int nTh)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nCnt = 0;
	int nPad = GetLineSize() - nW; //パディングサイズ
	BYTE *pData = GetBits();
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			if( *pData++ >= nTh ){
				nCnt++;
			}
		}
		pData += nPad;
	}	
	return nCnt;
}

//黒で塗りつぶし
void Dib256::PaintBlack()
{
	if( GetBits() != NULL ){
		memset(GetBits(),0,GetSize());
	}
}
//白で塗りつぶし
void Dib256::PaintWhite()
{
	if( GetBits() != NULL ){
		memset(GetBits(),MAXBYTE,GetSize());
	}
}

//最大地点取得
CPoint Dib256::GetMaxValuePoint(const CRect &rect)
{
	int nLeft = rect.left;
	int nTop  = rect.top;
	int nRight= rect.right;
	int nBtm  = rect.bottom;

	CPoint pt(0,0);
	BYTE byMax = 0;
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW; //パディングサイズ
	BYTE *pDataT = GetBits();
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pDataT + nW*(nH-y-1) + nLeft; //Start位置
		for(int x=nLeft;x<=nRight;x++,pData++){
			if(byMax < *pData){
				byMax = *pData;
				pt.x = x;
				pt.y = y;
			}
		}
	}	
	return pt;
}

//ヒストグラム取得
void Dib256::CollectHistgram(int nHist[])
{
	ZeroMemory(nHist,sizeof(int)*(MAXBYTE+1)); //0 clear
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW;
	BYTE *pSrc = GetBits();
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			nHist[*pSrc++]++; //ヒストグラムカウント
		}
		pSrc += nPad;
	}
	//for(int i=0;i<256;i++){
	//	TRACE( "nHist[%d]:%5d\n",i,nHist[i]);
	//}
}
//2値化処理
void Dib256::ChangeBinary(BYTE byTh)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW; //パディングサイズ
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pBits++){
			*pBits = (*pBits >= byTh) ?  MAXBYTE : 0;
		}
		pBits += nPad; //1Line終了
	}
}

//しきい値計算
int Dib256::CalcThreshold()
{
	int nTh = 128;
	//領域ごとに2値化
	int nBlack = 0; //黒の画素数
	int nHist[256];
	CollectHistgram(nHist);//ヒストグラム収集
	//nTh以上の最大要素取得		
	int nMax = 0;
	int nImax = 0;
	for(int i=nTh;i<=MAXBYTE;i++){
		if( nMax < nHist[i] ){//より大きいほうへ
			nMax = nHist[i];
			nImax = i;
		}
	}
	//nThより下の最大要素
	nMax = 0;
	int nImin = 0;
	for(int i=nTh-1;i>=0;i--){
		if( nMax < nHist[i] ){ //より大きい方へ
			nMax = nHist[i];
			nImin = i;
		}
	}
	return int( (nImin + nImax) / 2.0 + 0.5 ); //あたらしいしきい値
}

