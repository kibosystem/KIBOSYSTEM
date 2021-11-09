#pragma once

#ifdef USE_BITMAPCAMERA

//�J�����̑���Ƀr�b�g�}�b�v����摜���擾���܂�
class BitmapCamera : public ImageDevice
{
private:
	CSize m_siDraw; //�`��T�C�Y
	CString m_strFilePath; //Bitmap File Path
public:
	BitmapCamera(void);
	~BitmapCamera(void);

	static BitmapCamera* CreateInstance(); //���̂̍쐬
	static BitmapCamera* GetInstance() { return (BitmapCamera*)ImageDevice::GetInstance(); }
public: //�I�[�o�[���C�h
	BOOL SetTimer(int nElapse);//�L���v�`���^�C�}�[�J�n
	BOOL KillTimer() { return FALSE; }
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