// HistgramDlg.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "HistgramDlg.h"
#include "../LIB/color.h"
#include "../Script/Bitmap.h"
#include "../SCript/App.h"

#define HISTGRAM_CX 512
#define HISTGRAM_CY 400

// HistgramDlg �_�C�A���O

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
	Init(); //������
	return FALSE;
}
//������
void HistgramDlg::Init()
{
	CRect rect;
	GetClientRect(&rect);

	//�ő��f�������߂�
	m_nMaxPixels = 0;
	//�ʒu����	
	m_stKido.MoveWindow(HISTGRAM_CX,10,100,25);
	ChangeHistGram(rect.Width(),rect.Height());
	this->SetTimer(ID_TIMER,100,NULL);
}

//
void HistgramDlg::ChangeHistGram(int cx, int cy)
{
	if( m_stHistGram.GetSafeHwnd() != NULL){		
		m_stHistGram.MoveWindow(0,0,HISTGRAM_CX,cy);
		//�\���r�b�g�}�b�v�̍쐬
		CClientDC dc(this);
		CDC dcMem; //�������f�o�C�X�R���e�L�X�g
		dcMem.CreateCompatibleDC(&dc);
		CBitmap *pOld = dcMem.SelectObject( m_stHistGram.m_dib.GetBitmap() ); //Bitamp�I��
		if( m_nMaxPixels > 0 ){ //Data Set Check
			DrawHistGram(dcMem);
		}
		//��n��
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
	double dbHunit = (double)nBarMax / m_nMaxPixels; //�P��f������̖_�O���t�̍���
	CRect rcBar;
	rcBar.bottom = nBarMax;
	//Gray
	App *pApp = App::GetInstance(); //�A�v���P�[�V�����X�N���v�g�N���X
	int nTh = pApp->GetNum(L"�������l");
	int nThCa = pApp->GetNum(L"�L�����u���[�V�����������l");
	for(int i=0;i<=MAXBYTE;i++){
		rcBar.left  = i*nCx;
		rcBar.right = rcBar.left + nCx;
		rcBar.top   = (int)(rcBar.bottom - m_nGray[i] * dbHunit+0.5);
		dc.FillSolidRect(rcBar,COLOR_WHITE);		
		if( i==nTh){//�������C���̕`��
			rect = rcBar;
			rect.top = 0;
			dc.FillSolidRect(rect,COLOR_RED);
		}else if(i==nThCa){ //�L�����u���[�V�����������l
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

// HistgramDlg ���b�Z�[�W �n���h��

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
	int nV = nX / 2; //�P�x
	m_stKido.SetText(L"�P�x:%d %d",nV,m_nGray[nV]);
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

//�^�C�}�[���b�Z�[�W
void HistgramDlg::OnTimer(UINT_PTR nIDEvent)
{
	//Gray �擾
	Bitmap24 *pBitmap = Bitmap24::GetInstance(); //�t���J���[�r�b�g�}�b�v�N���X
	pBitmap->FromImageDevice(); //�J�����摜�擾
	pBitmap->GetDib().CollectHistgramGray(m_nGray);//�q�X�g�O�������W

	//�ő��f�������߂�
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

	//�\���r�b�g�}�b�v�̍쐬
	CDC dcMem; //�������f�o�C�X�R���e�L�X�g
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOld = dcMem.SelectObject( m_stHistGram.m_dib.GetBitmap() ); //Bitamp�I��
	if( m_nMaxPixels > 0 ){ //Data Set Check
		DrawHistGram(dcMem);
	}
	//��n��
	dcMem.SelectObject(pOld);
}
