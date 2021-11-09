#include "../stdafx.h"
#include <math.h>
#include "dib.h"
#include "functions.h"
#include "GlobalMgr.h"
#include "macro.h"
#include "gdiplus.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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

//�t�@�C�����̂̂ݎ擾
CString CDIB::GetFileName()
{
	return ::GetFileName(m_strFilePath);
}
//Image�̉��
void CDIB::DestroyBitmap()
{
	//Bitmap�J��
	if( m_hBitmap != NULL ){
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}
// Load a DIB from Resource �T�C�Y�w��
BOOL CDIB::Load(int nID)
{
	CBitmap bmp;
	bmp.LoadBitmap(nID);
	HBITMAP hBmp = (HBITMAP)bmp.GetSafeHandle();
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle �쐬
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


// Load a DIB from Resource �T�C�Y�w��
BOOL CDIB::Load(int nID,int nCx,int nCy)
{
	CBitmap bmp;
	bmp.LoadBitmap(nID);
	HBITMAP hBmpTmp = (HBITMAP)bmp.GetSafeHandle();
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo); //Bitmap Size �擾
	HBITMAP hBmp = hBmpTmp; //Default 
	if( bmpInfo.bmWidth != nCx || bmpInfo.bmHeight != nCy ){ //Size���Ⴄ�ꍇ�͎w��T�C�Y�ɕϊ�
		hBmp = (HBITMAP)::CopyImage(hBmpTmp,IMAGE_BITMAP,nCx,nCy,LR_COPYDELETEORG); //�T�C�Y�ύX & �R�s�[��̂ݕۑ� 
	}
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle �쐬
	}
	return FALSE;
}
// Load a DIB from File �T�C�Y�w��
BOOL CDIB::Load(CString strFileName,int nCx,int nCy)
{
	HBITMAP hBmpTmp = LoadBitmapHandle(strFileName); //Bitmap�ǂݍ���
	SIZE sizeBmp; //Bitmap Size �擾
	::GetBitmapDimensionEx(hBmpTmp,&sizeBmp);
	HBITMAP hBmp = hBmpTmp; //Default 
	if( sizeBmp.cx != nCx || sizeBmp.cy != nCy ){ //Size���Ⴄ�ꍇ�͎w��T�C�Y�ɕϊ�
		hBmp = (HBITMAP)::CopyImage(hBmpTmp,IMAGE_BITMAP,nCx,nCy,LR_COPYDELETEORG); //�T�C�Y�ύX & �R�s�[��̂ݕۑ� 
	}
	if( hBmp != NULL ){
		return CreateDibSectionHandle(hBmp);		//DibSection Handle �쐬
	}
	return FALSE;
}
//�w��r�b�g�}�b�v�ǂݍ���
HBITMAP CDIB::LoadBitmapHandle(CString &strFileName)
{
	if( strFileName.IsEmpty() ){
		return NULL;
	}
	m_strFilePath = strFileName;
	CString strExt = strFileName.Right(4);//�g���q�擾
	if( strExt == L".bmp" || strExt ==".BMP"){
		//Load Bitmap 
		return (HBITMAP)LoadImage(::AfxGetInstanceHandle(),strFileName,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);	
	}
	return NULL;
}

//DibSection Handle �쐬
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
	CBitmap *pBmpSrcOld = dcSrc.SelectObject(pBmp);//Src Bitmap �I��
	CBitmap *pBmpDstOld = dcDst.SelectObject( GetBitmap() );//Destination

	dcDst.BitBlt(0,0,nW,nH,&dcSrc,0,0,SRCCOPY);
	
	//��n��
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

		HDC hdc = GetDC(NULL); //Screen DC�g�p
		m_hBitmap = CreateDIBSection(hdc,(LPBITMAPINFO)pBI,DIB_RGB_COLORS,(void **)&m_pBits,NULL,0);
		m_nBitsSize  = ((iWidth*3 + 3) & ~3 ) * abs(iHeight); //Get New Size
		ReleaseDC(NULL,hdc);
	}
	PaintBlack();
    return TRUE;
}

