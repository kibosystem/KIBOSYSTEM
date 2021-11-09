#pragma once

#include <vector>
using namespace std;

#include "ScriptBase.h"
#include "../Lib/Dib.h"
#include "../Lib/Dib256.h"
#include "../Lib/DataBuf.h"
#include "../Lib/FindLine.h"
#include "../Lib/FindCircle.h"
#include "../Dialog/HistgramDlg.h"

//ビットマップ配列
class MapPDib : public CMap<int,int,CDIB*,CDIB*>
{
public:
	MapPDib(){}
	~MapPDib();

	CDIB* GetDibPtr(int nKey); //ビットマップクラス取得	
	void Clear();//全ビットマップ削除
};

//24bit Color Bitmapアクセスクラス
class Bitmap24 : public ScriptBase 
{
protected:
	CDIB	 m_dib;		//ビットマップ
	CDIB	 m_dibTmp;	//データ退避用ビットマップ
	Dib256  m_dib256;	//256色計算用ビットマップ
	IntBuf  m_bufCalc;	//ビットマップ計算用バッファ
	BYTE	 m_byTh;		//しきい値
	COLORREF m_crLine; //ライン色

	HistgramDlg m_dlgHistgram;//ヒストグラムダイアログ
	MapPDib m_mapDibPtr;//ビットマップ配列
public:
	PointF m_ptS; //ラインスタートポイント
	PointF m_ptE; //ラインエンドポイント
public:
	enum{
		FUNC_LOAD = 1	//読み込み
		,FUNC_SAVE		//書き込み
		,FUNC_SHOW		//表示
		,FUNC_SETTHRESHOLD	//しきい値設定
		,FUNC_GETTHRESHOLD  //2値化しきい値取得
		,FUNC_SETLINECOLOR	//ライン色設定
		,FUNC_FROMIMAGEDEVICE//カメラ画像取得
		,FUNC_IMAGEDEVICEDIFF//カメラ画像比較
		,FUNC_TOIMAGEDEVICE	//イメージデバイスへ画像送信
		,FUNC_COLORLENGTH	//色距離変換
		,FUNC_CHANGECOLOR	//色変換		
		,FUNC_GRADIENT		//微分
		,FUNC_SOBEL			//輪郭抽出 縦、横
		,FUNC_LAPLACIAN		//ラプラシアン
		,FUNC_PREWITT		//輪郭抽出
		,FUNC_GAMMA			//ガンマ補正
		,FUNC_INVERT		//各色反転
		,FUNC_CHANGERANGE	//コントラスト最大化
		,FUNC_EXPAND		//膨張
		,FUNC_SHRINK		//収縮
		,FUNC_COLLECTBITMAP	//ビットマップを連想配列に連続登録
		,FUNC_SETBITMAP		//ビットマップを連想配列に保存
		,FUNC_GETBITMAP		//連想配列からビットマップ取得
		,FUNC_GETBITMAPCOUNT//連想配列数取得
		,FUNC_DELTADIB		//ビットマップ差分
		,FUNC_DIVDIB		//ビットマップ割り算
		,FUNC_AVERAGE		//平均画像の作成
		,FUNC_DIBSETCLEAR	//ビットマップ連想配列クリア
		,FUNC_HISTGRAM		//ヒストグラムの表示
		,FUNC_GETBINARYLEVEL//指定面積割合になる２値化レベルの取得
		,FUNC_GETBRIGHTNESS //明るさの範囲を求める
		,FUNC_MEDIAN		//メディアンフィルタ
		,FUNC_MAPDELTA		//連想配列画像差分作成
		,FUNC_EDIT			//ビットマップ編集
		,FUNC_CALCTHRESHOOLD//しきい値自動計算
		,FUNC_SELECTCOLOR	//色抽出
		,FUNC_SELECT		//上限と下限選択
		,FUNC_FINDEDGE		//エッジ座標検出
		,FUNC_DRAWLINE		//ライン描画
		,FUNC_DRAWBITMAPLINE//ビットマップライン描画
		,FUNC_MASKBORDER	//ラベリング要素マスク
		,FUNC_CALCWIDTH		//幅を計算
		,FUNC_CALCHEIGHT	//高さを計算
		,FUNC_CALCWIDTHDIRECT	//最小幅を計算
		,FUNC_CALCHEIGHTDIRECT	//最小高さを計算
		, FUNC_RMAX			//R要素最大値
		, FUNC_RMIN			//R要素最小値
		, FUNC_GMAX			//G要素最大値
		, FUNC_GMIN			//G要素最小値
		, FUNC_BMAX			//B要素最大値
		, FUNC_BMIN			//B要素最小値
		,FUNC_DEFECTTEST	//欠陥検査
		,FUNC_LENSCHECK		//レンズチェック
		,FUNC_BINARYMATCHING//2値化マッチング
		,FUNC_OUTPUTFILEPATH//FilePath出力
		,FUNC_OUTPUTFILENAME//FileName出力
		,FUNC_MAX
	};
public:
	Bitmap24(void);
	virtual ~Bitmap24(void);

	static Bitmap24* GetInstance();

