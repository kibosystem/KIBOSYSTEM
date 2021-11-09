
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "KIBOSYSTEM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ButtonProprerty::ButtonProprerty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr /*= NULL*/, DWORD_PTR dwData/* = 0*/, LPCTSTR lpszEditMask /*= NULL*/, LPCTSTR lpszEditTemplate /*= NULL*/, LPCTSTR lpszValidChars /*= NULL*/)
	:CMFCPropertyGridProperty(strName, varValue, lpszDescr , dwData, lpszEditMask ,lpszEditTemplate ,lpszValidChars)
{

}

ButtonProprerty::~ButtonProprerty()
{

}
void ButtonProprerty::OnDrawValue(CDC* pDC, CRect rect)
{
	COLORREF crBtn = GetSysColor(COLOR_BTNFACE);
	pDC->FillSolidRect(rect, crBtn);

	LOGPEN lp; //指定色のペン
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 5;
	lp.lopnColor = GetSysColor(COLOR_BTNSHADOW); //色指定
	CPen *pPenB = g_GlobalMgr.GetPen(lp); //Pen取得
	lp.lopnColor = GetSysColor(COLOR_BTNHIGHLIGHT); //色指定
	CPen *pPenW = g_GlobalMgr.GetPen(lp); //Pen取得
	
	CPen *pPenOld = pDC->SelectObject(pPenB);
	pDC->MoveTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);	
	pDC->MoveTo(rect.left , rect.bottom);
	pDC->LineTo(rect.right, rect.bottom );

    pDC->SelectObject(pPenW);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);

	CString str = L"CLICK HERE";
	pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	
	pDC->SelectObject(pPenOld);
}
BOOL ButtonProprerty::OnClickValue(UINT uiMsg, CPoint point)
{
	CMainFrame* pFrame = GetMainFrame();

	//実行
	LPCTSTR lpszFileName = this->GetName(); //ファイル名称の取得
	//実行ディレクトリ
	CString strExePath = theApp.GetAppPath();
	CString strFilePath;
	strFilePath.Format(L"%s\\Command\\%s", strExePath,lpszFileName);



	pFrame->ScriptExec(strFilePath); //スクリプト実行
	pFrame->m_wndProperties.m_wndPropList.SetCurSel(NULL);//選択解除
	return TRUE;
}
//コマンド一覧読み込み
void ReadCommandMenu(CStringArray &aryMenuName)
{
	aryMenuName.RemoveAll(); //clear
	CString strPath = GetApp()->GetAppPath(); //アプリケーソン起動時のパス
	CFileFind find;
	BOOL isOK = find.FindFile(strPath + _T("\\Command\\*.js"));
	while (isOK) {
		isOK = find.FindNextFile();
		if (!find.IsDirectory()) {
			aryMenuName.Add(find.GetFilePath()); //FilePathセット			
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar メッセージ ハンドラー

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top , rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top+ cyTlb, rectClient.Width(), rectClient.Height() -cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("プロパティ グリッドを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ロックされています*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ロックされました*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* pCmdUI)
{

}

void CPropertiesWnd::OnProperties2()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します
}

void CPropertiesWnd::InitPropList()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	//設定値
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("設定値"));
	m_wndPropList.AddProperty(pGroup1);
	CString strPath;
	strPath.Format(L"%s\\%s",theApp.GetAppPath(), g_lpszIniFileName);
	LoadIniFile(strPath,  pGroup1);

	//コマンド
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("コマンド"));
	m_wndPropList.AddProperty(pCommand);

	CSortStringArray aryCommandPath;
	ReadCommandMenu(aryCommandPath); //コマンド一覧読み込み

	for (int i = 0; i < aryCommandPath.GetSize(); i++) {
		CString strName = GetFileName(aryCommandPath[i]); //コマンドファイルタイトル
		ButtonProprerty *pStr = new ButtonProprerty(strName, (variant_t)_T("実行中"), _T(""));
		pStr->AllowEdit(FALSE);
		m_wndPropList.AddProperty(pStr);
	}	
}

//名前取得
CString CPropertiesWnd::GetName(int nIndex,CString &strPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));
	CString strKey;
	strKey.Format(L"inputname%d", nIndex);
	::GetPrivateProfileString(L"GUIBAR", strKey, L"", szBuf, sizeof(szBuf), strPath);
	CString strName = szBuf;
	return strName;
}
//種別取得
CString CPropertiesWnd::GetType(int nIndex, CString &strPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));
	CString strKey;
	strKey.Format(L"input_type%d",nIndex);
	::GetPrivateProfileString(L"GUIBAR", strKey, L"", szBuf, sizeof(szBuf), strPath);
	CString strType = szBuf;
	return strType;
}
//最小値
CString CPropertiesWnd::GetMin(int nIndex, CString &strPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));
	CString strKey;
	strKey.Format(L"input_min%d",nIndex);
	::GetPrivateProfileString(L"GUIBAR", strKey, L"", szBuf, sizeof(szBuf), strPath);
	CString strMin = szBuf;
	return strMin;
}

