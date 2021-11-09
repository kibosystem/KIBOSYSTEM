// KIBOView.cpp : CKIBOView �N���X�̎���
//
#include "stdafx.h"
#include "enum.h"
#include "KIBO.h"
#include "KIBODoc.h"
#include "KIBOView.h"
#include "KIBOFrame.h"
#include "./Script/Labeling.h"
#include "./SCript/App.h"
#include "./SCript/Bitmap.h"
#include "./SCript/ImageDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString s_strClickLineName = L"ClickLine"; //Click Line����
static CKIBOView *s_pView = NULL;

CKIBOView* GetView()
{
	return s_pView; //View�擾
}

// CKIBOView

IMPLEMENT_DYNCREATE(CKIBOView, CView)

BEGIN_MESSAGE_MAP(CKIBOView, CView)
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CKIBOView �R���X�g���N�V����/�f�X�g���N�V����

CKIBOView::CKIBOView()
{
	s_pView = this;
	m_isInitOK = false;
	m_dbRateCx = 1;	//���W�\���{�� x ���W
	m_dbRateCy = 1;	//���W�\���{�� y ���W
	m_lineColor = Color::OrangeRed; //���C���F
}

CKIBOView::~CKIBOView()
{
}

BOOL CKIBOView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CKIBOView �`��

void CKIBOView::OnDraw(CDC *pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOld = dcMem.SelectObject( m_dib.GetBitmap() ); //Bitamp�I��
	pDC->BitBlt(0,0,m_dib.GetWidth(),m_dib.GetHeight(),&dcMem,0,0,SRCCOPY);
	//��n��
	dcMem.SelectObject( pOld);
}

void CKIBOView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_isInitOK = true;
}

// CKIBOView ���

BOOL CKIBOView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CKIBOView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CKIBOView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}

// CKIBOView ���b�Z�[�W �n���h��


//�w�i�`��֎~
BOOL CKIBOView::OnEraseBkgnd(CDC* pDC)
{	
	return TRUE;
}
//�}�X�N�`��
BOOL CKIBOView::DrawMask(CDC* pDC)
{
	return FALSE;
}

//�S�J���������_�����O
BOOL CKIBOView::Render(BOOL isDraw)
{
	if (m_dib.GetWidth() > 0){
		CDC *pDC = GetDC();


		m_dib.PaintBlack();
		//�J�����摜�̕`����s��
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOld = dcMem.SelectObject(m_dib.GetBitmap()); //View�p�r�b�g�}�b�v�I��

		ImageDevice *pImageDevice = ImageDevice::GetInstance();
		pImageDevice->Render(dcMem, m_mgrVO); //�S�J���������_�����O (���z�֐��j

		//�f�B�X�v���C�`��
		if (isDraw){ //View�ɕ`����s��
			pDC->BitBlt(0, 0, m_dib.GetWidth(), m_dib.GetHeight(), &dcMem, 0, 0, SRCCOPY);
		}
		//��n��
		dcMem.SelectObject(pOld);
		ReleaseDC(pDC);
	}
	return TRUE;
}

//2�l������
BOOL CKIBOView::DoBinary(BYTE byTh)
{
	m_dib.ChangeBinary(byTh);
	
	RedrawWindow();
	return TRUE;
}

//�\���摜�ۑ�
BOOL CKIBOView::SaveBitmap(LPCWSTR lpstrInitialDir)
{
	CString strFilePath = SelectBitmapFile(FALSE,lpstrInitialDir);
	if( !strFilePath.IsEmpty() ){
		m_dib.WriteBitmap( strFilePath );
		return TRUE;
	}
	return FALSE;
}

//�J�����摜�ۑ�
CString CKIBOView::SaveCameraImage(LPCWSTR lpstrInitialDir)
{
	CString strFilePath = SelectBitmapFile(FALSE,lpstrInitialDir);
	
	if( !strFilePath.IsEmpty() ){
		ImageDevice *pImageDevice = ImageDevice::GetInstance();
		if(pImageDevice != NULL){
			pImageDevice->Lock();
			CDIB &dib = pImageDevice->GetDib();
			dib.WriteImage( strFilePath , strFilePath.Right(3) );
			pImageDevice->Unlock();
		}
	}
	return strFilePath;
}

//�`��v�f���Z�b�g
BOOL CKIBOView::SetElementRect(int nEnum,CString &strName,Color color,int nDx/*=0*/,int nDy/*=0*/)
{
	Label &label = Labeling::GetInstance()->m_label; //���x�����O�N���X�擾
	Element *pElem = label.GetElement(nEnum); //���x�����O�v�f�擾
	if( pElem != NULL ){
		CRect rect;
		//�����_�����O�I�u�W�F�N�g�o�^
		rect.left  = pElem->GetMinX() + nDx;
		rect.right = pElem->GetMaxX() + nDx;
		rect.top   = pElem->GetMinY() + nDy;
		rect.bottom= pElem->GetMaxY() + nDy;
		m_mgrVO.CreateRectObject(strName,rect,color,false);
		return TRUE;
	}
	return FALSE;
}

