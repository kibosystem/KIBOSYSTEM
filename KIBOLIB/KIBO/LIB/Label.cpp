#include "../stdafx.h"
#include "../LIB/typedef.h"
#include "label.h"
#include "macro.h"

ArrayCheckImage Label::m_aryImageCheck; //比較用画像

Label::Label()
{
	m_nBufSize = 0;	//バッファサイズ
	m_nW = 0;		//ラベリング幅
	m_nH = 0;		//ラベリング高さ
    m_pSrcBuf = NULL;	//ラベリングソース
    m_pDstBuf = NULL;	//ラベリング結果
	m_pTmpBuf = NULL;	//ラベリングソーステンポラリバッファ
}

Label::~Label(void)
{
	Free();
}
//ラベリングバッファ解放
void Label::Free()
{
	if( m_pSrcBuf != NULL ){
		delete m_pSrcBuf;
		m_pSrcBuf = NULL;
	}
	if( m_pDstBuf != NULL ){
		delete m_pDstBuf;
		m_pDstBuf = NULL;
	}
	if( m_pTmpBuf != NULL ){
		delete m_pTmpBuf;
		m_pTmpBuf = NULL;
	}
}
//ラベリング用のバッファを作成
BOOL Label::CreateBuffer(int nSize)
{
	//バッファの作成
	if( m_nBufSize < nSize ){ //以前作成したバッファよりも大きなサイズが必要な時だけ
		Free();
		m_pSrcBuf = new int[ nSize ];
		if( m_pSrcBuf == NULL){
			return FALSE;
		}
		m_pDstBuf = new BYTE[ nSize ];
		if(m_pDstBuf == NULL){
			return FALSE;
		}
		m_pTmpBuf = new int[ nSize ];
		if(m_pTmpBuf == NULL){
			return FALSE;
		}
		m_nBufSize = nSize; //サイズ保存
	}
	return TRUE;
}
//ラベリング初期化
BOOL Label::Init(int nW,int nH)
{
	m_nW = nW; //ラベリングサイズ登録
	m_nH = nH;
	int nSize = nW*nH; //使用範囲クリア
	if( CreateBuffer(nSize) ){ //必要な時だけ作成
		ZeroMemory(m_pSrcBuf,nSize*sizeof(int));
		ZeroMemory(m_pDstBuf,nSize);
		ZeroMemory(m_pTmpBuf,nSize*sizeof(int));
		return TRUE;
	}
	return FALSE;
}
//マスク領域のソースバッファをクリア
void Label::Mask(BYTE byVal,CRect &rect,BOOL isPaint/*塗りつぶし*/)
{
	if( m_pSrcBuf != NULL ){ //ソースバッファをチェック
		int nW = GetWidth();
		int nH = GetHeight();

		int nLeft  = mid(0,rect.left  ,nW);
		int nRight = mid(0,rect.right ,nW);
		int nBtm   = mid(0,nH-rect.top-1,nH-1);
		int nTop   = mid(0,nH-rect.bottom-1,nH-1);
		int nLine = nRight - nLeft; //1ラインサイズ		
		if( isPaint ){ //塗りつぶしあり
			for(int y=nTop;y<=nBtm;y++){
				int *pData = m_pSrcBuf + (nH-y-1) * nW + nLeft;  //消去アドレス
				memset(pData,byVal,nLine * sizeof(int)); //１Line Clear
			}
		}else{ //塗りつぶし無し
			int *pData = m_pSrcBuf + (nH-nTop-1) * nW + nLeft;  //消去アドレス
			memset(pData,byVal,nLine * sizeof(int)); //１Line Clear
			for(int y=nTop+1;y<nBtm;y++){
				*(m_pSrcBuf + (nH-y-1) * nW + nLeft ) = byVal; //Left  Clear
				*(m_pSrcBuf + (nH-y-1) * nW + nRight) = byVal; //Right Clear
			}
			pData = m_pSrcBuf + (nH-nBtm-1) * nW + nLeft;  //消去アドレス
			memset(pData,byVal,nLine * sizeof(int)); //１Line Clear
		}
	}
}
//ラベリング全領域
int Label::Exec(int nMinSize,int nMaxSize)
{
	ZeroMemory(m_pDstBuf,GetSize());
	int nW = GetWidth();
	int nH = GetHeight();
	CRect rect(0,0,nW,nH);
	return CLabeling::Exec(m_pSrcBuf,nW,nH,nMinSize,nMaxSize,rect);
}

