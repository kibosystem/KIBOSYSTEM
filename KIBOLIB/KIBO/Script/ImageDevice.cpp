#include "../stdafx.h"
#include "../LIB/WinAppPlus.h"
#include "../LIB/functions.h"
#include "ImageDevice.h"
#include "App.h"

static ImageDevice *s_pInstance = NULL; //唯一のクラス実体
ImageDevice* ImageDevice::GetInstance()//実体取得
{
	return s_pInstance;
}

//コンストラクタ
ImageDevice::ImageDevice()
{
	s_pInstance = this;
}

//デストラクタ
ImageDevice::~ImageDevice() 
{
}

static CommandTable s_table[] =
{
	 {ImageDevice::FUNC_CREATE,2,L"Create"}			//カメラ作成
	,{ImageDevice::FUNC_ISINITOK,0,L"IsInitOK"}		//初期化しましたか？
	,{ImageDevice::FUNC_SETDRAWSIZE,2,L"SetDrawSize"}	 //描画サイズの設定
	,{ImageDevice::FUNC_SETIMAGEPOS	 ,2,L"SetImagePosition"}//カメラ表示画像座標追加
	,{ImageDevice::FUNC_SETIMAGEMODE ,1,L"SetImageMode"}	//画像キャプチャモード変更
	,{ImageDevice::FUNC_SETTIMER,1,L"SetTimer"} 	//タイマー設定
	,{ImageDevice::FUNC_KILLTIMER,0,L"KillTimer"}	//タイマー削除
	,{ImageDevice::FUNC_FROMFILE,1,L"FromFile"}	//画像ファイルパス指定
	,{ImageDevice::FUNC_CAPTURE,0,L"Capture"}		//撮像
	,{ImageDevice::FUNC_SAVEIMAGE,1,L"SaveImage"}	//画像保存
	,{ImageDevice::FUNC_SETEXPOSURE,1,L"SetExposure"}//露光時間の変更
	,{ImageDevice::FUNC_SETGAIN,1,L"SetGain"}//ゲインの変更
	,{ImageDevice::FUNC_SETCONTRAST,1,L"SetContrast"}//コントラストの変更
	,{ImageDevice::FUNC_SETPPT, 0, L"SetProperty" }//プロパティ設定
	,{ImageDevice::FUNC_WIDTH,0,L"Width" }	 //サイズ幅の取得
	,{ImageDevice::FUNC_HEIGHT,0,L"Height" }//サイズ高さの取得
	,{ImageDevice::FUNC_DRAWWIDTH,0,L"DrawWidth" }	 //描画サイズ幅の取得
	,{ImageDevice::FUNC_DRAWHEIGHT,0,L"DrawHeight" }//描画サイズ高さの取得
	,{ImageDevice::FUNC_MAX , 0,NULL}		//コマンド数
};

