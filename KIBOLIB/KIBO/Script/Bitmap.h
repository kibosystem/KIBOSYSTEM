#pragma once

#include <vector>
using namespace std;

#include "ScriptBase.h"
#include "../Lib/Dib.h"
#include "../Lib/Dib256.h"
#include "../Lib/DataBuf.h"
#include "../Lib/FindLine.h"
#include "../Lib/FindCircle.h"
#include "../Dialog/HistgramDlg.h"

//�r�b�g�}�b�v�z��
class MapPDib : public CMap<int,int,CDIB*,CDIB*>
{
public:
	MapPDib(){}
	~MapPDib();

	CDIB* GetDibPtr(int nKey); //�r�b�g�}�b�v�N���X�擾	
	void Clear();//�S�r�b�g�}�b�v�폜
};

//24bit Color Bitmap�A�N�Z�X�N���X
class Bitmap24 : public ScriptBase 
{
protected:
	CDIB	 m_dib;		//�r�b�g�}�b�v
	CDIB	 m_dibTmp;	//�f�[�^�ޔ�p�r�b�g�}�b�v
	Dib256  m_dib256;	//256�F�v�Z�p�r�b�g�}�b�v
	IntBuf  m_bufCalc;	//�r�b�g�}�b�v�v�Z�p�o�b�t�@
	BYTE	 m_byTh;		//�������l
	COLORREF m_crLine; //���C���F

	HistgramDlg m_dlgHistgram;//�q�X�g�O�����_�C�A���O
	MapPDib m_mapDibPtr;//�r�b�g�}�b�v�z��
public:
	PointF m_ptS; //���C���X�^�[�g�|�C���g
	PointF m_ptE; //���C���G���h�|�C���g
public:
	enum{
		FUNC_LOAD = 1	//�ǂݍ���
		,FUNC_SAVE		//��������
		,FUNC_SHOW		//�\��
		,FUNC_SETTHRESHOLD	//�������l�ݒ�
		,FUNC_GETTHRESHOLD  //2�l���������l�擾
		,FUNC_SETLINECOLOR	//���C���F�ݒ�
		,FUNC_FROMIMAGEDEVICE//�J�����摜�擾
		,FUNC_IMAGEDEVICEDIFF//�J�����摜��r
		,FUNC_TOIMAGEDEVICE	//�C���[�W�f�o�C�X�։摜���M
		,FUNC_COLORLENGTH	//�F�����ϊ�
		,FUNC_CHANGECOLOR	//�F�ϊ�		
		,FUNC_GRADIENT		//����
		,FUNC_SOBEL			//�֊s���o �c�A��
		,FUNC_LAPLACIAN		//���v���V�A��
		,FUNC_PREWITT		//�֊s���o
		,FUNC_GAMMA			//�K���}�␳
		,FUNC_INVERT		//�e�F���]
		,FUNC_CHANGERANGE	//�R���g���X�g�ő剻
		,FUNC_EXPAND		//�c��
		,FUNC_SHRINK		//���k
		,FUNC_COLLECTBITMAP	//�r�b�g�}�b�v��A�z�z��ɘA���o�^
		,FUNC_SETBITMAP		//�r�b�g�}�b�v��A�z�z��ɕۑ�
		,FUNC_GETBITMAP		//�A�z�z�񂩂�r�b�g�}�b�v�擾
		,FUNC_GETBITMAPCOUNT//�A�z�z�񐔎擾
		,FUNC_DELTADIB		//�r�b�g�}�b�v����
		,FUNC_DIVDIB		//�r�b�g�}�b�v����Z
		,FUNC_AVERAGE		//���ω摜�̍쐬
		,FUNC_DIBSETCLEAR	//�r�b�g�}�b�v�A�z�z��N���A
		,FUNC_HISTGRAM		//�q�X�g�O�����̕\��
		,FUNC_GETBINARYLEVEL//�w��ʐϊ����ɂȂ�Q�l�����x���̎擾
		,FUNC_GETBRIGHTNESS //���邳�͈̔͂����߂�
		,FUNC_MEDIAN		//���f�B�A���t�B���^
		,FUNC_MAPDELTA		//�A�z�z��摜�����쐬
		,FUNC_EDIT			//�r�b�g�}�b�v�ҏW
		,FUNC_CALCTHRESHOOLD//�������l�����v�Z
		,FUNC_SELECTCOLOR	//�F���o
		,FUNC_SELECT		//����Ɖ����I��
		,FUNC_FINDEDGE		//�G�b�W���W���o
		,FUNC_DRAWLINE		//���C���`��
		,FUNC_DRAWBITMAPLINE//�r�b�g�}�b�v���C���`��
		,FUNC_MASKBORDER	//���x�����O�v�f�}�X�N
		,FUNC_CALCWIDTH		//�����v�Z
		,FUNC_CALCHEIGHT	//�������v�Z
		,FUNC_CALCWIDTHDIRECT	//�ŏ������v�Z
		,FUNC_CALCHEIGHTDIRECT	//�ŏ��������v�Z
		, FUNC_RMAX			//R�v�f�ő�l
		, FUNC_RMIN			//R�v�f�ŏ��l
		, FUNC_GMAX			//G�v�f�ő�l
		, FUNC_GMIN			//G�v�f�ŏ��l
		, FUNC_BMAX			//B�v�f�ő�l
		, FUNC_BMIN			//B�v�f�ŏ��l
		,FUNC_DEFECTTEST	//���׌���
		,FUNC_LENSCHECK		//�����Y�`�F�b�N
		,FUNC_BINARYMATCHING//2�l���}�b�`���O
		,FUNC_OUTPUTFILEPATH//FilePath�o��
		,FUNC_OUTPUTFILENAME//FileName�o��
		,FUNC_MAX
	};
public:
	Bitmap24(void);
	virtual ~Bitmap24(void);

