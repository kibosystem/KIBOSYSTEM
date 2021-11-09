#pragma once

#ifdef USE_BITMAPCAMERA

//カメラの代わりにビットマップから画像を取得します
class BitmapCamera : public ImageDevice
{
private:
	CSize m_siDraw; //描画サイズ
	CString m_strFilePath; //Bitmap File Path
public:
	BitmapCamera(void);
	~BitmapCamera(void);

	static BitmapCamera* CreateInstance(); //実体の作成
	static BitmapCamera* GetInstance() { return (BitmapCamera*)ImageDevice::GetInstance(); }
public: //オーバーライド
	BOOL SetTimer(int nElapse);//キャプチャタイマー開始
	BOOL KillTimer() { return FALSE; }
	BOOL IsInitOK();				//初期化は出来ましたか？
	BOOL SetDrawSize(int nW, int nH); //描画サイズ設定
	BOOL FromFile(CString &strPath); //画像ファイル読み込み
	BOOL ToBitmap(CDIB &dib);	//画像受信
	CSize GetDrawSize();
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //レンダリング
	BOOL Create(int nCx, int nCy); //デバイス接続
	BOOL Capture();				//画像取得
};


#endif