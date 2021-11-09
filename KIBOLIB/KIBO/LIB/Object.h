#pragma once
#include "VisualObject.h"
#include "typedef.h"
#include "ArrayPoint.h"

//ライン描画オブジェクト
class LineObject : public VisualObject
{
private:	
	CPoint m_ptE;	  //終点
	Color m_color; //塗りつぶし色
public:
	LineObject(void);
	virtual ~LineObject(void);

	void SetPointS(int x,int y){SetX(x);SetY(y);}
	void SetPointS(CPoint &pt) {SetX(pt.x);SetY(pt.y);}
	void SetPointE(int x,int y){m_ptE.x=x;m_ptE.y=y;}
	void SetPointE(CPoint &pt) {m_ptE = pt;}
	CPoint GetPointS(){return CPoint(GetX(),GetY());} //始点取得
	CPoint GetPointE(){return m_ptE;} //終点取得
	Color  GetColor(){return m_color;}		//塗りつぶし色取得
	void SetColor(Color cr){m_color = cr;}	//塗りつぶし色取得
	void SetLine(CLine &line){
		SetPointS(line.m_ptS);
		SetPointE(line.m_ptE);
	}
	virtual void Draw(CDC *pDC); //矩形描画	
};

//矩形描画オブジェクト
class RectObject : public VisualObject
{
protected:
	bool m_isPaint; //塗りつぶしますか？
	int m_nWidth;	//幅
	int m_nHeight;	//高さ	
	Color m_color; //塗りつぶし色
public:
	RectObject(void);
	virtual ~RectObject(void);

	BOOL IsPaint(){return m_isPaint;}		//塗りつぶしますか？
	void SetPaint(bool b){m_isPaint = b;}	//塗りつぶし設定
	int  GetWidth(){return m_nWidth;}		//幅取得
	int  GetHeight(){return m_nHeight;}		//高さ取得
	void SetWidth(int w) {m_nWidth = w;}	//幅設定
	void SetHeight(int h){m_nHeight = h;}	//高さ設定
	Color GetColor(){return m_color;}			//塗りつぶし色取得
	void SetColor(Color color){m_color = color;}	//塗りつぶし色取得
	void SetRect(CRect &rect); //表示領域設定

	virtual void Draw(CDC *pDC); //矩形描画	
};

//円描画オブジェクト
class CircleObject : public RectObject
{
public:
	CircleObject(void);
	virtual ~CircleObject(void);
	virtual void Draw(CDC *pDC); //矩形描画	
};


//ポリゴンライン描画オブジェクト
class PolylineObject : public VisualObject
{
private:
	ArrayPoint m_ptAry; //境界線
	COLORREF m_color;	//塗りつぶし色
	bool m_isClosed;	//閉じますか？
public:
	PolylineObject();
	virtual ~PolylineObject(void);

	BOOL IsClosed(){return m_isClosed;}			//閉じますか？
	void SetClosed(bool b){m_isClosed = b;}		//閉じるかどうかを設定
	COLORREF GetColor(){return m_color;}		//塗りつぶし色取得
	void SetColor(COLORREF cr){m_color = cr;}	//塗りつぶし色取得
	void SetPoints(ArrayPoint& ptAry){
		m_ptAry.RemoveAll();
		for(int i=0;i<ptAry.GetSize();i++){
			if( ptAry[i].x <= 0 ){
				break;
			}
			m_ptAry.Add( ptAry[i] );
		}
	}
	virtual void Draw(CDC *pDC); //矩形描画	
};

//文字列描画オブジェクト
class StringObject : public VisualObject
{
private:
	CString  m_strText; //表示文字列
	COLORREF m_color;	//文字色
	HFONT m_hFont;		//フォント
	CRect m_rect;		//表示領域
	UINT m_unFormat;	//表示領域フォーマット
public:
	StringObject(void);
	virtual ~StringObject(void);

	CString&  GetText(){return m_strText;} //文字列取得
	void SetText(CString &s){m_strText=s;} //文字列設定
	COLORREF GetColor(){return m_color;}		//塗りつぶし色取得
	void SetColor(COLORREF cr){m_color = cr;}	//塗りつぶし色設定
	CFont*  GetFont(){return CFont::FromHandle(m_hFont);}//フォントを取得
	void	SetFont(HFONT h){m_hFont=h;} //フォント設定
	void SetRect(CString &strH,CString &strV,int nLeft,int nTop,int nRight,int nBottom); //表示領域設定	
	void InitRect();//文字列描画領域初期化
	virtual void Draw(CDC *pDC); //矩形描画	
};