	static Bitmap24* GetInstance();

	COLORREF GetLineColor() { return m_crLine; } //���C���F�擾
	void SetLineColor(COLORREF color) { m_crLine = color; } //���C���F�ݒ�
	BYTE GetTh(){return m_byTh;}		//�������l�擾
	void SetTh(BYTE by){m_byTh = by;}	//�������l�ݒ�
	CDIB& GetDib(){return m_dib;} //�r�b�g�}�b�v�擾
	Dib256&  GetDib256(){return m_dib256;}	//256�F�v�Z�p�r�b�g�}�b�v�擾
	CDIB& GetDibTmp(){return m_dibTmp;} //�ꎞ�I�ȃr�b�g�}�b�v�擾
	MapPDib& GetMapDib(){return m_mapDibPtr;} //�r�b�g�}�b�v�z��
	int GetMapCount(){ return (int)m_mapDibPtr.GetCount(); } //�r�b�g�}�b�v�z��
	void FromImageDevice(); //ImageDevice����摜�擾	
	BOOL ImageDeviceDiff(); //ImageDevice�Ƃ̉摜��r
	void ToImageDevice();//�C���[�W�f�o�C�X�։摜���M
	void ShowHistgram(); //�q�X�g�O�����\��		
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

	void AverageFilter(); //���σt�B���^
	void MedianFilter(){ //���f�B�A���t�B���^
		m_dib.MedianFilter();
	}
	void MedianFilter(CRect &rect){ //���f�B�A���t�B���^
		m_dib.MedianFilter(rect);
	}
	void Expand(int nNum);//�c��
	void Shrink(int nNum);//���k		
	void ClearR();//R��������
	void ClearG();//G��������
	void ClearB();//B��������
	void Prewitt(double dbAmp);//�֊s���o
	void Gradient(double dbAmp);//��������
	void Sobel(double dbAmp);//�֊s���o �c����
	void Laplacian(double dbAmp);//���v���V�A��		
	void Gamma(double dbExp);//�K���}�␳
	void DrawLine(int nX1,int nY1,int nX2,int nY2,COLORREF color,int nWidth);//���C���`��	
	void DrawBitmapLine();//m_ptS - m_ptE �Ԃ̃��C���`��
	int GetMaxElement(int nRGB); //R�v�f�ő�l�擾
	int GetMinElement(int nRGB); //R�v�f�ŏ��l�擾
	int GetMaxElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax);//RGB�v�f�ő�l�擾 �̈�w��
	int GetMinElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax);//RGB�v�f�ő�l�擾 �̈�w��	
	void CollectBitmap(CString &strPath); //�w��t�H���_���r�b�g�}�b�v���W
	BOOL SetDib(int nNo);//�w��ԍ��Ƀr�b�g�}�b�v�L��
	CDIB* GetDibPtr(int nNo){return m_mapDibPtr.GetDibPtr(nNo);}
	
	double FindEdge(CRect &rect,int nDirection,int nColor);//�������l���W�擾
	double FindEdgeH(int nTh,int nX,int nY,int nDelta);	//���������G�b�W���o
	double FindEdgeV(int nTh,int nX,int nY,int nDelta);	//���������G�b�W���o
	double CalcWidth(int nX, int  nY, int  nRange, CString& strType); //���擾
	double CalcHeight(int nX, int  nY, int  nRange, CString& strType);//�����擾																	  
	double CalcWidthDirect(int nX, int  nY, int  nRange, PointF &ptL, PointF &ptR);//�ŏ������v�Z
	double CalcHeightDirect(int nX, int  nY, int  nRange, PointF &ptT, PointF &ptB);//�ŏ��������v�Z
