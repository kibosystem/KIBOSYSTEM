#pragma once


// CFolderDialog

class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)
public:
	CString m_strFolder; //�I���t�H���_

public:
	CFolderDialog(BOOL bOpenFileDialog = TRUE, // FileOpen �� TRUE ���AFileSaveAs �� FALSE ���w�肵�܂��B
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


