#include "../stdafx.h"
#include <tchar.h>
#include "DXGraphic.h"

DXGraphic::DXGraphic(void)
{
	m_hWnd = NULL;		//�`��E�B���h�E
	m_nW = 0;			//Window��
	m_nH = 0;			//Window����
	m_isDeviceLost = false;// �f�o�C�X�̏����t���O
	m_pD3D = NULL;		 // Direct3D�C���^�[�t�F�C�X
	m_pD3DDevice = NULL; // Direct3DDevice�C���^�[�t�F�C�X
	m_pBackBuffer= NULL; // �T�[�t�F�X
	m_pD3DXFont  = NULL; // D3DXFont�C���^�[�t�F�C�X

	ZeroMemory(&m_D3DPP		 ,sizeof(D3DPRESENT_PARAMETERS)); //D3DDevice�̐ݒ�(����)
}

DXGraphic::~DXGraphic(void)
{
	if( m_pD3D != NULL ){
		RELEASE(m_pD3D);
	}
	if(m_pD3DDevice != NULL){
		RELEASE(m_pD3DDevice);
	}
	// �t�H���g�̏���
	if (m_pD3DXFont != NULL){
		RELEASE(m_pD3DXFont);
	}	
	if(m_pBackBuffer != NULL){	//�o�b�N�o�b�t�@
		RELEASE(m_pBackBuffer);
	}
	if( m_pSurface != NULL ){		//OffScreen
		RELEASE(m_pSurface);
	}

}

/*-------------------------------------------
	DirectX Graphics������
--------------------------------------------*/
HRESULT DXGraphic::InitDXGraphics(HWND hWnd,int nCx,int nCy)
{
	if(m_pD3D == NULL){ //�������͈�x�����s��
		//Direct3D�I�u�W�F�N�g�̍쐬
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		m_hWnd = hWnd; //Window�n���h��
		m_nW = nCx; //���ݒ�
		m_nH = nCy; //�����ݒ�

		if (m_pD3D == NULL){
			return DXTRACE_ERR(L"InitDXGraphics Direct3DCreate9", E_FAIL);
		}
		//�f�o�C�X�ݒ�̑I��
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
		m_pD3DDevice = pD3DDevice; //Direct3DDevice�C���^�[�t�F�C�X�ݒ�

	    //Back Buffer ���擾
	    hr= m_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer);
	    if (FAILED(hr)){
			return DXTRACE_ERR(L"GetBackBuffer Error",hr);
	    }

		//OffscreenSurface �̍쐬
	    if(FAILED(m_pD3DDevice->CreateOffscreenPlainSurface(nCx,nCy,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&m_pSurface,NULL))){
			return DXTRACE_ERR(L"CreateOffscreenPlainSurface Error",hr);
	    }

		// �r���[�|�[�g�̐ݒ�
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

		//// �t�H���g�̏���
		//D3DXFONT_DESC dxFont;
		//dxFont.Height = 16;
		//dxFont.Width  = 0;
		//dxFont.Weight = 0;
		//dxFont.MipLevels = 0;
		//dxFont.Italic = FALSE;
		//dxFont.CharSet= SHIFTJIS_CHARSET;
		//dxFont.OutputPrecision= OUT_DEFAULT_PRECIS;
		//dxFont.Quality= DEFAULT_QUALITY;
		//dxFont.PitchAndFamily= DEFAULT_PITCH || FF_DONTCARE; // �s�b�`
		//lstrcpy((LPWSTR)dxFont.FaceName, _T("")); // �t�H���g��

		//hr = D3DXCreateFontIndirect(pD3DDevice, &dxFont, &m_pD3DXFont);
		//if (FAILED(hr)){
		//	return DXTRACE_ERR(L"InitDXGraphics D3DXCreateFontIndirect", hr);
		//}
	}
	return S_OK;
}

// �f�o�C�X�\�͂̃`�F�b�N
HRESULT DXGraphic::CheckDeviceCaps(D3DCAPS9 *Caps)
{
	if(Caps->MaxStreams < 3){				// ���_�X�g���[����
		return DXTRACE_ERR(L"CheckDeviceCaps MaxStreams < 3", E_FAIL);
	}
	if(Caps->MaxPrimitiveCount < 0xFFFF){	// �v���~�e�B�u��
		return DXTRACE_ERR(L"CheckDeviceCaps MaxPrimitiveCount < 0xFFFF", E_FAIL);
	}
	if(Caps->MaxVertexIndex < 0xFFFF){		// �C���f�b�N�X��
		return DXTRACE_ERR(L"CheckDeviceCaps MaxVertexIndex < 0xFFFF", E_FAIL);
	}
	return S_OK;
}


