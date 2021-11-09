#pragma once

#include "../LIB/DxGraphic.h"

//DirectX FullScreen Mode専用ダイアログ
// DirectScreenDlg ダイアログ

class DirectScreenDlg : public CDialog
{
	DECLARE_DYNAMIC(DirectScreenDlg)
private:
	CSize	  m_size; //Screen Size
public:
	DXGraphic m_DXGraphic; //DirectX FullScreen Mode
public:
	DirectScreenDlg(CSize size,CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~DirectScreenDlg();

// ダイアログ データ
	enum { IDD = IDD_FULLSCREEN };

	void Render(); //描画
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
