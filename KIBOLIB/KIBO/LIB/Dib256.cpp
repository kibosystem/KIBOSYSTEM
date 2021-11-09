//
// Dib256.cpp --- Direct Draw CDib class
//
#include "../stdafx.h"
#include "dib256.h"

//
// Init --- �����o�[�̏�����
//
void Dib256::Init()
{
	ZeroMemory(&m_bmFileHeader,sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_bmInfoHeader,sizeof(BITMAPINFOHEADER));
	ZeroMemory(m_aryRGB,sizeof(m_aryRGB)); //256�F�p���b�g	
}
//
// �R���X�g���N�^
//
Dib256::Dib256()
{
	Init();
}
//
// �����ƃt�@�C���̃��[�h
//
Dib256::Dib256( LPCWSTR pszFilename )
{
	Init();
	LoadFile(pszFilename);
}
//
// �f�X�g���N�^(�������̉���j
//
Dib256::~Dib256()
{	
}

//
// ColorsUsed --- DIB�Ŏg���Ă���F�̐���Ԃ�
//
inline DWORD Dib256::ColorsUsed()
{
	//
	// �F���̏�񂪂���΁A������g��
	//
	if( m_bmInfoHeader.biClrUsed != 0 )
		return m_bmInfoHeader.biClrUsed;
	//
	// �����łȂ���΁A�s�N�Z������̃r�b�g�����猈�߂�
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
// �r�b�g�}�b�v�̕���Ԃ�(�s�N�Z���P��)
//
int Dib256::GetWidth()
{
	return m_bmInfoHeader.biWidth;
}
//
// �r�b�g�}�b�v�̕���Ԃ�(�s�N�Z���P��)
//
int	Dib256::GetHeight()
{
	return m_bmInfoHeader.biHeight;
}
//
// RowWidth --- �r�b�g�}�b�v��1�s(������)�̃o�C�g����Ԃ�
//
// ����:
//   ���4�̔{���ł��B
//
int Dib256::GetLineSize()
{
	return ((GetWidth() + 3) & ~3);
} 

//
// Bpp --- �s�N�Z������̃r�b�g����Ԃ��܂�
//
int	Dib256::Bpp()
{
	return m_bmInfoHeader.biBitCount;
}
//
// GetPaletteEntries --- �w�肳�ꂽ�͈͂̃p���b�g�G���g����Ԃ��܂�
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
// LoadFile --- DIB�t�@�C���̃��[�h
//
BOOL Dib256::LoadFile( LPCWSTR pszFilename )
{
	CFile file;
	file.Open((LPCTSTR)pszFilename,CFile::modeRead | CFile::typeBinary);
	//
	// �t�@�C���w�b�_�[��ǂݎ��܂��B
	//	

	if( file.Read(&m_bmFileHeader,sizeof(m_bmFileHeader)) ){
		DWORD colorTableSize;
		//
		// �t�@�C���̎�ނ��`�F�b�N���āA���ꂪ�r�b�g�}�b�v�̏ꍇ��
		// ����ǂݎ��܂��B
		//
		if( m_bmFileHeader.bfType != 0x4d42 ||
			file.Read(&m_bmInfoHeader, sizeof(BITMAPINFOHEADER)) <= 0){
			return FALSE;
		}
		//
		// �w�b�_�[���`�F�b�N���āA�r�b�g�}�b�v�Ƃ���
		// �L�����ǂ������m�F���܂��B
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
		// �J���[�e�[�u����ǂݎ��܂��B
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
	// �r�b�g�}�b�v�r�b�g��ǂݎ��܂��B
	//
	int nSize = m_bmInfoHeader.biSizeImage;
	if( nSize == 0 ){
		nSize = GetLineSize() * m_bmInfoHeader.biHeight;
		m_bmInfoHeader.biSizeImage = nSize; //�摜�f�[�^�T�C�Y��K���w�b�_�ɋL�^����
	}
	if( m_buf.Alloc(nSize) ){
		return (file.Read(GetBits(), m_bmInfoHeader.biSizeImage) > 0 );	
	}	
	return FALSE;
}
//
// LoadFile --- DIB�t�@�C���̃��[�h
//
BOOL Dib256::SaveFile( LPCWSTR pszFilename )
{
	BYTE *pBits = GetBits();
	if( pBits != NULL ){
		CFile file;
		if (file.Open((LPCTSTR)pszFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
			file.Write(&m_bmFileHeader, sizeof(BITMAPFILEHEADER)); // �r�b�g�}�b�v�t�@�C���w�b�_�[���o�͂���
			file.Write(&m_bmInfoHeader, sizeof(BITMAPINFOHEADER));	// �r�b�g�}�b�v�C���t�H�w�b�_�[���o�͂���
			for (int i = 0; i < 256; i++){// �r�b�g�}�b�v�J���[�p���b�g���o�͂���
				file.Write(&m_aryRGB[i], sizeof(RGBQUAD));
			}
			// �r�b�g�}�b�v�摜���o�͂���
			int nW = GetLineSize();
			int nH = GetHeight();
			file.Write(GetBits(), nW*nH);
			return TRUE;
		}
	}
	return FALSE;
}

//256�F�r�b�g�}�b�v�̍쐬
BOOL Dib256::Create(int nW,int nH)
{
	if( GetWidth() != nW || GetHeight() != nH ){
		int nLineSize  = (nW + 3) & ~3;
		int nImageSize = nLineSize * nH;
		 // �r�b�g�}�b�v�t�@�C���w�b�_�[���쐬����
		ZeroMemory(&m_bmFileHeader,sizeof(BITMAPFILEHEADER));
		m_bmFileHeader.bfType	 = 0x4d42;
		m_bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;	//offset
		m_bmFileHeader.bfSize	 = m_bmFileHeader.bfOffBits + nImageSize; //File Size
		// �r�b�g�}�b�v�C���t�H�w�b�_�[���쐬����
		ZeroMemory(&m_bmInfoHeader,sizeof(BITMAPINFOHEADER));
		m_bmInfoHeader.biSize		= sizeof(BITMAPINFOHEADER);
		m_bmInfoHeader.biWidth		= nW;
		m_bmInfoHeader.biHeight		= nH;
		m_bmInfoHeader.biPlanes		= 1;
		m_bmInfoHeader.biBitCount	= 8;
		m_bmInfoHeader.biCompression= BI_RGB;
		m_bmInfoHeader.biSizeImage	= nImageSize;
		m_bmInfoHeader.biClrUsed	= 256;
		// �r�b�g�}�b�v�J���[�p���b�g���쐬����
		for(int i=0;i<256;i++){
			m_aryRGB[i].rgbBlue = m_aryRGB[i].rgbGreen = m_aryRGB[i].rgbRed = i;
		}
		// �r�b�g�}�b�v�摜���쐬����
		if( !m_buf.Alloc(nImageSize) ){
			return FALSE;
		}		
	}
	return TRUE;
}

//�r�b�g�}�b�v�R�s�[�@�t���J���[->256�F
BOOL Dib256::Copy(CDIB &dibSrc)
{
	BYTE *pSrc = dibSrc.GetBits(); //�R�s�[���擾
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();
	if( GetWidth() != nW || GetHeight() != nH ){
		if( !Create(nW,nH) ){ //�T�C�Y�ύX
			return FALSE;
		}
	}
	BYTE *pDst = GetBits();	//�R�s�[��擾
	int nPadSrc = dibSrc.GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	int nPadDst = GetLineSize() - nW;	
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			BYTE byB = *pSrc++;//B
			BYTE byG = *pSrc++;//G
			BYTE byR = *pSrc++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
			*pDst++ = by256;
		}
		pSrc += nPadSrc; //1Line�I��
		pDst += nPadDst;
	}	
	return TRUE;
}
//�r�b�g�}�b�v�R�s�[�@256�F -> 256�F
BOOL Dib256::Copy(Dib256 &dibSrc)
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();
	if( GetWidth() != nW || GetHeight() != nH ){
		if( !Create(nW,nH) ){ //�T�C�Y�ύX
			return FALSE;
		}
	}
	BYTE *pSrc = dibSrc.GetBits(); //�R�s�[���擾
	BYTE *pDst = GetBits();	//�R�s�[��擾
	memcpy(pDst,pSrc,m_bmInfoHeader.biSizeImage); //�f�[�^�R�s�[
	return TRUE;	
}

