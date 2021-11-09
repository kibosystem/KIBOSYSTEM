#pragma once

#include "afxmt.h"
#include "ScriptBase.h"
#include "../LIB/DIB.h"
#include "../LIB/DIB256.h"
#include "../LIB/VisualObject.h"

//カメラタイプ
enum {
	CAMERA_TYPE_DIRECTX = 1,
	CAMERA_TYPE_BITMAP,
	CAMERA_TYPE_MEMORYMAP,
	CAMERA_TYPE_ICUBE,
	CAMERA_TYPE_XIMEA,
	CAMERA_TYPE_TELI
};



//画像入力機器
class ImageDevice : public ScriptBase 
{
public:
	CDIB   m_dib;		//24bit Bitmap 仮
	Dib256 m_dib256;	//8bit Bitmap 仮
	CCriticalSection m_CSection;
	bool m_isStreatchDraw; //拡大縮小をしますか？
	int m_nMode; //画像撮影モード

public:
	enum{ //Function ID 番号
		 FUNC_CREATE = 1	//カメラ作成
		,FUNC_ISINITOK		//初期化しましたか？
		,FUNC_SETDRAWSIZE	//描画サイズの設定
		,FUNC_SETIMAGEPOS	//カメラ表示画像座標追加
		,FUNC_SETIMAGEMODE	//画像キャプチャモード変更
		,FUNC_SETTIMER		//タイマー設定
		,FUNC_KILLTIMER		//タイマー削除
		,FUNC_FROMFILE		//画像ファイルパス指定
		,FUNC_FROMDEVICE	//入力をカメラへ切り替え
		,FUNC_CAPTURE		//撮像
		,FUNC_SAVEIMAGE		//画像保存
		,FUNC_SETEXPOSURE	//露光時間の変更
		,FUNC_SETGAIN		//ゲインの変更
		,FUNC_SETCONTRAST	//コントラストの変更
		,FUNC_SETPPT		//プロパティ設定
		,FUNC_WIDTH		//サイズ幅の取得
		,FUNC_HEIGHT		//サイズ高さの取得
		,FUNC_DRAWWIDTH	//描画サイズ幅の取得
		,FUNC_DRAWHEIGHT	//描画サイズ高さの取得
		,FUNC_MAX			//コマンド数
	};

public:
	//コンストラクタ
	ImageDevice();

	//デストラクタ
	virtual ~ImageDevice();

	static ImageDevice* GetInstance();	

	int GetDrawWidth();  //表示幅取得
	int GetDrawHeight(); //表示高さ取得

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
    BOOL Lock()  { return m_CSection.Lock(); }
    BOOL Unlock(){ return m_CSection.Unlock(); }
	bool IsStreatchDraw(){ return m_isStreatchDraw; } //拡大縮小描画をしますか？
	void SetStreatchDraw(bool b){ m_isStreatchDraw = b; } //計測フラグ設定		

public: //オーバーライド
	virtual int GetWidth() { return m_dib.GetWidth(); }  //幅取得
	virtual int GetHeight() { return m_dib.GetHeight(); } //高さ取得
	virtual CDIB&   GetDib(){return m_dib;}		//24bit Bitmap
	virtual Dib256& GetDib256(){return m_dib256;}	//8bit Bitmap
	virtual BOOL Create(int nCx,int nCy){return FALSE;} //デバイスオープン
	virtual BOOL IsInitOK(){return FALSE;} //初期化は出来ましたか？
	virtual BOOL SetImageMode(int nType); //画像撮影モード
	virtual BOOL SetDrawSize(int nW,int nH){return FALSE;}  //描画サイズ設定
	virtual BOOL SetImagePos(int nX,int nY){return FALSE;} //表示位置決定
	virtual BOOL SetTimer(int nElapse){return FALSE;}   //キャプチャタイマー開始
	virtual BOOL KillTimer(){return FALSE;}
	virtual BOOL SetExposure(int nTime){return FALSE;}
	virtual BOOL SetGain(int nVal){return FALSE;}
	virtual BOOL SetContrast(int nVal){return FALSE;}
	virtual BOOL Capture(){return FALSE;}
	virtual BOOL SaveImage(CString &strPath){return FALSE;}	
	virtual BOOL FromFile(CString &strPath){return FALSE;}
	virtual BOOL ToBitmap(CDIB& dib){return FALSE;} 
	virtual CSize GetDrawSize(){CSize sz(0,0); return sz;} 
	virtual BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO){ return FALSE; } //レンダリング
	virtual BOOL SetProperty(){return FALSE;} //プロパティ設定
};
