#pragma once

#include "ScriptBase.h"
#include "CharElement.h"
#include "../LIB/DIB.h"
#include "../LIB/Dib256.h"
#include "../LIB/Label.h"
#include "../LIB/PointDbl.h"
#include "../LIB/GdiPlus.h"

#define MIN_CONVERT		 60  //ラベリングコンバート最小値　Default
#define MAX_CONVERT		195  //ラベリングコンバート最大値　Default
#define MIN_EDGE_LEN	  5	 //エッジ判定最小距離
#define MIN_CONTOUR_LEN   5  //最低限必要な境界線の画素数
#define DEFAULT_CHAR_LENGTH	600.0	//文字認識距離

#define DEFAULT_CHAR_NUM  0		//標準文字番号
#define DEFAULT_CHECK_SPACE 2

//#define LINECHECK //線幅チェックデバッグプログラム用
//#define CALC_DELTA_CHECK

#ifdef USE_CHARRECOG 

//文字欠け検査結果
class CheckNickData
{
public:
	bool	m_isOK;		//欠け検査結果
	TCHAR	m_tChar;	//欠け検査文字
public:
	//コンストラクタ
	CheckNickData()
	{
		m_isOK = true;
		m_tChar= 0;
	}
	//デストラクタ
	~CheckNickData(){}
};

//検査領域情報
class CheckRect
{
public:
	CRect m_rect;		//検査領域
	int	  m_nCharNum;	//文字数
	CString m_str;		//固定文字列
public:
	//コンストラクタ
	CheckRect()
	:m_rect(0,0,0,0)
	{
		m_nCharNum = 0;
	}
	//デストラクタ
	~CheckRect(){}
};

//検査結果セット
class CheckResult
{
public:
	CPoint	m_ptTL;	//左上相対座標
	CLine	m_line;//検査結果ライン
public:
	CheckResult(){}  //コンストラクタ
	~CheckResult(){} //デストラクタ
	
	void Clear(){m_line.Clear();} //データクリア
	void SetTL(CPoint &pt){m_ptTL = pt;} //相対座標設定
	void SetLine(CLine &l);	//Y方向検査結果設定	Color::Orange
	void SetLine(CPoint &ptS,CPoint &ptE);
	void ShowLine();//x方向ライン描画
	void ShowMinLine(SizeDbl &size);//最小ラインの表示
private:
	void ShowLine(LPWSTR szName,CPoint &ptTL,CLine& line,Color color);
};

//特殊コマンド
//仕様組み込みのためだけのクラス
class CharRecog : public ScriptBase
{
private:
	CString m_strLearningDataFile;	//学習データファイルパス
	CMap<int,int,CRect,CRect&>				m_mapCheckRectOld;	//初期設定時の検査領域
	CMap<int,int,CheckRect,CheckRect&>		m_mapCheckRect;		//検査領域差分情報
	CMap<int,int,BOOL,BOOL>					m_mapCommonChar;	//共通文字領域
	map<int,bool>							m_mapIsDitrtyCheck;	//汚れ検査を行いますか？
	map<int,bool>							m_mapIsInclination; //文字に傾斜はありますか？
	map<int,int>							m_mapEraseDotNum;	//ドット抜けを削除数設定？
	map<int,bool>							m_mapIsWhite;		//白文字検査
	map<int,double>							m_mapLenTh;			//文字認識長さ
	CMap<int,int,CSize,CSize&>				m_mapCharPixels;	//領域ごとの文字サイズ
	CArray<CharElement>						m_aryCharElement;	//文字特徴配列(学習データ）
	map<DWORD,CharElement>					m_mapPrevElement;	//最新の文字列認識結果を記録
	//線幅に関して使用するのはSizeDblの"cx"のみ"cy"は予約とします。
	CMap<LONG,LONG,SizeDbl,SizeDbl&>		m_mapAryIndexWidth; //固定文字幅線幅指定配列のMap情報 Key:MAKELONG(nIndex,nRnum)
	CMap<LONG,LONG,SizeDbl,SizeDbl&>		m_mapAryLineWidth;	//線幅指定配列のMap情報 Key:MAKELONG(tChar,nRnum)
	CMap<LONG,LONG,double,double>			m_mapAryMinWidth;	//最小文字幅結果map情報
	CMap<LONG,LONG,BOOL,BOOL>				m_mapIsNumber;		//指定文字は数値ですか？Key:MAKELONG( nRnum(領域番号) (nCnum) )
	CMap<LONG,LONG,BOOL,BOOL>				m_mapIsAlpha;		//指定文字はアルファベットですか？Key:MAKELONG( nRnum(領域番号) (nCnum) )

