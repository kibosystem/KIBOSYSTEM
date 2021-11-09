
#pragma once

class ButtonProprerty : public CMFCPropertyGridProperty
{
public:
	ButtonProprerty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	virtual ~ButtonProprerty();
	virtual void OnDrawValue(CDC* pDC,CRect rect);
	virtual BOOL OnClickValue(UINT uiMsg,CPoint point);
};


class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// コンストラクション
public:
	CPropertiesWnd();

	void AdjustLayout();

// 属性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	CMFCPropertyGridCtrl m_wndPropList;

protected:
	CPropertiesToolBar m_wndToolBar;

// 実装
public:
	virtual ~CPropertiesWnd();

private:
	void LoadIniFile(CString& strPath, CMFCPropertyGridProperty* pGroup);	
	CString GetName(int nIndex, CString &strPath);//名前取得
	CString GetType(int nIndex, CString &strPath);//種別取得		
	CString GetMin(int nIndex, CString &strPath); //最小値取得
	CString GetMax(int nIndex, CString &strPath); //最大値取得

	int FindName(CString &strFindName, CString &strPath);//名前検索
	BOOL IsTypeInt(int nIndex, CString &strPath); //種別はintですか？

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	void InitPropList();

};

//コマンド一覧読み込み
extern void ReadCommandMenu(CStringArray &aryMenuName);