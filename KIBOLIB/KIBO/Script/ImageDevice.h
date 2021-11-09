#pragma once

#include "afxmt.h"
#include "ScriptBase.h"
#include "../LIB/DIB.h"
#include "../LIB/DIB256.h"
#include "../LIB/VisualObject.h"

//�J�����^�C�v
enum {
	CAMERA_TYPE_DIRECTX = 1,
	CAMERA_TYPE_BITMAP,
	CAMERA_TYPE_MEMORYMAP,
	CAMERA_TYPE_ICUBE,
	CAMERA_TYPE_XIMEA,
	CAMERA_TYPE_TELI
};



//�摜���͋@��
class ImageDevice : public ScriptBase 
{
public:
	CDIB   m_dib;		//24bit Bitmap ��
	Dib256 m_dib256;	//8bit Bitmap ��
	CCriticalSection m_CSection;
	bool m_isStreatchDraw; //�g��k�������܂����H
	int m_nMode; //�摜�B�e���[�h

public:
	enum{ //Function ID �ԍ�
		 FUNC_CREATE = 1	//�J�����쐬
		,FUNC_ISINITOK		//���������܂������H
		,FUNC_SETDRAWSIZE	//�`��T�C�Y�̐ݒ�
		,FUNC_SETIMAGEPOS	//�J�����\���摜���W�ǉ�
		,FUNC_SETIMAGEMODE	//�摜�L���v�`�����[�h�ύX
		,FUNC_SETTIMER		//�^�C�}�[�ݒ�
		,FUNC_KILLTIMER		//�^�C�}�[�폜
		,FUNC_FROMFILE		//�摜�t�@�C���p�X�w��
		,FUNC_FROMDEVICE	//���͂��J�����֐؂�ւ�
		,FUNC_CAPTURE		//�B��
		,FUNC_SAVEIMAGE		//�摜�ۑ�
		,FUNC_SETEXPOSURE	//�I�����Ԃ̕ύX
		,FUNC_SETGAIN		//�Q�C���̕ύX
		,FUNC_SETCONTRAST	//�R���g���X�g�̕ύX
		,FUNC_SETPPT		//�v���p�e�B�ݒ�
		,FUNC_WIDTH		//�T�C�Y���̎擾
		,FUNC_HEIGHT		//�T�C�Y�����̎擾
		,FUNC_DRAWWIDTH	//�`��T�C�Y���̎擾
		,FUNC_DRAWHEIGHT	//�`��T�C�Y�����̎擾
		,FUNC_MAX			//�R�}���h��
	};

public:
	//�R���X�g���N�^
	ImageDevice();

	//�f�X�g���N�^
	virtual ~ImageDevice();

	static ImageDevice* GetInstance();	

	int GetDrawWidth();  //�\�����擾
	int GetDrawHeight(); //�\�������擾

	//�֐����`�F�b�N
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//���s
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID �ԍ�
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script������
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);
    BOOL Lock()  { return m_CSection.Lock(); }
    BOOL Unlock(){ return m_CSection.Unlock(); }
	bool IsStreatchDraw(){ return m_isStreatchDraw; } //�g��k���`������܂����H
	void SetStreatchDraw(bool b){ m_isStreatchDraw = b; } //�v���t���O�ݒ�		

public: //�I�[�o�[���C�h
	virtual int GetWidth() { return m_dib.GetWidth(); }  //���擾
	virtual int GetHeight() { return m_dib.GetHeight(); } //�����擾
	virtual CDIB&   GetDib(){return m_dib;}		//24bit Bitmap
	virtual Dib256& GetDib256(){return m_dib256;}	//8bit Bitmap
	virtual BOOL Create(int nCx,int nCy){return FALSE;} //�f�o�C�X�I�[�v��
	virtual BOOL IsInitOK(){return FALSE;} //�������͏o���܂������H
	virtual BOOL SetImageMode(int nType); //�摜�B�e���[�h
	virtual BOOL SetDrawSize(int nW,int nH){return FALSE;}  //�`��T�C�Y�ݒ�
	virtual BOOL SetImagePos(int nX,int nY){return FALSE;} //�\���ʒu����
	virtual BOOL SetTimer(int nElapse){return FALSE;}   //�L���v�`���^�C�}�[�J�n
	virtual BOOL KillTimer(){return FALSE;}
	virtual BOOL SetExposure(int nTime){return FALSE;}
	virtual BOOL SetGain(int nVal){return FALSE;}
	virtual BOOL SetContrast(int nVal){return FALSE;}
	virtual BOOL Capture(){return FALSE;}
	virtual BOOL SaveImage(CString &strPath){return FALSE;}	
	virtual BOOL FromFile(CString &strPath){return FALSE;}
	virtual BOOL ToBitmap(CDIB& dib){return FALSE;} 
	virtual CSize GetDrawSize(){CSize sz(0,0); return sz;} 
	virtual BOOL Render(CDC &dcMem, VisualObjectMgr &mgrVO){ return FALSE; } //�����_�����O
	virtual BOOL SetProperty(){return FALSE;} //�v���p�e�B�ݒ�
};
