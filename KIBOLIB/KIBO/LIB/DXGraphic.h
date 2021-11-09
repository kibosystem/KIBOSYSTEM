#pragma once
#include <d3dx9.h>
#include <dxerr9.h>
#include "dib.h"
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxerr9.lib")
#pragma comment(lib,"dxguid.lib")

#define RELEASE(x)	if(x){x->Release();x=NULL;}

class DXGraphic
{
private:
	HWND m_hWnd;		//描画ウィンドウ
	int m_nW;			//Window幅
	int m_nH;			//Window高さ
	bool m_isDeviceLost;//デバイスは消失しましたか？	
// インターフェイス
	LPDIRECT3D9			m_pD3D;			//Direct3Dインターフェイス
	LPDIRECT3DDEVICE9	m_pD3DDevice;	//Direct3DDeviceインターフェイス
public:
	LPDIRECT3DSURFACE9	m_pBackBuffer;	//バックバッファ
	LPDIRECT3DSURFACE9	m_pSurface;		//OffScreen
public:
	LPD3DXFONT m_pD3DXFont;	// D3DXFontインターフェイス
	D3DPRESENT_PARAMETERS	m_D3DPP;		//D3DDeviceの設定
public:
	DXGraphic(void);
	~DXGraphic(void);

	HWND GetWnd(){return m_hWnd;} //描画ウィンドウ取得
	int GetWidth(){return m_nW;}	//Window幅
	int GetHeight(){return m_nH;}	//Window高さ
	BOOL IsDeviceLost(){return m_isDeviceLost;} //デバイスは消失しましたか？
	void SetDeviceLost(bool b){m_isDeviceLost=b;}
	LPDIRECT3D9 GetD3D(){return m_pD3D;}	//Direct3Dインターフェイス取得
	LPDIRECT3DDEVICE9 GetD3DDevice(){return m_pD3DDevice;}	//Direct3DDeviceインターフェイス取得

	HRESULT InitDXGraphics(HWND hWnd,int nCx,int nCy); //DirectX Graphics初期化
	HRESULT SelectD3DDevice(UINT &Adapter, D3DDEVTYPE &Device);//適切なDirect3DDeviceやフォーマットの選択
	HRESULT Render();

	// フォントの処理
	void OnResetDevice(){m_pD3DXFont->OnResetDevice();}

	//画面キャプチャ
	HRESULT SaveFrontBuffer(LPCTSTR lpszFileName,D3DXIMAGE_FILEFORMAT format);

	//画像転送
	void SetDib(CDIB &dib);
private: //内部関数
	HRESULT CheckDeviceCaps(D3DCAPS9 *Caps);							// デバイス能力のチェック
	HRESULT SelectAdapterFormat(UINT Adapter, D3DDEVTYPE Device, D3DDISPLAYMODE dmode);			// ディスプレイとバック・バッファのフォーマットの調査
	HRESULT SelectDisplayMode(UINT Adapter);							// ディスプレイ・モードを調べる
};
