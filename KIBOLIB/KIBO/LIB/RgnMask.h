#pragma once

#include "../Dialog/SelectDlg.h"

//リージョン情報
class RgnMask
{
private:
	bool m_isMoveOK; //移動禁止フラグ
	CDialogBar *m_pBar; //ダイアログバー 
	int m_nID;			//マスクID番号
	CRect m_rcImage; //イメージ座標
public:
	SelectDlg m_dlgLT; //左上
	SelectDlg m_dlgRT; //右上
	SelectDlg m_dlgLB; //左下
	SelectDlg m_dlgRB; //右下
public:
	RgnMask(void);
	~RgnMask(void);
public:
	int GetID(){return m_nID;} //マスクID番号取得
	CRect GetImageRect(){return m_rcImage;} //座標取得
	CRect GetClientRect(); //座標取得
	void SetClientRect(CRect &rect); //クライアント座標設定
	void SetClientRect(int nLeft,int nTop,int nRight,int nBottom);
	void SetImageRect(int nLeft,int nTop,int nRight,int nBottom);
	void Show(); //マスク表示
	void Hide(); //マスク非表示
	BOOL IsVisible(); //表示中ですか？
	void Draw(CDC* pDC); //マスク描画
	//選択ダイアログの連動
	void MoveSelectDlg(int nType);
	void SetWindow(CDialogBar *p){m_pBar=p;} //DialogBar設定
	void SetID(int n){m_nID=n;} //
private:

};
