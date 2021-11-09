
// KIBOSYSTEMView.cpp : CKIBOSYSTEMView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "KIBOSYSTEM.h"
#endif

#include "KIBOSYSTEMDoc.h"
#include "KIBOSYSTEMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKIBOSYSTEMView

IMPLEMENT_DYNCREATE(CKIBOSYSTEMView, CKIBOView)

BEGIN_MESSAGE_MAP(CKIBOSYSTEMView, CKIBOView)
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_RENDER, OnRender)	//�����_�����O�v��
END_MESSAGE_MAP()

// CKIBOSYSTEMView �R���X�g���N�V����/�f�X�g���N�V����

CKIBOSYSTEMView::CKIBOSYSTEMView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CKIBOSYSTEMView::~CKIBOSYSTEMView()
{
}

BOOL CKIBOSYSTEMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CKIBOSYSTEMView �`��

void CKIBOSYSTEMView::OnDraw(CDC* pDC)
{
	CKIBOSYSTEMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CKIBOView::OnDraw(pDC);
}
//�����_�����O�v��
LRESULT CKIBOSYSTEMView::OnRender(WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_RENDER, WM_RENDER, PM_REMOVE)) { //���̃��b�Z�[�W���폜
		;
	}
	BOOL isDraw = (BOOL)wParam;
	return Render(isDraw);
}

// CKIBOSYSTEMView ���


void CKIBOSYSTEMView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKIBOSYSTEMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CKIBOSYSTEMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CKIBOSYSTEMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}

void CKIBOSYSTEMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKIBOSYSTEMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKIBOSYSTEMView �f�f

#ifdef _DEBUG
void CKIBOSYSTEMView::AssertValid() const
{
	CView::AssertValid();
}

void CKIBOSYSTEMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKIBOSYSTEMDoc* CKIBOSYSTEMView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKIBOSYSTEMDoc)));
	return (CKIBOSYSTEMDoc*)m_pDocument;
}
#endif //_DEBUG


// CKIBOSYSTEMView ���b�Z�[�W �n���h���[


BOOL CKIBOSYSTEMView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CKIBOSYSTEMView::OnSize(UINT nType, int cx, int cy)
{
	CKIBOView::OnSize(nType, cx, cy);
	Render(FALSE);
}
