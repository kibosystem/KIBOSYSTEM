#include "../stdafx.h"
#include <math.h>
#include "dib.h"
#include "functions.h"
#include "GlobalMgr.h"
#include "macro.h"
#include "gdiplus.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CDIB::CDIB()
{
	m_hBitmap = NULL;					//Bimap Handle	
	ZeroMemory(&m_BMI,sizeof(m_BMI));	//Bitmap Header
	m_pBits	= NULL;						//Bitmap Bits
	m_nBitsSize = 0;
}

CDIB::~CDIB()
{
	DestroyBitmap();
}

//ファイル名称のみ取得
CString CDIB::GetFileName()
{
	return ::GetFileName(m_strFilePath);
}
//Imageの解放
void CDIB::DestroyBitmap()
{
	//Bitmap開放
	if( m_hBitmap != NULL ){
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}
// Load a DIB from Resource サイズ指定
BOOL CDIB::Load(int nID)
{
	CBitmap bmp;
	bmp.LoadBitmap(nID);
	HBITMAP hBmp = (HBITMAP)bmp.GetSafeHandle();
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle 作成
	}
	return FALSE;
}

// Load a DIB from File
BOOL CDIB::Load(CString strFileName)
{
	m_strFilePath = strFileName;	

	cv::String strPath(CW2A(strFileName.GetBuffer()));
	cv::Mat src_img = cv::imread(strPath, 1);
	return FromMat(src_img);
}


// Load a DIB from Resource サイズ指定
BOOL CDIB::Load(int nID,int nCx,int nCy)
{
	CBitmap bmp;
	bmp.LoadBitmap(nID);
	HBITMAP hBmpTmp = (HBITMAP)bmp.GetSafeHandle();
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo); //Bitmap Size 取得
	HBITMAP hBmp = hBmpTmp; //Default 
	if( bmpInfo.bmWidth != nCx || bmpInfo.bmHeight != nCy ){ //Sizeが違う場合は指定サイズに変換
		hBmp = (HBITMAP)::CopyImage(hBmpTmp,IMAGE_BITMAP,nCx,nCy,LR_COPYDELETEORG); //サイズ変更 & コピー先のみ保存 
	}
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle 作成
	}
	return FALSE;
}
// Load a DIB from File サイズ指定
BOOL CDIB::Load(CString strFileName,int nCx,int nCy)
{
	HBITMAP hBmpTmp = LoadBitmapHandle(strFileName); //Bitmap読み込み
	SIZE sizeBmp; //Bitmap Size 取得
	::GetBitmapDimensionEx(hBmpTmp,&sizeBmp);
	HBITMAP hBmp = hBmpTmp; //Default 
	if( sizeBmp.cx != nCx || sizeBmp.cy != nCy ){ //Sizeが違う場合は指定サイズに変換
		hBmp = (HBITMAP)::CopyImage(hBmpTmp,IMAGE_BITMAP,nCx,nCy,LR_COPYDELETEORG); //サイズ変更 & コピー先のみ保存 
	}
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle 作成
	}
	return FALSE;
}
//指定ビットマップ読み込み
HBITMAP CDIB::LoadBitmapHandle(CString &strFileName)
{
	if( strFileName.IsEmpty() ){
		return NULL;
	}
	m_strFilePath = strFileName;
	CString strExt = strFileName.Right(4);//拡張子取得
	if( strExt == L".bmp" || strExt ==".BMP"){
		//Load Bitmap 
		return (HBITMAP)LoadImage(::AfxGetInstanceHandle(),strFileName,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);	
	}
	return NULL;
}

//DibSection Handle 作成
BOOL CDIB::CreateDibSectionHandle(HBITMAP hBmpTmp)
{
	//Get CBitmap
	CBitmap *pBmp = CBitmap::FromHandle( hBmpTmp );
	
	BITMAP BitMap; //Bitmap Info
	pBmp->GetBitmap( &BitMap ); 
	//Create 24 bpp Bitmap Handle and Bitmap Bits
	int nW = BitMap.bmWidth;
	int nH = BitMap.bmHeight;
	Create(nW,nH);
	
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcSrc;
	CDC dcDst;
	dcSrc.CreateCompatibleDC(pDC);
	dcDst.CreateCompatibleDC(pDC);
	CBitmap *pBmpSrcOld = dcSrc.SelectObject(pBmp);//Src Bitmap 選択
	CBitmap *pBmpDstOld = dcDst.SelectObject( GetBitmap() );//Destination

	dcDst.BitBlt(0,0,nW,nH,&dcSrc,0,0,SRCCOPY);
	
	//後始末
	dcSrc.SelectObject(pBmpSrcOld);
	dcDst.SelectObject(pBmpDstOld);
	dcSrc.DeleteDC();
	dcDst.DeleteDC();
	DeleteObject(hBmpTmp);
	pDC->DeleteDC();
	return TRUE;
}

// Create a new empty 24bpp DIB
BOOL CDIB::Create(int iWidth, int iHeight)
{
	if( m_BMI.biWidth != iWidth || m_BMI.biHeight != iHeight ){ // Delete any existing stuff.
		DestroyBitmap();
		//Create 24bit color Info header.
		BITMAPINFOHEADER* pBI = &m_BMI;
		pBI->biSize = sizeof(BITMAPINFOHEADER);
		pBI->biWidth = iWidth;
		pBI->biHeight = iHeight;
		pBI->biPlanes = 1;
		pBI->biBitCount = 24;
		pBI->biCompression = BI_RGB; //An uncompressed format Only
		pBI->biSizeImage = 0;
		pBI->biXPelsPerMeter = 0;
		pBI->biYPelsPerMeter = 0;
		pBI->biClrUsed = 0;
		pBI->biClrImportant = 0;

		HDC hdc = GetDC(NULL); //Screen DC使用
		m_hBitmap = CreateDIBSection(hdc,(LPBITMAPINFO)pBI,DIB_RGB_COLORS,(void **)&m_pBits,NULL,0);
		m_nBitsSize  = ((iWidth*3 + 3) & ~3 ) * abs(iHeight); //Get New Size
		ReleaseDC(NULL,hdc);
	}
	PaintBlack();
    return TRUE;
}

