#pragma once


#define ID_TIMER_MP4 113

//カメラの代わりに動画ファイルから画像を取得します
class Mp4Camera : public ImageDevice
{
private:
	CSize m_siDraw; //描画サイズ
	CString m_strFilePath; //Mp4 File Path

	cv::VideoCapture m_cap; //DirectX Camera
	cv::Mat m_frame; //取得したフレーム
public:
	Mp4Camera(void);
	~Mp4Camera(void);

	static Mp4Camera* CreateInstance(); //実体の作成
	static Mp4Camera* GetInstance() { return (Mp4Camera*)ImageDevice::GetInstance(); }
public: //オーバーライド
	BOOL SetTimer(int nElapse);//キャプチャタイマー開始
	BOOL KillTimer();
	BOOL IsInitOK();				//初期化は出来ましたか？
	BOOL SetDrawSize(int nW, int nH); //描画サイズ設定
	BOOL FromFile(CString &strPath); //画像ファイル読み込み
	CSize GetDrawSize() { return m_siDraw; }
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //レンダリング
	BOOL Create(int nCx, int nCy); //デバイス接続
	BOOL Capture();				//画像取得
};

