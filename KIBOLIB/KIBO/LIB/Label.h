#pragma once

#pragma warning(disable : 4995)

#include "dib.h"
#include "dib256.h"
#include "CLabeling.h"
#include "ArrayPoint.h"
#include "typedef.h"
#include "ArrayPoint.h"

enum BORDER
{
     BORDER_RIGHT= 0	//右　を調査
	,BORDER_RIGHTUP		//右上を調査
	,BORDER_UP			//上　を調査
	,BORDER_LEFTUP		//左上を調査
	,BORDER_LEFT		//左　を調査
	,BORDER_LEFTDOWN	//左下を調査
	,BORDER_DOWN		//下を調査
	,BORDER_RIGHTDOWN	//右下を調査
};

//重ねあわせ用画像データ
class CheckImage
{
public:
	TCHAR m_Char;		//正解文字
	Dib256 *m_pDib256; //画像データ
public:
	CheckImage(){//コンストラクタ
		m_Char = 0;			//正解文字
		m_pDib256 = NULL;	//画像データ
	}
	~CheckImage(){} //デストラクタ
	CheckImage& operator=(const CheckImage&t)
	{
		m_Char = t.m_Char;
		m_pDib256 = t.m_pDib256;
		return *this;
	}
};

//比較用画像データ
class ArrayCheckImage : public CArray<CheckImage> 
{
public:
	ArrayCheckImage(){}
	~ArrayCheckImage(){
		for(int i=0;i<GetSize();i++){
			delete GetAt(i).m_pDib256;
		}
	}

	void AddData(TCHAR t,Dib256& dib256){
		CheckImage check;
		check.m_Char = t;
		check.m_pDib256 = new Dib256(); 
		if( check.m_pDib256 != NULL ){
			check.m_pDib256->Copy( dib256 ); //画像データコピー
			this->Add( check ); //追加
		}
	}
};

class Label : public CLabeling
{
private:
    int		*m_pSrcBuf;	//ラベリングソース
    BYTE	*m_pDstBuf;	//ラベリング結果
    int		*m_pTmpBuf; //ラベリングソーステンポラリバッファ
	int		m_nW;		//ラベリング幅
	int		m_nH;		//ラベリング高さ
	int		m_nBufSize; //バッファサイズ
public:
	static ArrayCheckImage m_aryImageCheck; //比較用画像
public:
	Label(void);
	~Label(void);

	//ソートタイプ
	//ラベリング結果ソート
	enum{
		 SORT_MIN_X = 0	//x座標小さい順
		,SORT_MAX_X		//x座標大きい順
		,SORT_MIN_Y		//y座標小さい順
		,SORT_MAX_Y		//y座標大きい順
		,SORT_GX		//重心x座標順
		,SORT_GY		//重心y座標順
		,SORT_PIXELS	//面積順
		,SORT_MAX
	};

	int*	GetSrcBuf(){return m_pSrcBuf;} //ラベリングソース
	BYTE*	GetDstBuf(){return m_pDstBuf;} //ラベリング結果
	int*	GetTmpBuf(){return m_pTmpBuf;} //テンポラリバッファ （ソースバッファサイズ）
	void ClearTmpBuf(){ZeroMemory(m_pTmpBuf,GetSize()*sizeof(int));} //ラベリングソースバッファクリア

	void WriteSrcBuf(LPCWSTR lpPath); //ラベリングSrcバッファをビットマップとして書き出し
	void WriteDstBuf(LPCWSTR lpPath); //バッファをビットマップとして書き出し

	int GetWidth() {return m_nW;} //ラベリング幅取得
	int GetHeight(){return m_nH;} //ラベリング高さ取得
	int GetSize(){return m_nW * m_nH;} //ラベリングサイズ取得
	void Free(); //ラベリングバッファ解放
	BOOL Init(int nW,int nH);	//ラベリング初期化
	void Mask(BYTE byVal,CRect &rect,BOOL isPaint); //マスク領域のソースバッファをクリア
	int	Exec(int nMinSize,int nMaxSize);	//ラベリング実行
	int Exec(int nMinSize,int nMaxSize,CRect &rect);//ラベリング領域指定	
	int ElementExec(int nEnum,int nMinSize,int nMaxSize);//ラベリング領域指定
	void Sort(int nSnum,BOOL isAscending); //ラベリング結果ソート
	BOOL CreateSrcBuf(CDIB &dib,BYTE byTh);	//ビットマップからのラベリングソースバッファの作成
	BOOL CreateSrcBuf(Dib256 &dib,BYTE byTh);

	void InvertSrcBuf();//ラベリングソースバッファの白黒反転
	void EraseDot();//ドット抜け削除

	BOOL FillDstBuf(Element *pElem,BYTE byVal); //ラベリング結果バッファの作成
	BOOL FillDstBuf(int nEnum,BYTE byVal); //ラベリング結果バッファの作成
	BOOL FillBitmapSrc(CDIB &dib); //ラベリングSrc画像ををビットマップへ転送
	BOOL FillBitmapDst(CDIB &dib); //ラベリング結果画像をビットマップへ転送
	BOOL FillBitmapDst(Dib256 &dib);
	//ラベリング結果の取得(Point)
	void FillFramePoint(Element *pElem,ArrayPoint &aryPoint);

	void ContractSrc();	//縮小
	void ExpandSrc();	//拡大
	void ContractDst();	//縮小
	void ExpandDst();	//拡大
	int  CountDst(BYTE byVal); //指定値のカウント

	void ExpandSrcX(int nNum);//指定回数X方向のみ拡大 Src
	void ExpandDstX(int nNum);//指定回数x方向のみ拡大 Dst

	int FindBorder(int nEnum,ArrayPoint &aryPoint);//境界線追跡(8-連結)
	int FindBorder(Element *pElem,ArrayPoint &aryPoint);
	int FindElement(CRect &rect,AryInt &aryElement); //領域内要素収集

	void FillElement(CDIB &dib,BYTE byVal);//Element部を指定値で塗りつぶす

	//ラベリング結果の描画
	void PaintElementColor(Element *pElem,CDIB &dib,BYTE byR,BYTE byG,BYTE byB);
	void PaintElementBitmap(Element *pElem, CDIB &dibDst, CDIB &dibSrc);

	//ラベリング用のバッファを作成
	BOOL CreateBuffer(int nSize);

	int LoadCheckImage(CString& strName); //比較用の画像を読み込み
	int MatchingImage(Dib256 &dib,int nTh,double &dbPer);//マッチング処理

private:
	int ChaseBorder(ArrayPoint &aryPoint,int x,int y,BORDER nDirection,BYTE *pBin,BYTE byBnum);//境界線追跡(8-連結)
};
