// FolderDialog.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "FolderDialog.h"


// CFolderDialog

IMPLEMENT_DYNAMIC(CFolderDialog, CFileDialog)

CFolderDialog::CFolderDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_strFolder.Empty();
}

CFolderDialog::~CFolderDialog()
{
}

BEGIN_MESSAGE_MAP(CFolderDialog, CFileDialog)
END_MESSAGE_MAP()

// CFolderDialog メッセージ ハンドラ

BOOL CFolderDialog::OnInitDialog()
{
	CFileDialog::OnInitDialog();	
    CDialog *pParent = (CDialog *)GetParent();
	if (pParent != NULL){
        pParent->SetWindowText(L"フォルダを選択して開くボタンをクリックしてください。");
	}
	return TRUE;
}

//フォルダ選択
void CFolderDialog::OnFolderChange()
{
    CDialog *pParent = (CDialog *)GetParent();
	if (pParent != NULL){
		CPoint point;
		GetCursorPos(&point);
		CRect rect;
		pParent->GetDlgItem(IDOK)->GetWindowRect(&rect);
		// マウスポインタが「開く」ボタンの上にある
		if (point.x >= rect.left && point.x <= rect.right &&
			point.y >= rect.top && point.y <= rect.bottom) {
			if (OnFileNameOK() == FALSE) {
				m_strFolder = GetFolderPath();//終了前にフォルダパスをセット
				// ダイアログを閉じる
				pParent->EndDialog(IDOK);
			}
		}
	}
    CFileDialog::OnFolderChange();
}