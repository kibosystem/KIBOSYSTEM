#include "../stdafx.h"
#include <tchar.h>
#include "DXGraphic.h"

DXGraphic::DXGraphic(void)
{
	m_hWnd = NULL;		//描画ウィンドウ
	m_nW = 0;			//Window幅
	m_nH = 0;			//Window高さ
	m_isDeviceLost = false;// デバイスの消失フラグ
	m_pD3D = NULL;		 // Direct3Dインターフェイス
	m_pD3DDevice = NULL; // Direct3DDeviceインターフェイス
	m_pBackBuffer= NULL; // サーフェス
	m_pD3DXFont  = NULL; // D3DXFontインターフェイス

	ZeroMemory(&m_D3DPP		 ,sizeof(D3DPRESENT_PARAMETERS)); //D3DDeviceの設定(現在)
}

DXGraphic::~DXGraphic(void)
{
	if( m_pD3D != NULL ){
		RELEASE(m_pD3D);
	}
	if(m_pD3DDevice != NULL){
		RELEASE(m_pD3DDevice);
	}
	// フォントの処理
	if (m_pD3DXFont != NULL){
		RELEASE(m_pD3DXFont);
	}	
	if(m_pBackBuffer != NULL){	//バックバッファ
		RELEASE(m_pBackBuffer);
	}
	if( m_pSurface != NULL ){		//OffScreen
		RELEASE(m_pSurface);
	}

}

/*-------------------------------------------
	DirectX Graphics初期化
--------------------------------------------*/
HRESULT DXGraphic::InitDXGraphics(HWND hWnd,int nCx,int nCy)
{
	if(m_pD3D == NULL){ //初期化は一度だけ行う
		//Direct3Dオブジェクトの作成
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		m_hWnd = hWnd; //Windowハンドル
		m_nW = nCx; //幅設定
		m_nH = nCy; //高さ設定

		if (m_pD3D == NULL){
			return DXTRACE_ERR(L"InitDXGraphics Direct3DCreate9", E_FAIL);
		}
		//デバイス設定の選択
		UINT unAdapter;
		D3DDEVTYPE typeD3ddev;
		HRESULT hr = SelectD3DDevice(unAdapter, typeD3ddev);
		if (FAILED(hr)){
			return DXTRACE_ERR(L"InitDXGraphics SelectD3DDevice", hr);
		}

		LPDIRECT3DDEVICE9 pD3DDevice = NULL;
		hr = m_pD3D->CreateDevice(unAdapter, typeD3ddev, hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3DPP, &pD3DDevice);
		if (FAILED(hr)){
			return DXTRACE_ERR(L"InitDXGraphics CreateDevice", hr);
		}
		m_pD3DDevice = pD3DDevice; //Direct3DDeviceインターフェイス設定

	    //Back Buffer を取得
	    hr= m_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer);
	    if (FAILED(hr)){
			return DXTRACE_ERR(L"GetBackBuffer Error",hr);
	    }

		//OffscreenSurface の作成
	    if(FAILED(m_pD3DDevice->CreateOffscreenPlainSurface(nCx,nCy,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&m_pSurface,NULL))){
			return DXTRACE_ERR(L"CreateOffscreenPlainSurface Error",hr);
	    }

		// ビューポートの設定
		D3DVIEWPORT9 vp;
		vp.X		= 0;
		vp.Y		= 0;
		vp.Width	= m_D3DPP.BackBufferWidth;
		vp.Height	= m_D3DPP.BackBufferHeight;
		vp.MinZ		= 0.0f;
		vp.MaxZ		= 1.0f;
		hr = pD3DDevice->SetViewport(&vp);
		if (FAILED(hr)){
			return DXTRACE_ERR(L"InitDXGraphics SetViewport", hr);
		}

		//// フォントの準備
		//D3DXFONT_DESC dxFont;
		//dxFont.Height = 16;
		//dxFont.Width  = 0;
		//dxFont.Weight = 0;
		//dxFont.MipLevels = 0;
		//dxFont.Italic = FALSE;
		//dxFont.CharSet= SHIFTJIS_CHARSET;
		//dxFont.OutputPrecision= OUT_DEFAULT_PRECIS;
		//dxFont.Quality= DEFAULT_QUALITY;
		//dxFont.PitchAndFamily= DEFAULT_PITCH || FF_DONTCARE; // ピッチ
		//lstrcpy((LPWSTR)dxFont.FaceName, _T("")); // フォント名

		//hr = D3DXCreateFontIndirect(pD3DDevice, &dxFont, &m_pD3DXFont);
		//if (FAILED(hr)){
		//	return DXTRACE_ERR(L"InitDXGraphics D3DXCreateFontIndirect", hr);
		//}
	}
	return S_OK;
}

// デバイス能力のチェック
HRESULT DXGraphic::CheckDeviceCaps(D3DCAPS9 *Caps)
{
	if(Caps->MaxStreams < 3){				// 頂点ストリーム数
		return DXTRACE_ERR(L"CheckDeviceCaps MaxStreams < 3", E_FAIL);
	}
	if(Caps->MaxPrimitiveCount < 0xFFFF){	// プリミティブ数
		return DXTRACE_ERR(L"CheckDeviceCaps MaxPrimitiveCount < 0xFFFF", E_FAIL);
	}
	if(Caps->MaxVertexIndex < 0xFFFF){		// インデックス数
		return DXTRACE_ERR(L"CheckDeviceCaps MaxVertexIndex < 0xFFFF", E_FAIL);
	}
	return S_OK;
}


