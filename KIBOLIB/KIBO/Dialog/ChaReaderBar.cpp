// ChaReaderBar.cpp : 実装ファイル
//
#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOFrm.h"
#include "ChaReaderBar.h"
#include "../KIBOView.h"
#include "../OutputView.h"
#include "../LIB/Dib.h"
#include "../SCript/App.h"
#include "../SCript/Camera.h"
#include "../SCript/SerialPort.h"
#include "../LIB/FilePlus.h"
#include "../LIB/macro.h"

#define MAX_POSITION 30000 //30000puls = 300mm


// ChaReaderBar ダイアログ

IMPLEMENT_DYNAMIC(ChaReaderBar, CDialogBar)

ChaReaderBar::ChaReaderBar()
	: CDialogBar()
{
}

ChaReaderBar::~ChaReaderBar()
{
}

//初期化
void ChaReaderBar::Init()
{	
	App *pApp = App::GetInstance();

}

BEGIN_MESSAGE_MAP(ChaReaderBar, CDialogBar)
//	ON_UPDATE_COMMAND_UI_RANGE(IDC_START,IDC_HISTGRAM,OnUpdateEnable)
ON_BN_CLICKED(IDC_CHECK_SELRECT, &ChaReaderBar::OnBnClickedCheckSelrect)
END_MESSAGE_MAP()

// ChaReaderBar メッセージ ハンドラ


void ChaReaderBar::OnUpdateEnable(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

//検査領域選択
void ChaReaderBar::OnBnClickedCheckSelrect()
{
	
}
