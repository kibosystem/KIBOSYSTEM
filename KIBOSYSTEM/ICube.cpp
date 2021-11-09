#include "StdAfx.h"
#include "KIBOSYSTEM.h"
#include "ICube.h"

#ifdef USE_ICUBE

#if defined _M_IX86
	#pragma comment(lib,"ICubeSDK.lib")
#else
	#pragma comment(lib,"ICubeSDK64.lib")
#endif

static LPCTSTR formatlist[] = 
{
	 L"320x240" ,			//0
	 L"640x480" ,			//1
	 L"752x480" ,			//2
	 L"800x600" ,			//3
	 L"1024x768" ,			//4
	 L"1280x1024" ,			//5
	 L"1600x1200" ,			//6
	 L"2048x1536" ,			//7
	 L"2592x1944" , 		//8
	 L"3840x2748" , 		//9
	 NULL 
};

static int formatW[] = 
{
	 320,		//0
	 640,		//1
	 752,		//2
	 800,		//3
	 1024,		//4
	 1280,		//5
	 1600,		//6
	 2048,		//7
	 2592, 		//8
	 3840, 		//9
	 -1
};

extern double GetLineWidthInRect(Dib256 &dib256,int nTh,CRect &rect); //ライン幅計測

//---------------------------------------------------------------------
//
// callback which is used for grabbing raw or RGB24 data from camera.
//
long CALLBACK MyCallbackFunc(BYTE* pSrc, long lSize,PVOID pContext )
{
	ICube *pICube = (ICube*)pContext;
	if (pICube->IsCaptureOn()){
		ICubeData &data = pICube->GetData();
		CDIB &dib = data.m_dib;
		int nLine = dib.GetLineSize();
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();
		BYTE *pDst = dib.GetBits(); //カメラデータコピー先
		//pICube->Lock();
		for (int y = 0; y < nH; y++){
			memcpy(pDst + (nH - y - 1)*nLine, pSrc, nLine);
			pSrc += nLine;
		}
		pICube->ChangeImageMode();
		GetView()->PostMessage(WM_RENDER, TRUE); //レンダリング要求
		//pICube->Unlock();
	}
	return 42;
}

//コンストラクタ
ICubeData::ICubeData(void)
{
	m_nX = 0; //表示x座標
	m_nY = 0; //表示y座標
}
//デストラクタ
ICubeData::~ICubeData(void)
{

}

ICube::ICube(void)
{
	m_nMode = 0; //画像撮影モード
	m_isStreatchDraw = true;
	m_isCaptureOn = FALSE; //カメラキャプチャフラグ
}

ICube::~ICube(void)
{
	int nCnum = 0;
	ICubeSDK_Stop(nCnum);
}

//実体の作成
ICube* ICube::CreateInstance() 
{
	//DLL読み込み
	if (LoadICubeApi("ICubeSDK64.dll") != IC_SUCCESS){//64bit優先
		if (LoadICubeApi("ICubeSDK.dll") != IC_SUCCESS){//失敗したら32bitライブラリ読み込み
			::ErrorMsg(L"Can not find ICubeSDK.dll\n");
			return NULL;
		}
	}
	//実体の作成
	ICube *pICube = new ICube;
	return pICube;
}
//デバイス接続
BOOL ICube::Create(int nCx, int nCy)
{
	int nCnum = 0;
	ICubeData& data = GetData();
	data.m_dib.Create(nCx, nCy); //バッファ作成
	data.m_dib256.Create(nCx, nCy);
	// Retrieving a handle to the camera device 
	int numDevices = ICubeSDK_Init(); //ICube 初期化
	int nStat = ICubeSDK_Open(nCnum); //カメラオープン
	if (nStat != IC_SUCCESS){
		::ErrorMsg(L"Error open Camera %d", nStat);
		return FALSE;
	}
	App *pApp = App::GetInstance();
	pApp->TextOutput(L"ICube Camera");

	int nFormat = 0;
	for (int i = 0; formatW[i]>0; i++){
		if (formatW[i] == nCx){
			nFormat = i;
			CString strMsg;
			strMsg.Format(L"Format %s", formatlist[i]);
			pApp->TextOutput(strMsg);
			break;
		}
	}

	unsigned long nVal = OFF;
	ICubeSDK_SetParamAuto(nCnum, REG_EXPOSURE_TIME, nVal);
	return StartCamera(nFormat);
}

//カメラ番号に対応するカメラデータを取得
ICubeData& ICube::GetData() 
{
	return m_data; //カメラが１台の場合
}
//24bit Bitmap
CDIB& ICube::GetDib()
{
	return m_data.m_dib;	
}
//8bit Bitmap
Dib256& ICube::GetDib256()	
{
	return m_data.m_dib256;
}