//指定座標の色を取得
COLORREF CDIB::GetColor(int x,int y)
{
	return RGB(GetBitsR(x,y),GetBitsG(x,y),GetBitsB(x,y));
}
//指定値画素数取得
int CDIB::GetPixels(int nVal)
{
	int nCnt = 0;
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	for(int y=0;y<nH;y++){
		for(int x=0,xmax=nW*3;x<xmax;x++){
			if( *pBits++ == nVal){
				nCnt++; //カウンターインクリメント
			}
		}
		pBits += nPad; //1Line終了
	}
	return nCnt;
}
//ライン描画
void CDIB::Line(int nX1,int nY1,int nX2,int nY2,COLORREF color)
{
	CPen pen(PS_SOLID,2,color);
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPenOld = dcMem.SelectObject(&pen);
	CBitmap *pBmpSrcOld = dcMem.SelectObject( GetBitmap() );//Src作成
	dcMem.MoveTo(nX1,nY1);
	dcMem.LineTo(nX2,nY2);
	//後始末
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpSrcOld);
}
//xy方向移動
void CDIB::Shift(CDIB &dib,double dbDx,double dbDy)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nX = (int)(dbDx + 0.5);
	int nY = (int)(dbDy + 0.5);
	PaintBlack();
	CDC dcDst;
	dcDst.CreateCompatibleDC(NULL);
	CBitmap *pBmpSrcOld = dcDst.SelectObject(GetBitmap());//Src作成
	dib.Draw(&dcDst, nX, nY, nW, nH, 0, 0);
	dcDst.SelectObject(pBmpSrcOld);
}
//ビットマップ描画
void CDIB::Draw(CDC *pDC,int nX1,int nY1,int nW1,int nH1,int nX2,int nY2)
{	
	if( pDC != NULL ){
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pBmpSrcOld = dcSrc.SelectObject( GetBitmap() );//Src作成
		pDC->BitBlt(nX1,nY1,nW1,nH1,&dcSrc,nX2,nY2,SRCCOPY);
		//後始末
		dcSrc.SelectObject(pBmpSrcOld);
	}
}
void CDIB::DrawLine(CPoint &ptS,CPoint &ptE,LOGPEN &lp)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBitmap *pBmpSrcOld = dcMem.SelectObject(GetBitmap());//Src作成
	dcMem.MoveTo(ptS);
	dcMem.LineTo(ptE);
	//後始末
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpSrcOld);
}
void CDIB::DrawRectangle(CRect &rect,LOGBRUSH &lb,LOGPEN &lp)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);//ブラシ取得
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBrush *pBrushOld = dcMem.SelectObject(pBrush);
	CBitmap *pBmpSrcOld = dcMem.SelectObject(GetBitmap());//Src作成
	dcMem.Rectangle(rect);
	//後始末
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBrushOld);
	dcMem.SelectObject(pBmpSrcOld);
}

//Bitmap -> Bitmap間
//拡大縮小描画
void CDIB::StretchDraw(CDC *pDC,int x,int y,int nW,int nH)
{
	if( pDC != NULL ){
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pBmpSrcOld = dcSrc.SelectObject( GetBitmap() );//Src作成
		pDC->SetStretchBltMode(STRETCH_DELETESCANS);//平均化はしない
		pDC->StretchBlt(x,y,nW,nH,&dcSrc,0,0,m_BMI.biWidth,m_BMI.biHeight,SRCCOPY);
		//後始末
		dcSrc.SelectObject(pBmpSrcOld);
	}
}
//白で塗りつぶし
void CDIB::PaintWhite()
{
	if( GetBits() != NULL ){
		memset(GetBits(),255,GetSize());
	}
}

//黒で塗りつぶし
void CDIB::PaintBlack()
{
	if( GetBits() != NULL ){
		memset(GetBits(),0,GetSize());
	}
}

//指定色で塗りつぶし
void CDIB::PaintColor(COLORREF color)
{
	BYTE byR = GetRValue(color);
	BYTE byG = GetGValue(color);
	BYTE byB = GetBValue(color);
	int nW = GetWidth();
	int nH = GetHeight();
	BYTE *pBits = GetBits(); //Buffer先頭	
	int nNextByte = nW % 4 ; //32bitの倍数に
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			*pBits++ = byB;
			*pBits++ = byG;
			*pBits++ = byR;
		}
		pBits += nNextByte;
	}
}
void CDIB::PaintColor(COLORREF cr1,COLORREF cr2)
{
	BYTE byR1 = GetRValue(cr1);
	BYTE byG1 = GetGValue(cr1);
	BYTE byB1 = GetBValue(cr1);
	BYTE byR2 = GetRValue(cr2);
	BYTE byG2 = GetGValue(cr2);
	BYTE byB2 = GetBValue(cr2);
	int nW = GetWidth();
	int nH = GetHeight();
	BYTE *pBits = GetBits(); //Buffer先頭
	
	int nNextByte = nW % 4 ; //32bitの倍数に
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pBits+=3){
			if( *pBits == byB1 && *(pBits+1) == byG1 && *(pBits+2) == byR1 ){
				*pBits     = byB2;
				*(pBits+1) = byG2;
				*(pBits+2) = byR2;
			}
		}
		pBits += nNextByte;
	}
}


