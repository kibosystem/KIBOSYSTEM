#pragma once

#include "ScriptBase.h"
#include "../LIB/DIB.h"
#include "../LIB/Label.h"

class Labeling : public ScriptBase
{
private:
	bool m_isInitOK; //初期化しましたか？
	CDIB m_dibTmp; //テンポラリビットマップ
	map<CString,ArrayPoint*> m_mapAryPointPtr;
public:
	Label m_label; //ラベリングクラス
public:
	//コマンド種類
	enum{
		FUNC_SETBITMAP = 1			//BITMAPクラスで初期化
		,FUNC_GETBITMAPSRC		//ラベリング画像取得
		,FUNC_GETBITMAPDST		//ラベリング結果画像取得
		,FUNC_EXEC				//ラベリング実行
		,FUNC_ELEMENTEXEC		//エレメント内部のラベリング
		,FUNC_SORT				//ラベリング結果ソート
		,FUNC_OUTPUTRESULT		//ラベリング結果の出力
		,FUNC_DRAWRECT			//ラベリング領域描画
		,FUNC_PAINTELEMENT		//ラベリング領域描画
		,FUNC_LOADELEMENT		//画像ファイル読み込み
		,FUNC_REMOVE_ALL_ELEM	//ラベリング要素削除
		,FUNC_INVERT			//白黒反転
		,FUNC_EXPAND			//膨張
		,FUNC_GETBORDER			//境界線抽出
		,FUNC_DRAWBORDER		//境界線描画
		,FUNC_SELECTBORDER		//境界線一部抽出
		,FUNC_SETWIDTHRANGE		//要素の幅指定
		,FUNC_SETHEIGHTRANGE	//要素の高さ指定
		,FUNC_COUNT				//ラベリング要素数取得
		,FUNC_MAXELEMENT			//最大要素番号取得
		,FUNC_INELEMENT			//要素内の点ですか？	
		,FUNC_GX					//重心ｘ座標
		,FUNC_GY					//重心ｙ座標
		,FUNC_MIN_X				//最小ｘ座標
		,FUNC_MIN_Y				//最小ｙ座標
		,FUNC_MAX_X				//最小ｘ座標
		,FUNC_MAX_Y				//最小ｙ座標
		,FUNC_MIN_L				//指定地点からの最短距離
		,FUNC_MAX_L				//指定地点からの最長距離
		,FUNC_WIDTH				//幅
		,FUNC_HEIGHT				//高さ
		,FUNC_PIXELS				//画素数
		,FUNC_BORDER				//周囲長
		,FUNC_R_AVR				//平均半径取得
		,FUNC_R_MAX				//最大半径取得
		,FUNC_CIRCLE				//円形度
		,FUNC_MAX				//コマンド数
	};
public:
	Labeling(void);
	virtual ~Labeling(void);
	static Labeling* GetInstance();	

	Label& GetLabel() { return m_label; } //ラベリングクラス取得
	BOOL IsInitOK(){return m_isInitOK;}//初期化しましたか？
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

	BOOL SetBitmap(int nTh); //Bitmapクラスで初期化
	BOOL Init(CDIB &dib,int nTh); //DIBから初期化
	int Exec(int nMin,int nMax,int nLeft,int nTop,int nRight,int nBottom);	//ラベリング実行
	int Exec(int nMin,int nMax);//ラベリング実行	
	int ElementExec(int nEnum,int nMin,int nMax);//エレメント内部のラベリング
	void Sort(int nSnum,BOOL isAscending); //ラベリング結果ソート
	void OutputResult(); //ラベリング結果の出力
	int GetCount();	//ラベリング要素数取得
	void DrawElementRect(int nEnum,int nR,int nG,int nB);//ラベリング領域描画
	void DrawElementRect(int nEnum,int nR,int nG,int nB,int nDx,int nDy);//ラベリング領域描画　描画領域をずらす時	
	void PaintElement(int nEnum,int nR,int nG,int nB);//ラベリング要素塗りつぶし	
	void Invert(); //白黒反転	
	int GetBorderPixels(int nBnum,ArrayPoint &aryPoint);//周囲長のPixel数を取得
	ArrayPoint* GetAryPoint(CString &str); //点配列取得
	ArrayPoint* GetAryPoint(int n);		
	void SelectBorder(int nEnum,CString strType,int nW);//Type別Border抽出	
	void SetWidthRange(int nWmin, int nWmax); //要素幅指定	
	void SetHeightRange(int nHmin, int nHmax);//要素高さ指定
	void CollectPointXmin(int nEnum,ArrayPoint &aryPtXmin,int nYS,int nYE);//指定Y座標の左端点を収集
	void CollectPointXmax(int nEnum,ArrayPoint &aryPtXmax,int nYS,int nYE);//指定Y座標の右端点を収集	
	double Gx(int nEnum);//重心x座標
	double Gy(int nEnum);//重心y座標
	int Pixels(int nEnum);//画素数
	int Xmin(int nEnum);//最小ｘ座標	
	int Ymin(int nEnum);//最小ｙ座標
	int Xmax(int nEnum);//最大ｘ座標
	int Ymax(int nEnum);//最大ｙ座標	
	int GetMaxElement();//最大要素番号取得

};