//---------------------------------------------------------------------
// 
// ICubeSDK_SetMode() sets the basis format (e.g. 640x480)
// ICubeSDK_SetBinSkip() sets Binning/Skipping on the basis format.
// If ICubeSDK_SetBinSkip->nParameter==0, ICubeSDK_SetBinSkip has
// no effect.
// ICubeSDK_SetResolution sets the Region of interest.
// If roi_prop.bEnabled==0,ICubeSDK_SetResolution has
// no effect.  
// It is not possible to set bin/skip and Roi.
// 
BOOL ICube::StartCamera(int nFormat)
{	
	int nCnum = 0;
	// optional to set format, different from highest format
	int nStat=ICubeSDK_SetMode(nCnum,nFormat); 
	if ( nStat != IC_SUCCESS ){
		::ErrorMsg(L"ICube Error SetMode %s", formatlist[nFormat]);
		return FALSE;
	}

	// optional to set bin skip mode
	nStat=ICubeSDK_SetBinSkip(nCnum,0,1);
	if ( nStat != IC_SUCCESS ){
		::ErrorMsg(L"ICube Error SetBinSkip %d", nStat);
		return FALSE;
	}
	ROI_PROPERTY roi_prop;
	ZeroMemory(&roi_prop,sizeof(ROI_PROPERTY));
	// optional to set region of interest mode
	nStat=ICubeSDK_SetResolution(nCnum,&roi_prop);
	if ( nStat != IC_SUCCESS ){
		::ErrorMsg(L"ICube Error SetRoi %d", nStat);
		return FALSE;
	}
	// necessary, if Callback-Flag==true 
	nStat=ICubeSDK_SetCallback(nCnum,CALLBACK_RGB,MyCallbackFunc,this); //RGB Data取得
	if ( nStat != IC_SUCCESS ){
		::ErrorMsg(L"ICube Error SetCallback %d", nStat);
		return FALSE;
	}

	//nStat=ICubeSDK_SetTrigger(nCnum,TRIG_SW_START);
	//if ( nStat != IC_SUCCESS ){
	//	::ErrorMsg(L"Error SetTrigger %d", nStat);
	//	return FALSE;
	//}

    nStat=ICubeSDK_Start(nCnum,NULL,false,true);
	//nStat=ICubeSDK_Start(nCnum,NULL,true,true);
	if ( nStat != IC_SUCCESS ){
		::ErrorMsg(L"ICube Error Start %d", nStat);
		return FALSE;
	}
	return TRUE;
}

//初期化は出来ましたか？
BOOL ICube::IsInitOK()
{
	ICubeData& data = GetData();
	return TRUE; //ハンドルチェック
}
//描画サイズ設定
BOOL ICube::SetDrawSize(int nW,int nH) 
{
	ICubeData& data = GetData();
	data.m_siDraw.cx = nW;
	data.m_siDraw.cy = nH;
	return TRUE;
}
//描画サイズ取得
CSize ICube::GetDrawSize() 
{
	ICubeData& data = GetData();
	return data.m_siDraw;	
}
//表示位置決定
BOOL ICube::SetImagePos(int nX,int nY)
{
	ICubeData& data = GetData();
	data.m_nX = nX;
	data.m_nY = nY;
	return TRUE;
}
//キャプチャタイマー開始
BOOL ICube::SetTimer(int nElapse)
{
	//int nStat=ICubeSDK_Start(0,NULL,false,true);
	m_isCaptureOn = TRUE;
	return TRUE;
}
//タイマー終了
BOOL ICube::KillTimer()
{
	//::ICubeSDK_Stop(0);
	m_isCaptureOn = FALSE;
	return TRUE;
}