	CheckResult m_ckResult; //検査結果
	int m_nAnum;			//平均取得用撮影回数
	bool m_isIniFile;		//IniFileは必要ですか？
	DWORD m_dwWaitTime;		//キャプチャフレーム間隔タイム msec
	BYTE m_byContrastMin;	//コントラスト最小値
	BYTE m_byContrastMax;	//コントラスト最大値

	Dib256 m_dibGray;		//Gray画像
	ArrayPoint m_aryPoint;	//境界線座標配列
public:
	enum{ //Function ID 番号
		 FUNC_RECOGNIZE_CHAR = 1//文字認識
		,FUNC_WHITE				//白文字検査
		,FUNC_SETERASEDOT		//ドット抜け削除数
		,FUNC_DIRTY_CHECK		//汚れ検査
		,FUNC_INCLINATION		//傾斜
		,FUNC_SET_COMMON		//検査領域ごとの文字種を分けるかどうか
		,FUNC_SET_CHECK_RECT	//検査領域の設定
		,FUNC_READ_SAMPLE		//サンプル画像読み込み
		,FUNC_READ_LEARNING_DATA//学習データの読み込み
		,FUNC_DRAWRESULT		//認識文字列の描画
		,FUNC_SETCHARWIDTH		//文字幅指定
		,FUNC_SETCHARINDEXWIDTH //固定文字幅指定
		,FUNC_DEFAULT_CHARWIDTH //標準文字幅指定
		,FUNC_SET_CHAR_SIZE		//領域内文字文字画素数設定
		,FUNC_SET_AVENUM		//平均用撮影回数取得
		,FUNC_SET_CONTRAST		//コントラストの設定
		,FUNC_SET_NUMBER		//数値設定
		,FUNC_SET_ALPHA			//アルファベット設定
		,FUNC_SET_INIFILE		//IniFileは使用しますか
		,FUNC_GET_CHECKSTRING	//指定検査領域の文字列を取得
		,FUNC_GET_MIN_WIDTH		//指定検査領域の文字幅最小値を取得
		,FUNC_TEXTOUT_CHARWIDTH //文字幅設定出力
		,FUNC_SET_LENGTH_TH		//文字認識しきい値設定
		,FUNC_BIN_CHECK_RECT	//検査領域ごとに2値化
		,FUNC_MAX				//コマンド数
	};

public:
	CharRecog(void);
	virtual ~CharRecog(void);
	static CharRecog* GetInstance();	

	CRect GetCheckRectOld(int n){return m_mapCheckRectOld[n];}
	CRect GetCheckRect(int n){CheckRect ck; m_mapCheckRect.Lookup(n,ck); return ck.m_rect;}
	void SetAverage(int nAnum,int nTime){m_nAnum=nAnum;m_dwWaitTime=nTime;} //画像平均パラメータ設定
	DWORD GetWaitTime(){return m_dwWaitTime;}		//キャプチャフレーム間隔タイム取得
	void  SetWaitTime(DWORD dw){m_dwWaitTime=dw;}	//キャプチャフレーム間隔タイム取得
	int  GetAverageNum(){return m_nAnum;} //平均取得用撮影回数取得
	void SetAverageNum(int n){m_nAnum=n;} //平均取得用撮影回数設定
	CSize GetCharPixels(int nRnum);//領域ごとの文字サイズ取得
	void  SetCharPixels(int nRnum,int cx,int cy);//領域ごとの文字サイズ設定
	double GetLengthTh(int nRnum){return ( m_mapLenTh.find(nRnum) != m_mapLenTh.end() ) ? m_mapLenTh[nRnum] : DEFAULT_CHAR_LENGTH;} //文字認識距離の取得
	int  GetEraseDotNum(int nRnum){return (m_mapEraseDotNum.find(nRnum) != m_mapEraseDotNum.end()) ? m_mapEraseDotNum[nRnum] : 0;} //ドット抜け削除数取得
	BOOL IsEraseDot(int nRnum){return (m_mapEraseDotNum.find(nRnum) != m_mapEraseDotNum.end()) ? m_mapEraseDotNum[nRnum]>0 : FALSE;} //ドット抜け削除はしますか？ Default:FALSE
	bool IsWhite(int nRnum){return (m_mapIsWhite.find(nRnum) != m_mapIsWhite.end()) ? m_mapIsWhite[nRnum] : false;}		//白文字の検査をしますか？Default:false
	bool IsDirtyCheck (int nRnum){return (m_mapIsDitrtyCheck.find(nRnum) != m_mapIsDitrtyCheck.end() ) ? m_mapIsDitrtyCheck[nRnum] : true;} //汚れ検査はありますか？ Default:true
	bool IsInclination(int nRnum){return (m_mapIsInclination.find(nRnum) != m_mapIsInclination.end() ) ? m_mapIsInclination[nRnum] : false;} //傾斜文字ですか？ Default:false
	BOOL IsCommon(int nRnum){BOOL is=FALSE; return m_mapCommonChar.Lookup(nRnum,is) ? is : FALSE;} //共通文字ですか？
	void SetLineWidth(TCHAR tChar,int nRnum,double dbCx,double dbCy);//線幅の指定
	void SetLineWidthIndex(int nIndex,int nRnum,double dbCx,double dbCy);//固定文字線幅の指定
	SizeDbl GetLineWidthAll(int nIndex,TCHAR tChar,int nRnum);//総合的な判断による線幅取得	
	void SetMinWidth(TCHAR tChar,int nRnum,double dbWmin);//最小線幅登録
	double GetMinWidth(TCHAR tChar,int nRnum); //最小線幅取得
	void TextOutMinWidth(int nRnum);//検査領域内の存在する文字すべての最小線幅を出力