//ラベリング領域指定
int Label::Exec(int nMinSize,int nMaxSize,CRect &rect)
{
	ZeroMemory(m_pDstBuf,GetSize());
	int nW = GetWidth();
	int nH = GetHeight();
	CRect rcMax(0,0,nW,nH);
	CRect rcNorm; //正規化される矩形
	if( rcNorm.IntersectRect(rect,rcMax) ){
		return CLabeling::Exec(m_pSrcBuf,nW,nH,nMinSize,nMaxSize,rcNorm);
	}
	return 0;
}

//ラベリング領域指定
int Label::ElementExec(int nEnum,int nMinSize,int nMaxSize)
{
	Element *pElem = GetElement(nEnum);
	if( pElem != NULL ){
		CRect rect = pElem->GetRect(); //要素全体の領域取得
		int nEmax = Exec(nMinSize,nMaxSize,rect);
		if(nEmax > 0){
			rect.DeflateRect(1,1); //縮める
			//現在の要素をすべて退避
			RIPVector vRip;
			vRip = m_result_region_info;

			//ラベリング要素クリア
			m_result_region_info.clear();
			m_aryRip.Clear();
			m_nEmax = 0;
			
			for(int i=0,imax=(int)vRip.size();i<imax;i++){
				Element *pElem = m_aryRip.GetBack();
				pElem->Copy( vRip[i] ); //Src -> Dst
				CRect rcElem = pElem->GetRect();//要素内要素の領域取得
				if( rect.left < rcElem.left && rcElem.right < rect.right && rect.top < rcElem.top && rcElem.bottom < rect.bottom ){//要素として残す
					m_result_region_info.push_back( pElem );
					m_nEmax++;//ラベリング要素を増やす
				}else{//要素として残さない
					m_aryRip.Back();
				}
			}
			return GetEmax();
		}
	}
	return 0;
}

//並び替えクラス
class AscendMinX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinX() < pRight->GetMinX());
    }
};
class DescendMinX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinX() > pRight->GetMinX());
    }
};
class AscendMinY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinY() < pRight->GetMinY());
    }
};
class DescendMinY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMinY() > pRight->GetMinY());
    }
};
class AscendMaxX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxX() < pRight->GetMaxX());
    }
};
class DescendMaxX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxX() > pRight->GetMaxX());
    }
};
class AscendMaxY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxY() < pRight->GetMaxY());
    }
};
class DescendMaxY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetMaxY() > pRight->GetMaxY());
    }
};
class AscendGX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGx() < pRight->GetGx());
    }
};
class DescendGX
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGx() > pRight->GetGx());
    }
};
class AscendGY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGy() < pRight->GetGy());
    }
};
class DescendGY
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetGy() > pRight->GetGy());
    }
};
class AscendPixels
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetPixels() < pRight->GetPixels());
    }
};
class DescendPixels
{
public:
    bool operator()(Element* pLeft,Element* pRight) const 
	{
		return (pLeft->GetPixels() > pRight->GetPixels());
    }
};


//ラベリング結果ソート
void Label::Sort(int nSnum,BOOL isAscending/*=true*/) 
{
	switch(nSnum){
		case SORT_MIN_X:	 //x座標順	
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMinX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMinX());
			}
			break;
		case SORT_MAX_X: //x座標順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMaxX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMaxX());
			}
			break;
		case SORT_MIN_Y: //y座標順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMinY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMinY());
			}
			break;
		case SORT_MAX_Y: //y座標順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendMaxY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendMaxY());
			}
			break;
		case SORT_GX: //重心x座標順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendGX());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendGX());
			}
			break;
		case SORT_GY: //重心y座標順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendGY());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendGY());
			}
			break;
		case SORT_PIXELS: //面積順
			if( isAscending ){
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),AscendPixels());
			}else{
				::sort(this->m_result_region_info.begin(),this->m_result_region_info.end(),DescendPixels());
			}
			break;
		default: //不正な引数
			return ;
	}
}