//矩形Maskを描画します（描画のみ）
void CDIB::MaskRect(CRect &rcMask,COLORREF color)
{
	BYTE byR = GetRValue(color);
	BYTE byG = GetGValue(color);
	BYTE byB = GetBValue(color);
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize();
	BYTE *pTop = GetBits(); //Buffer先頭
		
	int nXmin = MAX(0,rcMask.left);
	int nXmax = MIN(nW,rcMask.right);
	int nYmin = MAX(0,rcMask.top);
	int nYmax = MIN(nH,rcMask.bottom);
	for(int y=nYmin;y<nYmax;y++){
		BYTE *pData = pTop + (nH-y-1)*nLine + nXmin*3;
		for(int x=nXmin;x<nXmax;x++){
			*pData++ = byB;
			*pData++ = byG;
			*pData++ = byR;
		}
	}
}

//円形のマスクを描画します
void CDIB::MaskCircle(CRect &rcMask,COLORREF color)
{
	LOGPEN lp; //指定色のペン
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = color; //色指定
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得
	LOGBRUSH lb; //ブラシ
	lb.lbColor = color; //色指定
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID; //NULLブラシ
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);

	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pBmpOld = dcMem.SelectObject( GetBitmap() );//ビットマップ選択
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBrush *pBrOld  = dcMem.SelectObject(pBrush);	

	dcMem.Ellipse(rcMask);
	
	//後始末
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBrOld);
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}

//領域の塗りつぶし
void CDIB::PaintRgn(CRgn &rgn,COLORREF cr)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	LOGBRUSH lb; //ブラシ
	lb.lbColor = cr; //色指定
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID; //塗りつぶしブラシ
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);//ブラシ取得
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pBmpOld = dcMem.SelectObject( GetBitmap() );//ビットマップ選択
	BOOL isOK = dcMem.FillRgn(&rgn,pBrush);
	//後始末
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}

//Bitmap Data 複製
void CDIB::Copy(CDIB& dibDest)
{
	dibDest.Create(GetWidth(),GetHeight());		//同サイズdib作成
	memcpy(dibDest.GetBits(),GetBits(),GetSize());	//Buffer Copy		
	dibDest.m_strFilePath = m_strFilePath; //File名 Copy
}

//Bitmap Data 複製 Size 指定
void CDIB::CopyDib(CDIB& dibSrc ,int nW,int nH)
{
	Create(nW,nH);	
	HBITMAP hBmp = (HBITMAP)::CopyImage(dibSrc.GetHandle(),IMAGE_BITMAP,nW,nH,0); //サイズ変更 
	if( hBmp != NULL ){
		CreateDibSectionHandle(hBmp);		//DibSection Handle 作成
	}
}

//指定されたビットマップを描画します。
void CDIB::SetDib(int nX,int nY,CDIB& dibSrc) 
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();

	int nNewW = max( GetWidth() ,nW ); 
	int nNewH = max( GetHeight(),nH );
	if( GetWidth() < nNewW ||  GetHeight() < nNewH){ //大きさ変更の必要あり
		 Create(nNewW,nNewH);
	}

	//▼描画すべき範囲を調べる(スクリーン座標)
	RECT	rcSrc = { nX, nY, nX+dibSrc.GetWidth(), nY+dibSrc.GetHeight() }, 
			rcDst = { 0, 0, GetWidth(), GetHeight() }, 
			rcDraw;
	IntersectRect(&rcDraw, &rcSrc, &rcDst); //描画領域取得

	//▼スキャンラインのバイト数を計算
	int nSrcLine = dibSrc.GetLineSize();
	int nDstLine = GetLineSize();
	int nCopyLine = (rcDraw.right-rcDraw.left)*3;
	int nCopyH = (rcDraw.bottom-rcDraw.top);

	//▼読み込み・書き込み開始アドレスを計算
	BYTE *pSrc = (BYTE*)dibSrc.GetBits() + (nH-1)*nSrcLine;	
	BYTE *pDst = GetBits() + (nNewH-1-nY)* nDstLine + nX*3;

	for (int j = 0; j < nCopyH; j++){
		memcpy(pDst,pSrc,nCopyLine);
		// スキャンラインの移動
		pSrc -= nSrcLine;
		pDst -= nDstLine;
	}
}

//スクリーン画像作成
void CDIB::ScreenCapture(const CRect &rect) 
{
	CDC dc;
	dc.CreateDC(L"DISPLAY", NULL, NULL, NULL);
	CWnd *pWnd = CWnd::GetDesktopWindow();
	if( pWnd != NULL ){
		CRect rcDeskTop;
		pWnd->GetClientRect(&rcDeskTop);
		//描画ビットマップ
		if( GetWidth() != rect.Width() && GetHeight() != rect.Height() ){ //大きさ変更の必要あり
			 Create(rect.Width(),rect.Height()); //Bitmap 作成
		}
		//Screen -> Bitmap
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap *pOld = dcMem.SelectObject( GetBitmap() );//ビットマップ選択
		dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&dc,rect.left,rect.top,SRCCOPY);//Bitmap描画
		dcMem.SelectObject(pOld);
	}		
}

