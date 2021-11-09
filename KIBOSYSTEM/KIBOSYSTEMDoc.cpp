
// KIBOSYSTEMDoc.cpp : CKIBOSYSTEMDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "KIBOSYSTEM.h"
#endif

#include "KIBOSYSTEMDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKIBOSYSTEMDoc

IMPLEMENT_DYNCREATE(CKIBOSYSTEMDoc, CKIBODoc)

BEGIN_MESSAGE_MAP(CKIBOSYSTEMDoc, CKIBODoc)
END_MESSAGE_MAP()


// CKIBOSYSTEMDoc コンストラクション/デストラクション

CKIBOSYSTEMDoc::CKIBOSYSTEMDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CKIBOSYSTEMDoc::~CKIBOSYSTEMDoc()
{
}

BOOL CKIBOSYSTEMDoc::OnNewDocument()
{
	if (!CKIBODoc::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

	return TRUE;
}




// CKIBOSYSTEMDoc シリアル化

void CKIBOSYSTEMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CKIBOSYSTEMDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CKIBOSYSTEMDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKIBOSYSTEMDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKIBOSYSTEMDoc 診断

#ifdef _DEBUG
void CKIBOSYSTEMDoc::AssertValid() const
{
	CKIBODoc::AssertValid();
}

void CKIBOSYSTEMDoc::Dump(CDumpContext& dc) const
{
	CKIBODoc::Dump(dc);
}
#endif //_DEBUG


// CKIBOSYSTEMDoc コマンド
