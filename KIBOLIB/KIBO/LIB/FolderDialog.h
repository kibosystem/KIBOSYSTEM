#pragma once


// CFolderDialog

class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)
public:
	CString m_strFolder; //選択フォルダ

public:
	CFolderDialog(BOOL bOpenFileDialog = TRUE, // FileOpen に TRUE を、FileSaveAs に FALSE を指定します。
		LPCTSTR lpszDefExt = L"",
		LPCTSTR lpszFileName = L"",
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = L"Folder |*.||",
		CWnd* pParentWnd = NULL);
	virtual ~CFolderDialog();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnFolderChange();
	virtual BOOL OnInitDialog();
};