//�w����W�̐F���擾
COLORREF CDIB::GetColor(int x,int y)
{
	return RGB(GetBitsR(x,y),GetBitsG(x,y),GetBitsB(x,y));
}
//�w��l��f���擾
int CDIB::GetPixels(int nVal)
{
	int nCnt = 0;
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	for(int y=0;y<nH;y++){
		for(int x=0,xmax=nW*3;x<xmax;x++){
			if( *pBits++ == nVal){
				nCnt++; //�J�E���^�[�C���N�������g
			}
		}
		pBits += nPad; //1Line�I��
	}
	return nCnt;
}
//���C���`��
void CDIB::Line(int nX1,int nY1,int nX2,int nY2,COLORREF color)
{
	CPen pen(PS_SOLID,2,color);
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPenOld = dcMem.SelectObject(&pen);
	CBitmap *pBmpSrcOld = dcMem.SelectObject( GetBitmap() );//Src�쐬
	dcMem.MoveTo(nX1,nY1);
	dcMem.LineTo(nX2,nY2);
	//��n��
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpSrcOld);
}
//xy�����ړ�
void CDIB::Shift(CDIB &dib,double dbDx,double dbDy)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nX = (int)(dbDx + 0.5);
	int nY = (int)(dbDy + 0.5);
	PaintBlack();
	CDC dcDst;
	dcDst.CreateCompatibleDC(NULL);
	CBitmap *pBmpSrcOld = dcDst.SelectObject(GetBitmap());//Src�쐬
	dib.Draw(&dcDst, nX, nY, nW, nH, 0, 0);
	dcDst.SelectObject(pBmpSrcOld);
}
//�r�b�g�}�b�v�`��
void CDIB::Draw(CDC *pDC,int nX1,int nY1,int nW1,int nH1,int nX2,int nY2)
{	
	if( pDC != NULL ){
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pBmpSrcOld = dcSrc.SelectObject( GetBitmap() );//Src�쐬
		pDC->BitBlt(nX1,nY1,nW1,nH1,&dcSrc,nX2,nY2,SRCCOPY);
		//��n��
		dcSrc.SelectObject(pBmpSrcOld);
	}
}
void CDIB::DrawLine(CPoint &ptS,CPoint &ptE,LOGPEN &lp)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBitmap *pBmpSrcOld = dcMem.SelectObject(GetBitmap());//Src�쐬
	dcMem.MoveTo(ptS);
	dcMem.LineTo(ptE);
	//��n��
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpSrcOld);
}
void CDIB::DrawRectangle(CRect &rect,LOGBRUSH &lb,LOGPEN &lp)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);//�u���V�擾
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBrush *pBrushOld = dcMem.SelectObject(pBrush);
	CBitmap *pBmpSrcOld = dcMem.SelectObject(GetBitmap());//Src�쐬
	dcMem.Rectangle(rect);
	//��n��
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBrushOld);
	dcMem.SelectObject(pBmpSrcOld);
}

//Bitmap -> Bitmap��
//�g��k���`��
void CDIB::StretchDraw(CDC *pDC,int x,int y,int nW,int nH)
{
	if( pDC != NULL ){
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pBmpSrcOld = dcSrc.SelectObject( GetBitmap() );//Src�쐬
		pDC->SetStretchBltMode(STRETCH_DELETESCANS);//���ω��͂��Ȃ�
		pDC->StretchBlt(x,y,nW,nH,&dcSrc,0,0,m_BMI.biWidth,m_BMI.biHeight,SRCCOPY);
		//��n��
		dcSrc.SelectObject(pBmpSrcOld);
	}
}
//���œh��Ԃ�
void CDIB::PaintWhite()
{
	if( GetBits() != NULL ){
		memset(GetBits(),255,GetSize());
	}
}

//���œh��Ԃ�
void CDIB::PaintBlack()
{
	if( GetBits() != NULL ){
		memset(GetBits(),0,GetSize());
	}
}