//�r�b�g�}�b�v�R�s�[�@256�F->�t���J���[
BOOL Dib256::Paste(CDIB &dibDst)
{
	int nW = GetWidth();
	int nH = GetHeight();
	if( dibDst.GetWidth() != nW || dibDst.GetHeight() != nH ){
		if( !dibDst.Create(nW,nH) ){ //�T�C�Y�ύX
			return FALSE;
		}
	}
	BYTE *pSrc = GetBits(); //�R�s�[���擾
	int nPadSrc = GetLineSize() - nW;
	BYTE *pDst = dibDst.GetBits(); //�R�s�[��擾
	int nPadDst = dibDst.GetLineSize() - nW * 3; //�p�f�B���O�T�C�Y
	
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pSrc++){
			*pDst++ = *pSrc;//B
			*pDst++ = *pSrc;//G
			*pDst++ = *pSrc;//R
		}
		pSrc += nPadSrc; //1Line�I��
		pDst += nPadDst;
	}	
	return TRUE;
}

//�r�b�g�}�b�v�؂���@�t���J���[->256�F
BOOL Dib256::Cut(CDIB &dibSrc,CRect &rect)
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
			for(int x=0;x<nW;x++){
				BYTE byB = *pSrc++;//B
				BYTE byG = *pSrc++;//G
				BYTE byR = *pSrc++;//R
				BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //�t���J���[ -> 256�F
				*pDst++ = by256;
			}
		}	
		return TRUE;		
	}
	return FALSE;
}
//�r�b�g�}�b�v�؂���@�t���J���[->256�F
BOOL Dib256::Cut(CDIB &dibSrc,CRect &rect,int nCx,int nCy)
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
	if( nCx <= 0 || nCy <= 0 ){
		nCx = rect.Width();
		nCy = rect.Height();
	}
	CDIB dib;
	if( dib.Create(nCx,nCy) ){ //�P�������̃T�C�Y
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL); //Display�݊�������DC
		CBitmap *pSrcOld = dcSrc.SelectObject( dibSrc.GetBitmap() );

		CDC dcDst;
		dcDst.CreateCompatibleDC(NULL); //Display�݊�������DC
		CBitmap *pDstOld = dcDst.SelectObject( dib.GetBitmap() );

		dcDst.SetStretchBltMode(STRETCH_DELETESCANS);//���ω��͂��Ȃ�
		dcDst.StretchBlt(0,0,nCx,nCy,&dcSrc,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);

		//��n��
		dcSrc.SelectObject( pSrcOld );
		dcDst.SelectObject( pDstOld );

		if( Create(nCx,nCy) ){
			this->Copy( dib ); //�t���J��- -> 256�F
			return TRUE;
		}
	}
	return FALSE;
}

