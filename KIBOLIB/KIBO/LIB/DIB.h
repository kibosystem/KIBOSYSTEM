// Copyright (C) Yoshinori Ito 
// All rights reserved.
//

#pragma once

#include "Databuf.h"
#include "gdiplus.h"

#include "../OpencvInc.h"

//CDIBは直接書き込み可能なデバイス独立のビットマップ（DIB）を作成するクラスです。

class CDIB  
{
	//▼メンバ
private:

	//ビットマップサイズ
	int  m_nBitsSize;		

	//ビットマップヘッダ情報
	BITMAPINFOHEADER m_BMI; 

	//ビットマップデータを直接操作するためのポインタ
	BYTE *m_pBits;			

	//作成されるビットマップハンドル
	HBITMAP m_hBitmap;	

	CString m_strFilePath; //ビットマップファイルパス

	DataBuf m_data; //画像処理用テンポラリバッファ
public:
	CDIB();				//コンストラクタ
	virtual ~CDIB();	//デストラクタ

	enum{ //画像変換タイプ
		 TYPE_COLOR = 0 //RGB				NORMAL
		,TYPE_GRAY  //256色Gray				GRAY
		,TYPE_RGRAY //R値のみのGray表示		RGRAY
		,TYPE_GGRAY //G値のみのGray表示		GGRAY
		,TYPE_BGRAY //B値のみのGray表示		BGRAY
		,TYPE_R	//R値のみ					R
		,TYPE_G	//G値のみ					G		
		,TYPE_B	//B値のみ					B
		,TYPE_Y //輝度						Y
		,TYPE_H //色相						H
		,TYPE_S //彩度						S
		,TYPE_YHS //RGB->YSH				YHS
		,TYPE_BIN //Binary Mode				2
		,TYPE_ACCENT_R	//R強調
		,TYPE_MEDIAN //メディアン
		,TYPE_NUM
	};

	CString& GetFilePath(){return m_strFilePath;} //ビットマップファイルパス取得
	void SetFilePath(CString& str){ m_strFilePath = str; }
	CString GetFileName(); //ファイル名称のみ取得

	//アーカイブを利用してBitmapのセーブ・ロードをします。
	void Serialize(CArchive& ar); //アーカイブクラス

	// 指定DIBファイルを読み込み、指定サイズに変更します。
	BOOL Load(CString strFileName,int nCx,int nCy);

	// 指定DIBファイルを読み込みます。
	BOOL Load(CString sFileName);		

	// 指定DIBファイルをリソースから読み込み、指定サイズに変更します。
	BOOL Load(int nID,int nCx,int nCy);

	// 指定DIBファイルをリソースから読み込みます。
	BOOL Load(int nID);

	//直接書き込み可能なデバイス独立のビットマップ（24bpp DIB）を作成します。
	BOOL Create(int iWidth, int iHeight);

	//ビットマップの解放をします。
	void DestroyBitmap();	

	//白で塗りつぶし	
	void PaintWhite();		

	//黒で塗りつぶし
	void PaintBlack();
	
	//指定色で塗りつぶし
	void PaintColor(COLORREF color);
	void PaintColor(COLORREF cr1,COLORREF cr2);

	void MaskRect(CRect &rcMask,COLORREF color);	//矩形Maskを描画します（描画のみ）	
	void MaskCircle(CRect &rcMask,COLORREF color);	//円形マスクを描画します	
	void PaintRgn(CRgn &rgn,COLORREF color);//領域の塗りつぶし

	//指定デバイスコンテキストにビットマップを描画		
	void Shift(CDIB &dib, double dbDx, double dbDy);//xy方向移動
	void Draw(CDC *pDC,int nX1,int nY1,int nW1,int nH1,int nX2,int nY2);
	void DrawLine(CPoint &ptS, CPoint &ptE, LOGPEN &lp); //ライン描画	
	void DrawRectangle(CRect &rect, LOGBRUSH &lb, LOGPEN &lp); //矩形描画
	void StretchDraw(CDC *pDc,int x,int y,int nW,int nH); //拡大縮小機能付き描画
	void Line(int nX1,int nY1,int nX2,int nY2,COLORREF color);
	//DIBのデータ先頭アドレス取得
	BYTE* GetBits(){return m_pBits;}

	//DIBの幅を取得
	int GetWidth (){return m_BMI.biWidth;}

	//DIBの高さを取得
	int GetHeight(){return abs(m_BMI.biHeight);}

	//DIBのビットマップサイズを取得
	int	GetSize(){return m_nBitsSize;}

	int GetLineSize(){return ((GetWidth() * 3 + 3) & ~3);} //1 Line Size取得します。

	//一時的なビットマップクラス取得
	CBitmap* GetBitmap(){return CBitmap::FromHandle(m_hBitmap);}