//関数名チェック
STDMETHODIMP ImageDevice::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId)
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//処理実行
STDMETHODIMP ImageDevice::Invoke(DISPID dispIdMember	//Function ID 番号
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script文字列
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr) 
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //引数チェック
		return S_FALSE;
	}		
	switch(dispIdMember){
		case FUNC_CREATE:	//カメラ作成
			{
				int nCx = pDispParams->rgvarg[1].ulVal; //x方向解像度
				int nCy = pDispParams->rgvarg[0].ulVal; //y方向解像度
				if( Create(nCx,nCy) ){
					App *pApp = App::GetInstance();
					//CString strKeyName;
					//strKeyName.Format(L"Name%d",nCnum);
					//CString strPath;
					//strPath.Format(L"%s\\%s", GetApp()->GetAppPath(),g_lpszIniFileName);
					//::WritePrivateProfileString(L"CAMERA", strKeyName,strName,strPath);
					return S_OK;
				}
				::ErrorMsg(L"Camera Size Error %d ✕ %d",nCx,nCy);
			}
			break;
		case FUNC_ISINITOK:	//初期化しましたか？
			{
				if( IsInitOK() ){
					ReturnBool(wFlags,pVarResult,TRUE);
					return S_OK;
				}
			}
			break;
		case FUNC_WIDTH:		//サイズ幅の取得
		{
			int nW = GetWidth();
			ReturnInt(wFlags, pVarResult, nW);
			return S_OK;
		}
			break;
		case FUNC_HEIGHT:		//サイズ高さの取得
		{
			int nH = GetHeight();
			ReturnInt(wFlags, pVarResult, nH);
			return S_OK;
		}
			break;
		case FUNC_DRAWWIDTH:	//描画サイズ幅の取得
		{
			int nW = GetDrawWidth();
			ReturnInt(wFlags, pVarResult, nW);
			return S_OK;
		}
		break;
		case FUNC_DRAWHEIGHT:	//描画サイズ高さの取得
		{
			int nH = GetDrawHeight();
			ReturnInt(wFlags, pVarResult, nH);
			return S_OK;
		}
		break;
		case FUNC_SETDRAWSIZE:	//描画サイズの設定
			{
				int nW = pDispParams->rgvarg[1].intVal; //幅
				int nH = pDispParams->rgvarg[0].intVal; //高さ
				if( SetDrawSize(nW,nH) ){
					return S_OK;
				}
			}
			break;
		case FUNC_SETIMAGEPOS: //カメラ表示画像座標追加
			{
				int nX = pDispParams->rgvarg[1].ulVal; //x座標
				int nY = pDispParams->rgvarg[0].ulVal; //y座標								
				if( SetImagePos(nX,nY) ){
					return S_OK;
				}
			}
			break;
		case FUNC_SETIMAGEMODE:	//画像キャプチャモード変更
			{				
				CString strMode = pDispParams->rgvarg[0].bstrVal; //色モード
				int nType = CDIB::TYPE_COLOR; //Default Type
				if(strMode== _T("RGRAY")){
					nType = CDIB::TYPE_RGRAY;
				}else if(strMode== _T("GGRAY")){
					nType = CDIB::TYPE_GGRAY;
				}else if(strMode== _T("BGRAY")){
					nType = CDIB::TYPE_BGRAY;
				}else if(strMode== _T("R")){
					nType = CDIB::TYPE_R;
				}else if(strMode== _T("G")){
					nType = CDIB::TYPE_G;
				}else if(strMode== _T("B")){
					nType = CDIB::TYPE_B;
				}else if(strMode== _T("GRAY")){
					nType = CDIB::TYPE_GRAY;
				}else if(strMode== _T("Y")){
					nType = CDIB::TYPE_Y;
				}else if(strMode== _T("H")){
					nType = CDIB::TYPE_H;
				}else if(strMode== _T("S")){
					nType = CDIB::TYPE_S;
				}else if(strMode== _T("2")){//2値化
					nType = CDIB::TYPE_BIN;
				}else if(strMode== _T("R-G-B")){//R強調処理
					nType = CDIB::TYPE_ACCENT_R;
				}else if(strMode==_T("MEDIAN")){
					nType = CDIB::TYPE_MEDIAN;
				}
				SetImageMode(nType);
			}
			break;
		case FUNC_SETTIMER: //タイマー設定
			{
				int nIndex = pDispParams->cArgs;
				int nElapse = pDispParams->rgvarg[0].intVal; //タイマー間隔
				if( SetTimer(nElapse) ){ //動画用タイマー
					return S_OK;
				}
			}
			break;
		case FUNC_KILLTIMER://タイマー削除
			{				
				if( KillTimer() ){
					return S_OK;
				}
			}
			break;
		case FUNC_FROMFILE://画像ファイルパス指定
			{
				CString strPath = pDispParams->rgvarg[0].bstrVal;//画像ファイルパス
				if( FromFile(strPath) ){
					return S_OK;
				}
			}
			break;
		case FUNC_CAPTURE:	//撮像
			{
				if( Capture() ){
					return S_OK;
				}
			}
			break;
		case FUNC_SAVEIMAGE://画像保存
			{				
				Lock();
				CString strPath = pDispParams->rgvarg[0].bstrVal;//画像ファイルパス
				SaveImage(strPath);
				Unlock();
				return S_OK;				
			}
			break;
		case FUNC_SETEXPOSURE:	//露光時間の変更
			{
				int nTime = pDispParams->rgvarg[0].ulVal; //露光時間
				if( SetExposure(nTime) ){
					return S_OK;
				}
			}
			break;
		case FUNC_SETGAIN: //ゲインの変更
			{
				int nVal  = pDispParams->rgvarg[0].ulVal; //露光時間 msec
				if( SetGain(nVal) ){
					return S_OK;
				}
			}
			break;
		case FUNC_SETCONTRAST: //コントラストの変更
			{
				int nVal  = pDispParams->rgvarg[0].ulVal; //コントラスト
				if( SetContrast(nVal) ){
					return S_OK;
				}
			}
			break;
		case FUNC_SETPPT:
			{
				if (SetProperty()){
					return S_OK;
				}
			}
			break;
	}
	return S_FALSE;
}

//表示幅取得
int ImageDevice::GetDrawWidth()
{
	CSize size = GetDrawSize();
	return size.cx;
}
//表示高さ取得
int ImageDevice::GetDrawHeight()
{
	CSize size = GetDrawSize();
	return size.cy;
}
//画像撮影モード
BOOL ImageDevice::SetImageMode(int nType)
{
	m_nMode = nType; //画像撮影モード
	return TRUE;
}