//�f�o�C�X�ݒ�̑I��
HRESULT DXGraphic::SelectD3DDevice(UINT &unAdapter, D3DDEVTYPE &typeD3ddev)
{
	LPDIRECT3D9 pD3D = GetD3D(); 

	// �A�_�v�^�̑I��
	unAdapter = D3DADAPTER_DEFAULT;	// �v���C�}���E�f�B�X�v���C�E�A�_�v�^

	// �f�o�C�X�\�͂̎擾	
	typeD3ddev = D3DDEVTYPE_HAL;	// HAL�f�o�C�X���`�F�b�N
	D3DCAPS9 Caps; // �\�͂��󂯎��D3DCAPS9�\����
	HRESULT hr = pD3D->GetDeviceCaps(unAdapter, typeD3ddev, &Caps);
	if (FAILED(hr) || FAILED(CheckDeviceCaps(&Caps))){
		DXTRACE_ERR(L"SelectD3DDevice CheckDeviceCaps HAL", E_FAIL);

		typeD3ddev = D3DDEVTYPE_REF;	// REF�f�o�C�X���`�F�b�N
		hr = pD3D->GetDeviceCaps(unAdapter, typeD3ddev, &Caps);
		if (FAILED(hr) || FAILED(CheckDeviceCaps(&Caps))){
			return DXTRACE_ERR(L"SelectD3DDevice CheckDeviceCaps REF", E_FAIL);
		}
	}

	// �f�B�X�v���C�ƃo�b�N�E�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
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

//��ʃL���v�`��
HRESULT DXGraphic::SaveFrontBuffer(LPCTSTR lpszFileName,D3DXIMAGE_FILEFORMAT format)
{
	HRESULT hr = D3DXSaveSurfaceToFile(lpszFileName,		//�ۑ��t�@�C����
										format,				//�t�@�C���t�H�[�}�b�g
										m_pBackBuffer,		//�ۑ�����T�[�t�F�X
										NULL,				//�p���b�g
										NULL);				//�ۑ��̈�
	if (FAILED(hr)){
		return DXTRACE_ERR(L"SaveFrontBuffert D3DXSaveSurfaceToFile", hr);
	}
	return S_OK;
}

/*--------------------------------------------
	��ʂ̕`�揈��
--------------------------------------------*/
HRESULT DXGraphic::Render()
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetD3DDevice(); //Direct3DDevice�C���^�[�t�F�C�X�擾

	// �V�[���̃N���A
	DWORD Flags = D3DCLEAR_TARGET;
	pD3DDevice->Clear(0, NULL, Flags, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	RECT rect;
    SetRect(&rect,0,0,GetWidth(),GetHeight());
	pD3DDevice->StretchRect(m_pSurface,&rect,m_pBackBuffer,&rect,D3DTEXF_NONE);

	////�V�[���̕`��J�n
	//if( SUCCEEDED(pD3DDevice->BeginScene()) ){
	//	// �t�H���g�̕`��
	//	// ������\��
	//	RECT rect1 = { 8, 32, 108, 50 }; //�`��̈�(8, 32)�|(108, 50)
	//	m_pD3DXFont->DrawText(NULL,
	//		_T("������\���@"),
	//		-1,		// ������(NULL�I�[�܂ŕ\��)
	//		&rect1,	// �`��̈�
	//		DT_LEFT | DT_NOCLIP,       // ���������N���b�v���Ȃ�
	//		D3DCOLOR_XRGB(255, 255, 0)); // �����F(R255,G255, B0)

	//	// �V�[���̕`��I��
	//	pD3DDevice->EndScene();
	//}
	// �V�[���̕\��
	return pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//�摜�]��
void DXGraphic::SetDib(CDIB &dib)
{
	int nW = GetWidth();
	int nH = GetHeight();

	CRect rect(0,0,nW,nH); //�̈�S�̂̃��b�N
	D3DLOCKED_RECT dRect;

	nW = min(nW,dib.GetWidth());
	nH = min(nH,dib.GetHeight());
	if (m_pSurface->LockRect(&dRect,&rect,D3DLOCK_DISCARD)==D3D_OK){
		BYTE *pDstT = (BYTE *)dRect.pBits; //�]����
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