	//ビットマップハンドルを取得
	HBITMAP GetHandle(){return m_hBitmap;}
	
	//指定Bitmap Data 複製	
	void Copy  (CDIB& dibDest);

	void ScreenCapture(const CRect &rect); //指定領域スクリーンキャプチャ

	//指定したビットマップをメンバにコピーします。
	void SetDib(int x,int y,CDIB& dibSrc );//指定ビットマップを指定座標に転送します。

	//指定したビットマップを指定サイズに変更してコピー転送します。
	void CopyDib(CDIB& dibSrc	//指定ビットマップ
				,int nW			//ビットマップ幅
				,int nH);		//ビットマップ高さ

	//指定DIBの左下の色を透明色としてスプライト転送を行う
	void Sprite(CDIB& dibSrc //Bitmap Sprite 転送
				,int nX		//スプライト転送x座標
				,int nY);	//スプライト転送y座標

	//ビットマップ切り取り
	BOOL Cut(CDIB &dibSrc,CRect &rect);

	//CDIBからCBitmap* へのcast
	operator CBitmap*(){return GetBitmap();} 

	COLORREF GetColor(int x,int y); //指定座標の色を取得

	int GetPixels(int nVal); //指定値画素数取得

	//Bit操作
	BYTE GetBitsB(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )  );}//pixel Blue
	BYTE GetBitsG(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+1);}//pixel Green
	BYTE GetBitsR(int x,int y) {return *( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+2);}//pixel Red
	BYTE* GetPBitsB(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )  );}//pixel Blue ptr
	BYTE* GetPBitsG(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+1);}//pixel Green ptr
	BYTE* GetPBitsR(int x,int y) {return ( m_pBits + 3*x + (GetHeight()-1-y) * ( (GetWidth()*3 + 3) & ~3 )+2);}//pixel Red ptr

	CRect Scroll(CWnd *pWnd,int nDx,int nDy); //ビットマップスクロール
	
	BOOL WriteBitmap(LPCTSTR lpszFilePath);//ビットマップファイル保存
	BOOL WriteImage(LPCTSTR lpszPathName,LPCTSTR lpszExt);//画像ファイル保存
	BOOL Save(CString strPathName,const WCHAR* wpszFormat);//指定フォーマットでSave

	void And(CDIB *pDib); //And値セット
	void ChangeBinary(BYTE byTH);//2値化処理			
	void ChangeBinary(BYTE byThMin,BYTE byThMax);//2値化処理
	void ChangeColor(int nType);//色変換処理
	BOOL GetRange(BYTE &byMin,BYTE &byMax);//最大最小値の取得
	BOOL GetBlockRange(BYTE *pBits,int nBw,int nBh,BYTE& byMin,BYTE& byMax); //ブロック単位での最大最小値の取得
	void ChangeRange(BYTE byMin,BYTE byMax);//コントラスト最大化	
	void CollectHistgram(int nHistR[],int nHistG[],int nHistB[]);//ヒストグラム収集
	void CollectHistgramGray(int nGray[]); //ヒストグラム収集　Gray
	int CollectHistgramRect(int nHistR[],int nHistG[],int nHistB[],const CRect &rect);
	void CollectHistgramRgn (int nHistR[],int nHistG[],int nHistB[],const CRgn  &rgn);
	void ColorLength(BYTE byR,BYTE byG,BYTE byB);//指定色との距離画像に変換
	void Invert();//各色反転
	BOOL IsEqual(CDIB &dib); //画像比較
	void MedianFilter(); //メディアンフィルタ
	void MedianFilter(CRect &rect);
	int CountColor(BYTE byR,BYTE byG,BYTE byB);//指定色の画素数をカウント	
	double GetFocusValue(const CRect &rect);//オートフォーカス用計算値取得	
	int DeltaY(const CRect &rect);//明度の差分取得

	
	BOOL IsExistR(double dbAmp);//簡易R判定
	BOOL IsExistG(double dbAmp);//簡易G判定
	BOOL IsExistB(double dbAmp);//簡易B判定	
	BOOL IsExistTh(int nTh, int nXmin, int nYmin, int nXmax, int nYmax);//しきい値判定

	BOOL ToMat(cv::Mat &matImg); //DIB -> Mat
	BOOL FromMat(cv::Mat &matImg); //Mat -> DIB

	//以下のメンバ関数にはアクセスしないでください。
private: //内部関数
	HBITMAP LoadBitmapHandle(CString &strFileName);	//指定ビットマップ読み込み	
	BOOL CreateDibSectionHandle(HBITMAP hBmpTmp);	//DibSection Handle 作成	
	BYTE Median(BYTE *pByte,int nLineSize);//メディアン値取得
};