//Bitmap Sprite 転送
void CDIB::Sprite(CDIB& dibSrc ,int nX,int nY)
{
	//▼描画すべき範囲を調べる(スクリーン座標)
	RECT	rcSrc = { nX, nY, nX+dibSrc.GetWidth(), nY+dibSrc.GetHeight() }, 
			rcDst = { 0, 0, GetWidth(), GetHeight() }, 
			rcDraw;
	IntersectRect(&rcDraw, &rcSrc, &rcDst); //描画領域取得

	//▼スキャンラインのバイト数を計算
	int nSrcLine = dibSrc.GetLineSize();
	int nDstLine = GetLineSize();

	//▼読み込み・書き込み開始アドレスを計算
	BYTE *pSrc = (BYTE*)dibSrc.GetBits()
				+ ((nY + dibSrc.GetHeight()) - rcDraw.bottom) * nSrcLine	
				+ (rcDraw.left - nX) * 3;
	BYTE *pDst = GetBits()
				+ (rcDst.bottom - rcDraw.bottom)* nDstLine 
				+ rcDraw.left * 3;

	// スプライト処理を行う
	
	// 透明色
	BYTE bySpB = dibSrc.GetBitsB(0,0);
	BYTE bySpG = dibSrc.GetBitsG(0,0);
	BYTE bySpR = dibSrc.GetBitsR(0,0);

	//▼書き込む範囲を計算
	int nWidth  = rcDraw.right  - rcDraw.left;
	int	nHeight = rcDraw.bottom - rcDraw.top;

	// 次のスキャンラインの書き込み開始位置までの距離を計算
	int nSrcNextByte  = nSrcLine - nWidth * 3;
	int nDstNextByte = nDstLine - nWidth * 3;

	for (int j = 0; j < nHeight; j++){
		for (int i = 0; i < nWidth; i++){
		// Sprite処理用
			if(bySpB != *pSrc || bySpG != *(pSrc+1) || bySpR != *(pSrc+2) ){
				// 透明でない場合は書き込む
				*pDst++ = *pSrc++; // Blue
				*pDst++ = *pSrc++; // Green
				*pDst++ = *pSrc++; // Red
			}else {
				// 透明の場合はアドレスを送るだけ
				pSrc += 3;
				pDst += 3;
			}
		}
		// スキャンラインの移動
		pSrc += nSrcNextByte;
		pDst += nDstNextByte;
	}
}

//ビットマップ切り取り　フルカラー->256色
BOOL CDIB::Cut(CDIB &dibSrc,CRect &rect)
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
			memcpy(pDst,pSrc,nLineDst); //1Line Copy
		}	
		return TRUE;		
	}
	return FALSE;
}

//シリアライズ
void CDIB::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){//Save
		ar << m_BMI.biBitCount;		//Bitmap Header
		ar << m_BMI.biSize;
		ar << m_BMI.biWidth;
		ar << m_BMI.biHeight;
		ar << m_BMI.biPlanes;
		ar << m_BMI.biBitCount;
		ar << m_BMI.biCompression;
		ar << m_BMI.biSizeImage;
		ar << m_BMI.biXPelsPerMeter;
		ar << m_BMI.biYPelsPerMeter;
		ar << m_BMI.biClrUsed;
		ar << m_BMI.biClrImportant;
		ar << m_nBitsSize;				//Bimap Size
		ar.Write(m_pBits,m_nBitsSize);	//Bitmap Bits
	}else{//Load
		ar >> m_BMI.biBitCount;		//Bitmap Header
		ar >> m_BMI.biSize;
		ar >> m_BMI.biWidth;
		ar >> m_BMI.biHeight;
		ar >> m_BMI.biPlanes;
		ar >> m_BMI.biBitCount;
		ar >> m_BMI.biCompression;
		ar >> m_BMI.biSizeImage;
		ar >> m_BMI.biXPelsPerMeter;
		ar >> m_BMI.biYPelsPerMeter;
		ar >> m_BMI.biClrUsed;
		ar >> m_BMI.biClrImportant;
		ar >> m_nBitsSize;				//Bimap Size

		//Create 24 bpp Bitmap Handle and Bitmap Bits
		int nW = m_BMI.biWidth;
		int nH = m_BMI.biHeight;
		Create(nW,nH);
		UINT nr = ar.Read(m_pBits,m_nBitsSize);	//Bitmap Bits
	}
}

//ビットマップスクロール
CRect CDIB::Scroll(CWnd *pWnd,int nDx,int nDy)
{
	CClientDC dc(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBmp = dcMem.SelectObject( GetBitmap() );
	CRect rcUpdate;
	dcMem.ScrollDC(-nDx,-nDy,NULL,NULL,NULL,&rcUpdate);
	dcMem.SelectObject( pOldBmp );
	return rcUpdate; //更新しなければならない領域を戻す
}

//ビットマップファイル保存
BOOL CDIB::WriteBitmap(LPCTSTR lpszFilePath)
{
	BITMAPFILEHEADER bf;
	ZeroMemory(&bf,sizeof(BITMAPFILEHEADER));
	bf.bfType = 0x4d42;
	bf.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + GetSize();
	bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	CFile file;
	if( file.Open(lpszFilePath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) ){
		file.Write(&bf,sizeof(BITMAPFILEHEADER));		// ビットマップファイルヘッダーを出力する
		file.Write(&m_BMI,sizeof(BITMAPINFOHEADER));	// ビットマップインフォヘッダーを出力する
		file.Write(GetBits(),GetSize()); //データ出力
		return TRUE;
	}
	return FALSE;
}


static int GetEncoderClsid(const WCHAR* wpszFormat, CLSID &clsid)
{
	UINT  num = 0;   // image encoder  種類数
	UINT  size = 0;  // image encoder　サイズ
	GetImageEncodersSize(&num, &size);
	if(size == 0){
		return -1;
	}
	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL){
		return -1;
	}
	GetImageEncoders(num, size, pImageCodecInfo);

	int nRet = -1;
	for(UINT i = 0; i < num; i++){
		if( wcscmp(pImageCodecInfo[i].MimeType, wpszFormat) == 0 ){//見つかった
			clsid = pImageCodecInfo[i].Clsid;
			nRet = i; //戻り値セット
			break;
		}    
	}

	free(pImageCodecInfo);
	return nRet;
}

