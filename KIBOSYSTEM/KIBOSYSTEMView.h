
// KIBOSYSTEMView.h : CKIBOSYSTEMView クラスのインターフェイス
//

#pragma once


class CKIBOSYSTEMView : public CKIBOView
{
protected: // シリアル化からのみ作成します。
	CKIBOSYSTEMView();
	DECLARE_DYNCREATE(CKIBOSYSTEMView)

// 属性
public:
	CKIBOSYSTEMDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CKIBOSYSTEMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
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

#ifndef _DEBUG  // KIBOSYSTEMView.cpp のデバッグ バージョン
inline CKIBOSYSTEMDoc* CKIBOSYSTEMView::GetDocument() const
   { return reinterpret_cast<CKIBOSYSTEMDoc*>(m_pDocument); }
#endif

