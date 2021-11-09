#pragma once

#include "ScriptBase.h"
#include "CharElement.h"
#include "../LIB/DIB.h"
#include "../LIB/Dib256.h"
#include "../LIB/Label.h"
#include "../LIB/PointDbl.h"
#include "../LIB/GdiPlus.h"

#define MIN_CONVERT		 60  //���x�����O�R���o�[�g�ŏ��l�@Default
#define MAX_CONVERT		195  //���x�����O�R���o�[�g�ő�l�@Default
#define MIN_EDGE_LEN	  5	 //�G�b�W����ŏ�����
#define MIN_CONTOUR_LEN   5  //�Œ���K�v�ȋ��E���̉�f��
#define DEFAULT_CHAR_LENGTH	600.0	//�����F������

#define DEFAULT_CHAR_NUM  0		//�W�������ԍ�
#define DEFAULT_CHECK_SPACE 2

//#define LINECHECK //�����`�F�b�N�f�o�b�O�v���O�����p
//#define CALC_DELTA_CHECK

#ifdef USE_CHARRECOG 

//����������������
class CheckNickData
{
public:
	bool	m_isOK;		//������������
	TCHAR	m_tChar;	//������������
public:
	//�R���X�g���N�^
	CheckNickData()
	{
		m_isOK = true;
		m_tChar= 0;
	}
	//�f�X�g���N�^
	~CheckNickData(){}
};

//�����̈���
class CheckRect
{
public:
	CRect m_rect;		//�����̈�
	int	  m_nCharNum;	//������
	CString m_str;		//�Œ蕶����
public:
	//�R���X�g���N�^
	CheckRect()
	:m_rect(0,0,0,0)
	{
		m_nCharNum = 0;
	}
	//�f�X�g���N�^
	~CheckRect(){}
};

//�������ʃZ�b�g
class CheckResult
{
public:
	CPoint	m_ptTL;	//���㑊�΍��W
	CLine	m_line;//�������ʃ��C��
public:
	CheckResult(){}  //�R���X�g���N�^
	~CheckResult(){} //�f�X�g���N�^
	
	void Clear(){m_line.Clear();} //�f�[�^�N���A
	void SetTL(CPoint &pt){m_ptTL = pt;} //���΍��W�ݒ�
	void SetLine(CLine &l);	//Y�����������ʐݒ�	Color::Orange
	void SetLine(CPoint &ptS,CPoint &ptE);
	void ShowLine();//x�������C���`��
	void ShowMinLine(SizeDbl &size);//�ŏ����C���̕\��
private:
	void ShowLine(LPWSTR szName,CPoint &ptTL,CLine& line,Color color);
};

//����R�}���h
//�d�l�g�ݍ��݂̂��߂����̃N���X
class CharRecog : public ScriptBase
{
private:
	CString m_strLearningDataFile;	//�w�K�f�[�^�t�@�C���p�X
	CMap<int,int,CRect,CRect&>				m_mapCheckRectOld;	//�����ݒ莞�̌����̈�
	CMap<int,int,CheckRect,CheckRect&>		m_mapCheckRect;		//�����̈捷�����
	CMap<int,int,BOOL,BOOL>					m_mapCommonChar;	//���ʕ����̈�
	map<int,bool>							m_mapIsDitrtyCheck;	//���ꌟ�����s���܂����H
	map<int,bool>							m_mapIsInclination; //�����ɌX�΂͂���܂����H
	map<int,int>							m_mapEraseDotNum;	//�h�b�g�������폜���ݒ�H
	map<int,bool>							m_mapIsWhite;		//����������
	map<int,double>							m_mapLenTh;			//�����F������
	CMap<int,int,CSize,CSize&>				m_mapCharPixels;	//�̈悲�Ƃ̕����T�C�Y
	CArray<CharElement>						m_aryCharElement;	//���������z��(�w�K�f�[�^�j
	map<DWORD,CharElement>					m_mapPrevElement;	//�ŐV�̕�����F�����ʂ��L�^
	//�����Ɋւ��Ďg�p����̂�SizeDbl��"cx"�̂�"cy"�͗\��Ƃ��܂��B
	CMap<LONG,LONG,SizeDbl,SizeDbl&>		m_mapAryIndexWidth; //�Œ蕶���������w��z���Map��� Key:MAKELONG(nIndex,nRnum)
	CMap<LONG,LONG,SizeDbl,SizeDbl&>		m_mapAryLineWidth;	//�����w��z���Map��� Key:MAKELONG(tChar,nRnum)
	CMap<LONG,LONG,double,double>			m_mapAryMinWidth;	//�ŏ�����������map���
	CMap<LONG,LONG,BOOL,BOOL>				m_mapIsNumber;		//�w�蕶���͐��l�ł����HKey:MAKELONG( nRnum(�̈�ԍ�) (nCnum) )
	CMap<LONG,LONG,BOOL,BOOL>				m_mapIsAlpha;		//�w�蕶���̓A���t�@�x�b�g�ł����HKey:MAKELONG( nRnum(�̈�ԍ�) (nCnum) )