//指定フォーマットでSave
BOOL CDIB::Save(CString strPathName,const WCHAR* wpszFormat)
{
	Bitmap bitmap( GetHandle(),NULL );	//Bitmap class 作成
	CLSID clsid;
	if( ::GetEncoderClsid(wpszFormat, clsid) >= 0){
		BSTR bstr = strPathName.AllocSysString();
		bitmap.Save(bstr, &clsid, NULL);
		::SysFreeString(bstr);	
		return TRUE;
	}
	return FALSE;
}
BOOL CDIB::WriteImage(LPCTSTR lpszPathName,LPCTSTR lpszExt)
{
	CString strExt = lpszExt;
	if( strExt == "bmp" ){
		//return Save(lpszPathName,L"image/bmp");
		return WriteBitmap(lpszPathName);
	}else if(strExt == "jpg"){
		return Save(lpszPathName,L"image/jpeg");
	}else if(strExt == "gif"){
		return Save(lpszPathName,L"image/gif");
	}else if(strExt == "tif"){
		return Save(lpszPathName,L"image/tiff");
	}else if(strExt == "png"){
		return Save(lpszPathName,L"image/png");
	}
	return FALSE;
}

//And値セット
void CDIB::And(CDIB *pDib)
{
	if( pDib != NULL ){//引数チェック
		int nSize = GetSize();	//データサイズ
		if(nSize == pDib->GetSize()){//サイズチェック
			BYTE *pSrc = pDib->GetBits();
			BYTE *pDst = GetBits(); //先頭アドレス取得			
			for(int n=0;n<nSize;n++,pDst++,pSrc++){
				*pDst = (*pDst & *pSrc); //And値セット
			}
		}
	}
}

//2値化処理
void CDIB::ChangeBinary(BYTE byTh)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pBits;//B
			BYTE byG = *(pBits+1);//G
			BYTE byR = *(pBits+2);//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
			if(by256 >= byTh){
				*pBits++ = MAXBYTE; //B
				*pBits++ = MAXBYTE; //G
				*pBits++ = MAXBYTE; //R
			}else{
				*pBits++ = 0; //B
				*pBits++ = 0; //G
				*pBits++ = 0; //R
			}
		}
		pBits += nPad; //1Line終了
	}	
}

//2値化処理
void CDIB::ChangeBinary(BYTE byThMin,BYTE byThMax)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pBits;//B
			BYTE byG = *(pBits+1);//G
			BYTE byR = *(pBits+2);//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
			if(byThMin <=by256 && by256 <= byThMax){
				*pBits++ = MAXBYTE; //B
				*pBits++ = MAXBYTE; //G
				*pBits++ = MAXBYTE; //R
			}else{
				*pBits++ = 0; //B
				*pBits++ = 0; //G
				*pBits++ = 0; //R
			}
		}
		pBits += nPad; //1Line終了
	}	
}
//コントラスト最大化
void CDIB::ChangeRange(BYTE byMin, BYTE byMax)
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


//最大最小値の取得
BOOL CDIB::GetRange(BYTE& byMin,BYTE& byMax)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	byMin = *pBits; //先頭値セット
	byMax = *pBits; //先頭値セット
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	for(int y=0;y<nH;y++){
		for(int x=0,xmax=nW*3;x<xmax;x++){//B,G,R
			BYTE byVal = *pBits++;
			if( byMax < byVal ){
				byMax = byVal;
			}else if( byMin > byVal ){
				byMin = byVal;
			}
		}
		pBits += nPad; //1Line終了
	}	
	return (byMin < byMax);
}

//ブロックの幅
BOOL CDIB::GetBlockRange(BYTE *pTop,int nBw,int nBh,BYTE& byMin,BYTE& byMax)
{
	byMin = *pTop; //先頭値セット
	byMax = *pTop; //先頭値セット
	int nLine = GetLineSize(); //1ラインサイズ
	for(int y=0;y<nBh;y++){
		BYTE *pBits = pTop + nLine * y;
		for(int x=0;x<nBw;x++){//B,G,R
			BYTE byVal = *pBits++;
			if( byMax < byVal ){
				byMax = byVal;
			}else if( byMin > byVal ){
				byMin = byVal;
			}
		}
	}
	return (byMin < byMax);	
}

//ヒストグラム取得
void CDIB::CollectHistgram(int nHistR[],int nHistG[],int nHistB[])
{
	ZeroMemory(nHistR,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistG,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistB,sizeof(int)*(MAXBYTE+1)); //0 clear
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW*3;

	BYTE *pSrc = GetBits();
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			nHistB[*pSrc++]++; //ヒストグラムカウント
			nHistG[*pSrc++]++; //ヒストグラムカウント
			nHistR[*pSrc++]++; //ヒストグラムカウント
		}
		pSrc += nPad;
	}
}
void CDIB::CollectHistgramGray(int nGray[])
{
	ZeroMemory(nGray,sizeof(int)*(MAXBYTE+1)); //0 clear
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW*3;

	BYTE *pSrc = GetBits();
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pSrc++;
			BYTE byG = *pSrc++;
			BYTE byR = *pSrc++;
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
			nGray[by256]++; //ヒストグラムカウント
		}
		pSrc += nPad;
	}
}
int CDIB::CollectHistgramRect(int nHistR[],int nHistG[],int nHistB[],const CRect &rect)
{
	int nCnt = 0;
	ZeroMemory(nHistR,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistG,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistB,sizeof(int)*(MAXBYTE+1)); //0 clear
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize(); //1 Lineサイズ
	int nXmin = MAX(0,rect.left);	//左
	int nYmin = MAX(0,rect.top );	//上
	int nXmax = MIN(rect.right ,nW-1);//右
	int nYmax = MIN(rect.bottom,nH-1);//下

	BYTE *pTop = GetBits();
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine * (nH-y-1) + nXmin*3;
		for(int x=nXmin;x<=nXmax;x++){
			nHistB[*pSrc++]++; //ヒストグラムカウント
			nHistG[*pSrc++]++; //ヒストグラムカウント
			nHistR[*pSrc++]++; //ヒストグラムカウント
			nCnt++;
		}		
	}
	//for(int i=0;i<256;i++){
	//	TRACE( "nHist[%d]:%5d\n",i,nHist[i]);
	//}
	return nCnt;
}
void CDIB::CollectHistgramRgn(int nHistR[],int nHistG[],int nHistB[],const CRgn &rgn)
{
	ZeroMemory(nHistR,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistG,sizeof(int)*(MAXBYTE+1)); //0 clear
	ZeroMemory(nHistB,sizeof(int)*(MAXBYTE+1)); //0 clear
	CRect rect;
	rgn.GetRgnBox(&rect);
	if(!rect.IsRectEmpty()){ //領域取得
		int nW = GetWidth();
		int nH = GetHeight();
		int nLine = GetLineSize(); //1 Lineサイズ
		int nXmin = MAX(0,rect.left);	//左
		int nYmin = MAX(0,rect.top );	//上
		int nXmax = MIN(rect.right ,nW-1);//右
		int nYmax = MIN(rect.bottom,nH-1);//下

		BYTE *pTop = GetBits();
		for(int y=nYmin;y<=nYmax;y++){
			BYTE *pSrc = pTop + nLine * (nH-y-1) + nXmin*3;
			for(int x=nXmin;x<=nXmax;x++){
				if( rgn.PtInRegion(x,y) ){
					nHistB[*pSrc++]++; //ヒストグラムカウント
					nHistG[*pSrc++]++; //ヒストグラムカウント
					nHistR[*pSrc++]++; //ヒストグラムカウント
				}
			}		
		}
		//for(int i=0;i<256;i++){
		//	TRACE( "nHist[%d]:%5d\n",i,nHist[i]);
		//}
	}
}

