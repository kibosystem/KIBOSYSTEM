#pragma once

#include "afxcview.h"
#include <afxtempl.h>

// ListViewPlus ビュー

class ListViewPlus : public CListView
{
	DECLARE_DYNCREATE(ListViewPlus)
private:
	//背景色は行番号が変わる時使用できません。
	CMap<int,int,COLORREF,COLORREF> m_mapBackColor; //文字色指定 Key<ROW> val<COLORREF>

protected:
	ListViewPlus();          
	virtual ~ListViewPlus();
public:
	int GetHitRow();			//Click行を取得
	int GetColCount();			//カラム数取得	

	COLORREF GetTextColor(int nRow,BOOL isSelect);//文字色取得	
	COLORREF GetBackColor(int nRow,BOOL isSelect);//背景色を取得
	void SetTextColor(int nRow,COLORREF color){//文字色設定	
		GetListCtrl().SetItemData(nRow,color); //文字色をデータとして使用
	}
	void SetBackColor(int nRow,COLORREF color){//文字色設定	
		m_mapBackColor.SetAt(nRow,color);
	}

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();
	virtual void PreSubclassWindow();
};