//�v�f�h��Ԃ�
BOOL CKIBOView::PaintElement(int nEnum,BYTE byR,BYTE byG,BYTE byB)
{
	Label &label = Labeling::GetInstance()->m_label; //���x�����O�N���X�擾
	Bitmap24* pBmp24 = Bitmap24::GetInstance();
	if(pBmp24 != NULL){
		CDIB &dib = pBmp24->GetDib(); //�J�����摜�擾
		Element *pElem = label.GetElement(nEnum); //���x�����O�v�f�擾
		if( pElem != NULL ){
			label.PaintElementColor(pElem,dib,byR,byG,byB); //�`��
			return TRUE;	
		}		
	}
	return FALSE;	
}

//�}�E�X�ړ�
void CKIBOView::OnMouseMove(UINT nFlags, CPoint point)
{
	KIBOFrame *pFrame = GetFrame();

	//�X�e�[�^�X�o�[�\��
	//���W
	int nX = (int)(point.x/m_dbRateCx+0.5);
	int nY = (int)(point.y/m_dbRateCy+0.5);
	CString strText;
	strText.Format(L"x:%d,y%d",nX,nY);//���W�\��
	pFrame->SetStatusBarText(strText);

	//�F
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nX = point.x;
	nY = point.y;
	if( 0 <= nX && nX < nW && 0 <= nY && nY < nH ){
		CString strColor;
		BYTE byB = m_dib.GetBitsB(nX,nY);
		BYTE byG = m_dib.GetBitsG(nX,nY);
		BYTE byR = m_dib.GetBitsR(nX,nY);
		strColor.Format(L"R,G,B=%u,%u,%u",byR,byG,byB);
		//pFrame->GetStatusBarPtr()->SetPaneText(1,strColor);
	}
	//CView::OnMouseMove(nFlags, point);
}

BOOL CKIBOView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	BOOL isOK = CView::OnScroll(nScrollCode, nPos, bDoScroll);
	Invalidate();
	return isOK;
}

//�T�C�Y�ύX
void CKIBOView::OnSize(UINT nType, int cx, int cy)
{
	if( cx > 0 && cy > 0){
		int nHmax = GetScrollMax(SB_HORZ);//�X�N���[���T�C�Y�擾
		int nVmax = GetScrollMax(SB_VERT);
		nHmax = max(nHmax,cx);//��ʃT�C�Y�Ɣ�r
		nVmax = max(nVmax,cy);
		m_dib.Create(nHmax,nVmax); //Bitmap�T�C�Y�ύX
		//Render(TRUE);
	}
	//�X�N���[���o�[�\��OFF
	ScrollBarOff(SB_HORZ,cx);
	ScrollBarOff(SB_VERT,cy);
	CView::OnSize(nType, cx, cy);
}
//�X�N���[���o�[�\��OFF
void CKIBOView::ScrollBarOff(int nBar,int nSize)
{
	int nMax = GetScrollMax(nBar);
	CScrollBar* pScroll = GetScrollBarCtrl(nBar);
	if(pScroll != NULL && nMax < nSize){
		pScroll->ShowWindow(SW_HIDE);
	}	
}
//�X�N���[���ő�T�C�Y�擾
int CKIBOView::GetScrollMax(int nBar)
{
	int nMin = 0;
	int nMax = 0;
	GetScrollRange(nBar,&nMin,&nMax);
	return nMax;
}


//�J�n�|�C���g & �I���|�C���g
void CKIBOView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}

//�N���b�N���C���̒������擾
double CKIBOView::GetClickLineLength()
{
	double dbVal = 0;
	if( m_lineClick.IsPtSOK() && m_lineClick.IsPtEOK() ){
		dbVal = ::GetLength(m_lineClick.m_ptS,m_lineClick.m_ptE);
	}
	return dbVal;
}
void CKIBOView::OnRButtonUp(UINT nFlags, CPoint point)
{
	//ClickLine�폜
	if( m_lineClick.IsPtSOK() ){ //�J�n�n�_��OK�ł����H
		m_lineClick.Clear();
		m_mgrVO.RemoveByName(s_strClickLineName); //���C���폜
		Render(TRUE);
	}
	CView::OnRButtonUp(nFlags, point);
}

//���S�ɃX�N���[���ʒu�擾
CPoint CKIBOView::GetScrollPos()
{
	return 0;
}
