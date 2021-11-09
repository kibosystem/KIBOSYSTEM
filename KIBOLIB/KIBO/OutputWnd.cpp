// OutputWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OutputWnd.h"


static OutputWnd *s_pOutputWnd = NULL;
OutputWnd *GetOutputWnd() { return s_pOutputWnd; }

// OutputWnd

IMPLEMENT_DYNAMIC(OutputWnd, CDockablePane)

OutputWnd::OutputWnd()
{
	s_pOutputWnd = this;
}

OutputWnd::~OutputWnd()
{
}

BEGIN_MESSAGE_MAP(OutputWnd, CDockablePane)
END_MESSAGE_MAP()



// OutputWnd メッセージ ハンドラー