	COLORREF GetLineColor() { return m_crLine; } //ライン色取得
	void SetLineColor(COLORREF color) { m_crLine = color; } //ライン色設定
	BYTE GetTh(){return m_byTh;}		//しきい値取得
	void SetTh(BYTE by){m_byTh = by;}	//しきい値設定
	CDIB& GetDib(){return m_dib;} //ビットマップ取得
	Dib256&  GetDib256(){return m_dib256;}	//256色計算用ビットマップ取得
	CDIB& GetDibTmp(){return m_dibTmp;} //一時的なビットマップ取得
	MapPDib& GetMapDib(){return m_mapDibPtr;} //ビットマップ配列
	int GetMapCount(){ return (int)m_mapDibPtr.GetCount(); } //ビットマップ配列
	void FromImageDevice(); //ImageDeviceから画像取得	
	BOOL ImageDeviceDiff(); //ImageDeviceとの画像比較
	void ToImageDevice();//イメージデバイスへ画像送信
	void ShowHistgram(); //ヒストグラム表示		
	//関数名チェック
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//実行
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID 番号
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script文字列
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);

	void AverageFilter(); //平均フィルタ
	void MedianFilter(){ //メディアンフィルタ
		m_dib.MedianFilter();
	}
	void MedianFilter(CRect &rect){ //メディアンフィルタ
		m_dib.MedianFilter(rect);
	}
	void Expand(int nNum);//膨張
	void Shrink(int nNum);//収縮		
	void ClearR();//R成分消去
	void ClearG();//G成分消去
	void ClearB();//B成分消去
	void Prewitt(double dbAmp);//輪郭抽出
	void Gradient(double dbAmp);//微分処理
	void Sobel(double dbAmp);//輪郭抽出 縦＆横
	void Laplacian(double dbAmp);//ラプラシアン		
	void Gamma(double dbExp);//ガンマ補正
	void DrawLine(int nX1,int nY1,int nX2,int nY2,COLORREF color,int nWidth);//ライン描画	
	void DrawBitmapLine();//m_ptS - m_ptE 間のライン描画
	int GetMaxElement(int nRGB); //R要素最大値取得
	int GetMinElement(int nRGB); //R要素最小値取得
	int GetMaxElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax);//RGB要素最大値取得 領域指定
	int GetMinElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax);//RGB要素最大値取得 領域指定	
	void CollectBitmap(CString &strPath); //指定フォルダ内ビットマップ収集
	BOOL SetDib(int nNo);//指定番号にビットマップ記憶
	CDIB* GetDibPtr(int nNo){return m_mapDibPtr.GetDibPtr(nNo);}
	
	double FindEdge(CRect &rect,int nDirection,int nColor);//しきい値座標取得
	double FindEdgeH(int nTh,int nX,int nY,int nDelta);	//水平方向エッジ検出
	double FindEdgeV(int nTh,int nX,int nY,int nDelta);	//垂直方向エッジ検出
	double CalcWidth(int nX, int  nY, int  nRange, CString& strType); //幅取得
	double CalcHeight(int nX, int  nY, int  nRange, CString& strType);//高さ取得																	  
	double CalcWidthDirect(int nX, int  nY, int  nRange, PointF &ptL, PointF &ptR);//最小幅を計算
	double CalcHeightDirect(int nX, int  nY, int  nRange, PointF &ptT, PointF &ptB);//最小高さを計算
protected:	
	virtual BOOL ShowBitmap(){return FALSE;}//ビットマップ表示仮想関数

private:
	void Average(CStringArray &strAryPath);//指定ビットマップの平均を計算	
	void And();//ビットマップ配列のANDを計算
	void Expand(int nW,int nH,int nLine,int nPad);//膨張
	void Shrink(int nW,int nH,int nLine,int nPad);//収縮	
	void SelectColor(double dbLength,BYTE byR,BYTE byG,BYTE byB);//近似色抽出
	int GetBinaryLevel(double dbPer);//指定割合の白値で区分するためのしきい値を取得
	int CalcThreshold(CString &strColor,int nPixels);//面積比からしきい値を計算	
	int CalcThreshold();//自動しきい値計算	
	int GetBrightnessMin(const CRect &rect);//指定範囲明るさ最小値取得	
	int GetBrightnessMax(const CRect &rect);//指定範囲明るさ最大値取得
	int GetBrightnessRange(const CRect &rect);//指定範囲明るさ範囲取得
	int GetBrightnessDiff(const CRect &rect);//指定範囲明るさ微分値取得	
	BYTE Average(BYTE *pByte,int nLineSize);//平均値取得
	void MapDelta(double dbAmp);//連想配列画像差分作成	
	void SelectLH(double dbLow,double dbHight);//上限と下限の選択	
	void FindCircle(int nXmax,int nYmax,int nRmax);//円検出	
	BOOL DeltaDib(CDIB *pDib1,CDIB *pDib2);//ビットマップの引き算	
	BOOL DivDib(CDIB *pDib1,CDIB *pDib2);//ビットマップの割り算
	void DefectTest(int nTh,int nRange,int nMin); //欠陥検査
	void PaintBorder(int nEdge);//境界線塗りつぶし	
	void MaskBorder(int nEnum);//ラベリングマスク	
	double BinaryMatching(int nTh, double dbX, double dbY, int nMin, int nMax);//2値化マッチング
};
