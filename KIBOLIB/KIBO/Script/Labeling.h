#pragma once

#include "ScriptBase.h"
#include "../LIB/DIB.h"
#include "../LIB/Label.h"

class Labeling : public ScriptBase
{
private:
	bool m_isInitOK; //���������܂������H
	CDIB m_dibTmp; //�e���|�����r�b�g�}�b�v
	map<CString,ArrayPoint*> m_mapAryPointPtr;
public:
	Label m_label; //���x�����O�N���X
public:
	//�R�}���h���
	enum{
		FUNC_SETBITMAP = 1			//BITMAP�N���X�ŏ�����
		,FUNC_GETBITMAPSRC		//���x�����O�摜�擾
		,FUNC_GETBITMAPDST		//���x�����O���ʉ摜�擾
		,FUNC_EXEC				//���x�����O���s
		,FUNC_ELEMENTEXEC		//�G�������g�����̃��x�����O
		,FUNC_SORT				//���x�����O���ʃ\�[�g
		,FUNC_OUTPUTRESULT		//���x�����O���ʂ̏o��
		,FUNC_DRAWRECT			//���x�����O�̈�`��
		,FUNC_PAINTELEMENT		//���x�����O�̈�`��
		,FUNC_LOADELEMENT		//�摜�t�@�C���ǂݍ���
		,FUNC_REMOVE_ALL_ELEM	//���x�����O�v�f�폜
		,FUNC_INVERT			//�������]
		,FUNC_EXPAND			//�c��
		,FUNC_GETBORDER			//���E�����o
		,FUNC_DRAWBORDER		//���E���`��
		,FUNC_SELECTBORDER		//���E���ꕔ���o
		,FUNC_SETWIDTHRANGE		//�v�f�̕��w��
		,FUNC_SETHEIGHTRANGE	//�v�f�̍����w��
		,FUNC_COUNT				//���x�����O�v�f���擾
		,FUNC_MAXELEMENT			//�ő�v�f�ԍ��擾
		,FUNC_INELEMENT			//�v�f���̓_�ł����H	
		,FUNC_GX					//�d�S�����W
		,FUNC_GY					//�d�S�����W
		,FUNC_MIN_X				//�ŏ������W
		,FUNC_MIN_Y				//�ŏ������W
		,FUNC_MAX_X				//�ŏ������W
		,FUNC_MAX_Y				//�ŏ������W
		,FUNC_MIN_L				//�w��n�_����̍ŒZ����
		,FUNC_MAX_L				//�w��n�_����̍Œ�����
		,FUNC_WIDTH				//��
		,FUNC_HEIGHT				//����
		,FUNC_PIXELS				//��f��
		,FUNC_BORDER				//���͒�
		,FUNC_R_AVR				//���ϔ��a�擾
		,FUNC_R_MAX				//�ő唼�a�擾
		,FUNC_CIRCLE				//�~�`�x
		,FUNC_MAX				//�R�}���h��
	};
public:
	Labeling(void);
	virtual ~Labeling(void);
	static Labeling* GetInstance();	

	Label& GetLabel() { return m_label; } //���x�����O�N���X�擾
	BOOL IsInitOK(){return m_isInitOK;}//���������܂������H
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

	BOOL SetBitmap(int nTh); //Bitmap�N���X�ŏ�����
	BOOL Init(CDIB &dib,int nTh); //DIB���珉����
	int Exec(int nMin,int nMax,int nLeft,int nTop,int nRight,int nBottom);	//���x�����O���s
	int Exec(int nMin,int nMax);//���x�����O���s	
	int ElementExec(int nEnum,int nMin,int nMax);//�G�������g�����̃��x�����O
	void Sort(int nSnum,BOOL isAscending); //���x�����O���ʃ\�[�g
	void OutputResult(); //���x�����O���ʂ̏o��
	int GetCount();	//���x�����O�v�f���擾
	void DrawElementRect(int nEnum,int nR,int nG,int nB);//���x�����O�̈�`��
	void DrawElementRect(int nEnum,int nR,int nG,int nB,int nDx,int nDy);//���x�����O�̈�`��@�`��̈�����炷��	
	void PaintElement(int nEnum,int nR,int nG,int nB);//���x�����O�v�f�h��Ԃ�	
	void Invert(); //�������]	
	int GetBorderPixels(int nBnum,ArrayPoint &aryPoint);//���͒���Pixel�����擾
	ArrayPoint* GetAryPoint(CString &str); //�_�z��擾
	ArrayPoint* GetAryPoint(int n);		
	void SelectBorder(int nEnum,CString strType,int nW);//Type��Border���o	
	void SetWidthRange(int nWmin, int nWmax); //�v�f���w��	
	void SetHeightRange(int nHmin, int nHmax);//�v�f�����w��
	void CollectPointXmin(int nEnum,ArrayPoint &aryPtXmin,int nYS,int nYE);//�w��Y���W�̍��[�_�����W
	void CollectPointXmax(int nEnum,ArrayPoint &aryPtXmax,int nYS,int nYE);//�w��Y���W�̉E�[�_�����W	
	double Gx(int nEnum);//�d�Sx���W
	double Gy(int nEnum);//�d�Sy���W
	int Pixels(int nEnum);//��f��
	int Xmin(int nEnum);//�ŏ������W	
	int Ymin(int nEnum);//�ŏ������W
	int Xmax(int nEnum);//�ő傘���W
	int Ymax(int nEnum);//�ő備���W	
	int GetMaxElement();//�ő�v�f�ԍ��擾

};
