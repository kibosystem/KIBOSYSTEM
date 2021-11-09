#pragma once

#ifdef USE_ICUBE

#include "NET_ICube_API.h"

//カメラデータ
class ICubeData
{
public:
	CDIB m_dib;			//24bit Bitmap
	Dib256 m_dib256;	//8bit Bitmap
	CSize m_siDraw; //描画サイズ
	int m_nX; //表示x座標
	int m_nY; //表示y座標
public:
	ICubeData(void);
	~ICubeData(void);
};


//現在同時接続可能なカメラは1台のみです。
//複数台同時接続に対応する場合はメンバを配列に変更してください
class ICube : public ImageDevice
{
private:
	ICubeData m_data; //カメラデータ	
	BOOL m_isCaptureOn; //カメラキャプチャフラグ
public:
	ICube(void);
	~ICube(void);

	BOOL IsCaptureOn(){ return m_isCaptureOn; }
	static ICube* CreateInstance(); //実体の作成
	ICubeData& GetData(); //カメラ番号に対応するカメラデータを取得	
	void ChangeImageMode(); //ImageMode変更

public:
	BOOL StartCamera(int nFormat); //撮影開始
public: //オーバーライド
	CString GetName(){ return L"ICube USB Camera"; } //カメラ名称取得
	CDIB&   GetDib();		//24bit Bitmap
	Dib256& GetDib256();	//8bit Bitmap
	BOOL Create(int nCx,int nCy); //デバイス接続
	BOOL IsInitOK();				//初期化は出来ましたか？
	BOOL SetDrawSize(int nW,int nH); //描画サイズ設定
	CSize GetDrawSize();//描画サイズ取得
	BOOL SetImagePos(int nX,int nY); //表示位置決定
	BOOL SetTimer(int nElapse);				//キャプチャタイマー開始
	BOOL KillTimer();				//タイマー終了
	BOOL SetExposure(int nTime/*msec*/);	//露光時間の変更
	BOOL SetGain(int nVal); //ゲインの変更
	BOOL SetContrast(int nVal);//コントラスト設定
	BOOL SaveImage(CString &strPath);	//画像の保存
	BOOL SaveImage(int nW,int nH,int nRow,int nCol);
	BOOL FromFile(CString &strPath); //画像ファイル読み込み
	BOOL ToBitmap(CDIB &dib);	//画像受信
	BOOL Capture();	//画像取得
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //レンダリング
	virtual int GetWidth() { return GetDib().GetWidth(); }  //幅取得
	virtual int GetHeight(){ return GetDib().GetHeight(); } //高さ取得

};

#endif