//�R���g���X�g�ő剻
void Dib256::MinMax(BYTE byMin,BYTE byMax)
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

//�R���g���X�g�ő剻
void Dib256::MinMax()
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nLine = GetLineSize(); //���C���T�C�Y
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

//�P�x���]
void Dib256::Invert()
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nSize = GetSize();
	for(int i=0;i<nSize;i++){
		*pBits++ = MAXBYTE - *pBits; //���]
	}	
}

//�ߎ��l�擾
double Dib256::GetAprxVal(double dbX,double dbY)
{
	int nX = (int)dbX;
	int nY = (int)dbY;
	if( nX < GetWidth()-1 && nY < GetHeight()-1){
		double dbVal0 = GetValue(nX  ,nY);
		double dbVal1 = GetValue(nX+1,nY);
		double dbVal2 = GetValue(nX  ,nY+1);
		double dbVal3 = GetValue(nX+1,nY+1);
		//x�����ߎ�
		double dbVal01 = dbVal0*(nX+1-dbX) + dbVal1*(dbX-nX);
		double dbVal23 = dbVal2*(nX+1-dbX) + dbVal3*(dbX-nX);
		//y�����ߎ�
		return (dbVal01*(nY+1-dbY)+dbVal23*(dbY-nY));
	}
	return GetValue(nX,nY);	
}

