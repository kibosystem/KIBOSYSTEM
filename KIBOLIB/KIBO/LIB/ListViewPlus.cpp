// ListViewPlus.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "ListViewPlus.h"

// ListViewPlus

IMPLEMENT_DYNCREATE(ListViewPlus, CListView)

ListViewPlus::ListViewPlus()
{
}

ListViewPlus::~ListViewPlus()
{
}

//カラム数を取得
int ListViewPlus::GetColCount()
{
	CHeaderCtrl *pHeader = GetListCtrl().GetHeaderCtrl();
	if( pHeader != NULL){
		return pHeader->GetItemCount();
	}
	return 0;
}


BEGIN_MESSAGE_MAP(ListViewPlus, CListView)
END_MESSAGE_MAP()


// ListViewPlus メッセージ ハンドラ

void ListViewPlus::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl &ctrlList = GetListCtrl();
	ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_OWNERDRAWFIXED|LVS_REPORT); //Report View 表示
}

//Click行を取得
int ListViewPlus::GetHitRow()
{
	CPoint ptMouse;
	::GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);
	UINT unFlag = LVHT_ONITEM;
	return GetListCtrl().HitTest(ptMouse,&unFlag);
}

//予約されています
void ListViewPlus::PreSubclassWindow()
{
	CListView::PreSubclassWindow();
}

//文字色取得
COLORREF ListViewPlus::GetTextColor(int nRow,BOOL isSelect)
{	
	DWORD_PTR dwData = GetListCtrl().GetItemData(nRow);	
	if( dwData != 0 ){//文字色が登録されている時
		return (COLORREF)dwData;
	}
	//文字色が登録されていない時
	if( isSelect ){ //選択されている時
		return ::GetSysColor(COLOR_HIGHLIGHTTEXT); //選択色
	}
	return GetListCtrl().GetTextColor(); //Window背景色	
}
//背景色を取得
COLORREF ListViewPlus::GetBackColor(int nRow,BOOL isSelect)
{
	COLORREF color;
	if( m_mapBackColor.Lookup(nRow,color) ){//背景色が登録されている時
		return color;
	}
	if( isSelect ){ //選択されている時
		return ::GetSysColor(COLOR_ACTIVECAPTION); //選択色
	}
	return GetListCtrl().GetBkColor(); //Window背景色
}
