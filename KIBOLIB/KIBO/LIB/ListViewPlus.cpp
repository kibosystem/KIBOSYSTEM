// ListViewPlus.cpp : �����t�@�C��
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

//�J���������擾
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


// ListViewPlus ���b�Z�[�W �n���h��

void ListViewPlus::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl &ctrlList = GetListCtrl();
	ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_OWNERDRAWFIXED|LVS_REPORT); //Report View �\��
}

//Click�s���擾
int ListViewPlus::GetHitRow()
{
	CPoint ptMouse;
	::GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);
	UINT unFlag = LVHT_ONITEM;
	return GetListCtrl().HitTest(ptMouse,&unFlag);
}

//�\�񂳂�Ă��܂�
void ListViewPlus::PreSubclassWindow()
{
	CListView::PreSubclassWindow();
}

//�����F�擾
COLORREF ListViewPlus::GetTextColor(int nRow,BOOL isSelect)
{	
	DWORD_PTR dwData = GetListCtrl().GetItemData(nRow);	
	if( dwData != 0 ){//�����F���o�^����Ă��鎞
		return (COLORREF)dwData;
	}
	//�����F���o�^����Ă��Ȃ���
	if( isSelect ){ //�I������Ă��鎞
		return ::GetSysColor(COLOR_HIGHLIGHTTEXT); //�I��F
	}
	return GetListCtrl().GetTextColor(); //Window�w�i�F	
}
//�w�i�F���擾
COLORREF ListViewPlus::GetBackColor(int nRow,BOOL isSelect)
{
	COLORREF color;
	if( m_mapBackColor.Lookup(nRow,color) ){//�w�i�F���o�^����Ă��鎞
		return color;
	}
	if( isSelect ){ //�I������Ă��鎞
		return ::GetSysColor(COLOR_ACTIVECAPTION); //�I��F
	}
	return GetListCtrl().GetBkColor(); //Window�w�i�F
}