	CheckResult m_ckResult; //��������
	int m_nAnum;			//���ώ擾�p�B�e��
	bool m_isIniFile;		//IniFile�͕K�v�ł����H
	DWORD m_dwWaitTime;		//�L���v�`���t���[���Ԋu�^�C�� msec
	BYTE m_byContrastMin;	//�R���g���X�g�ŏ��l
	BYTE m_byContrastMax;	//�R���g���X�g�ő�l

	Dib256 m_dibGray;		//Gray�摜
	ArrayPoint m_aryPoint;	//���E�����W�z��
public:
	enum{ //Function ID �ԍ�
		 FUNC_RECOGNIZE_CHAR = 1//�����F��
		,FUNC_WHITE				//����������
		,FUNC_SETERASEDOT		//�h�b�g�����폜��
		,FUNC_DIRTY_CHECK		//���ꌟ��
		,FUNC_INCLINATION		//�X��
		,FUNC_SET_COMMON		//�����̈悲�Ƃ̕�����𕪂��邩�ǂ���
		,FUNC_SET_CHECK_RECT	//�����̈�̐ݒ�
		,FUNC_READ_SAMPLE		//�T���v���摜�ǂݍ���
		,FUNC_READ_LEARNING_DATA//�w�K�f�[�^�̓ǂݍ���
		,FUNC_DRAWRESULT		//�F��������̕`��
		,FUNC_SETCHARWIDTH		//�������w��
		,FUNC_SETCHARINDEXWIDTH //�Œ蕶�����w��
		,FUNC_DEFAULT_CHARWIDTH //�W���������w��
		,FUNC_SET_CHAR_SIZE		//�̈������������f���ݒ�
		,FUNC_SET_AVENUM		//���ϗp�B�e�񐔎擾
		,FUNC_SET_CONTRAST		//�R���g���X�g�̐ݒ�
		,FUNC_SET_NUMBER		//���l�ݒ�
		,FUNC_SET_ALPHA			//�A���t�@�x�b�g�ݒ�
		,FUNC_SET_INIFILE		//IniFile�͎g�p���܂���
		,FUNC_GET_CHECKSTRING	//�w�茟���̈�̕�������擾
		,FUNC_GET_MIN_WIDTH		//�w�茟���̈�̕������ŏ��l���擾
		,FUNC_TEXTOUT_CHARWIDTH //�������ݒ�o��
		,FUNC_SET_LENGTH_TH		//�����F���������l�ݒ�
		,FUNC_BIN_CHECK_RECT	//�����̈悲�Ƃ�2�l��
		,FUNC_MAX				//�R�}���h��
	};

public:
	CharRecog(void);
	virtual ~CharRecog(void);
	static CharRecog* GetInstance();	