//色変換
void CDIB::ChangeColor(int nType)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	switch(nType){
		case TYPE_Y://輝度
		case TYPE_GRAY://モノクロ
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
					*pBits++ = by256; //B
					*pBits++ = by256; //G
					*pBits++ = by256; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_RGRAY: //R値のみのGray表示
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					//BYTE byB = *pBits;//B
					//BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					*pBits++ = byR; //B
					*pBits++ = byR; //G
					*pBits++ = byR; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_GGRAY: //G値のみのGray表示
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					//BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					//BYTE byR = *(pBits+2);//R
					*pBits++ = byG; //B
					*pBits++ = byG; //G
					*pBits++ = byG; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_BGRAY: //B値のみのGray表示
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					//BYTE byG = *(pBits+1);//G
					//BYTE byR = *(pBits+2);//R
					*pBits++ = byB; //B
					*pBits++ = byB; //G
					*pBits++ = byB; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_R:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					*pBits++ = 0;	//B
					*pBits++ = 0;	//G
					pBits++;		//R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_G:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					*pBits++ = 0;	//B
					 pBits++;		//G
					*pBits++ = 0;	//R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_B:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					 pBits++;		//B
					*pBits++ = 0;	//G
					*pBits++ = 0;	//R
				}
				pBits += nPad; //1Line終了
			}
			break;
		
		case TYPE_H://色相
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					double dbY = (0.299*byR + 0.587*byG + 0.114*byB);
					double dbC1 = byR - dbY;
					double dbC2 = byB - dbY;
					double dbH = atan2(dbC1,dbC2) + PAI; //0..2Π
					BYTE byH = (BYTE)( (dbH*MAXBYTE) / (2*PAI) );
					*pBits++ = byH; //B
					*pBits++ = byH; //G
					*pBits++ = byH; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_S://彩度
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					double dbY = (0.299*byR + 0.587*byG + 0.114*byB);
					double dbC1 = byR - dbY;
					double dbC2 = byB - dbY;
					BYTE byS = (BYTE)sqrt(dbC1*dbC1+dbC2*dbC2);
					*pBits++ = byS; //B
					*pBits++ = byS; //G
					*pBits++ = byS; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_YHS:	//YHS
			for (int y = 0; y<nH; y++){
				for (int x = 0; x<nW; x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits + 1);//G
					BYTE byR = *(pBits + 2);//R
					double dbY = (0.299*byR + 0.587*byG + 0.114*byB);
					double dbC1 = byR - dbY;
					double dbC2 = byB - dbY;
					double dbH = atan2(dbC1, dbC2) + PAI; //0..2Π
					BYTE byH = (BYTE)((dbH*MAXBYTE) / (2 * PAI));
					BYTE byS = (BYTE)sqrt(dbC1*dbC1 + dbC2*dbC2);
					*pBits++ = (BYTE)dbY; //Y
					*pBits++ = byH; //H
					*pBits++ = byS; //S
				}
				pBits += nPad; //1Line終了
			}
			break;
		case TYPE_ACCENT_R:	//R強調
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;		//B
					BYTE byG = *(pBits+1);	//G
					BYTE byR = *(pBits+2);	//R
					BYTE byVal = mid(0,2*byR-byB-byG,MAXBYTE); //2R-B-G
					*pBits++ = byVal; //B
					*pBits++ = byVal; //G
					*pBits++ = byVal; //R
				}
				pBits += nPad; //1Line終了
			}
			break;
	}
}

//指定色との距離画像に変換
void CDIB::ColorLength(BYTE byR,BYTE byG,BYTE byB)
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ

	double dbDeltaR,dbDeltaG,dbDeltaB; //色ごとの距離差分
	double dbLen;//距離
	BYTE byLen; //最終データ
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			dbDeltaB = byB-*pBits;	  //B
			dbDeltaG = byG-*(pBits+1); //G
			dbDeltaR = byR-*(pBits+2); //R
			dbLen = sqrt(dbDeltaB*dbDeltaB+dbDeltaG*dbDeltaG+dbDeltaR*dbDeltaR);
			byLen = (BYTE)MIN(dbLen,MAXBYTE);
			*pBits++ = byLen;
			*pBits++ = byLen;
			*pBits++ = byLen;
		}
		pBits += nPad; //1Line終了
	}
}

