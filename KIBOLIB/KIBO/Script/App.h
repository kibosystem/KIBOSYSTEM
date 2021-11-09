#pragma once
#include "ScriptBase.h"
#include "../LIB/color.h"
#include "../LIB/typedef.h"

//�A�v���P�[�V�����A�N�Z�X�N���X
class App : public ScriptBase 
{
private:
	HWND				m_hWnd;	  //���C���E�B���h�E�n���h��
	COLORREF			m_crText; //�����F�̐F
	LOGFONT				m_lf;	  //�t�H���g�f�[�^
	CMapStringToString	m_mapString; //�ėp������z��	
	CRect				m_rect;		 //�ėp�̈�
	ULONGLONG			m_ulCount; //���Ԍv���J�E���^�[
public:
	HACCEL				m_hAccel; //�A�N�Z�����[�^�L�[
	CString m_strSectionInt;//���W�X�g���l�Z�N�V����
	CString m_strSectionString;//���W�X�g��������Z�N�V����

	StringArrayPlus m_aryText;
public:
	enum{
		 FUNC_WAIT = 1		//�w�莞�ԑ҂�
		,FUNC_CLICK_WAIT	//�}�E�X�N���b�N�҂�
		,FUNC_KEY_WAIT		//�L�[�{�[�h���b�Z�[�W�҂�
		,FUNC_COUNTSTART	//���Ԍv���J�n
		,FUNC_COUNTEND		//���Ԍv���I��
		,FUNC_STARTSCRIPTTIMER  //�X�N���v�g�^�C�}�[�J�n
		,FUNC_STOPSCRIPTTIMER   //�X�N���v�g�^�C�}�[��~
		,FUNC_SET_REPEAT_SCRIPT //�J��Ԃ��X�N���v�g�t�@�C���ݒ�
		,FUNC_SETWINDOWTEXT		//���C��Window Caption�ύX
		,FUNC_DRAWRECT		//��`�`�� 10
		,FUNC_DRAWMARK		//�w��}�[�N�`��
		,FUNC_DRAWLINE		//�w�胉�C���`��
		,FUNC_PAINTMASK		//�}�X�N�̈�`��
		,FUNC_RENDER		//�����_�����O
		,FUNC_REMOVEALL		//�S�폜
		,FUNC_REMOVE		//�w��I�u�W�F�N�g�폜
		,FUNC_REMOVEEXCEPT  //�w��I�u�W�F�N�g�ȊO���폜
		,FUNC_SETTEXTCOLOR	//�����F�̐ݒ�
		,FUNC_SETFONTNAME	//Font���̐ݒ�
		,FUNC_SETFONTSIZE	//FontSize�ݒ� 20
		,FUNC_DRAWTEXT		//������ݒ�
		,FUNC_DRAWCHAR		//�����`��
		,FUNC_SETVISIBLE	//�\��ON�EOFF�؂�ւ�
		,FUNC_SETTEXTRECT	//������\���̈�w��
		,FUNC_BINARYIMAGE	//2�l���摜
		,FUNC_SETKEYFILE	//�R�}���h�t�@�C���̃L�[���蓖��
		,FUNC_GETBITMAP		//�w��t�H���_���r�b�g�}�b�v�̎擾
		,FUNC_GETBITMAPNUM	//�w��t�H���_���r�b�g�}�b�v���̎擾
		,FUNC_TEXTOUTPUT	//������o��
		,FUNC_MESSAGE		//���b�Z�[�W�\�� 30
		,FUNC_SETSTATUSBARTEXT//�X�e�[�^�X�o�[�����Z�b�g
		,FUNC_OUTPUTCLEAR	//������o�̓N���A
		,FUNC_SETVIEWRATE	//�\���{���ݒ�
		,FUNC_GETVIEWRATECX	//�\���{���擾 x���� 
		,FUNC_GETVIEWRATECY	//�\���{���擾 y����
		,FUNC_SHOWWINDOW	//���C���E�B���h�E�\����ԕύX
		,FUNC_MOVEWINDOW	//���C���E�B���h�E�\���T�C�Y�ύX
		,FUNC_VIEWHEIGHT	//���ʏo�͍����ύX	
		,FUNC_VIEWWIDTH		//���ʏo�͕��ύX
		,FUNC_SETTEXT	//�ėp������L��
		,FUNC_GETTEXT	//�ėp������擾
		,FUNC_SETNUM	//�ėp���l�ݒ�
		,FUNC_GETNUM	//�ėp���l�擾
		,FUNC_SETDBL	//�ėp�����ݒ�
		,FUNC_GETDBL	//�ėp�����擾
		,FUNC_CREATE_FOLDER	//�t�H���_�쐬
		,FUNC_SENDMESSAGE	//�R�}���h�Ăяo��
		,FUNC_POSTMESSAGE	//�R�}���h�Ăяo��
		,FUNC_SETRECT		//�̈�ݒ�
		,FUNC_GETTIME		//�}���e�B���f�A�^�C�}�[�擾
		,FUNC_GETVALUE		//�w��l�擾
		,FUNC_SETLINECOLOR	//���F�ݒ�
		,FUNC_GETPRIVATEPROFILEINT   //.ini�t�@�C���l�擾
		,FUNC_WRITEPRIVATEPROFILEINT //.ini�t�@�C���l��������
		,FUNC_GETPRIVATEPROFILEDBL		//.ini�t�@�C��Dbl�l�擾
		,FUNC_WRITEPRIVATEPROFILEDBL	//.ini�t�@�C��Dbl�l��������
		,FUNC_GETPRIVATEPROFILESTRING	//.ini�t�@�C��������擾
		,FUNC_WRITEPRIVATEPROFILESTRING	//.ini�t�@�C�������񏑂�����
		,FUNC_GETPROFILEINT	//���W�X�g���l�擾
		,FUNC_WRITEPROFILEINT	//���W�X�g���l�ݒ�
		,FUNC_GETPROFILEDBL	//���W�X�g���l�����擾
		,FUNC_WRITEPROFILEDBL	//���W�X�g���l�����ݒ�
		,FUNC_GETPROFILESTRING	//���W�X�g���l�擾
		,FUNC_WRITEPROFILESTRING	//���W�X�g���l�ݒ�
		,FUNC_SETINPUTNAME		//GUI���͒l�ύX
		,FUNC_CENTERPOINT	//���S�_�\���E��\��
		,FUNC_ENABLEWINDOW	//Menu���{�^�� Enable/DisEnable
		,FUNC_GETPIXELSIZE	//1��f������̑傫�����擾
		,FUNC_SELECTFOLDER	//�t�H���_�I��
		,FUNC_SELECTBITMAP	//�r�b�g�}�b�v�t�@�C���I��
		,FUNC_EXEC			//EXE���s
		,FUNC_READTEXT		//������S�s�ǂݍ���
		,FUNC_READSTRING	//������P�s�ǂݍ���
		,FUNC_MAX			//�ő吔
	};
	HWND GetWindow(){return m_hWnd;}
	void SetWindow(HWND h){m_hWnd = h;}
	void SetTextColor(COLORREF cr){m_crText=cr;}//�����F�ݒ�
	COLORREF GetTextColor(){return m_crText;}	//�����F�擾
	LOGFONT& GetLogFont(){return m_lf;} //�t�H���g�f�[�^�擾
	void SetRect(int nLeft,int nTop,int nRight,int nBottom){ //�̈�ݒ�
		m_rect.SetRect(nLeft,nTop,nRight,nBottom);
	}
	CRect GetRect(){return m_rect;} //�̈�擾
	double GetPixelSize(double dbPixel);//�P��f������̑傫����mm�P�ʂŎ擾
public:
	App(void);
	virtual ~App(void);

