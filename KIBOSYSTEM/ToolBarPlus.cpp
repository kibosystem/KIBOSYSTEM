// ToolBarPlus.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KIBOSYSTEM.h"
#include "ToolBarPlus.h"


// ToolBarPlus

IMPLEMENT_DYNAMIC(ToolBarPlus, CMFCToolBar)

ToolBarPlus::ToolBarPlus()
{

}

ToolBarPlus::~ToolBarPlus()
{
}


BEGIN_MESSAGE_MAP(ToolBarPlus, CMFCToolBar)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()



// ToolBarPlus ���b�Z�[�W �n���h���[


BOOL ToolBarPlus::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	static CString s_strName;

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID = (UINT)pNMHDR->idFrom;
	CString strFilePath;
	if (GetFrame()->m_mapKeyFilePath.Lookup(VK_F1 + (UINT)pNMHDR->idFrom - 1, strFilePath)) { //File���w�肳��Ă���ꍇ��
		s_strName = ::GetFileName(strFilePath);
		pTTT->lpszText = (LPTSTR)(LPCTSTR)s_strName;
		pTTT->hinst = AfxGetResourceHandle();
		return(TRUE);
	}
	return FALSE;
}