#pragma once


#define ID_TIMER_MP4 113

//�J�����̑���ɓ���t�@�C������摜���擾���܂�
class Mp4Camera : public ImageDevice
{
private:
	CSize m_siDraw; //�`��T�C�Y
	CString m_strFilePath; //Mp4 File Path

	cv::VideoCapture m_cap; //DirectX Camera
	cv::Mat m_frame; //�擾�����t���[��
public:
	Mp4Camera(void);
	~Mp4Camera(void);

	static Mp4Camera* CreateInstance(); //���̂̍쐬
	static Mp4Camera* GetInstance() { return (Mp4Camera*)ImageDevice::GetInstance(); }
public: //�I�[�o�[���C�h
	BOOL SetTimer(int nElapse);//�L���v�`���^�C�}�[�J�n
	BOOL KillTimer();
	BOOL IsInitOK();				//�������͏o���܂������H
	BOOL SetDrawSize(int nW, int nH); //�`��T�C�Y�ݒ�
	BOOL FromFile(CString &strPath); //�摜�t�@�C���ǂݍ���
	CSize GetDrawSize() { return m_siDraw; }
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //�����_�����O
	BOOL Create(int nCx, int nCy); //�f�o�C�X�ڑ�
	BOOL Capture();				//�摜�擾
};

