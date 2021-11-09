
// KIBOView.h : CKIBOView クラスのインターフェイス
//


#pragma once

#include "./LIB/Label.h"
#include "./LIB/VisualObject.h"
#include "KIBO.h"
#include "KiboCriticalSection.h"

class CKIBOView : public CView
{
protected: // シリアル化からのみ作成します。
	CKIBOView();
	virtual ~CKIBOView();
	DECLARE_DYNCREATE(CKIBOView)
protected: //メンバ変数
	bool	m_isInitOK;		//初期化終了しましたか？
	CDIB	m_dib;			//表示画像
	CDIB	m_dibScript;	//Script内で使用する画像
	double  m_dbRateCx;	//座標表示倍率 x 座標
	double  m_dbRateCy;	//座標表示倍率 y 座標
	CLine	m_lineClick; //クリックライン
	Color	m_lineColor; //クリックライン色
	KiboCriticalSection m_section; //クリティカルセクション
public:

	VisualObjectMgr	m_mgrVO;	//表示オブジェクトマネージャー
// 属性
public:
	BOOL IsInitOK(){return m_isInitOK;}//初期化終了しましたか？

	CDIB& GetDib(){return m_dib;}		//表示画像取得
	void SetViewRate(double dbRateX,double dbRateY){ //座標表示倍率設定
		m_dbRateCx = dbRateX;
		m_dbRateCy = dbRateY;
	}
	double GetRateCx(){return m_dbRateCx;}	//座標表示倍率 x 座標取得
	double GetRateCy(){return m_dbRateCy;}	//座標表示倍率 y 座標取得
	double GetClickLineLength();//クリックラインの長さを取得
	void   SetClickClineColor(Color col){m_lineColor=col;}//クリックライン色設定
// 操作
public:	
	//実験用処理
	BOOL DoBinary(BYTE byTh); //2値化処理
	BOOL SaveBitmap(LPCWSTR lpstrInitialDir); //Bitmap保存	
	CString SaveCameraImage(LPCWSTR lpstrInitialDir);//カメラ画像保存

	//描画要素をセット
	BOOL SetElementRect(int nEnum,CString &strName,Color color,int nDx=0,int nDy=0);

	//要素塗りつぶし
	BOOL PaintElement(int nEnum,BYTE byR,BYTE byG,BYTE byB);

private: //内部関数		
	CPoint GetScrollPos();//安全にスクロール位置取得
	void ScrollBarOff(int nBar,int nSize);//スクロールバー表示OFF	
	int  GetScrollMax(int nBar);//スクロール最大サイズ取得
// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Render(BOOL isDraw);//全カメラレンダリング

protected:
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL DrawMask(CDC* pDC);

// 実装
public:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

extern CKIBOView* GetView();
