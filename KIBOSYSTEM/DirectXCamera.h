#pragma once

#ifdef USE_DX_CAMERA
#define ID_TIMER_DXCAMERA 112

//カメラの代わりにビットマップから画像を取得します
class DXCamera : public ImageDevice
{
private:
	CSize m_siDraw; //描画サイズ
	CString m_strFilePath; //Bitmap File Path

	cv::VideoCapture m_cap; //DirectX Camera
	cv::Mat m_frame; //取得したフレーム
	cv::UMat  m_gray; //Gray画像
	
public:
	DXCamera(void);
	~DXCamera(void);

	static DXCamera* CreateInstance(); //実体の作成
	static DXCamera* GetInstance() { return (DXCamera*)ImageDevice::GetInstance(); }

private:
	BOOL Test();

public: //オーバーライド
	virtual BOOL SetExposure(int nTime);
	virtual BOOL SetGain(int nVal);
	BOOL SetTimer(int nElapse);//キャプチャタイマー開始
	BOOL KillTimer();
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