	CRect GetCheckRectOld(int n){return m_mapCheckRectOld[n];}
	CRect GetCheckRect(int n){CheckRect ck; m_mapCheckRect.Lookup(n,ck); return ck.m_rect;}
	void SetAverage(int nAnum,int nTime){m_nAnum=nAnum;m_dwWaitTime=nTime;} //�摜���σp�����[�^�ݒ�
	DWORD GetWaitTime(){return m_dwWaitTime;}		//�L���v�`���t���[���Ԋu�^�C���擾
	void  SetWaitTime(DWORD dw){m_dwWaitTime=dw;}	//�L���v�`���t���[���Ԋu�^�C���擾
	int  GetAverageNum(){return m_nAnum;} //���ώ擾�p�B�e�񐔎擾
	void SetAverageNum(int n){m_nAnum=n;} //���ώ擾�p�B�e�񐔐ݒ�
	CSize GetCharPixels(int nRnum);//�̈悲�Ƃ̕����T�C�Y�擾
	void  SetCharPixels(int nRnum,int cx,int cy);//�̈悲�Ƃ̕����T�C�Y�ݒ�
	double GetLengthTh(int nRnum){return ( m_mapLenTh.find(nRnum) != m_mapLenTh.end() ) ? m_mapLenTh[nRnum] : DEFAULT_CHAR_LENGTH;} //�����F�������̎擾
	int  GetEraseDotNum(int nRnum){return (m_mapEraseDotNum.find(nRnum) != m_mapEraseDotNum.end()) ? m_mapEraseDotNum[nRnum] : 0;} //�h�b�g�����폜���擾
	BOOL IsEraseDot(int nRnum){return (m_mapEraseDotNum.find(nRnum) != m_mapEraseDotNum.end()) ? m_mapEraseDotNum[nRnum]>0 : FALSE;} //�h�b�g�����폜�͂��܂����H Default:FALSE
	bool IsWhite(int nRnum){return (m_mapIsWhite.find(nRnum) != m_mapIsWhite.end()) ? m_mapIsWhite[nRnum] : false;}		//�������̌��������܂����HDefault:false
	bool IsDirtyCheck (int nRnum){return (m_mapIsDitrtyCheck.find(nRnum) != m_mapIsDitrtyCheck.end() ) ? m_mapIsDitrtyCheck[nRnum] : true;} //���ꌟ���͂���܂����H Default:true
	bool IsInclination(int nRnum){return (m_mapIsInclination.find(nRnum) != m_mapIsInclination.end() ) ? m_mapIsInclination[nRnum] : false;} //�X�Ε����ł����H Default:false
	BOOL IsCommon(int nRnum){BOOL is=FALSE; return m_mapCommonChar.Lookup(nRnum,is) ? is : FALSE;} //���ʕ����ł����H
	void SetLineWidth(TCHAR tChar,int nRnum,double dbCx,double dbCy);//�����̎w��
	void SetLineWidthIndex(int nIndex,int nRnum,double dbCx,double dbCy);//�Œ蕶�������̎w��
	SizeDbl GetLineWidthAll(int nIndex,TCHAR tChar,int nRnum);//�����I�Ȕ��f�ɂ������擾	
	void SetMinWidth(TCHAR tChar,int nRnum,double dbWmin);//�ŏ������o�^
	double GetMinWidth(TCHAR tChar,int nRnum); //�ŏ������擾
	void TextOutMinWidth(int nRnum);//�����̈���̑��݂��镶�����ׂĂ̍ŏ��������o��

	void SetNumber(int nRnum,int nCnum);//�w�蕶�����l�݂̂ɐݒ�
	BOOL IsNumber(int nRnum,int nCnum); //�w�蕶���͐��l�ł���
	void SetAlpha(int nRnum,int nCnum);	//�w�蕶�����A���t�@�x�b�g�݂̂̐ݒ�ɕύX
	BOOL IsAlpha(int nRnum,int nCnum);	//�w�蕶���̓A���t�@�x�b�g�ł���
	bool IsIniFile(){return m_isIniFile;}//IniFile�͕K�v�ł����H

