#include "StdAfx.h"
#include "KIBOSYSTEM.h"
#include "Mp4Camera.h"

//�J�����摜�\���^�C�}�[
static void CALLBACK EXPORT TimerProc(HWND hWnd, UINT nMsg, UINT_PTR unID, DWORD dwTime)
{
	Mp4Camera *pImageDevice = (Mp4Camera *)ImageDevice::GetInstance();

	if (pImageDevice != NULL) {
		pImageDevice->Capture();

		CKIBOView *pView = GetView();
		pView->Render(TRUE);
	}
}

Mp4Camera::Mp4Camera()
{
}
Mp4Camera::~Mp4Camera()
{
}
// ���̂̍쐬
Mp4Camera* Mp4Camera::CreateInstance()
{
	Mp4Camera *pCamera = new Mp4Camera;
	return pCamera;
}

//�L���v�`���^�C�}�[�J�n
BOOL Mp4Camera::SetTimer(int nElapse)
{
	::AfxGetMainWnd()->SetTimer(ID_TIMER_MP4, nElapse, TimerProc);
	return TRUE;
}
BOOL Mp4Camera::KillTimer()
{
	::AfxGetMainWnd()->KillTimer(ID_TIMER_MP4);
	return FALSE; 
}
//�������͏o���܂������H
BOOL Mp4Camera::IsInitOK()
{
	return TRUE;
}
//�`��T�C�Y�ݒ�
BOOL Mp4Camera::SetDrawSize(int nW, int nH)
{
	m_siDraw.cx = nW;
	m_siDraw.cy = nH;
	return TRUE;
}
//�摜�t�@�C���ǂݍ���
BOOL Mp4Camera::FromFile(CString &strPath)
{
	m_strFilePath = strPath;
	cv::String strFilePath(CW2A(strPath.GetBuffer()));

	BOOL isOK = (BOOL)m_cap.open(strFilePath);
	//m_siDraw.cx = (LONG)m_cap.get(CV_CAP_PROP_FRAME_WIDTH);
	//m_siDraw.cy = (LONG)m_cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	return isOK;
}

//�����_�����O
BOOL Mp4Camera::Render(CDC &dcMem, VisualObjectMgr &mgrVO)
{
	if (m_dib.GetWidth() > 0) {
		m_dib.StretchDraw(&dcMem
			, 0
			, 0
			, m_siDraw.cx
			, m_siDraw.cy
		);
		mgrVO.Draw(&dcMem);//VisualObject�̕`��
		return TRUE;
	}
	return FALSE;
}
//�f�o�C�X�ڑ�
BOOL Mp4Camera::Create(int nCx, int nCy)
{
	return m_dib.Create(nCx, nCy); //�o�b�t�@�쐬
}

//�摜�擾
BOOL Mp4Camera::Capture()
{
	m_cap >> m_frame; //1�t���[�������o��

	int nW = m_frame.cols;
	int nH = m_frame.rows;

	BYTE *pDst = m_dib.GetBits();
	int nLine = m_dib.GetLineSize();
	BYTE *pSrc = (BYTE *)m_frame.dataend - nLine;

	int nSizeSrc = (int)(m_frame.dataend - m_frame.datastart);
	if (nSizeSrc == m_dib.GetSize()) {
		for (int y = 0; y < nH; y++) { //�㉺�t�]��
			::CopyMemory(pDst, pSrc, nLine);
			pDst += nLine;
			pSrc -= nLine;
		}
		return TRUE;
	}
	return FALSE;


	return TRUE;
}