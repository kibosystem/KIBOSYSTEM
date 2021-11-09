
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList ウィンドウ

class COutputList : public CListBox
{
// コンストラクション
public:
	COutputList();

// 実装
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public OutputWnd
{
// コンストラクション
public:
	COutputWnd();
	virtual ~COutputWnd();
	

// 属性
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutput1;//出力１
	COutputList m_wndOutput2;//出力２
	COutputList m_wndOutput3;//出力３

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

// 実装
public:
	void UpdateFonts();
	virtual void OutputText(int nNo, LPWSTR lpsz, ...);
	virtual void OutputText(LPWSTR lpsz, ...);
	virtual void OutputText(int nNo, CString &str);
	virtual void OutputText(CString &str);
	virtual void Clear();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