//各色反転
void CDIB::Invert()
{
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nSize = GetSize();
	for(int i=0;i<nSize;i++){
		*pBits++ = MAXBYTE - *pBits; //反転
	}	
}

//画像比較
BOOL CDIB::IsEqual(CDIB &dib)
{
	DWORD dw1 = 0;
	DWORD dw2 = 0;
	int nSize = GetSize();
	if(GetWidth() == dib.GetWidth() && GetHeight() == dib.GetHeight() && nSize == dib.GetSize()){ //Bitmap Size Check
		BYTE *pBits1 = GetBits(); //先頭アドレス取得
		BYTE *pBits2 = dib.GetBits(); //先頭アドレス取得
		for(int i=0;i<nSize;i+=4){ //4Byteごと比較する
			dw1 = *pBits1++;
			dw1 = dw1 << 4;
			dw1 |= *pBits1++;
			dw1 = dw1 << 4;
			dw1 |= *pBits1++;
			dw1 = dw1 << 4;
			dw1 |= *pBits1++;
			dw2 = *pBits2++;
			dw2 = dw2 << 4;
			dw2 |= *pBits2++;
			dw2 = dw2 << 4;
			dw2 |= *pBits2++;
			dw2 = dw2 << 4;
			dw2 |= *pBits2++;
			if( dw1 != dw2 ){ //違いが見つかった
				return FALSE;
			}
		}
		//データは同じでした
		return TRUE;
	}
	return FALSE;		
}

//メディアンフィルタ
void CDIB::MedianFilter()
{
	if( m_data.Alloc(GetSize()) ){ //必要なデータサイズの確保
		BYTE *pDstTop = m_data.m_pBuf; //書き込み先アドレス
		BYTE *pSrcTop = GetBits(); //先頭アドレス
		int nLineSize = GetLineSize();
		int nW = GetWidth();
		int nH = GetHeight();
		//Src -> Dst
		BYTE *pSrc,*pDst;
		for(int y=1,ymax=nH-1;y<ymax;y++){
			pSrc = pSrcTop + y*nLineSize + 3;
			pDst = pDstTop + y*nLineSize + 3;
			for(int x=1,xmax=nW-1;x<xmax;x++){
				for(int i=0;i<3;i++){ //BGR
					*pDst++ = Median(pSrc++,nLineSize);//B
				}
			}
		}
		//Dst -> Src
		for(int y=1,ymax=nH-1;y<ymax;y++){
			pSrc = pSrcTop + y*nLineSize + 3;
			pDst = pDstTop + y*nLineSize + 3;		
			memcpy(pSrc,pDst,nLineSize-6);
		}
	}
}
void CDIB::MedianFilter(CRect &rect)
{
	if( m_data.Alloc(GetSize()) ){ //必要なデータサイズの確保
		BYTE *pDstTop = m_data.m_pBuf; //書き込み先アドレス
		BYTE *pSrcTop = GetBits(); //先頭アドレス
		int nLineSize = GetLineSize();
		int nW = GetWidth();
		int nH = GetHeight();
		rect.left = MAX(1,rect.left);
		rect.right = MIN(rect.right,nW-1);
		rect.top  = MAX(1,rect.top);
		rect.bottom = MIN(rect.bottom,nH-1);
		//Src -> Dst
		BYTE *pSrc,*pDst;
		for(int y=rect.top,ymax=rect.bottom;y<ymax;y++){
			pSrc = pSrcTop + y*nLineSize + 3*rect.left;
			pDst = pDstTop + y*nLineSize + 3*rect.left;
			for(int x=rect.left,xmax=rect.right;x<xmax;x++){
				for(int i=0;i<3;i++){ //BGR
					*pDst++ = Median(pSrc++,nLineSize);//B
				}
			}
		}
		//Dst -> Src
		memcpy(pSrcTop,pDstTop,GetSize());
	}
}

//オートフォーカス用計算値取得
double CDIB::GetFocusValue(const CRect &rect)
{
	double dbRet = 0;
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize();

	int nXmin = MAX(1,rect.left);
	int nXmax = MIN(rect.right,nW-2);
	int nYmin = MAX(1,rect.top);
	int nYmax = MIN(rect.bottom,nH-2);

	//オペレーター
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	int d[9]; //計算結果
	double dbDelta;
	//８方向を検査してもっとも小さな値に変換します
	BYTE *pTop = GetBits();   //先頭アドレス取得		
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine*(nH-y-1) + nXmin*3; 
		for(int x=nXmin;x<=nXmax;x++){
			for(int i=0;i<3;i++){//RGB
				dbDelta = 0;//clear
				d[0] = *(pSrc+nLine-3);	//左上
				d[1] = *(pSrc+nLine);	//上	
				d[2] = *(pSrc+nLine+3);	//右上	
				d[3] = *(pSrc-3);		//左
				d[4] = *pSrc;			//
				d[5] = *(pSrc+3);		//右
				d[6] = *(pSrc-nLine-3);	//左下
				d[7] = *(pSrc-nLine);	//下
				d[8] = *(pSrc-nLine+3);	//右下
				for(int i=0;i<9;i++){
					dbDelta += d[i]*c[i];
				}
				dbRet += abs(dbDelta);
				pSrc++;
			}
		}
	}
	return dbRet;
}