//デバイス設定の選択
HRESULT DXGraphic::SelectD3DDevice(UINT &unAdapter, D3DDEVTYPE &typeD3ddev)
{
	LPDIRECT3D9 pD3D = GetD3D(); 

	// アダプタの選択
	unAdapter = D3DADAPTER_DEFAULT;	// プライマリ・ディスプレイ・アダプタ

	// デバイス能力の取得	
	typeD3ddev = D3DDEVTYPE_HAL;	// HALデバイスをチェック
	D3DCAPS9 Caps; // 能力を受け取るD3DCAPS9構造体
	HRESULT hr = pD3D->GetDeviceCaps(unAdapter, typeD3ddev, &Caps);
	if (FAILED(hr) || FAILED(CheckDeviceCaps(&Caps))){
		DXTRACE_ERR(L"SelectD3DDevice CheckDeviceCaps HAL", E_FAIL);

		typeD3ddev = D3DDEVTYPE_REF;	// REFデバイスをチェック
		hr = pD3D->GetDeviceCaps(unAdapter, typeD3ddev, &Caps);
		if (FAILED(hr) || FAILED(CheckDeviceCaps(&Caps))){
			return DXTRACE_ERR(L"SelectD3DDevice CheckDeviceCaps REF", E_FAIL);
		}
	}

	// ディスプレイとバック・バッファのフォーマットの設定
	m_D3DPP.BackBufferCount		= 1;
	m_D3DPP.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	m_D3DPP.hDeviceWindow		= GetWnd();
	m_D3DPP.Windowed			= FALSE;
	m_D3DPP.PresentationInterval= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_D3DPP.BackBufferWidth  = GetWidth();
	m_D3DPP.BackBufferHeight = GetHeight();

	return S_OK;
}

//画面キャプチャ
HRESULT DXGraphic::SaveFrontBuffer(LPCTSTR lpszFileName,D3DXIMAGE_FILEFORMAT format)
{
	HRESULT hr = D3DXSaveSurfaceToFile(lpszFileName,		//保存ファイル名
										format,				//ファイルフォーマット
										m_pBackBuffer,		//保存するサーフェス
										NULL,				//パレット
										NULL);				//保存領域
	if (FAILED(hr)){
		return DXTRACE_ERR(L"SaveFrontBuffert D3DXSaveSurfaceToFile", hr);
	}
	return S_OK;
}

/*--------------------------------------------
	画面の描画処理
--------------------------------------------*/
HRESULT DXGraphic::Render()
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetD3DDevice(); //Direct3DDeviceインターフェイス取得

	// シーンのクリア
	DWORD Flags = D3DCLEAR_TARGET;
	pD3DDevice->Clear(0, NULL, Flags, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	RECT rect;
    SetRect(&rect,0,0,GetWidth(),GetHeight());
	pD3DDevice->StretchRect(m_pSurface,&rect,m_pBackBuffer,&rect,D3DTEXF_NONE);

	////シーンの描画開始
	//if( SUCCEEDED(pD3DDevice->BeginScene()) ){
	//	// フォントの描画
	//	// 文字列表示
	//	RECT rect1 = { 8, 32, 108, 50 }; //描画領域(8, 32)－(108, 50)
	//	m_pD3DXFont->DrawText(NULL,
	//		_T("文字列表示①"),
	//		-1,		// 文字数(NULL終端まで表示)
	//		&rect1,	// 描画領域
	//		DT_LEFT | DT_NOCLIP,       // 左揃え＆クリップしない
	//		D3DCOLOR_XRGB(255, 255, 0)); // 文字色(R255,G255, B0)

	//	// シーンの描画終了
	//	pD3DDevice->EndScene();
	//}
	// シーンの表示
	return pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//画像転送
void DXGraphic::SetDib(CDIB &dib)
{
	int nW = GetWidth();
	int nH = GetHeight();

	CRect rect(0,0,nW,nH); //領域全体のロック
	D3DLOCKED_RECT dRect;

	nW = min(nW,dib.GetWidth());
	nH = min(nH,dib.GetHeight());
	if (m_pSurface->LockRect(&dRect,&rect,D3DLOCK_DISCARD)==D3D_OK){
		BYTE *pDstT = (BYTE *)dRect.pBits; //転送先
		BYTE *pSrcT = dib.GetBits();
		int nLineD = dRect.Pitch;
		int nLineS = dib.GetLineSize();
		for(int y=0;y<nH;y++){
			BYTE *pDst = pDstT + y*nLineD;
			BYTE *pSrc = pSrcT + (nH-y-1)*nLineS;
			for(int x=0;x<nW;x++){
				*pDst++ = *pSrc++; //B
				*pDst++ = *pSrc++; //G
				*pDst++ = *pSrc++; //R
				pDst++;//X				
			}
		}
	}
	m_pSurface->UnlockRect();
}