protected:	
	virtual BOOL ShowBitmap(){return FALSE;}//�r�b�g�}�b�v�\�����z�֐�

private:
	void Average(CStringArray &strAryPath);//�w��r�b�g�}�b�v�̕��ς��v�Z	
	void And();//�r�b�g�}�b�v�z���AND���v�Z
	void Expand(int nW,int nH,int nLine,int nPad);//�c��
	void Shrink(int nW,int nH,int nLine,int nPad);//���k	
	void SelectColor(double dbLength,BYTE byR,BYTE byG,BYTE byB);//�ߎ��F���o
	int GetBinaryLevel(double dbPer);//�w�芄���̔��l�ŋ敪���邽�߂̂������l���擾
	int CalcThreshold(CString &strColor,int nPixels);//�ʐϔ䂩�炵�����l���v�Z	
	int CalcThreshold();//�����������l�v�Z	
	int GetBrightnessMin(const CRect &rect);//�w��͈͖��邳�ŏ��l�擾	
	int GetBrightnessMax(const CRect &rect);//�w��͈͖��邳�ő�l�擾
	int GetBrightnessRange(const CRect &rect);//�w��͈͖��邳�͈͎擾
	int GetBrightnessDiff(const CRect &rect);//�w��͈͖��邳�����l�擾	
	BYTE Average(BYTE *pByte,int nLineSize);//���ϒl�擾
	void MapDelta(double dbAmp);//�A�z�z��摜�����쐬	
	void SelectLH(double dbLow,double dbHight);//����Ɖ����̑I��	
	void FindCircle(int nXmax,int nYmax,int nRmax);//�~���o	
	BOOL DeltaDib(CDIB *pDib1,CDIB *pDib2);//�r�b�g�}�b�v�̈����Z	
	BOOL DivDib(CDIB *pDib1,CDIB *pDib2);//�r�b�g�}�b�v�̊���Z
	void DefectTest(int nTh,int nRange,int nMin); //���׌���
	void PaintBorder(int nEdge);//���E���h��Ԃ�	
	void MaskBorder(int nEnum);//���x�����O�}�X�N	
	double BinaryMatching(int nTh, double dbX, double dbY, int nMin, int nMax);//2�l���}�b�`���O
};