//ラベリングソースバッファの作成
//CDIB &dib:GRAYImage
BOOL Label::CreateSrcBuf(CDIB &dib,BYTE byTh)
{
	int *pSrcBuf = GetSrcBuf();
	BYTE *pDibBits = dib.GetBits(); //先頭アドレス取得
	if(pSrcBuf != NULL && pDibBits != NULL){
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();
		int nLine = dib.GetLineSize();
		//Src Buf 作成
		for(int y=0;y<nH;y++){
			BYTE *pBits = pDibBits + (nH-1-y)*nLine; //上下反転
			for(int x=0;x<nW;x++){
				BYTE byB = *pBits++;//B
				BYTE byG = *pBits++;//G
				BYTE byR = *pBits++;//R
				BYTE bySet = 0;
				if( (BYTE)(0.299*byR + 0.587*byG + 0.114*byB) >= byTh ){ //フルカラー -> 256色
					bySet = 255; //２値化
				}			
				*pSrcBuf++ = bySet;
			}
		}
		return TRUE;
	}
	return FALSE;
}
//ラベリング結果バッファをビットマップとして書き出し
void Label::WriteDstBuf(LPCWSTR lpPath)
{
	int nW = GetWidth();
	int nH = GetHeight();
	CDIB dib;
	if( dib.Create(nW,nH) ){
		this->FillBitmapDst(dib);
		dib.WriteBitmap(lpPath);
	}
}
//ラベリングSrcバッファをビットマップとして書き出し
void Label::WriteSrcBuf(LPCWSTR lpPath)
{
	int nW = GetWidth();
	int nH = GetHeight();
	CDIB dib;
	if( dib.Create(nW,nH) ){
		this->FillBitmapSrc(dib);
		dib.WriteBitmap(lpPath);
	}
}

BOOL Label::CreateSrcBuf(Dib256 &dib,BYTE byTh)
{
	int *pSrcBuf = GetSrcBuf();
	BYTE *pDibBits = dib.GetBits(); //先頭アドレス取得
	if(pSrcBuf != NULL && pDibBits != NULL){
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();
		int nLine = dib.GetLineSize();
		//Src Buf 作成
		for(int y=0;y<nH;y++){
			BYTE *pBits = pDibBits + (nH-1-y)*nLine; //上下反転
			for(int x=0;x<nW;x++){
				*pSrcBuf++ = ( *pBits++ >= byTh ) ? MAXBYTE : 0;
			}
		}
		return TRUE;
	}
	return FALSE;
}

//ラベリングソースバッファの白黒反転
void Label::InvertSrcBuf()
{
	int *pSrcBuf = GetSrcBuf();
	for(int y=0,ymax=GetHeight();y<ymax;y++){
		for(int x=0,xmax=GetWidth();x<xmax;x++){
			*pSrcBuf++ = (*pSrcBuf) ? 0 : 255;
		}
	}	
}
//ドット抜け削除
void Label::EraseDot() 
{
	int *pSrcBuf = GetSrcBuf();
	int nH = GetHeight();
	int nW = GetWidth();
	pSrcBuf += nW;//y==0
	for(int y=1,ymax=nH-1;y<ymax;y++){
		pSrcBuf++; //x==0
		for(int x=1,xmax=nW-1;x<xmax;x++){
			if( *pSrcBuf == 0 && *(pSrcBuf+1) && *(pSrcBuf-1) && *(pSrcBuf+nW) && *(pSrcBuf-nW)){
				*pSrcBuf = 255; //ドット消去
			}
			pSrcBuf++;
		}
		pSrcBuf++; //x==nW-1
	}	
}

//ラベリング結果バッファの作成
BOOL Label::FillDstBuf(Element *pElem,BYTE byVal)
{
	if( pElem != NULL ){		
		FillFrameByte(m_pDstBuf,pElem,byVal);
		return TRUE;
	}
	return FALSE;
}
BOOL Label::FillDstBuf(int nEnum,BYTE byVal)
{
	Element *pElem = GetElement(nEnum);
	if( pElem != NULL ){		
		FillFrameByte(m_pDstBuf,pElem,byVal);
		return TRUE;
	}
	return FALSE;
}

