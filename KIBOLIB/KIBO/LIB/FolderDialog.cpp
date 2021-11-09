// FolderDialog.cpp : �����t�@�C��
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

// CFolderDialog ���b�Z�[�W �n���h��

BOOL CFolderDialog::OnInitDialog()
{
	CFileDialog::OnInitDialog();	
    CDialog *pParent = (CDialog *)GetParent();
	if (pParent != NULL){
        pParent->SetWindowText(L"�t�H���_��I�����ĊJ���{�^�����N���b�N���Ă��������B");
	}
	return TRUE;
}

//�t�H���_�I��
void CFolderDialog::OnFolderChange()
{
    CDialog *pParent = (CDialog *)GetParent();
	if (pParent != NULL){
		CPoint point;
		GetCursorPos(&point);
		CRect rect;
		pParent->GetDlgItem(IDOK)->GetWindowRect(&rect);
		// �}�E�X�|�C���^���u�J���v�{�^���̏�ɂ���
		if (point.x >= rect.left && point.x <= rect.right &&
			point.y >= rect.top && point.y <= rect.bottom) {
			if (OnFileNameOK() == FALSE) {
				m_strFolder = GetFolderPath();//�I���O�Ƀt�H���_�p�X���Z�b�g
				// �_�C�A���O�����
				pParent->EndDialog(IDOK);
			}
		}
	}
    CFileDialog::OnFolderChange();
}