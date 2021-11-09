#pragma once

#ifdef USE_ICUBE

#include "NET_ICube_API.h"

//�J�����f�[�^
class ICubeData
{
public:
	CDIB m_dib;			//24bit Bitmap
	Dib256 m_dib256;	//8bit Bitmap
	CSize m_siDraw; //�`��T�C�Y
	int m_nX; //�\��x���W
	int m_nY; //�\��y���W
public:
	ICubeData(void);
	~ICubeData(void);
};


//���ݓ����ڑ��\�ȃJ������1��݂̂ł��B
//�����䓯���ڑ��ɑΉ�����ꍇ�̓����o��z��ɕύX���Ă�������
class ICube : public ImageDevice
{
private:
	ICubeData m_data; //�J�����f�[�^	
	BOOL m_isCaptureOn; //�J�����L���v�`���t���O
public:
	ICube(void);
	~ICube(void);

	BOOL IsCaptureOn(){ return m_isCaptureOn; }
	static ICube* CreateInstance(); //���̂̍쐬
	ICubeData& GetData(); //�J�����ԍ��ɑΉ�����J�����f�[�^���擾	
	void ChangeImageMode(); //ImageMode�ύX

public:
	BOOL StartCamera(int nFormat); //�B�e�J�n
public: //�I�[�o�[���C�h
	CString GetName(){ return L"ICube USB Camera"; } //�J�������̎擾
	CDIB&   GetDib();		//24bit Bitmap
	Dib256& GetDib256();	//8bit Bitmap
	BOOL Create(int nCx,int nCy); //�f�o�C�X�ڑ�
	BOOL IsInitOK();				//�������͏o���܂������H
	BOOL SetDrawSize(int nW,int nH); //�`��T�C�Y�ݒ�
	CSize GetDrawSize();//�`��T�C�Y�擾
	BOOL SetImagePos(int nX,int nY); //�\���ʒu����
	BOOL SetTimer(int nElapse);				//�L���v�`���^�C�}�[�J�n
	BOOL KillTimer();				//�^�C�}�[�I��
	BOOL SetExposure(int nTime/*msec*/);	//�I�����Ԃ̕ύX
	BOOL SetGain(int nVal); //�Q�C���̕ύX
	BOOL SetContrast(int nVal);//�R���g���X�g�ݒ�
	BOOL SaveImage(CString &strPath);	//�摜�̕ۑ�
	BOOL SaveImage(int nW,int nH,int nRow,int nCol);
	BOOL FromFile(CString &strPath); //�摜�t�@�C���ǂݍ���
	BOOL ToBitmap(CDIB &dib);	//�摜��M
	BOOL Capture();	//�摜�擾
	BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO); //�����_�����O
	virtual int GetWidth() { return GetDib().GetWidth(); }  //���擾
	virtual int GetHeight(){ return GetDib().GetHeight(); } //�����擾

};

#endif