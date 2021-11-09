
// KIBOView.h : CKIBOView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "./LIB/Label.h"
#include "./LIB/VisualObject.h"
#include "KIBO.h"
#include "KiboCriticalSection.h"

class CKIBOView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CKIBOView();
	virtual ~CKIBOView();
	DECLARE_DYNCREATE(CKIBOView)
protected: //�����o�ϐ�
	bool	m_isInitOK;		//�������I�����܂������H
	CDIB	m_dib;			//�\���摜
	CDIB	m_dibScript;	//Script���Ŏg�p����摜
	double  m_dbRateCx;	//���W�\���{�� x ���W
	double  m_dbRateCy;	//���W�\���{�� y ���W
	CLine	m_lineClick; //�N���b�N���C��
	Color	m_lineColor; //�N���b�N���C���F
	KiboCriticalSection m_section; //�N���e�B�J���Z�N�V����
public:

	VisualObjectMgr	m_mgrVO;	//�\���I�u�W�F�N�g�}�l�[�W���[
// ����
public:
	BOOL IsInitOK(){return m_isInitOK;}//�������I�����܂������H

	CDIB& GetDib(){return m_dib;}		//�\���摜�擾
	void SetViewRate(double dbRateX,double dbRateY){ //���W�\���{���ݒ�
		m_dbRateCx = dbRateX;
		m_dbRateCy = dbRateY;
	}
	double GetRateCx(){return m_dbRateCx;}	//���W�\���{�� x ���W�擾
	double GetRateCy(){return m_dbRateCy;}	//���W�\���{�� y ���W�擾
	double GetClickLineLength();//�N���b�N���C���̒������擾
	void   SetClickClineColor(Color col){m_lineColor=col;}//�N���b�N���C���F�ݒ�
// ����
public:	
	//�����p����
	BOOL DoBinary(BYTE byTh); //2�l������
	BOOL SaveBitmap(LPCWSTR lpstrInitialDir); //Bitmap�ۑ�	
	CString SaveCameraImage(LPCWSTR lpstrInitialDir);//�J�����摜�ۑ�

	//�`��v�f���Z�b�g
	BOOL SetElementRect(int nEnum,CString &strName,Color color,int nDx=0,int nDy=0);

	//�v�f�h��Ԃ�
	BOOL PaintElement(int nEnum,BYTE byR,BYTE byG,BYTE byB);

private: //�����֐�		
	CPoint GetScrollPos();//���S�ɃX�N���[���ʒu�擾
	void ScrollBarOff(int nBar,int nSize);//�X�N���[���o�[�\��OFF	
	int  GetScrollMax(int nBar);//�X�N���[���ő�T�C�Y�擾
// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Render(BOOL isDraw);//�S�J���������_�����O

protected:
	virtual void OnInitialUpdate(); // �\�z��ɏ��߂ČĂяo����܂��B
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL DrawMask(CDC* pDC);

// ����
public:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

extern CKIBOView* GetView();
