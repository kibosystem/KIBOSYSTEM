
// KIBOSYSTEMView.h : CKIBOSYSTEMView �N���X�̃C���^�[�t�F�C�X
//

#pragma once


class CKIBOSYSTEMView : public CKIBOView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CKIBOSYSTEMView();
	DECLARE_DYNCREATE(CKIBOSYSTEMView)

// ����
public:
	CKIBOSYSTEMDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CKIBOSYSTEMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnRender(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // KIBOSYSTEMView.cpp �̃f�o�b�O �o�[�W����
inline CKIBOSYSTEMDoc* CKIBOSYSTEMView::GetDocument() const
   { return reinterpret_cast<CKIBOSYSTEMDoc*>(m_pDocument); }
#endif