	SizeDbl GetDefLineWidth(int nRnum);//�W�������̎擾
	void SetContrast(BYTE byMin,BYTE byMax){m_byContrastMin=byMin;m_byContrastMax=byMax;} //�R���g���X�g�ݒ�
	BYTE GetContrastMin(){return m_byContrastMin;}//�R���g���X�g�ŏ��l�擾
	BYTE GetContrastMax(){return m_byContrastMax;}//�R���g���X�g�ő�l�擾

	int  GetCharNum(int nRnum){return m_mapCheckRect[nRnum].m_nCharNum;} //�̈�ԍ����當�����擾
	CString GetCharString(int nRnum){return m_mapCheckRect[nRnum].m_str;} //�̈�ԍ�����Œ蕶����擾
	//�ߋ��̔F�����ʐݒ�
	void SetPrevElement(int nRnum,int nCnum,CharElement &celem)
	{
		DWORD dwKey = MAKELONG(nCnum,nRnum);
		m_mapPrevElement[dwKey] = celem; 
	}
	//�ߋ��̔F�����ʌ���
	CharElement& GetPrevElement(int nRnum,int nCnum)
	{
		DWORD dwKey = MAKELONG(nCnum,nRnum);
		return m_mapPrevElement[dwKey];
	}

	BOOL GetCharImage(CDIB &dib,int nRnum,int nCnum); //�w��ԍ��̉摜���擾

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

	//�����F��
	CString RecognizeChar(int nCnum,int nThAll);

	//�F�����ʕ`��
	BOOL DrawResult(CString& strNumbers,int nX,int nHeight);

private:
	//�摜�t�@�C������f�[�^�̎擾
	BOOL ReadSample(int nThAll,const CString &strFile,const CString &strFileBin,CString &strNumber);

	//�����������當����\�z
	double GetPredictElement(CharElement &celem,WORD wRnum,WORD wCnum);

	//���������݂���̈�����W����
	int CollectCharRect(int nRnum,int nTh,CArray<CRect> &m_aryRect,CDIB &dibSrc);

	//1�������̃��x�����O���s
	Element* LabelCharElement(int nRnum,CPoint &ptTL,Label &label,int nTh,Dib256 &dibGray,ArrayPoint &aryPoint);

	//���x�����O�Ƌ��E�����當���̓������擾
	CharElement GetCharElement(CRect& rect,Label &label,Element *pElem,TCHAR tchar,WORD wRnum,ArrayPoint &aryPoint);

	//���E���\���I�u�W�F�N�g�쐬
	void CreateBorderObject(CPoint &ptTL,ArrayPoint &aryPoint);

	//�����`�挋�ʕ\��
	BOOL CreateStringResult(CString strName,int x,int y,CString strString,COLORREF color,LOGFONT &lf);

	//�����ʊԂ̋������擾
	double CalcDelta(CharElement &elem,CharElement &celem,BOOL isDebug=FALSE);

	//�F��������̎擾
	int CollectRecognizeChar(CString strNumber,CStringArray &strAry,CStringArray &strAryR);

