#include "StdAfx.h"
#include "KIBOSystem.h"
#include "BitmapCamera.h"
#ifdef USE_BITMAPCAMERA

BitmapCamera::BitmapCamera(void)
{

}

BitmapCamera ::~BitmapCamera(void)
{
}
//�L���v�`���^�C�}�[�J�n
BOOL BitmapCamera::SetTimer(int nElapse)
{
	Capture();
	App::GetInstance()->Render();
	return TRUE;
}
//���̂̍쐬
BitmapCamera * BitmapCamera::CreateInstance()
{
	BitmapCamera *pBitmapCamera = new BitmapCamera;
	return pBitmapCamera;
}
//�������͏o���܂������H
BOOL BitmapCamera::IsInitOK()
{
	return (m_dib.GetHandle() != NULL); //�n���h���`�F�b�N
}
//�`��T�C�Y�ݒ�
BOOL BitmapCamera::SetDrawSize(int nW, int nH)
{
	m_siDraw.cx = nW;
	m_siDraw.cy = nH;
	return TRUE;
}
CSize BitmapCamera::GetDrawSize()
{
	return m_siDraw;
}

//�摜�t�@�C������r�b�g�}�b�v�ǂݍ���
BOOL BitmapCamera::FromFile(CString &strPath)
{
	m_strFilePath = strPath;
	BOOL isOK = m_dib.Load(strPath);
	return isOK;
}
//�摜��M
BOOL BitmapCamera::ToBitmap(CDIB &dib)
{
	dib.Copy(m_dib);
	return TRUE;
}
//�����_�����O
BOOL BitmapCamera::Render(CDC &dcMem, VisualObjectMgr &mgrVO)
{
	if (m_dib.GetWidth() > 0) {
		m_dib.StretchDraw(&dcMem
			, 0
			, 0
			, m_siDraw.cx
			,m_siDraw.cy
		);
		mgrVO.Draw(&dcMem);//VisualObject�̕`��
	}
	return TRUE;
}

//�f�o�C�X�ڑ�
BOOL BitmapCamera::Create(int nCx, int nCy)
{
	m_dib.Create(nCx, nCy);
	m_dib.PaintColor(COLOR_GRAY);
	return TRUE;
}

//�摜�擾
BOOL BitmapCamera::Capture()
{
	return m_dib.Load(m_strFilePath);
}
#endif



