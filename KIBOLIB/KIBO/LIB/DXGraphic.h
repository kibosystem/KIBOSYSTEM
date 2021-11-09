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
	HWND m_hWnd;		//�`��E�B���h�E
	int m_nW;			//Window��
	int m_nH;			//Window����
	bool m_isDeviceLost;//�f�o�C�X�͏������܂������H	
// �C���^�[�t�F�C�X
	LPDIRECT3D9			m_pD3D;			//Direct3D�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9	m_pD3DDevice;	//Direct3DDevice�C���^�[�t�F�C�X
public:
	LPDIRECT3DSURFACE9	m_pBackBuffer;	//�o�b�N�o�b�t�@
	LPDIRECT3DSURFACE9	m_pSurface;		//OffScreen
public:
	LPD3DXFONT m_pD3DXFont;	// D3DXFont�C���^�[�t�F�C�X
	D3DPRESENT_PARAMETERS	m_D3DPP;		//D3DDevice�̐ݒ�
public:
	DXGraphic(void);
	~DXGraphic(void);

	HWND GetWnd(){return m_hWnd;} //�`��E�B���h�E�擾
	int GetWidth(){return m_nW;}	//Window��
	int GetHeight(){return m_nH;}	//Window����
	BOOL IsDeviceLost(){return m_isDeviceLost;} //�f�o�C�X�͏������܂������H
	void SetDeviceLost(bool b){m_isDeviceLost=b;}
	LPDIRECT3D9 GetD3D(){return m_pD3D;}	//Direct3D�C���^�[�t�F�C�X�擾
	LPDIRECT3DDEVICE9 GetD3DDevice(){return m_pD3DDevice;}	//Direct3DDevice�C���^�[�t�F�C�X�擾

	HRESULT InitDXGraphics(HWND hWnd,int nCx,int nCy); //DirectX Graphics������
	HRESULT SelectD3DDevice(UINT &Adapter, D3DDEVTYPE &Device);//�K�؂�Direct3DDevice��t�H�[�}�b�g�̑I��
	HRESULT Render();

	// �t�H���g�̏���
	void OnResetDevice(){m_pD3DXFont->OnResetDevice();}

	//��ʃL���v�`��
	HRESULT SaveFrontBuffer(LPCTSTR lpszFileName,D3DXIMAGE_FILEFORMAT format);

	//�摜�]��
	void SetDib(CDIB &dib);
private: //�����֐�
	HRESULT CheckDeviceCaps(D3DCAPS9 *Caps);							// �f�o�C�X�\�͂̃`�F�b�N
	HRESULT SelectAdapterFormat(UINT Adapter, D3DDEVTYPE Device, D3DDISPLAYMODE dmode);			// �f�B�X�v���C�ƃo�b�N�E�o�b�t�@�̃t�H�[�}�b�g�̒���
	HRESULT SelectDisplayMode(UINT Adapter);							// �f�B�X�v���C�E���[�h�𒲂ׂ�
};