	static App* GetInstance();
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

	CString GetText(const CString &strKey);//������擾
	void SetText(CString &strKey,CString &strVal); //������ݒ�
	int  GetNum(const CString &strKey); //�l�擾	
	void SetNum(CString strKey,int nVal); //�����ݒ�
	void SetNum(int nVal,LPWSTR lpsz,...);//�����ݒ�
	double GetDbl(const CString &strKey);//�����擾 
	double GetDbl(LPWSTR lpsz,...);//�����擾 
	void   SetDbl(CString strKey,double dbVal); //�����ݒ�
	void   SetDbl(double dbVal,LPWSTR lpsz,...); //�����ݒ�
	int GetProfileInt(LPCWSTR strEntry); //���W�X�g���l�擾
	void SetProfileInt(LPCWSTR strEntry,int nVal); //���W�X�g���l�ݒ�
	double GetProfileDbl(LPCWSTR strEntry); //���W�X�g���l�����擾
	void SetProfileDbl(LPCWSTR strEntry,double dbVal); //���W�X�g���l�����ݒ�
	CString GetProfileString(LPCWSTR strEntry);//���W�X�g��������擾
	void SetProfileString(LPCWSTR strEntry,CString& strVal);//���W�X�g��������ݒ�	
	CString GetIniString(LPCTSTR lpszSec,LPCTSTR lpszKey,LPCTSTR lpszDef,LPCTSTR lpszFileName);//.exe�t�@�C�����̕�����擾
	BOOL Wait(DWORD dwTime);//�w�莞�ԑ҂�
	BOOL ClickWait();		//�}�E�X�N���b�N�҂�
	BOOL KeyWait(int nKeyID);//�L�[�{�[�h���b�Z�[�W�҂�	
	void DrawCircle(CString strName, int nLeft, int nTop, int nRight, int nBottom, int nA, int nR, int nG, int nB, BOOL isPaint);//�ȉ~�`��
	void DrawRect(CString strName,int nLeft,int nTop,int nRight,int nBottom,int nA,int nR,int nG,int nB,BOOL isPaint);	//��`�`��	
	void DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nA,int nR,int nG,int nB);//���C���`��
	void DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nR,int nG,int nB);//���C���`��
	void DrawLine(CString strName, int nXS, int nYS, int nXE, int nYE, COLORREF cr);//���C���`��
	void Render();		//�����_�����O
	void RemoveAll();	//�S�폜		
	void SetViewRate(double dbRateX,double dbRateY);	
	void RemoveExcept(LPCTSTR lpszName); //�w��I�u�W�F�N�g�ȊO���폜
	void DrawText(LPCTSTR lpszID,LPCTSTR lpszText,int nX,int nY);
	void Message(int nNo,LPCTSTR lpszText,COLORREF color);	
	int SendMessage(UINT unID,WPARAM wParam=0,LPARAM lParam=0);//���b�Z�[�W����
	void SetVisible(CString &strID,bool isVisible);//�\���؂�ւ�	
	void OutputClear();//������o�̓N���A
	void TextOutput(CString &str);//������o��
	void TextOutput(LPCTSTR lpszText);
protected: //virtual
	virtual BOOL CenterPoint(BOOL isVisible){return FALSE;}
	virtual BOOL EnableWindow(BOOL isEnable){return FALSE;}
};