//�w��̈���̍��[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-y-1)+nLeft; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData++; //nLeft+1����
		for(int x=nLeft+1;x<=nRight;x++,pData++){
			if( *pData < nTh ){ //�������l���������
				//   ��x-1       ��x
				//  byPrev    *pData
				//    x - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //�������l�Ƃ̍���
				double dbD2 = byPrev - *pData; //�ȑO�̃f�[�^�Ƃ̍���
				double dbLeft = x;
				if( dbD2 > 0 && dbD1 < dbD2 ){
					dbLeft -= dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin && dbLeft < dbMin){//�ŏ��l�@��
					dbMin = dbLeft;
				}else if(isMax && dbMax < dbLeft){//�ő�l�@��
					dbMax = dbLeft;
				}else{
					dbSum += dbLeft;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l�@��
		return dbMin;
	}else if(isMax){//�ő�l�@��
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}	
	return -1;
}

//�w��̈���̍��[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-y-1)+nLeft; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData++; //nLeft+1����
		for(int x=nLeft+1;x<=nRight;x++,pData++){
			if( *pData >= nTh ){ //�������l��������
				//   ��x-1    <  ��x
				//  byPrev    *pData
				//    x - (*pData-nTh)/(*pData-byPrev)
				double dbD1 = *pData - nTh;    //�������l�Ƃ̍���
				double dbD2 = *pData - byPrev; //�ȑO�̃f�[�^�Ƃ̍���
				double dbLeft = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbLeft -= dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin && dbLeft < dbMin){//�ŏ��l�@��
					dbMin = dbLeft;
				}else if(isMax && dbMax < dbLeft){//�ő�l�@��
					dbMax = dbLeft;
				}else{
					dbSum += dbLeft;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l�@��
		return dbMin;
	}else if(isMax){//�ő�l�@��
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}	
	return -1;
}

//�w��̈���̏�[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-nTop-1) + x; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData -= nW; //nTop+1����
		for(int y=nTop+1;y<=nBtm;y++,pData-=nW){
			if( *pData < nTh){ //�������l���������
				//   ��y-1       ��y
				//  byPrev    *pData
				//    y - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //�������l�Ƃ̍���
				double dbD2 = byPrev - *pData; //�ȑO�̃f�[�^�Ƃ̍���
				double dbTop = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbTop -= dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin && dbTop < dbMin){//�ŏ��l�@��
					dbMin = dbTop;
				}else if(isMax && dbMax < dbTop){//�ő�l�@��
					dbMax = dbTop;
				}else{
					dbSum += dbTop;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l�@��
		return dbMin;
	}else if(isMax){//�ő�l�@��
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}	
	return -1;	
}

//�w��̈���̏�[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-nTop-1) + x; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData -= nW; //nTop+1����
		for(int y=nTop+1;y<=nBtm;y++,pData-=nW){
			if( *pData >= nTh ){ //�������l��������
				//   ��y-1       ��y
				//  byPrev    *pData
				//    y - (*pData-nTh)/(*pData-byPrev)
				double dbD1 = *pData - nTh; //�������l�Ƃ̍���
				double dbD2 = *pData - byPrev; //�ȑO�̃f�[�^�Ƃ̍���
				double dbTop = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbTop -= dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin ){
					if( dbTop < dbMin){//�ŏ��l�@
						dbMin = dbTop;
					}
				}else if(isMax ){
					if(dbMax < dbTop){//�ő�l
						dbMax = dbTop;
					}
				}else{
					dbSum += dbTop;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l
		return dbMin;
	}else if(isMax){//�ő�l
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}
	return -1;
}

//�w��̈���̉E�[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-y-1) + nRight; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData--; //nRight-1����
		for(int x=nRight;x>nLeft;x--,pData--){
			if( *pData < nTh ){ //�������l���������
				//   ��x    <    ��x+1
				//*(pData)   byPrev
				//    x +  (nTh-*pData) / (byPrev-*pData)
				double dbD1 = nTh-*pData;	//�������l�Ƃ̍���
				double dbD2 = byPrev-*pData; //�ȑO�̃f�[�^�Ƃ̍���
				double dbRight = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbRight -= dbD1/dbD2; //�E���W�̌v�Z
				}
				if( isMin && dbRight < dbMin){//�ŏ��l�@��
					dbMin = dbRight;
				}else if(isMax && dbMax < dbRight){//�ő�l�@��
					dbMax = dbRight;
				}else{
					dbSum += dbRight;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l�@��
		return dbMin;
	}else if(isMax){//�ő�l�@��
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}
	return -1;
}

//�w��̈���̉E�[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-y-1) + nRight; //Start�ʒu
		BYTE byPrev = *pData; //�ȑO�̒l
		pData--; //nRight-1����
		for(int x=nRight;x>nLeft;x--,pData--){
			if( *pData >= nTh ){ //�������l���������
				//   ��x    >    ��x+1
				//*(pData)    *(pData+1)
				//    x + (*pData - nTh) / (*pData-byPrev)
				double dbD1 = *pData - nTh; //�������l�Ƃ̍���
				double dbD2 = *pData - byPrev; //�ȑO�̃f�[�^�Ƃ̍���
				double dbRight = x;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbRight += dbD1/dbD2; //�E���W�̌v�Z
				}
				if( isMin && dbRight < dbMin){//�ŏ��l
					dbMin = dbRight;
				}else if(isMax && dbMax < dbRight){//�ő�l
					dbMax = dbRight;
				}else{
					dbSum += dbRight;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l
		return dbMin;
	}else if(isMax){//�ő�l
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}
	return -1;
}

//�w��̈���̉��[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-nBtm) + x; //Start�ʒu y==nBtm+1
		BYTE byPrev = *pData; //�ȑO�̒l
		pData += nW; //nTop+1����
		for(int y=nBtm;y>nTop;y--,pData+=nW){
			if( *pData < nTh ){ //�������l���������
				//   ��y       ��y+1
				//  *pData    byPrev
				//    y+1 - (nTh-*pData)/(byPrev-*pData)
				double dbD1 = nTh    - *pData; //�������l�Ƃ̍���
				double dbD2 = byPrev - *pData; //�ȑO�̃f�[�^�Ƃ̍���
				double dbBtm = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbBtm += dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin && dbBtm < dbMin){//�ŏ��l
					dbMin = dbBtm;
				}else if(isMax && dbMax < dbBtm){//�ő�l
					dbMax = dbBtm;
				}else{
					dbSum += dbBtm;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l
		return dbMin;
	}else if(isMax){//�ő�l
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
		return (dbSum / nSum);
	}
	return -1;
}


//�w��̈���̉��[���ʒu�����W
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
		BYTE *pData = pBits + nW*(nH-nBtm) + x; //Start�ʒu y==nBtm+1
		BYTE byPrev = *pData; //�ȑO�̒l
		pData += nW; //nBtm+1����
		for(int y=nBtm;y>nTop;y--,pData+=nW){
			if( *pData >= nTh ){ //�������l��������
				//   ��y       ��y+1
				//  *pData    byPrev
				//    y + (nTh-*pData)/(byPrev-*pData)
				double dbD1 = *pData - nTh; //�������l�Ƃ̍���
				double dbD2 = *pData - byPrev; //�ȑO�̃f�[�^�Ƃ̍���
				double dbBtm = y;
				if( dbD2 > 0 && dbD1 < dbD2){
					dbBtm += dbD1/dbD2; //�����W�̌v�Z
				}
				if( isMin && dbBtm < dbMin){//�ŏ��l
					dbMin = dbBtm;
				}else if(isMax && dbMax < dbBtm){//�ő�l
					dbMax = dbBtm;
				}else{
					dbSum += dbBtm;
					nSum++;
				}
				break;
			}
			byPrev = *pData; //�ȑO�̒l���L��
		}
	}
	if(isMin){//�ŏ��l
		return dbMin;
	}else if(isMax){//�ő�l
		return dbMax;
	}else if(isAvg && nSum > 0){//���ϒl
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
		//�^�C�v�ʃf�[�^���W
		switch(nType){
			case Dib256::BLACK_MIN:		//�ŏ��l�@��
			case Dib256::BLACK_MAX:		//�ő�l�@��
			case Dib256::BLACK_AVG:		//���ϒl�@��
				return CollectLeftEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//�ŏ��l�@��
			case Dib256::WHITE_MAX:		//�ő�l�@��
			case Dib256::WHITE_AVG:		//���ϒl�@��	
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
		//�^�C�v�ʃf�[�^���W
		switch(nType){
			case Dib256::BLACK_MIN:		//�ŏ��l�@��
			case Dib256::BLACK_MAX:		//�ő�l�@��
			case Dib256::BLACK_AVG:		//���ϒl�@��
				return CollectTopEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//�ŏ��l�@��
			case Dib256::WHITE_MAX:		//�ő�l�@��
			case Dib256::WHITE_AVG:		//���ϒl�@��	
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
		//�^�C�v�ʃf�[�^���W
		switch(nType){
			case Dib256::BLACK_MIN:		//�ŏ��l�@��
			case Dib256::BLACK_MAX:		//�ő�l�@��
			case Dib256::BLACK_AVG:		//���ϒl�@��
				return CollectRightEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//�ŏ��l�@��
			case Dib256::WHITE_MAX:		//�ő�l�@��
			case Dib256::WHITE_AVG:		//���ϒl�@��	
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
		//�^�C�v�ʃf�[�^���W
		switch(nType){
			case Dib256::BLACK_MIN:		//�ŏ��l�@��
			case Dib256::BLACK_MAX:		//�ő�l�@��
			case Dib256::BLACK_AVG:		//���ϒl�@��
				return CollectBottomEdgeBlack(nType,nTh,pGray,pSize,pRect);
			case Dib256::WHITE_MIN:		//�ŏ��l�@��
			case Dib256::WHITE_MAX:		//�ő�l�@��
			case Dib256::WHITE_AVG:		//���ϒl�@��	
				return CollectBottomEdgeWhite(nType,nTh,pGray,pSize,pRect);
		}
	}
	return -1;
}

//�w��̈���̍��[��x���W���擾
double Dib256::GetLeftEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetLeftEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//�w��̈���̉E�[��x���W���擾
double Dib256::GetRightEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetRightEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//�w��̈���̏�[��y���W���擾
double Dib256::GetTopEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetTopEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}
//�w��̈���̉��[��y���W���擾
double Dib256::GetBottomEdge(int nType,int nTh,CRect &rect)
{
	SIZE size;
	size.cx = GetWidth();
	size.cy = GetHeight();	
	return ::GetBottomEdge(nType,nTh,m_buf.m_pBuf,&size,&rect);
}

//�������l�ȏ�̉�f�����擾
int Dib256::CountPixels(int nTh)
{
	int nW = GetWidth();
	int nH = GetHeight();
	int nCnt = 0;
	int nPad = GetLineSize() - nW; //�p�f�B���O�T�C�Y
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

//���œh��Ԃ�
void Dib256::PaintBlack()
{
	if( GetBits() != NULL ){
		memset(GetBits(),0,GetSize());
	}
}
//���œh��Ԃ�
void Dib256::PaintWhite()
{
	if( GetBits() != NULL ){
		memset(GetBits(),MAXBYTE,GetSize());
	}
}

//�ő�n�_�擾
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
	int nPad = GetLineSize() - nW; //�p�f�B���O�T�C�Y
	BYTE *pDataT = GetBits();
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = pDataT + nW*(nH-y-1) + nLeft; //Start�ʒu
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

//�q�X�g�O�����擾
void Dib256::CollectHistgram(int nHist[])
{
	ZeroMemory(nHist,sizeof(int)*(MAXBYTE+1)); //0 clear
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW;
	BYTE *pSrc = GetBits();
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++){
			nHist[*pSrc++]++; //�q�X�g�O�����J�E���g
		}
		pSrc += nPad;
	}
	//for(int i=0;i<256;i++){
	//	TRACE( "nHist[%d]:%5d\n",i,nHist[i]);
	//}
}
//2�l������
void Dib256::ChangeBinary(BYTE byTh)
{
	BYTE *pBits = GetBits(); //�擪�A�h���X�擾
	int nW = GetWidth();
	int nH = GetHeight();
	int nPad = GetLineSize() - nW; //�p�f�B���O�T�C�Y
	for(int y=0;y<nH;y++){
		for(int x=0;x<nW;x++,pBits++){
			*pBits = (*pBits >= byTh) ?  MAXBYTE : 0;
		}
		pBits += nPad; //1Line�I��
	}
}

//�������l�v�Z
int Dib256::CalcThreshold()
{
	int nTh = 128;
	//�̈悲�Ƃ�2�l��
	int nBlack = 0; //���̉�f��
	int nHist[256];
	CollectHistgram(nHist);//�q�X�g�O�������W
	//nTh�ȏ�̍ő�v�f�擾		
	int nMax = 0;
	int nImax = 0;
	for(int i=nTh;i<=MAXBYTE;i++){
		if( nMax < nHist[i] ){//���傫���ق���
			nMax = nHist[i];
			nImax = i;
		}
	}
	//nTh��艺�̍ő�v�f
	nMax = 0;
	int nImin = 0;
	for(int i=nTh-1;i>=0;i--){
		if( nMax < nHist[i] ){ //���傫������
			nMax = nHist[i];
			nImin = i;
		}
	}
	return int( (nImin + nImax) / 2.0 + 0.5 ); //�����炵���������l
}