	double TopToBtm(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);	//�ォ�牺�����̐��̑����`�F�b�N
	double BtmToTop(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);	//�����������̐��̑����`�F�b�N
	double LeftToRight  (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//������E�����̐��̕��`�F�b�N
	double RightToLeft  (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//�E���獶�����̐��̕��`�F�b�N 
	double CenterToLeft (ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//�������獶
	double CenterToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin);//��������E

	//�ォ�牺�����ɐ��̑����`�F�b�N
	double TopToBtmMin(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs=0);

	//�����������ɐ��̑����`�F�b�N
	double BtmToTopMin(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL);

	//�������E���ƊO�����E���Ԃ̋�������
	SizeDbl CheckInOut(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint);

	//�������E���ƊO�����E���Ԃ̋�������
	SizeDbl CheckIn2Out1(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint);

	//��ɂ����ꕶ������܂����H
	BOOL IsExpandCharTop(TCHAR tc);

	int GetEdgeNumLeftToRight(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL);//������E�����ւ̃G�b�W�̐������߂�
	int GetEdgeNumRightToLeft(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL);//�E���獶�����ւ̃G�b�W�̐������߂�
	int GetEdgeNumTopToBtm(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs=0);//�ォ�牺�����̃G�b�W��
	int GetEdgeNumBtmToTop(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL);//�����������̃G�b�W��
	
	BOOL CenterLineVCheck(int nRnum,TCHAR tch,Label &label,int nCheck);//�c�����Z���^�[���C�������؂���̐����J�E���g���܂�

	//���������������C�����W
	int CollectLineH(AryLine &aryLineH,Label &label);

	//���������������C�����W
	int CollectLineV(AryLine &aryLineV,Label &label);

	//�������J�n�|�C���g
	BOOL IsInPointH(BYTE *pFind,int x,int nCx,int nCy);

	//�������I���|�C���g
	BOOL IsOutPointH(BYTE *pFind,int x,int nCx,int nCy);

	//�c�����J�n�|�C���g
	BOOL IsInPointV(BYTE *pFind,int y,int nCx,int nCy);

	//�c�����I���|�C���g
	BOOL IsOutPointV(BYTE *pFind,int y,int nCx,int nCy);

	//�G���[���C���`��
	void CreateErrorRect(int nRnum,int nEnum,CRect &rect);
	
	//�����̈�g�ړ�
	void MoveRectObject(int nRnum,CRect rect);

	//���E����̔z����������擾
	int GetIndexLen(CPoint &pt1,CPoint &pt2);

	//�ŒZ�����̔z��ԍ����擾
	int GetMinLengthIndex(CPoint &pt);

	//�����̐����`�F�b�N
	bool CheckSize(TCHAR tch,SizeDbl &size,double dbWmin,double dbLen);

	//�ŏ������v�Z�A���C���ԍ��̎擾
	int GetWminIndex(AryLine *pAryLine,double &dbWmin); 

	//�v�f�`�F�b�N
	BOOL CheckElement(CharElement &celem,int nRnum,int nCnum);

	//�Z�N�V��������v�f�f�[�^�̏�������
	BOOL ReadProfileElement(const CString &strFile,int nRnum,int nCnum,CharElement &elem);

	//�Z�N�V�����ɗv�f�f�[�^�̏�������
	void WriteProfileElement(const CString &strFile,CharElement &elem,int nCnum);

	//.ini�t�@�C���ɂɕ�����̏�������
	void WriteIniString(const CString &strSection,const CString &strKey,LPCWSTR lpsz,...);

	//.ini�t�@�C�����當����̓ǂݍ���
	CString ReadIniString(const CString &strSection,const CString &strKey);

	//�㉺�Ώ̕����̎�
	BOOL IsSymmetryUpDown(TCHAR tc);

	//���E�Ώ̕����̎�
	BOOL IsSymmetryLR(TCHAR tc);

	//�V���A���C�Y
	BOOL Serialize(CArchive& ar,CharElement &elem);

	//���������ǂݍ��݁@& m_aryCharElement�֒ǉ�
	BOOL ReadElement(CString &strFileBin,CStringArray &sAry);

	//�폜�����擾
	double GetDeleteWidth( const CRect &rect,double dbLen);

	void ShowPoint(LPWSTR szName,CPoint &pt,Color color);//Point�̕\��

	//������������
	CheckNickData CheckNick(int nIndex,Label &label,CharElement &celem,CRect &rect,ArrayPoint &aryPoint,int nRnum,double dbLen,int nTh);

	//�̈悲�Ƃ�2�l��
	void BinaryCheckRect(int nCnum,int nRnum,int nTh);

	//�̈���̂������l�v�Z
	int CalcThreshold(int nTh,CDIB &dibSrc,CRect &rcCheck);
};

#endif