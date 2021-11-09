#pragma once

#include "globalmgr.h"
#include "dib.h"

class ArrayPoint : public CArray<CPoint> 
{
public:
	ArrayPoint(void){}
	~ArrayPoint(void){}
	
	ArrayPoint& operator=(const ArrayPoint &t);
	int Find(CPoint pt);			//要素検索
	INT_PTR AddPoint(int x,int y);	//座標追加

	CPoint GetLeft();	//最も左の点を取得
	CPoint GetRight();	//最も右の点を取得
	CPoint GetTop();	//最も上の点を取得
	CPoint GetBottom(); //最も下の点を取得		
	CRect GetRect();//点配列から矩形を取得

	double GetLength(CPoint& pt); //指定座標との最短距離を求める
	CPoint GetLengthPoint(CPoint& pt,double &dbLmin);//指定座標との最短距離と最短距離となるPointを取得
	int CollectPointX(int x,ArrayPoint &ptCollect);//指定x座標の点取得
	int CollectPointY(int y,ArrayPoint &ptCollect);//指定y座標の点取得
	CPoint FindX(int x); //指定x座標検索
	CPoint FindX(int nIndex,int x); //指定x座標検索	
	CPoint FindY(int y); //指定y座標検索	
	CPoint FindY(int nIndex,int y) ;//指定y座標検索

	void SetX(int x,ArrayPoint &ptCollect); //指定PointのX座標を指定値に設定
	void SortX(); //x座標でソート
	void SortY(); //y座標でソート
	double GetLength(ArrayPoint &ptAry,CPoint &ptS,CPoint &ptE); //最短距離取得
	void SplitX(int nX,ArrayPoint &aryL,ArrayPoint &aryR); //x座標によって2つに分ける

	BOOL IsConnect(CPoint &pt); //接続Pointですか？
	void CollectConnectPoint(CPoint &pt,ArrayPoint &aryConnect); //頂点座標との隣接Pointのみ
	void CollectExceptPoint(ArrayPoint &aryCollect,ArrayPoint &aryExcept); //隣接Point以外を収集

	void RangeX(double dbXmin,double dbXmax); //指定x範囲内の座標だけを残す
	void RangeY(double dbYmin,double dbYmax);//指定y範囲内の座標だけを残す

	//指定数接続Pointのみを選択
	void SelectConnectPoint(int nDelete);	
	void DrawPolyline(CDIB &dib,COLORREF color);//線描画	
	void DrawPoints(int nNo);//点描画
};
