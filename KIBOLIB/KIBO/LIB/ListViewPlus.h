#pragma once

#include "afxcview.h"
#include <afxtempl.h>

// ListViewPlus �r���[

class ListViewPlus : public CListView
{
	DECLARE_DYNCREATE(ListViewPlus)
private:
	//�w�i�F�͍s�ԍ����ς�鎞�g�p�ł��܂���B
	CMap<int,int,COLORREF,COLORREF> m_mapBackColor; //�����F�w�� Key<ROW> val<COLORREF>

protected:
	ListViewPlus();          
	virtual ~ListViewPlus();
public:
	int GetHitRow();			//Click�s���擾
	int GetColCount();			//�J�������擾	

	COLORREF GetTextColor(int nRow,BOOL isSelect);//�����F�擾	
	COLORREF GetBackColor(int nRow,BOOL isSelect);//�w�i�F���擾
	void SetTextColor(int nRow,COLORREF color){//�����F�ݒ�	
		GetListCtrl().SetItemData(nRow,color); //�����F���f�[�^�Ƃ��Ďg�p
	}
	void SetBackColor(int nRow,COLORREF color){//�����F�ݒ�	
		m_mapBackColor.SetAt(nRow,color);
	}

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();
	virtual void PreSubclassWindow();
};