//�w��F�œh��Ԃ�
void CDIB::PaintColor(COLORREF color)
{
	BYTE byR = GetRValue(color);
	BYTE byG = GetGValue(color);
	BYTE byB = GetBValue(color);
	int nW = GetWidth();
	int nH = GetHeight();
	BYTE *pBits = GetBits(); //Buffer�擪	
	int nNextByte = nW % 4 ; //32bit�̔{����
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
	BYTE *pBits = GetBits(); //Buffer�擪
	
	int nNextByte = nW % 4 ; //32bit�̔{����
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


//��`Mask��`�悵�܂��i�`��̂݁j
void CDIB::MaskRect(CRect &rcMask,COLORREF color)
{
	BYTE byR = GetRValue(color);
	BYTE byG = GetGValue(color);
	BYTE byB = GetBValue(color);
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize();
	BYTE *pTop = GetBits(); //Buffer�擪
		
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

//�~�`�̃}�X�N��`�悵�܂�
void CDIB::MaskCircle(CRect &rcMask,COLORREF color)
{
	LOGPEN lp; //�w��F�̃y��
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 1;
	lp.lopnColor = color; //�F�w��
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾
	LOGBRUSH lb; //�u���V
	lb.lbColor = color; //�F�w��
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID; //NULL�u���V
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);

	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pBmpOld = dcMem.SelectObject( GetBitmap() );//�r�b�g�}�b�v�I��
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBrush *pBrOld  = dcMem.SelectObject(pBrush);	

	dcMem.Ellipse(rcMask);
	
	//��n��
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBrOld);
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}

//�̈�̓h��Ԃ�
void CDIB::PaintRgn(CRgn &rgn,COLORREF cr)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	LOGBRUSH lb; //�u���V
	lb.lbColor = cr; //�F�w��
	lb.lbHatch = 0;
	lb.lbStyle = BS_SOLID; //�h��Ԃ��u���V
	CBrush *pBrush = g_GlobalMgr.GetBrush(lb);//�u���V�擾
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pBmpOld = dcMem.SelectObject( GetBitmap() );//�r�b�g�}�b�v�I��
	BOOL isOK = dcMem.FillRgn(&rgn,pBrush);
	//��n��
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}

//Bitmap Data ����
void CDIB::Copy(CDIB& dibDest)
{
	dibDest.Create(GetWidth(),GetHeight());		//���T�C�Ydib�쐬
	memcpy(dibDest.GetBits(),GetBits(),GetSize());	//Buffer Copy		
	dibDest.m_strFilePath = m_strFilePath; //File�� Copy
}

//Bitmap Data ���� Size �w��
void CDIB::CopyDib(CDIB& dibSrc ,int nW,int nH)
{
	Create(nW,nH);	
	HBITMAP hBmp = (HBITMAP)::CopyImage(dibSrc.GetHandle(),IMAGE_BITMAP,nW,nH,0); //�T�C�Y�ύX 
	if( hBmp != NULL ){
		CreateDibSectionHandle(hBmp);		//DibSection Handle �쐬
	}
}

//�w�肳�ꂽ�r�b�g�}�b�v��`�悵�܂��B
void CDIB::SetDib(int nX,int nY,CDIB& dibSrc) 
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();

	int nNewW = max( GetWidth() ,nW ); 
	int nNewH = max( GetHeight(),nH );
	if( GetWidth() < nNewW ||  GetHeight() < nNewH){ //�傫���ύX�̕K�v����
		 Create(nNewW,nNewH);
	}

	//���`�悷�ׂ��͈͂𒲂ׂ�(�X�N���[�����W)
	RECT	rcSrc = { nX, nY, nX+dibSrc.GetWidth(), nY+dibSrc.GetHeight() }, 
			rcDst = { 0, 0, GetWidth(), GetHeight() }, 
			rcDraw;
	IntersectRect(&rcDraw, &rcSrc, &rcDst); //�`��̈�擾

	//���X�L�������C���̃o�C�g�����v�Z
	int nSrcLine = dibSrc.GetLineSize();
	int nDstLine = GetLineSize();
	int nCopyLine = (rcDraw.right-rcDraw.left)*3;
	int nCopyH = (rcDraw.bottom-rcDraw.top);

	//���ǂݍ��݁E�������݊J�n�A�h���X���v�Z
	BYTE *pSrc = (BYTE*)dibSrc.GetBits() + (nH-1)*nSrcLine;	
	BYTE *pDst = GetBits() + (nNewH-1-nY)* nDstLine + nX*3;

	for (int j = 0; j < nCopyH; j++){
		memcpy(pDst,pSrc,nCopyLine);
		// �X�L�������C���̈ړ�
		pSrc -= nSrcLine;
		pDst -= nDstLine;
	}
}

