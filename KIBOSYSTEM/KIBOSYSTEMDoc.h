
// KIBOSYSTEMDoc.h : CKIBOSYSTEMDoc クラスのインターフェイス
//


#pragma once


class CKIBOSYSTEMDoc : public CKIBODoc
{
protected: // シリアル化からのみ作成します。
	CKIBOSYSTEMDoc();
	DECLARE_DYNCREATE(CKIBOSYSTEMDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装
public:
	virtual ~CKIBOSYSTEMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