	void SetNumber(int nRnum,int nCnum);//指定文字数値のみに設定
	BOOL IsNumber(int nRnum,int nCnum); //指定文字は数値ですか
	void SetAlpha(int nRnum,int nCnum);	//指定文字をアルファベットのみの設定に変更
	BOOL IsAlpha(int nRnum,int nCnum);	//指定文字はアルファベットですか
	bool IsIniFile(){return m_isIniFile;}//IniFileは必要ですか？

	SizeDbl GetDefLineWidth(int nRnum);//標準線幅の取得
	void SetContrast(BYTE byMin,BYTE byMax){m_byContrastMin=byMin;m_byContrastMax=byMax;} //コントラスト設定
	BYTE GetContrastMin(){return m_byContrastMin;}//コントラスト最小値取得
	BYTE GetContrastMax(){return m_byContrastMax;}//コントラスト最大値取得

	int  GetCharNum(int nRnum){return m_mapCheckRect[nRnum].m_nCharNum;} //領域番号から文字数取得
	CString GetCharString(int nRnum){return m_mapCheckRect[nRnum].m_str;} //領域番号から固定文字列取得
	//過去の認識結果設定
	void SetPrevElement(int nRnum,int nCnum,CharElement &celem)
	{
		DWORD dwKey = MAKELONG(nCnum,nRnum);
		m_mapPrevElement[dwKey] = celem; 
	}
	//過去の認識結果検索
	CharElement& GetPrevElement(int nRnum,int nCnum)
	{
		DWORD dwKey = MAKELONG(nCnum,nRnum);
		return m_mapPrevElement[dwKey];
	}

	BOOL GetCharImage(CDIB &dib,int nRnum,int nCnum); //指定番号の画像を取得

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

	//文字認識
	CString RecognizeChar(int nCnum,int nThAll);

	//認識結果描画
	BOOL DrawResult(CString& strNumbers,int nX,int nHeight);

private:
	//画像ファイルからデータの取得
	BOOL ReadSample(int nThAll,const CString &strFile,const CString &strFileBin,CString &strNumber);

	//文字特徴から文字種予想
	double GetPredictElement(CharElement &celem,WORD wRnum,WORD wCnum);

	//文字が存在する領域を収集する
	int CollectCharRect(int nRnum,int nTh,CArray<CRect> &m_aryRect,CDIB &dibSrc);

	//1文字分のラベリング実行
	Element* LabelCharElement(int nRnum,CPoint &ptTL,Label &label,int nTh,Dib256 &dibGray,ArrayPoint &aryPoint);

	//ラベリングと境界線から文字の特徴を取得
	CharElement GetCharElement(CRect& rect,Label &label,Element *pElem,TCHAR tchar,WORD wRnum,ArrayPoint &aryPoint);

	//境界線表示オブジェクト作成
	void CreateBorderObject(CPoint &ptTL,ArrayPoint &aryPoint);

	//文字描画結果表示
	BOOL CreateStringResult(CString strName,int x,int y,CString strString,COLORREF color,LOGFONT &lf);

	//特徴量間の距離を取得
	double CalcDelta(CharElement &elem,CharElement &celem,BOOL isDebug=FALSE);