//�X�N���[���摜�쐬
void CDIB::ScreenCapture(const CRect &rect) 
{
	CDC dc;
	dc.CreateDC(L"DISPLAY", NULL, NULL, NULL);
	CWnd *pWnd = CWnd::GetDesktopWindow();
	if( pWnd != NULL ){
		CRect rcDeskTop;
		pWnd->GetClientRect(&rcDeskTop);
		//�`��r�b�g�}�b�v
		if( GetWidth() != rect.Width() && GetHeight() != rect.Height() ){ //�傫���ύX�̕K�v����
			 Create(rect.Width(),rect.Height()); //Bitmap �쐬
		}
		//Screen -> Bitmap
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap *pOld = dcMem.SelectObject( GetBitmap() );//�r�b�g�}�b�v�I��
		dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&dc,rect.left,rect.top,SRCCOPY);//Bitmap�`��
		dcMem.SelectObject(pOld);
	}		
}

//Bitmap Sprite �]��
void CDIB::Sprite(CDIB& dibSrc ,int nX,int nY)
{
	//���`�悷�ׂ��͈͂𒲂ׂ�(�X�N���[�����W)
	RECT	rcSrc = { nX, nY, nX+dibSrc.GetWidth(), nY+dibSrc.GetHeight() }, 
			rcDst = { 0, 0, GetWidth(), GetHeight() }, 
			rcDraw;
	IntersectRect(&rcDraw, &rcSrc, &rcDst); //�`��̈�擾

	//���X�L�������C���̃o�C�g�����v�Z
	int nSrcLine = dibSrc.GetLineSize();
	int nDstLine = GetLineSize();

	//���ǂݍ��݁E�������݊J�n�A�h���X���v�Z
	BYTE *pSrc = (BYTE*)dibSrc.GetBits()
				+ ((nY + dibSrc.GetHeight()) - rcDraw.bottom) * nSrcLine	
				+ (rcDraw.left - nX) * 3;
	BYTE *pDst = GetBits()
				+ (rcDst.bottom - rcDraw.bottom)* nDstLine 
				+ rcDraw.left * 3;

	// �X�v���C�g�������s��
	
	// �����F
	BYTE bySpB = dibSrc.GetBitsB(0,0);
	BYTE bySpG = dibSrc.GetBitsG(0,0);
	BYTE bySpR = dibSrc.GetBitsR(0,0);

	//���������ޔ͈͂��v�Z
	int nWidth  = rcDraw.right  - rcDraw.left;
	int	nHeight = rcDraw.bottom - rcDraw.top;

	// ���̃X�L�������C���̏������݊J�n�ʒu�܂ł̋������v�Z
	int nSrcNextByte  = nSrcLine - nWidth * 3;
	int nDstNextByte = nDstLine - nWidth * 3;

	for (int j = 0; j < nHeight; j++){
		for (int i = 0; i < nWidth; i++){
		// Sprite�����p
			if(bySpB != *pSrc || bySpG != *(pSrc+1) || bySpR != *(pSrc+2) ){
				// �����łȂ��ꍇ�͏�������
				*pDst++ = *pSrc++; // Blue
				*pDst++ = *pSrc++; // Green
				*pDst++ = *pSrc++; // Red
			}else {
				// �����̏ꍇ�̓A�h���X�𑗂邾��
				pSrc += 3;
				pDst += 3;
			}
		}
		// �X�L�������C���̈ړ�
		pSrc += nSrcNextByte;
		pDst += nDstNextByte;
	}
}

//�r�b�g�}�b�v�؂���@�t���J���[->256�F
BOOL CDIB::Cut(CDIB &dibSrc,CRect &rect)
{
	int nSrcW = dibSrc.GetWidth(); //Src�摜��
	int nSrcH = dibSrc.GetHeight();//Src�摜����
	//�T�C�Y����
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
	
	int nW = rect.Width()+1; //�T���摜�̃T�C�Y�擾
	int nH = rect.Height()+1;	
	if( Create(nW,nH) ){
		BYTE *pSrcTop = dibSrc.GetBits(); //�R�s�[���擾
		BYTE *pDstTop = GetBits();		//�R�s�[��擾
		int nLineSrc = dibSrc.GetLineSize();//�R�s�[�����C���T�C�Y
		int nLineDst = GetLineSize();		//�R�s�[�惉�C���T�C�Y
		for(int y=0;y<nH;y++){
			BYTE *pSrc = pSrcTop + (nSrcH-(rect.top+y)-1)*nLineSrc + rect.left * 3;
			BYTE *pDst = pDstTop + (nH-y-1)*nLineDst;
			memcpy(pDst,pSrc,nLineDst); //1Line Copy
		}	
		return TRUE;		
	}
	return FALSE;
}

