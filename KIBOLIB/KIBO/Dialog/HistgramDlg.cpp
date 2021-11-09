// HistgramDlg.cpp : 実装ファイル
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "HistgramDlg.h"
#include "../LIB/color.h"
#include "../Script/Bitmap.h"
#include "../SCript/App.h"

#define HISTGRAM_CX 512
#define HISTGRAM_CY 400

// HistgramDlg ダイアログ

IMPLEMENT_DYNAMIC(HistgramDlg, CDialog)

HistgramDlg::HistgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HISTGRAM , pParent)
{
	ZeroMemory(m_nGray,sizeof(m_nGray));
	m_nMaxPixels = 0;
}

HistgramDlg::~HistgramDlg()
{
}

void HistgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTGRAM, m_stHistGram);
	DDX_Control(pDX, IDC_V, m_stKido);
}

BOOL HistgramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Init(); //初期化
	return FALSE;
}
//初期化
void HistgramDlg::Init()
{
	CRect rect;
	GetClientRect(&rect);

	//最大画素数を求める
	m_nMaxPixels = 0;
	//位置調整	
	m_stKido.MoveWindow(HISTGRAM_CX,10,100,25);
	ChangeHistGram(rect.Width(),rect.Height());
	this->SetTimer(ID_TIMER,100,NULL);
}

//
void HistgramDlg::ChangeHistGram(int cx, int cy)
{
	if( m_stHistGram.GetSafeHwnd() != NULL){		
		m_stHistGram.MoveWindow(0,0,HISTGRAM_CX,cy);
		//表示ビットマップの作成
		CClientDC dc(this);
		CDC dcMem; //メモリデバイスコンテキスト
		dcMem.CreateCompatibleDC(&dc);
		CBitmap *pOld = dcMem.SelectObject( m_stHistGram.m_dib.GetBitmap() ); //Bitamp選択
		if( m_nMaxPixels > 0 ){ //Data Set Check
			DrawHistGram(dcMem);
		}
		//後始末
		dcMem.SelectObject(pOld);
	}
}

void HistgramDlg::DrawHistGram(CDC &dc)
{
	CRect rect;
	m_stHistGram.GetClientRect(&rect);
	dc.FillSolidRect(&rect,COLOR_BLACK);
	int nCx = 2;
	int nBarMax = rect.Height();
	double dbHunit = (double)nBarMax / m_nMaxPixels; //１画素あたりの棒グラフの高さ
	CRect rcBar;
	rcBar.bottom = nBarMax;
	//Gray
	App *pApp = App::GetInstance(); //アプリケーションスクリプトクラス
	int nTh = pApp->GetNum(L"しきい値");
	int nThCa = pApp->GetNum(L"キャリブレーションしきい値");
	for(int i=0;i<=MAXBYTE;i++){
		rcBar.left  = i*nCx;
		rcBar.right = rcBar.left + nCx;
		rcBar.top   = (int)(rcBar.bottom - m_nGray[i] * dbHunit+0.5);
		dc.FillSolidRect(rcBar,COLOR_WHITE);		
		if( i==nTh){//中央ラインの描画
			rect = rcBar;
			rect.top = 0;
			dc.FillSolidRect(rect,COLOR_RED);
		}else if(i==nThCa){ //キャリブレーションしきい値
			rect = rcBar;
			rect.top = 0;
			dc.FillSolidRect(rect,COLOR_BLUE);
		}
	}		
}

BEGIN_MESSAGE_MAP(HistgramDlg, CDialog)
	ON_MESSAGE(WM_POSITION,OnShowInfo)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// HistgramDlg メッセージ ハンドラ

void HistgramDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ChangeHistGram(cx,cy);
	Invalidate(FALSE);
}

LRESULT HistgramDlg::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
	int nX = (int)wParam;
	int nY = (int)lParam;
	int nV = nX / 2; //輝度
	m_stKido.SetText(L"輝度:%d %d",nV,m_nGray[nV]);
	return 0;
}

void HistgramDlg::OnClose()
{
	CDialog::OnClose();
}

void HistgramDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

//タイマーメッセージ
void HistgramDlg::OnTimer(UINT_PTR nIDEvent)
{
	//Gray 取得
	Bitmap24 *pBitmap = Bitmap24::GetInstance(); //フルカラービットマップクラス
	pBitmap->FromImageDevice(); //カメラ画像取得
	pBitmap->GetDib().CollectHistgramGray(m_nGray);//ヒストグラム収集

	//最大画素数を求める
	m_nMaxPixels = 0;
	for(int i=0;i<256;i++){
		if( m_nMaxPixels < m_nGray[i] ){
			m_nMaxPixels = m_nGray[i];
		}
	}
	Invalidate(FALSE);
	CDialog::OnTimer(nIDEvent);
}

void HistgramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//表示ビットマップの作成
	CDC dcMem; //メモリデバイスコンテキスト
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOld = dcMem.SelectObject( m_stHistGram.m_dib.GetBitmap() ); //Bitamp選択
	if( m_nMaxPixels > 0 ){ //Data Set Check
		DrawHistGram(dcMem);
	}
	//後始末
	dcMem.SelectObject(pOld);
}