	//認識文字列の取得
	int CollectRecognizeChar(CString strNumber,CStringArray &strAry,CStringArray &strAryR);

	double TopToBtm(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);	//上から下方向の線の太さチェック
	double BtmToTop(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);	//下から上方向の線の太さチェック
	double LeftToRight  (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//左から右方向の線の幅チェック
	double RightToLeft  (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//右から左方向の線の幅チェック 
	double CenterToLeft (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//中央から左
	double CenterToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//中央から右

	//上から下方向に線の太さチェック
	double TopToBtmMin(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs=0);

	//下から上方向に線の太さチェック
	double BtmToTopMin(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL);

	//内側境界線と外側境界線間の距離検査
	SizeDbl CheckInOut(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint);

	//内側境界線と外側境界線間の距離検査
	SizeDbl CheckIn2Out1(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint);

	//上にもう一文字ありますか？
	BOOL IsExpandCharTop(TCHAR tc);

	int GetEdgeNumLeftToRight(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL);//左から右方向へのエッジの数を求める
	int GetEdgeNumRightToLeft(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL);//右から左方向へのエッジの数を求める
	int GetEdgeNumTopToBtm(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs=0);//上から下方向のエッジ数
	int GetEdgeNumBtmToTop(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL);//下から上方向のエッジ数
	
	BOOL CenterLineVCheck(int nRnum,TCHAR tch,Label &label,int nCheck);//縦方向センターラインを横切る線の数をカウントします

	//水平方向線幅ライン収集
	int CollectLineH(AryLine &aryLineH,Label &label);

	//垂直方向線幅ライン収集
	int CollectLineV(AryLine &aryLineV,Label &label);

	//横方向開始ポイント
	BOOL IsInPointH(BYTE *pFind,int x,int nCx,int nCy);

	//横方向終了ポイント
	BOOL IsOutPointH(BYTE *pFind,int x,int nCx,int nCy);

	//縦方向開始ポイント
	BOOL IsInPointV(BYTE *pFind,int y,int nCx,int nCy);

	//縦方向終了ポイント
	BOOL IsOutPointV(BYTE *pFind,int y,int nCx,int nCy);

	//エラーライン描画
	void CreateErrorRect(int nRnum,int nEnum,CRect &rect);
	
	//検査領域枠移動
	void MoveRectObject(int nRnum,CRect rect);

	//境界線上の配列内距離を取得
	int GetIndexLen(CPoint &pt1,CPoint &pt2);

	//最短距離の配列番号を取得
	int GetMinLengthIndex(CPoint &pt);

	//文字の線幅チェック
	bool CheckSize(TCHAR tch,SizeDbl &size,double dbWmin,double dbLen);

	//最小幅を計算、ライン番号の取得
	int GetWminIndex(AryLine *pAryLine,double &dbWmin); 

	//要素チェック
	BOOL CheckElement(CharElement &celem,int nRnum,int nCnum);

	//セクションから要素データの書き込み
	BOOL ReadProfileElement(const CString &strFile,int nRnum,int nCnum,CharElement &elem);

	//セクションに要素データの書き込み
	void WriteProfileElement(const CString &strFile,CharElement &elem,int nCnum);

	//.iniファイルにに文字列の書き込み
	void WriteIniString(const CString &strSection,const CString &strKey,LPCWSTR lpsz,...);

	//.iniファイルから文字列の読み込み
	CString ReadIniString(const CString &strSection,const CString &strKey);

	//上下対称文字の時
	BOOL IsSymmetryUpDown(TCHAR tc);

	//左右対称文字の時
	BOOL IsSymmetryLR(TCHAR tc);

	//シリアライズ
	BOOL Serialize(CArchive& ar,CharElement &elem);

	//文字特徴読み込み　& m_aryCharElementへ追加
	BOOL ReadElement(CString &strFileBin,CStringArray &sAry);

	//削除線幅取得
	double GetDeleteWidth( const CRect &rect,double dbLen);

	void ShowPoint(LPWSTR szName,CPoint &pt,Color color);//Pointの表示

	//文字欠け検査
	CheckNickData CheckNick(int nIndex,Label &label,CharElement &celem,CRect &rect,ArrayPoint &aryPoint,int nRnum,double dbLen,int nTh);

	//領域ごとの2値化
	void BinaryCheckRect(int nCnum,int nRnum,int nTh);

	//領域内のしきい値計算
	int CalcThreshold(int nTh,CDIB &dibSrc,CRect &rcCheck);
};

#endif