//�V���A���C�Y
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

//�r�b�g�}�b�v�X�N���[��
CRect CDIB::Scroll(CWnd *pWnd,int nDx,int nDy)
{
	CClientDC dc(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBmp = dcMem.SelectObject( GetBitmap() );
	CRect rcUpdate;
	dcMem.ScrollDC(-nDx,-nDy,NULL,NULL,NULL,&rcUpdate);
	dcMem.SelectObject( pOldBmp );
	return rcUpdate; //�X�V���Ȃ���΂Ȃ�Ȃ��̈��߂�
}

//�r�b�g�}�b�v�t�@�C���ۑ�
BOOL CDIB::WriteBitmap(LPCTSTR lpszFilePath)
{
	BITMAPFILEHEADER bf;
	ZeroMemory(&bf,sizeof(BITMAPFILEHEADER));
	bf.bfType = 0x4d42;
	bf.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + GetSize();
	bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	CFile file;
	if( file.Open(lpszFilePath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) ){
		file.Write(&bf,sizeof(BITMAPFILEHEADER));		// �r�b�g�}�b�v�t�@�C���w�b�_�[���o�͂���
		file.Write(&m_BMI,sizeof(BITMAPINFOHEADER));	// �r�b�g�}�b�v�C���t�H�w�b�_�[���o�͂���
		file.Write(GetBits(),GetSize()); //�f�[�^�o��
		return TRUE;
	}
	return FALSE;
}


static int GetEncoderClsid(const WCHAR* wpszFormat, CLSID &clsid)
{
	UINT  num = 0;   // image encoder  ��ސ�
	UINT  size = 0;  // image encoder�@�T�C�Y
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
		if( wcscmp(pImageCodecInfo[i].MimeType, wpszFormat) == 0 ){//��������
			clsid = pImageCodecInfo[i].Clsid;
			nRet = i; //�߂�l�Z�b�g
			break;
		}    
	}

	free(pImageCodecInfo);
	return nRet;
}

//�w��t�H�[�}�b�g��Save
BOOL CDIB::Save(CString strPathName,const WCHAR* wpszFormat)
{
	Bitmap bitmap( GetHandle(),NULL );	//Bitmap class �쐬
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

//And�l�Z�b�g
void CDIB::And(CDIB *pDib)
{
	if( pDib != NULL ){//�����`�F�b�N
		int nSize = GetSize();	//�f�[�^�T�C�Y
		if(nSize == pDib->GetSize()){//�T�C�Y�`�F�b�N
			BYTE *pSrc = pDib->GetBits();
			BYTE *pDst = GetBits(); //�擪�A�h���X�擾			
			for(int n=0;n<nSize;n++,pDst++,pSrc++){
				*pDst = (*pDst & *pSrc); //And�l�Z�b�g
			}
		}
	}
}

//2�l������
void CDIB::ChangeBinary(BYTE byTh)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pBits;//B
			BYTE byG = *(pBits+1);//G
			BYTE byR = *(pBits+2);//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
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
		pBits += nPad; //1Line�I��
	}	
}

//2�l������
void CDIB::ChangeBinary(BYTE byThMin,BYTE byThMax)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pBits;//B
			BYTE byG = *(pBits+1);//G
			BYTE byR = *(pBits+2);//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
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
		pBits += nPad; //1Line�I��
	}	
}
//�R���g���X�g�ő剻
void CDIB::ChangeRange(BYTE byMin, BYTE byMax)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nH = GetHeight();
	int xmax = GetLineSize(); //���C���T�C�Y
	BYTE byRange = byMax-byMin;
	BYTE byVal;
	for(int y=0;y<nH;y++){
		for(int x=0;x<xmax;x++){
			if(*pBits <= byMin){
				byVal = 0;
			}else if(*pBits >= byMax){
				byVal = MAXBYTE;
			}else{
				byVal = (*pBits - byMin) * MAXBYTE / byRange; //Range���L����
			}
			*pBits++ = byVal; //�l�ύX
		}
	}
}