//明度の差分取得
int CDIB::DeltaY(const CRect &rect) 
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize();

	int nXmin = MAX(1,rect.left);
	int nXmax = MIN(rect.right,nW-2);
	int nYmin = MAX(1,rect.top);
	int nYmax = MIN(rect.bottom,nH-2);

	BYTE *pTop = GetBits();   //先頭アドレス取得		
	int nMax = *pTop; //明るさ最大値
	int nMin = *pTop; //明るさ最小値
	//８方向を検査してもっとも小さな値に変換します
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine*(nH-y-1) + nXmin*3; 
		for(int x=nXmin;x<=nXmax;x++){
			for(int i=0;i<3;i++){ //RGB
				if( *pSrc < nMin ){ //最小値探索
					nMin = *pSrc;
				}else if(nMax < *pSrc){//最大値探索
					nMax = *pSrc;
				}
				pSrc++;
			}
		}
	}
	return (nMax-nMin);	
}

//メディアン値取得
BYTE CDIB::Median(BYTE *pByte,int nLineSize)
{
	BYTE byVal[9];
	byVal[0] = *(pByte-nLineSize-3);
	byVal[1] = *(pByte-nLineSize);
	byVal[2] = *(pByte-nLineSize+3);
	byVal[3] = *(pByte-3);
	byVal[4] = *(pByte);
	byVal[5] = *(pByte+3);
	byVal[6] = *(pByte+nLineSize-3);
	byVal[7] = *(pByte+nLineSize);
	byVal[8] = *(pByte+nLineSize+3);
	//バブルソート
	for(int j=0;j<8;j++){
		for(int i=0;i<8;i++){
			if(byVal[i+1] < byVal[i]){
				BYTE byTmp = byVal[i+1];
				byVal[i+1] = byVal[i];
				byVal[i] = byTmp;
			}
		}
	}
	return byVal[4]; //メディアン値取得
}

//指定色の画素数をカウント
int CDIB::CountColor(BYTE byR,BYTE byG,BYTE byB)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //パディングサイズ
	BYTE *pBits = GetBits(); //先頭アドレス取得
	int nRet = 0;
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE bySrcB = *pBits++;//B
			BYTE bySrcG = *pBits++;//G
			BYTE bySrcR = *pBits++;//R
			if(bySrcB == byB && bySrcG == byG && bySrcR == byR){
				nRet++;
			}
		}
		pBits += nPad; //1Line終了
	}
	return nRet;
}

//簡易R判定
BOOL CDIB::IsExistR(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//Bは存在しますか？
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byR >(byG + byB)*dbAmp){ //簡易R判定
				return TRUE;
			}
		}
	}
	return FALSE;
}
//簡易G判定
BOOL CDIB::IsExistG(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//Gは存在しますか？
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byG >(byB + byR)*dbAmp){ //簡易G判定
				return TRUE;
			}
		}
	}
	return FALSE;
}
//簡易B判定
BOOL CDIB::IsExistB(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//Rは存在しますか？
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byB >(byR + byG)*dbAmp){ //簡易B判定
				return TRUE;
			}
		}
	}
	return FALSE;
}
//しきい値判定
BOOL CDIB::IsExistTh(int nTh,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = GetWidth();
	int nH = GetHeight();
	nXmin = max(0,nXmin);
	nYmin = max(0, nYmin);
	nXmax = min(nXmax,nW-1);
	nYmax = min(nYmax, nH - 1);
	//Rは存在しますか？
	for (int y = nYmin; y <= nYmax; y++){
		BYTE *pBits = GetPBitsB(nXmin, y);
		for (int x = nXmin; x <= nXmax; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byB >= nTh && byG >= nTh && byR >= nTh){ //しきい値判定
				return TRUE;
			}
		}
	}
	return FALSE;
}

//フォーマット変更
BOOL CDIB::ToMat(cv::Mat &matImg)
{
	int nW = GetWidth();
	int nH = GetHeight();
	if (nW > 0 && nH > 0) {
		if (matImg.cols != nW || matImg.rows != nH) {
			matImg.create(nH, nW, matImg.type());//8bit GRAYSCALE
		}
		if (matImg.type() == CV_8UC1) {
			BYTE *pDst = (BYTE *)matImg.data;
			for (int y = 0; y < nH; y++) {
				BYTE *pSrc = GetPBitsB(0, y);
				for (int x = 0; x < nW; x++) {
					BYTE byB = *pSrc++;//B
					BYTE byG = *pSrc++;//G
					BYTE byR = *pSrc++;//R
					BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色
					*pDst++ = by256;
				}
			}
		}
		else if (matImg.type() == CV_8UC3) {
			int nLine = nW * 3;
			BYTE *pDst = (BYTE *)matImg.data;
			for (int y = 0; y < nH; y++) {
				BYTE *pSrc = GetPBitsB(0, y);
				memcpy(pDst, pSrc, nLine);
				pDst += nLine;
			}
		}
		return TRUE;
	}
	return FALSE;
}

//Mat -> DIB
BOOL CDIB::FromMat(cv::Mat &matImg)
{
	int nW = GetWidth();
	int nH = GetHeight();
	if (matImg.cols != nW || matImg.rows != nH) {
		Create(matImg.cols, matImg.rows);
		nW = matImg.cols;
		nH = matImg.rows;
	}
	if (matImg.type() == CV_8UC3) {
		BYTE *pSrc = (BYTE *)matImg.data;
		for (int y = 0; y < nH; y++) {
			BYTE *pDst = this->GetPBitsB(0, y);
			memcpy(pDst, pSrc, nW * 3);
			pSrc += nW * 3;
		}
		return TRUE;
	}
	else if( matImg.type() == (CV_8UC1)) { //グレースケール
		BYTE *pSrc = (BYTE *)matImg.data;
		for (int y = 0; y < nH; y++) {
			BYTE *pDst = this->GetPBitsB(0, y);
			for (int x = 0; x < nW; x++) {
				*pDst++ = *pSrc; //B
				*pDst++ = *pSrc; //G
				*pDst++ = *pSrc++; //R
			}
		}
		return TRUE;
	}
	return FALSE;
}