//最大値
CString CPropertiesWnd::GetMax(int nIndex, CString &strPath)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));
	CString strKey;
	strKey.Format(L"input_max%d",nIndex);
	::GetPrivateProfileString(L"GUIBAR", strKey, L"", szBuf, sizeof(szBuf), strPath);
	CString strMax = szBuf;
	return strMax;
}

//種別はintですか？
BOOL CPropertiesWnd::IsTypeInt(int nIndex, CString &strPath)
{
	CString strType = GetType(nIndex, strPath);
	return (strType == L"int" || strType == L"INT");
}
//名前検索
int CPropertiesWnd::FindName(CString &strFindName, CString &strPath)
{
	CString strName;
	for (int i = 0; ; i++) {
		strName = GetName(i, strPath);
		if (strName.GetLength() <= 0) { //検索名なし
			break;
		}
		if (strFindName == strName) {//発見
			return i;
		}
	}
	return -1;
}
//.iniファイルの読み込み
void CPropertiesWnd::LoadIniFile(CString& strPath, CMFCPropertyGridProperty* pGroup)
{
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf, sizeof(szBuf));
	//.inifileの読み込み	
	CString strKey;
	for (int i = 0; ; i++) {
		//入力値取得
		strKey.Format(L"inputvalue%d", i);
		ZeroMemory(szBuf, sizeof(szBuf));
		if (::GetPrivateProfileString(L"GUIBAR", strKey, L"", szBuf, sizeof(szBuf), strPath) > 0) {//入力の存在
			CString strVal = szBuf;

			//名前取得
			CString strName = GetName(i,strPath);

			//種別
			App *pApp = App::GetInstance();			
			if ( IsTypeInt(i, strPath) ) {
				pApp->SetNum(strName, _wtoi(strVal)); //int value set
			}
			else {
				pApp->SetDbl(strName, _wtof(strVal)); //double value set
			}
			CString strMin = GetMin(i, strPath); //最小値
			CString strMax = GetMin(i, strPath);//最大値
			CMFCPropertyGridProperty *pVal = NULL;
			if (IsTypeInt(i, strPath)) {
				pVal = new CMFCPropertyGridProperty(strName, (variant_t)(int)_wtoi(strVal), L"");
				pVal->EnableSpinControl(TRUE, _wtoi(strMin), _wtoi(strMax));
			}
			else {//実数型のプロパティ
				pVal = new CMFCPropertyGridProperty(strName, (variant_t)(double)_wtof(strVal), L"");
			}			
			if (pVal != NULL) {
				pGroup->AddSubItem(pVal);
			}
		}
		else { //終了
			break;
		}
	}
}





void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}


LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam)
{
	CMFCPropertyGridProperty * pProperty = (CMFCPropertyGridProperty *)lparam;
	if (pProperty != NULL) {
		App *pApp = App::GetInstance();
		CString strPath;
		strPath.Format(L"%s\\%s", theApp.GetAppPath(), g_lpszIniFileName);

		CString strName = pProperty->GetName(); //名前取得
		int nIndex = FindName(strName,strPath);
		if( nIndex >= 0){
			if (IsTypeInt(nIndex, strPath)) { // int
				int nVal = pProperty->GetValue().iVal;

				CString strMin = GetMin(nIndex, strPath); //最小値
				CString strMax = GetMax(nIndex, strPath);//最大値

				int nMin = _wtoi(strMin);
				int nMax = _wtoi(strMax);
				if (nVal < nMin) {//最小値より下
					::ErrorMsg(L"最小値より小さな値が入力されました。");
				}
				else if (nMax < nVal) { //最大値超え
					::ErrorMsg(L"最大値より大きな値が入力されました。");
				}
				else { //変更問題なし
					CString strKey;
					strKey.Format(L"inputvalue%d", nIndex);
					::WritePrivateProfileInt(L"GUIBAR", strKey, nVal, strPath);
					pApp->SetNum(strName, nVal);
				}
			}
			else {//double
				double dbVal = pProperty->GetValue().dblVal;
				CString strMin = GetMin(nIndex, strPath); //最小値
				CString strMax = GetMax(nIndex, strPath);//最大値
				double dbMin = _wtof(strMin);
				double dbMax = _wtof(strMax);
				if (dbVal < dbMin) {//最小値より下
					::ErrorMsg(L"最小値より小さな値が入力されました。");
				}
				else if (dbMax < dbVal) { //最大値超え
					::ErrorMsg(L"最大値より大きな値が入力されました。");
				}
				else { //変更問題なし
					CString strKey;
					strKey.Format(L"inputvalue%d", nIndex);
					::WritePrivateProfileDbl(L"GUIBAR", strKey, L"%.10f", dbVal, strPath);
					pApp->SetDbl(strName, dbVal);
				}
			}
		}
	}
	return 0;
}