//露光時間の変更
BOOL ICube::SetExposure(int nTime/*msec*/)
{
	int nCnum = 0;
	int nRet = ICubeSDK_SetExposure(nCnum,(float)(nTime));
	if (nRet != IC_SUCCESS){
		PARAM_PROPERTY_f param_prop;
		if (ICubeSDK_GetExposureRange(nCnum, &param_prop) == IC_SUCCESS){
			CString strMsg;
			strMsg.Format(L"Exposure Range %.3f-%.3f", param_prop.nMin, param_prop.nMax);
			App *pApp = App::GetInstance();
			pApp->TextOutput(strMsg);
		}
		return FALSE;
	}
	//float fRet = 0.0;
	//ICubeSDK_GetExposure(nCnum,&fRet);
	return TRUE;
}
//ゲインの変更
BOOL ICube::SetGain(int nVal)
{
	int nCnum = 0;
	if (ICubeSDK_SetCamParameter(nCnum, REG_GAIN, nVal) != IC_SUCCESS){
		PARAM_PROPERTY param_prop;
		if (ICubeSDK_GetCamParameterRange(nCnum, REG_GAIN, &param_prop) == IC_SUCCESS){			
			CString strMsg;
			strMsg.Format(L"Gain Range %u-%u", param_prop.nMin, param_prop.nMax);
			App *pApp = App::GetInstance();
			pApp->TextOutput(strMsg);
		}
		return FALSE;
	}
	return TRUE;
}
//コントラスト設定
BOOL ICube::SetContrast(int nVal)
{
	int nCnum = 0;
	int nRet = ICubeSDK_SetCamParameter(nCnum,REG_CONTRAST,nVal);
	return (nRet == IC_SUCCESS);	
}
//画像の保存
BOOL ICube::SaveImage(CString &strPath)
{
	return m_data.m_dib256.SaveFile(strPath); //画像の保存
}
static CDIB s_dibSave; //Save用Bitmap
BOOL ICube::SaveImage(int nW,int nH,int nRow,int nCol)
{
	CString strPath;
	strPath.Format(L"%s\\%s",theApp.GetAppPath(),g_lpszIniFileName);
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));	
	::GetPrivateProfileString(L"INPUTDATA",L"SAMPLEID",L"",szBuf,sizeof(szBuf),strPath);
	CString strID = szBuf;

	ZeroMemory(szBuf,sizeof(szBuf));
	::GetPrivateProfileString(L"LOG",L"imagepath",L"",szBuf,sizeof(szBuf),strPath);

	CString strFilePath;
	if( nRow < 0 || nCol < 0 ){
		strFilePath.Format(L"%s\\%s.jpg",szBuf,strID);
	}else{
		strFilePath.Format(L"%s\\%s_%d-%d.jpg",szBuf,strID,nRow+1,nCol);
	}


	if( s_dibSave.Create(nW,nH) ){
		CDIB &dib = m_data.m_dib;

		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL);
		CBitmap *pOld = dcMem.SelectObject( s_dibSave.GetBitmap() ); //Viewに描画
		dib.StretchDraw(&dcMem,0,0,nW,nH);
		//後始末
		dcMem.SelectObject(pOld);

		s_dibSave.WriteImage(strFilePath,L"jpg"); //保存
	}
	return TRUE;
}

//画像ファイルからビットマップ読み込み
BOOL ICube::FromFile(CString &strPath)
{
	return m_data.m_dib.Load(strPath);
}
//画像受信
BOOL ICube::ToBitmap(CDIB &dib)	
{
	dib.Copy(m_data.m_dib);
	return TRUE;
}
//画像取得 & Render
BOOL ICube::Capture()
{
	return TRUE;
}

void ICube::ChangeImageMode()
{
	if( m_nMode == CDIB::TYPE_BIN ){ //2値化
		App *pApp = App::GetInstance();
		int nTh = pApp->GetNum(L"しきい値");

		CDIB &dib = GetDib();
		dib.ChangeBinary(nTh);
	}else if(m_nMode == CDIB::TYPE_BGRAY){
		CDIB &dib = GetDib();
		dib.ChangeColor(CDIB::TYPE_BGRAY);
	}
}

//レンダリング
BOOL ICube::Render(CDC &dcMem, VisualObjectMgr &mgrVO)
{
//	Lock();
	ICubeData &data = GetData();
	CDIB& dibSrc = data.m_dib;//描画ビットマップ
	if (IsStreatchDraw()){ //拡大縮小の必要がある時
		dibSrc.StretchDraw(&dcMem
			, data.m_nX
			, data.m_nY
			, data.m_siDraw.cx
			, data.m_siDraw.cy
			);		
		mgrVO.Draw(&dcMem);//VisualObjectの描画
	}
	else{//等倍で描画するとき
		CDC dcSrc; //カメラ画像用DC
		dcSrc.CreateCompatibleDC(&dcMem);
		CBitmap *pSrcOld = dcSrc.SelectObject(dibSrc.GetBitmap());
		mgrVO.Draw(&dcSrc);//カメラ画像へVisualObjectの描画			
		int nSrcX = (dibSrc.GetWidth() - data.m_siDraw.cx) / 2;
		int nSrcY = (dibSrc.GetHeight() - data.m_siDraw.cy) / 2;
		dcMem.BitBlt(0, 0, data.m_siDraw.cx, data.m_siDraw.cy, &dcSrc, nSrcX, nSrcY, SRCCOPY);
		dcSrc.SelectObject(pSrcOld); //後始末
	}
//	Unlock();
	return TRUE;
}
#endif