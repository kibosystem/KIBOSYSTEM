
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList �E�B���h�E

class COutputList : public CListBox
{
// �R���X�g���N�V����
public:
	COutputList();

// ����
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
// �R���X�g���N�V����
public:
	COutputWnd();
	virtual ~COutputWnd();
	

// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutput1;//�o�͂P
	COutputList m_wndOutput2;//�o�͂Q
	COutputList m_wndOutput3;//�o�͂R

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

// ����
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