//�ő�ŏ��l�̎擾
BOOL CDIB::GetRange(BYTE& byMin,BYTE& byMax)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	byMin = *pBits; //�擪�l�Z�b�g
	byMax = *pBits; //�擪�l�Z�b�g
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	for(int y=0;y<nH;y++){
		for(int x=0,xmax=nW*3;x<xmax;x++){//B,G,R
			BYTE byVal = *pBits++;
			if( byMax < byVal ){
				byMax = byVal;
			}else if( byMin > byVal ){
				byMin = byVal;
			}
		}
		pBits += nPad; //1Line�I��
	}	
	return (byMin < byMax);
}

//�u���b�N�̕�
BOOL CDIB::GetBlockRange(BYTE *pTop,int nBw,int nBh,BYTE& byMin,BYTE& byMax)
{
	byMin = *pTop; //�擪�l�Z�b�g
	byMax = *pTop; //�擪�l�Z�b�g
	int nLine = GetLineSize(); //1���C���T�C�Y
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

//�q�X�g�O�����擾
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
			nHistB[*pSrc++]++; //�q�X�g�O�����J�E���g
			nHistG[*pSrc++]++; //�q�X�g�O�����J�E���g
			nHistR[*pSrc++]++; //�q�X�g�O�����J�E���g
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
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
			nGray[by256]++; //�q�X�g�O�����J�E���g
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
	int nLine = GetLineSize(); //1 Line�T�C�Y
	int nXmin = MAX(0,rect.left);	//��
	int nYmin = MAX(0,rect.top );	//��
	int nXmax = MIN(rect.right ,nW-1);//�E
	int nYmax = MIN(rect.bottom,nH-1);//��

	BYTE *pTop = GetBits();
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine * (nH-y-1) + nXmin*3;
		for(int x=nXmin;x<=nXmax;x++){
			nHistB[*pSrc++]++; //�q�X�g�O�����J�E���g
			nHistG[*pSrc++]++; //�q�X�g�O�����J�E���g
			nHistR[*pSrc++]++; //�q�X�g�O�����J�E���g
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
	if(!rect.IsRectEmpty()){ //�̈�擾
		int nW = GetWidth();
		int nH = GetHeight();
		int nLine = GetLineSize(); //1 Line�T�C�Y
		int nXmin = MAX(0,rect.left);	//��
		int nYmin = MAX(0,rect.top );	//��
		int nXmax = MIN(rect.right ,nW-1);//�E
		int nYmax = MIN(rect.bottom,nH-1);//��

		BYTE *pTop = GetBits();
		for(int y=nYmin;y<=nYmax;y++){
			BYTE *pSrc = pTop + nLine * (nH-y-1) + nXmin*3;
			for(int x=nXmin;x<=nXmax;x++){
				if( rgn.PtInRegion(x,y) ){
					nHistB[*pSrc++]++; //�q�X�g�O�����J�E���g
					nHistG[*pSrc++]++; //�q�X�g�O�����J�E���g
					nHistR[*pSrc++]++; //�q�X�g�O�����J�E���g
				}
			}		
		}
		//for(int i=0;i<256;i++){
		//	TRACE( "nHist[%d]:%5d\n",i,nHist[i]);
		//}
	}
}

//�F�ϊ�
void CDIB::ChangeColor(int nType)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	switch(nType){
		case TYPE_Y://�P�x
		case TYPE_GRAY://���m�N��
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
					*pBits++ = by256; //B
					*pBits++ = by256; //G
					*pBits++ = by256; //R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_RGRAY: //R�l�݂̂�Gray�\��
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					//BYTE byB = *pBits;//B
					//BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					*pBits++ = byR; //B
					*pBits++ = byR; //G
					*pBits++ = byR; //R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_GGRAY: //G�l�݂̂�Gray�\��
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					//BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					//BYTE byR = *(pBits+2);//R
					*pBits++ = byG; //B
					*pBits++ = byG; //G
					*pBits++ = byG; //R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_BGRAY: //B�l�݂̂�Gray�\��
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					//BYTE byG = *(pBits+1);//G
					//BYTE byR = *(pBits+2);//R
					*pBits++ = byB; //B
					*pBits++ = byB; //G
					*pBits++ = byB; //R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_R:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					*pBits++ = 0;	//B
					*pBits++ = 0;	//G
					pBits++;		//R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_G:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					*pBits++ = 0;	//B
					 pBits++;		//G
					*pBits++ = 0;	//R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_B:
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					 pBits++;		//B
					*pBits++ = 0;	//G
					*pBits++ = 0;	//R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		
		case TYPE_H://�F��
			for(int y=0;y<nH;y++){
				for(int x=0;x<nW;x++){
					BYTE byB = *pBits;//B
					BYTE byG = *(pBits+1);//G
					BYTE byR = *(pBits+2);//R
					double dbY = (0.299*byR + 0.587*byG + 0.114*byB);
					double dbC1 = byR - dbY;
					double dbC2 = byB - dbY;
					double dbH = atan2(dbC1,dbC2) + PAI; //0..2��
					BYTE byH = (BYTE)( (dbH*MAXBYTE) / (2*PAI) );
					*pBits++ = byH; //B
					*pBits++ = byH; //G
					*pBits++ = byH; //R
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_S://�ʓx
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
				pBits += nPad; //1Line�I��
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
					double dbH = atan2(dbC1, dbC2) + PAI; //0..2��
					BYTE byH = (BYTE)((dbH*MAXBYTE) / (2 * PAI));
					BYTE byS = (BYTE)sqrt(dbC1*dbC1 + dbC2*dbC2);
					*pBits++ = (BYTE)dbY; //Y
					*pBits++ = byH; //H
					*pBits++ = byS; //S
				}
				pBits += nPad; //1Line�I��
			}
			break;
		case TYPE_ACCENT_R:	//R����
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
				pBits += nPad; //1Line�I��
			}
			break;
	}
}