//ラベリングSrc画像をビットマップへ転送
//ビットマップは必ずラベリングサイズと一致すること
BOOL Label::FillBitmapSrc(CDIB &dib) 
{
	BYTE *pBits = dib.GetBits();
	int *pData = m_pSrcBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //パディングサイズ
	if( GetWidth() != nW || GetHeight() != nH ){ //サイズチェック
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		for(int x=0;x<nW;x++){
			*pDst++	  = *pData;//B
			*pDst++ = *pData;//G
			*pDst++ = *pData++;//R
		}
	}	
	return TRUE;
}
//ラベリング結果画像をビットマップへ転送
//ビットマップは必ずラベリングサイズと一致すること
BOOL Label::FillBitmapDst(CDIB &dib) 
{
	BYTE *pBits = dib.GetBits();
	BYTE *pData = m_pDstBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //パディングサイズ
	if( GetWidth() != nW || GetHeight() != nH ){ //サイズチェック
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		for(int x=0;x<nW;x++){
			*pDst++	= *pData;//B
			*pDst++ = *pData;//G
			*pDst++ = *pData++;//R
		}
	}	
	return TRUE;
}

//ラベリング結果画像をビットマップへ転送
//ビットマップは必ずラベリングサイズと一致すること
BOOL Label::FillBitmapDst(Dib256 &dib) 
{
	BYTE *pBits = dib.GetBits();
	BYTE *pData = m_pDstBuf;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); //パディングサイズ
	if( GetWidth() != nW || GetHeight() != nH ){ //サイズチェック
		return FALSE;
	}
	for(int y=0;y<nH;y++){
		BYTE *pDst = pBits + ((nH-y-1)*nLine);
		memcpy(pDst,pData,nW);
		pData+=nW;
	}	
	return TRUE;
}

//ラベリング結果の取得(Point)
void Label::FillFramePoint(Element *pElem,ArrayPoint &aryPoint)
{
	aryPoint.RemoveAll();
	RSPList& rspl = pElem->GetRasterSegmentList();
	for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//全セグメントリスト検索
		RasterSegment *pRs = *rspi;
		int	lx = pRs->GetLeftX();
		int	rx = pRs->GetRightX();
		int	y = pRs->GetY();
		for(int x=lx;x<=rx;x++){
			aryPoint.AddPoint(x,y);
		}
	}
}

