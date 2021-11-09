#pragma once

#include "color.h"
#include "GlobalMgr.h"
#include "typedef.h"
#include "gdiplus.h"

//表示用基本オブジェクト
class VisualObject
{
public:
	enum{ //VisualObject Type
		 TYPE_NONE = 0	//指定なし
		,TYPE_BITMAP	//Bitmap
		,TYPE_LINE		//線
		,TYPE_RECT		//矩形
		,TYPE_CIRCLE	//円
		,TYPE_STRING	//文字列
		,TYPE_POLYLINE  //ポリゴンの枠線
	};
private: //表示座標
	bool m_isVisible; //表示しますか
	BYTE m_byType; //オブジェクトタイプ
	int m_nX;	//x座標
	int m_nY;	//y座標
	CString m_strName; //オブジェクトを特定するための名前
public:
	VisualObject(void);
	virtual ~VisualObject(void);

	bool IsVisible(){return m_isVisible;} //表示しますか
	void SetVisible(bool is){m_isVisible=is;} 
	BYTE GetType(){return m_byType;} //オブジェクトタイプ
	void SetType(BYTE by){m_byType=by;}
	int GetX(){return m_nX;}	//x座標取得
	int GetY(){return m_nY;}	//y座標取得
	void SetX(int x){m_nX = x;} //x座標設定
	void SetY(int y){m_nY = y;} //y座標設定
	CString& GetName(){return m_strName;} //オブジェクト名称取得
	void SetName(CString s){ if(!s.IsEmpty()){m_strName=s;} } //オブジェクト名称設定

	virtual void Draw(CDC *pDC) = 0; //Visual Objectの描画
};

//表示用基本オブジェクト動的配列
class AryVO : public CArray<VisualObject *>
{
public:
	AryVO(void);
	~AryVO(void);

	int Find(VisualObject *pVO);
};

class ArrayPoint;

//Visual Object登録ルール
//指定したタイプが無効配列にあればそれを有効配列に移す
//指定したタイプがなければ作成する
class VisualObjectMgr
{
private:
	bool m_isVisible;	//表示・非表示切り替え
	AryVO m_aryValid;	//有効配列
	AryVO m_aryInvalid;	//無効配列
public:
	VisualObjectMgr();	//コンストラクタ
	~VisualObjectMgr();	//デストラクタ
	void Draw(CDC *pDC); //Visual Objectの描画

	void SetVisible(bool b){m_isVisible = b;} //表示・非表示切り替え
	bool IsVisible(){return m_isVisible;} //表示しますか？
	void CreateLineObject(CString strName,const CLine &line,Color color);//ライン追加
	void CreateRectObject(CString strName,const CRect &rect,Color color,BOOL isPaint); //矩形追加
	void CreateCircleObject(CString strName,const CRect &rect,Color color,BOOL isPaint); //円追加
	void CreatePolylineObject(CString strName,ArrayPoint &ptAry,COLORREF color,bool isClosed=true); //ポリゴン枠線追加	
	void CreatePolylineObject(CString strName,const LPPOINT lpPoints,int nCnt,COLORREF color,bool isClosed =true);//ポリゴン枠線追加
	void CreateStringObject(CString strName,int nX,int nY,CString strText,COLORREF color,LOGFONT &lf); //文字列追加

	void SetFront(VisualObject *pVO); //指定VisualObjectを最前面に移動
	void Remove(VisualObject *pVO); //指定VisualObjectを無効化
	void RemoveAll(); //全VisualObjectを無効化
	void RemoveByName(const CString &strName); //指定名称を含むオブジェクトを無効化します	
	void RemoveExcept(const CString &strName);//指定オブジェクト以外を削除
	VisualObject* GetVO(CString strName); //指定Visual Object取得
private:
	VisualObject* CreateVisualObject(int nType,CString &strName);//Objectの作成
};