//�w��F�Ƃ̋����摜�ɕϊ�
void CDIB::ColorLength(BYTE byR,BYTE byG,BYTE byB)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y

	double dbDeltaR,dbDeltaG,dbDeltaB; //�F���Ƃ̋�������
	double dbLen;//����
	BYTE byLen; //�ŏI�f�[�^
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
		pBits += nPad; //1Line�I��
	}
}

//�e�F���]
void CDIB::Invert()
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nSize = GetSize();
	for(int i=0;i<nSize;i++){
		*pBits++ = MAXBYTE - *pBits; //���]
	}	
}

//�摜��r
BOOL CDIB::IsEqual(CDIB &dib)
{
	DWORD dw1 = 0;
	DWORD dw2 = 0;
	int nSize = GetSize();
	if(GetWidth() == dib.GetWidth() && GetHeight() == dib.GetHeight() && nSize == dib.GetSize()){ //Bitmap Size Check
		BYTE *pBits1 = GetBits(); //�擪�A�h���X�擾
		BYTE *pBits2 = dib.GetBits(); //�擪�A�h���X�擾
		for(int i=0;i<nSize;i+=4){ //4Byte���Ɣ�r����
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
			if( dw1 != dw2 ){ //�Ⴂ����������
				return FALSE;
			}
		}
		//�f�[�^�͓����ł���
		return TRUE;
	}
	return FALSE;		
}