//縮小
void Label::ContractSrc()
{
	int *pOut = GetTmpBuf();
	int *pIn  = GetSrcBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	ZeroMemory(pOut,nW*nH*sizeof(int)); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( *pIn ){ //値が存在する
				if( *(pIn-1) && *(pIn+1)
					&& *(pIn-nW  ) && *(pIn+nW)
					&& *(pIn-nW-1) && *(pIn+nW-1) 
					&& *(pIn-nW+1) && *(pIn+nW+1) )
				{
					*pOut = *pIn;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //縮小状態をCopy
}
void Label::ContractDst()
{
	BYTE *pOut = (BYTE *)GetTmpBuf();
	BYTE *pIn  = GetDstBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	ZeroMemory(pOut,nW*nH); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( *pIn ){ //値が存在する
				if( *(pIn-1) && *(pIn+1)
					&& *(pIn-nW  ) && *(pIn+nW)
					&& *(pIn-nW-1) && *(pIn+nW-1) 
					&& *(pIn-nW+1) && *(pIn+nW+1) )
				{
					*pOut = *pIn;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //縮小状態をCopy
}

//拡大
void Label::ExpandSrc()
{
	int *pOut = GetTmpBuf();
	int *pIn  = GetSrcBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	CopyMemory(pOut,pIn,nW*nH*sizeof(int)); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( !*pIn ){ //値が存在しない
				if( *(pIn-1) || *(pIn+1)
					|| *(pIn-nW  ) || *(pIn+nW)
					|| *(pIn-nW-1) || *(pIn+nW-1) 
					|| *(pIn-nW+1) || *(pIn+nW+1) )
				{
					*pOut = 255;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //拡大状態をCopy
}

//X方向のみ拡大
void Label::ExpandSrcX(int nNum)
{
	for(int i=0;i<nNum;i++){
		int *pOut = GetTmpBuf();
		int *pIn  = GetSrcBuf();
		int nW = GetWidth();
		int nH = GetHeight();

		CopyMemory(pOut,pIn,nW*nH*sizeof(int)); //out is clear
		pOut += nW;// y==0
		pIn  += nW;
		for(int y=1;y<nH-1;y++){
			pIn++;
			pOut++; //x==0
			for(int x=1;x<nW-1;x++){
				if( !*pIn ){ //値が存在しない
					if( *(pIn-1) || *(pIn+1) ){
						*pOut = 255;
					}
				}
				pIn++;
				pOut++;
			}
			pIn++;
			pOut++; //x==nW-1
		}
		CopyMemory(GetSrcBuf(),GetTmpBuf(),nW*nH*sizeof(int)); //拡大状態をCopy
	}
}

void Label::ExpandDst()
{
	BYTE *pOut = (BYTE *)GetTmpBuf(); //ソース用テンポラリバッファ
	BYTE *pIn  = GetDstBuf();
	int nW = GetWidth();
	int nH = GetHeight();

	CopyMemory(pOut,pIn,nW*nH); //out is clear
	pOut += nW;// y==0
	pIn  += nW;
	for(int y=1;y<nH-1;y++){
		pIn++;
		pOut++; //x==0
		for(int x=1;x<nW-1;x++){
			if( !*pIn ){ //値が存在しない
				if( *(pIn-1) || *(pIn+1)
				|| *(pIn-nW  ) || *(pIn+nW)
				|| *(pIn-nW-1) || *(pIn+nW-1) 
				|| *(pIn-nW+1) || *(pIn+nW+1) )
				{
					*pOut = 255;
				}
			}
			pIn++;
			pOut++;
		}
		pIn++;
		pOut++; //x==nW-1
	}
	CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //拡大状態をCopy
}

//X方向のみの拡大
void Label::ExpandDstX(int nNum)
{
	for(int i=0;i<nNum;i++){
		BYTE *pOut = (BYTE *)GetTmpBuf(); //ソース用テンポラリバッファ
		BYTE *pIn  = GetDstBuf();
		int nW = GetWidth();
		int nH = GetHeight();

		CopyMemory(pOut,pIn,nW*nH); //out is clear
		pOut += nW;// y==0
		pIn  += nW;
		for(int y=1;y<nH-1;y++){
			pIn++;
			pOut++; //x==0
			for(int x=1;x<nW-1;x++){
				if( !*pIn ){ //値が存在しない
					if( *(pIn-1) || *(pIn+1) )
					{
						*pOut = 255;
					}
				}
				pIn++;
				pOut++;
			}
			pIn++;
			pOut++; //x==nW-1
		}
		CopyMemory(GetDstBuf(),GetTmpBuf(),nW*nH); //拡大状態をCopy
	}
}

//境界線追跡(8-連結)
int Label::FindBorder(int nEnum,ArrayPoint &aryPoint)
{
	Element *pElem = GetElement(nEnum);
	return FindBorder(pElem,aryPoint);
}
int Label::FindBorder(Element *pElem,ArrayPoint &aryPoint)
{
	aryPoint.RemoveAll();
	int nW = GetWidth();
	int nH = GetHeight();
	FillDstBuf(pElem,MAXBYTE); //ラベリング結果に255
	BYTE byBnum = 1; //境界線番号
	for(int y=1;y<nH-1;y++){
		BYTE *pBuf = GetDstBuf() + y*nW + 1;
		for(int x=1;x<nW-1;x++){
			if( *pBuf == MAXBYTE ){ //ラベリング結果の時
				if( *(pBuf-1) == 0 ){ //左側が境界
					ChaseBorder(aryPoint,x,y,BORDER_LEFTDOWN,pBuf,byBnum); //外側境界探索
					byBnum++;
				}else if(*(pBuf+1) == 0 ){ //右側が境界
					ChaseBorder(aryPoint,x,y,BORDER_RIGHTUP,pBuf,byBnum); //右側境界探索
					byBnum++;
				}
			}
			pBuf++;
		}
	}
	return (byBnum-1);
}

//境界線追跡(8-連結)
int Label::ChaseBorder(ArrayPoint &aryPoint,int x,int y,BORDER nDirection,BYTE *pBin,BYTE byBnum)
{
	int nW = GetWidth();	//ラベリング幅取得
	int nH = GetHeight();//ラベリング高さ取得

	int xStart = x; //始点
	int yStart = y;

	int xNow = x; //現在位置
	int yNow = y;

	int xCheck = -1;//探索位置
	int yCheck = -1; 

	int nCnt = 0;
	while(xCheck != xStart || yCheck != yStart){ //始点に戻るまで
		BOOL isMove = FALSE; //移動はありましたか？
		switch(nDirection){
            case BORDER_RIGHT: //右を調査				
				xCheck = xNow + 1; //右をチェック
				yCheck = yNow;
				if( xNow+1 < nW && *(pBin+1) ){ //右可
					pBin	+= 1;
					nDirection = BORDER_RIGHTDOWN;//右下へ
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHTUP; //右上チェック
				}
				break;
			case BORDER_RIGHTUP:		//右上を調査
				xCheck = xNow + 1; //右をチェック
				yCheck = yNow - 1;
				if( xNow+1 < nW && yNow-1>=0 && *(pBin-nW+1) ){ //右上可
					pBin	+= (-nW+1);
					nDirection = BORDER_RIGHTDOWN;//右下へ
					isMove = TRUE;
				}else{
					nDirection = BORDER_UP; //上チェック
				}
				break;
            case BORDER_UP: //上を調査
				xCheck = xNow;
				yCheck = yNow - 1;
				if( yNow-1>=0 && *(pBin-nW) ){ //上可
					pBin += -nW;
					nDirection = BORDER_RIGHTUP;
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFTUP;
				}
				break;
			case BORDER_LEFTUP:		//左上を調査
				xCheck = xNow - 1;
				yCheck = yNow - 1;
				if( xNow-1>=0 && yNow-1>=0 && *(pBin-nW-1) ){ //上可
					pBin += -nW-1;
					nDirection = BORDER_RIGHTUP;
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFT;
				}
				break;
            case BORDER_LEFT: //左を調査
				xCheck = xNow - 1;
				yCheck = yNow;
				if( xNow-1>=0 && *(pBin-1) ){
					pBin += -1;
					nDirection = BORDER_LEFTUP;	                    
					isMove = TRUE;
				}else{
					nDirection = BORDER_LEFTDOWN;
				}
				break;
			case BORDER_LEFTDOWN:	//左下を調査
				xCheck = xNow - 1;
				yCheck = yNow + 1;
				if( xNow-1>=0 && yNow+1<nH && *(pBin+nW-1) ){
					pBin += nW-1;
					nDirection = BORDER_LEFTUP;	
					isMove = TRUE;
				}else{
					nDirection = BORDER_DOWN;
				}
				break;				
            case BORDER_DOWN: //下を調査				
				xCheck = xNow;		//探索位置は現在の下へ
				yCheck = yNow + 1;
				if( yNow+1<nH && *(pBin+nW) ){ //下可
					pBin += nW; //アドレス追加
					nDirection = BORDER_LEFTDOWN; //左下へ
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHTDOWN; //右下へ
				}
				break;
			case BORDER_RIGHTDOWN:	//右下を調査
				xCheck = xNow + 1;
				yCheck = yNow + 1;
				if( xNow+1<nW && yNow+1<nH && *(pBin+nW+1) ){ //右下可
					pBin += nW+1; //アドレス追加
					nDirection = BORDER_LEFTDOWN; //左下へ
					isMove = TRUE;
				}else{
					nDirection = BORDER_RIGHT; //右へ
				}
				break;
			default:
				ASSERT(FALSE);
				break;
		}
		if( isMove ){//移動があったときだけ
			*pBin = byBnum; //境界線番号セット
			aryPoint.AddPoint(xNow,yNow); //境界点登録
			//TRACE("x,y=%d,%d\n",xNow,yNow);
			xNow = xCheck;	//現在位置の更新
			yNow = yCheck; 
			nCnt++;
		}		
	}
	aryPoint.AddPoint(-1,-1); //終点登録
	return nCnt;
}

//領域内要素収集
int Label::FindElement(CRect &rect,AryInt &aryElement)
{
	aryElement.RemoveAll();
	int nEmax = GetEmax();
	CRect rcElem;
	for(int i=0;i<nEmax;i++){
		Element *pElem = GetElement(i);
		rcElem.left  = pElem->GetMinX();
		rcElem.right = pElem->GetMaxX();
		rcElem.top   = pElem->GetMinY();
		rcElem.bottom= pElem->GetMaxY();
		//完全に含まれる
		if( rect.left <= rcElem.left && rect.top <= rcElem.top && rcElem.right <= rect.right && rcElem.bottom <=rect.bottom){
			aryElement.Add( i ); //要素番号をセット
		}
	}
	return (int)aryElement.GetSize();
}

//指定値のカウント
int Label::CountDst(BYTE byVal) 
{
	int nCnt = 0;
	int nSize = GetSize();
	BYTE *pDst = GetDstBuf();	
	for(int i=0;i<nSize;i++){
		if( *pDst++ == byVal ){
			nCnt++;
		}
	}	
	return nCnt;
}

//ラベリング結果の描画
void Label::PaintElementColor(Element *pElem,CDIB &dib,BYTE byR,BYTE byG,BYTE byB)
{
	BYTE *pTop = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	if(GetWidth() == nW && GetHeight() == nH){
		int nLine = dib.GetLineSize();
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//全セグメントリスト検索
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pBits = pTop + (nH-1-y)*nLine + lx*3; //上下反転
			for(int x=lx;x<=rx;x++){
				*pBits++ = byB; //B
				*pBits++ = byG; //G
				*pBits++ = byR; //R
			}
		}
	}
}
void Label::PaintElementBitmap(Element *pElem, CDIB &dibDst,CDIB &dibSrc)
{
	BYTE *pBitsD = dibDst.GetBits();
	BYTE *pBitsS = dibSrc.GetBits();
	int nW = dibDst.GetWidth();
	int nH = dibDst.GetHeight();
	if (GetWidth() == nW && GetHeight() == nH) {
		int nLine = dibDst.GetLineSize();
		RSPList& rspl = pElem->GetRasterSegmentList();
		for (RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++) {//全セグメントリスト検索
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pDst = pBitsD + (nH - 1 - y)*nLine + lx * 3; //上下反転
			BYTE *pSrc = pBitsS + (nH - 1 - y)*nLine + lx * 3; //上下反転
			
			for (int x = lx; x <= rx; x++) {
				if (*pDst < *pSrc) {
					*pDst++ = *pSrc++; //B
					*pDst++ = *pSrc++; //G
					*pDst++ = *pSrc++; //R
				}
				else {
					pDst += 3;
					pSrc += 3;
				}
			}
		}
	}
}


//Element部を指定値で塗りつぶす
void Label::FillElement(CDIB &dib,BYTE byVal)
{
	BYTE *pBitsT = dib.GetBits();

	int nW = dib.GetWidth();//幅取得
	int nH = dib.GetHeight(); //高さ取得
	int nLine = dib.GetLineSize();
	int nSize = dib.GetSize();//サイズ取得
	int nMax = GetEmax();
	for(int i=0;i<nMax;i++){
		Element *pElem = GetElement(i);
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//全セグメントリスト検索
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *pSrc = pBitsT + (nH-y-1) * nLine + lx*3; 
			memset(pSrc,byVal,(rx-lx)*3);
		}		
	}	
}

//比較用の画像を読み込み
int Label::LoadCheckImage(CString& strName) 
{
	CFileFind find;

	BOOL isWorking = find.FindFile(strName);
	while(isWorking){
	   isWorking = find.FindNextFile();
		if( find.GetLength() > 0 ){//ファイルサイズが存在するときだけ
			Dib256 dib;
			if( dib.LoadFile( find.GetFilePath() ) ){
				CString strTitle = find.GetFileTitle();
				TRACE(L"%c\n",strTitle[0]);
				m_aryImageCheck.AddData(strTitle[0],dib); 							
			}
		}
	}
	return (int)m_aryImageCheck.GetSize(); 
}
//マッチング処理
//画像データの比較
int Label::MatchingImage(Dib256 &dib,int nTh,double &dbPer)
{
	dib.ChangeBinary(nTh);
	BYTE *pTop = dib.GetBits();
	int nSize1 = dib.GetSize();
	int nCmax = 0; //カウンター Max
	int nIndex = -1;
	for( int i=0;i<m_aryImageCheck.GetSize();i++){ //比較用画像データ		
		Dib256 *pDibData = m_aryImageCheck[i].m_pDib256;
		int nSize2 = pDibData->GetSize();
		int nSize = min(nSize1,nSize2);

		BYTE *pData = pDibData->GetBits();//比較データ
		BYTE *pSrc  = pTop; //撮影データ
		int nGood = 0; //
		int nErr = 0;
		for(int n=0;n<nSize;n++){ //全データループ
			BYTE bySrc  = *pSrc++;
			BYTE byData = *pData++;

			if( bySrc == 0 && byData == 0 ){
				nGood++;
			}else if( bySrc == 0){
				nErr++;
			}else if( byData == 0){
				nErr++;
			}			
		}
		int nCnt = nGood - nErr;
		if( nCmax < nCnt ){ //最大値変更
			nCmax = nCnt;
			nIndex = i;
		}
	}
	int nCount = dib.CountPixels(nTh);
	dbPer = 100.0 * nCmax / nCount; //%
	return nIndex;
}
