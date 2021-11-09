#include "StdAfx.h"
#include "KIBOSYSTEM.h"
#include "DirectXCamera.h"
#include "Mp4Camera.h"

#ifdef USE_DX_CAMERA

static Mp4Camera *s_pMp4Camera = NULL; //MP4再生


//カメラ画像表示タイマー
static void CALLBACK EXPORT TimerProc(HWND hWnd, UINT nMsg, UINT_PTR unID, DWORD dwTime)
{
	DXCamera *pImageDevice = (DXCamera *)ImageDevice::GetInstance();
	
	if (pImageDevice != NULL) {
		pImageDevice->Capture();

		CKIBOView *pView = GetView();
		pView->Render(TRUE);
	}
}

DXCamera::DXCamera(void)
{
}

DXCamera ::~DXCamera(void)
{
	if (s_pMp4Camera != NULL) { //MP4再生
		delete s_pMp4Camera;
		s_pMp4Camera = NULL;
	}
}
// 実体の作成
DXCamera * DXCamera::CreateInstance()
{
	DXCamera *pDXCamera = new DXCamera;
	return pDXCamera;
}


BOOL DXCamera::SetExposure(int nTime)
{
	return m_cap.set(CV_CAP_PROP_EXPOSURE, nTime);
}
BOOL DXCamera::SetGain(int nVal)
{
	m_cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 0xFF);
	//if (!) {
	//	return m_cap.set(CV_CAP_PROP_GAIN, nVal);
	//}
	return FALSE;
}

//キャプチャタイマー開始
BOOL DXCamera::SetTimer(int nElapse)
{
	::AfxGetMainWnd()->SetTimer(ID_TIMER_DXCAMERA, nElapse, TimerProc);	
	return TRUE;
}

BOOL DXCamera::KillTimer()
{
	return ::AfxGetMainWnd()->KillTimer(ID_TIMER_DXCAMERA);
}

//初期化は出来ましたか？
BOOL DXCamera::IsInitOK()
{
	return m_cap.isOpened();
}
//描画サイズ設定
BOOL DXCamera::SetDrawSize(int nW, int nH)
{
	m_siDraw.cx = nW;
	m_siDraw.cy = nH;
	return TRUE;
}
CSize DXCamera::GetDrawSize()
{
	return m_siDraw;
}

//画像ファイルからビットマップ読み込み
BOOL DXCamera::FromFile(CString &strPath)
{
	CString strExt = strPath.Right(4);
	strExt.MakeLower();
	if (strExt == L".bmp") {
		m_strFilePath = strPath;
		return m_dib.Load(strPath);
	}
	else if (strExt == L".mp4") {
		m_strFilePath = strPath;
		s_pMp4Camera = Mp4Camera::CreateInstance();
	}
	return FALSE;
}
//画像受信
BOOL DXCamera::ToBitmap(CDIB &dib)
{
	dib.Copy(m_dib);
	return TRUE;
}
//レンダリング
BOOL DXCamera::Render(CDC &dcMem, VisualObjectMgr &mgrVO)
{
	if (m_dib.GetWidth() > 0) {
		m_dib.StretchDraw(&dcMem
			, 0
			, 0
			, m_siDraw.cx
			,m_siDraw.cy
		);
		mgrVO.Draw(&dcMem);//VisualObjectの描画
	}
	return TRUE;
}

//デバイス接続
BOOL DXCamera::Create(int nCx, int nCy)
{	
	if (m_cap.open(0)) { //デバイスオープン

		bool isOKW = m_cap.set(CV_CAP_PROP_FRAME_WIDTH, nCx);
		bool isOKH = m_cap.set(CV_CAP_PROP_FRAME_HEIGHT, nCy);
		m_dib.Create(nCx, nCy); //バッファ作成
		return (isOKW && isOKH);
	}
	return FALSE;
}

//画像取得
BOOL DXCamera::Capture()
{
	m_cap.read(m_frame); //DirectX Camera Capture

	int nW = m_frame.cols;
	int nH = m_frame.rows;

	BYTE *pDst = m_dib.GetBits();	
	int nLine = m_dib.GetLineSize();
	BYTE *pSrc = (BYTE *)m_frame.dataend-nLine;

	int nSizeSrc = (int)(m_frame.dataend - m_frame.datastart);
	if (nSizeSrc == m_dib.GetSize()) {
		for (int y = 0; y < nH; y++) { //上下逆転送
			::CopyMemory(pDst, pSrc, nLine);
			pDst += nLine;
			pSrc -= nLine;
		}		
		return TRUE;
	}
	return FALSE;
}

BOOL DXCamera::Test()
{
	try
	{

		cvtColor(m_frame, m_gray, COLOR_RGB2GRAY);

		//UMat  dst = gray.clone();
		static vector< Point2f > corners;
		const int maxCorners = 100;
		const int  blockSize = 10;

		//CV_EXPORTS_W void goodFeaturesToTrack(InputArray image, OutputArray corners,
		//	int maxCorners, double qualityLevel, double minDistance,
		//	InputArray mask = noArray(), int blockSize = 3,
		//	bool useHarrisDetector = false, double k = 0.04);
		double qualityLevel = 0.01;
		double minDistance = 20.0;
		const bool useHarrisDetector = false;
		goodFeaturesToTrack(m_gray, corners, maxCorners, qualityLevel, minDistance, UMat(), blockSize, useHarrisDetector, 0.04);

		for (size_t i = 0; i < corners.size(); i++) {
			circle(m_frame, corners[i], 8, Scalar(255, 255, 0), 2);
		}

		//imshow("src", m_frame);
		//imshow("dst", m_gray);
		//waitKey(0);
		return TRUE;
	}
	catch (const char* str)
	{
		cerr << str << endl;
	}
	return FALSE;
}


#endif