//���f�B�A���t�B���^
void CDIB::MedianFilter()
{
	if( m_data.Alloc(GetSize()) ){ //�K�v�ȃf�[�^�T�C�Y�̊m��
		BYTE *pDstTop = m_data.m_pBuf; //�������ݐ�A�h���X
		BYTE *pSrcTop = GetBits(); //�擪�A�h���X
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
	if( m_data.Alloc(GetSize()) ){ //�K�v�ȃf�[�^�T�C�Y�̊m��
		BYTE *pDstTop = m_data.m_pBuf; //�������ݐ�A�h���X
		BYTE *pSrcTop = GetBits(); //�擪�A�h���X
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

//�I�[�g�t�H�[�J�X�p�v�Z�l�擾
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

	//�I�y���[�^�[
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	int d[9]; //�v�Z����
	double dbDelta;
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	BYTE *pTop = GetBits();   //�擪�A�h���X�擾		
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine*(nH-y-1) + nXmin*3; 
		for(int x=nXmin;x<=nXmax;x++){
			for(int i=0;i<3;i++){//RGB
				dbDelta = 0;//clear
				d[0] = *(pSrc+nLine-3);	//����
				d[1] = *(pSrc+nLine);	//��	
				d[2] = *(pSrc+nLine+3);	//�E��	
				d[3] = *(pSrc-3);		//��
				d[4] = *pSrc;			//
				d[5] = *(pSrc+3);		//�E
				d[6] = *(pSrc-nLine-3);	//����
				d[7] = *(pSrc-nLine);	//��
				d[8] = *(pSrc-nLine+3);	//�E��
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

//���x�̍����擾
int CDIB::DeltaY(const CRect &rect) 
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nLine = GetLineSize();

	int nXmin = MAX(1,rect.left);
	int nXmax = MIN(rect.right,nW-2);
	int nYmin = MAX(1,rect.top);
	int nYmax = MIN(rect.bottom,nH-2);

	BYTE *pTop = GetBits();   //�擪�A�h���X�擾		
	int nMax = *pTop; //���邳�ő�l
	int nMin = *pTop; //���邳�ŏ��l
	//�W�������������Ă����Ƃ������Ȓl�ɕϊ����܂�
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pSrc = pTop + nLine*(nH-y-1) + nXmin*3; 
		for(int x=nXmin;x<=nXmax;x++){
			for(int i=0;i<3;i++){ //RGB
				if( *pSrc < nMin ){ //�ŏ��l�T��
					nMin = *pSrc;
				}else if(nMax < *pSrc){//�ő�l�T��
					nMax = *pSrc;
				}
				pSrc++;
			}
		}
	}
	return (nMax-nMin);	
}

//���f�B�A���l�擾
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
	//�o�u���\�[�g
	for(int j=0;j<8;j++){
		for(int i=0;i<8;i++){
			if(byVal[i+1] < byVal[i]){
				BYTE byTmp = byVal[i+1];
				byVal[i+1] = byVal[i];
				byVal[i] = byTmp;
			}
		}
	}
	return byVal[4]; //���f�B�A���l�擾
}

//�w��F�̉�f�����J�E���g
int CDIB::CountColor(BYTE byR,BYTE byG,BYTE byB)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
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
		pBits += nPad; //1Line�I��
	}
	return nRet;
}

//�Ȉ�R����
BOOL CDIB::IsExistR(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//B�͑��݂��܂����H
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byR >(byG + byB)*dbAmp){ //�Ȉ�R����
				return TRUE;
			}
		}
	}
	return FALSE;
}
//�Ȉ�G����
BOOL CDIB::IsExistG(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//G�͑��݂��܂����H
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byG >(byB + byR)*dbAmp){ //�Ȉ�G����
				return TRUE;
			}
		}
	}
	return FALSE;
}
//�Ȉ�B����
BOOL CDIB::IsExistB(double dbAmp)
{
	int nW = GetWidth();
	int nH = GetHeight();
	//R�͑��݂��܂����H
	for (int y = 0; y < nH; y++){
		BYTE *pBits = GetPBitsB(0, y);
		for (int x = 0; x < nW; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byB >(byR + byG)*dbAmp){ //�Ȉ�B����
				return TRUE;
			}
		}
	}
	return FALSE;
}
//�������l����
BOOL CDIB::IsExistTh(int nTh,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = GetWidth();
	int nH = GetHeight();
	nXmin = max(0,nXmin);
	nYmin = max(0, nYmin);
	nXmax = min(nXmax,nW-1);
	nYmax = min(nYmax, nH - 1);
	//R�͑��݂��܂����H
	for (int y = nYmin; y <= nYmax; y++){
		BYTE *pBits = GetPBitsB(nXmin, y);
		for (int x = nXmin; x <= nXmax; x++){
			BYTE byB = *pBits++;//B
			BYTE byG = *pBits++;//G
			BYTE byR = *pBits++;//R
			if (byB >= nTh && byG >= nTh && byR >= nTh){ //�������l����
				return TRUE;
			}
		}
	}
	return FALSE;
}

//�t�H�[�}�b�g�ύX
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
					BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
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
	else if( matImg.type() == (CV_8UC1)) { //�O���[�X�P�[��
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
