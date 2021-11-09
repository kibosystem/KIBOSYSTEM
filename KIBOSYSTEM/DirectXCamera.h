#pragma once

#ifdef USE_DX_CAMERA
#define ID_TIMER_DXCAMERA 112

//�J�����̑���Ƀr�b�g�}�b�v����摜���擾���܂�
class DXCamera : public ImageDevice
{
private:
	CSize m_siDraw; //�`��T�C�Y
	CString m_strFilePath; //Bitmap File Path

	cv::VideoCapture m_cap; //DirectX Camera
	cv::Mat m_frame; //�擾�����t���[��
	cv::UMat  m_gray; //Gray�摜
	
public:
	DXCamera(void);
	~DXCamera(void);

	static DXCamera* CreateInstance(); //���̂̍쐬
	static DXCamera* GetInstance() { return (DXCamera*)ImageDevice::GetInstance(); }

private:
	BOOL Test();

public: //�I�[�o�[���C�h
	virtual BOOL SetExposure(int nTime);
	virtual BOOL SetGain(int nVal);
	BOOL SetTimer(int nElapse);//�L���v�`���^�C�}�[�J�n
	BOOL KillTimer();
	BOOL IsInitOK();				//�������͏o���܂������H
	BOOL SetDrawSize(int nW, int nH); //�`��T�C�Y�ݒ�
	BOOL FromFile(CString &strPath); //�摜�t�@�C���ǂݍ���
	BOOL ToBitmap(CDIB &dib);	//�摜��M
	CSize GetDrawSize();
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //�����_�����O
	BOOL Create(int nCx, int nCy); //�f�o�C�X�ڑ�
	BOOL Capture();				//�摜�擾
};


#endif