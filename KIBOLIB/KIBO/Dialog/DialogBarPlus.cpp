// DialogBarPlus.cpp : 実装ファイル
//

#include "../stdafx.h"
//#include "DialogBarPlus.h"
//
//
//// DialogBarPlus
//
//IMPLEMENT_DYNAMIC(DialogBarPlus, CDialogBar)
//
//DialogBarPlus::DialogBarPlus()
//{
//
//}
//
//DialogBarPlus::~DialogBarPlus()
//{
//}
//
////文字列取得
//CString DialogBarPlus::GetText(int nID)
//{
//	CString strText;
//	GetDlgItemText(nID,strText);
//	return strText;
//}
//
////浮動小数点数取得
//double DialogBarPlus::GetDlgItemDbl(int nID)
//{
//	CString strValue = GetText(nID);
//	return _wtof(strValue);
//}
////浮動小数点数設定
//void DialogBarPlus::SetDlgItemDbl(int nID,LPCTSTR lpFormat,double dbVal)
//{
//	CString strValue;
//	strValue.Format(lpFormat,dbVal);
//	SetDlgItemText(nID,strValue);
//}
//
//BEGIN_MESSAGE_MAP(DialogBarPlus, CDialogBar)
//END_MESSAGE_MAP()
//
//
//
//// DialogBarPlus メッセージ ハンドラ
//
//
//void DialogBarPlus::OnUpdateEnable(CCmdUI* pCmdUI)
//{
//	pCmdUI->Enable(TRUE);
//}
