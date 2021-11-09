#include "../stdafx.h"
#include "../KIBO.h"
#include "../SCript/App.h"
#include "RegistryDlg.h"

// RegistryDlg ダイアログ

IMPLEMENT_DYNAMIC(RegistryDlg, CDialog)

RegistryDlg::RegistryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RegistryDlg::IDD, pParent)
{

}

RegistryDlg::~RegistryDlg()
{
}

void RegistryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
	DDX_Control(pDX, IDC_EDIT, m_edit);
}


BEGIN_MESSAGE_MAP(RegistryDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &RegistryDlg::OnNMClickList)
END_MESSAGE_MAP()


// RegistryDlg メッセージ ハンドラ

BOOL RegistryDlg::OnInitDialog()
{
	App *pApp = App::GetInstance(); //アプリケーションスクリプトクラス

	CDialog::OnInitDialog();

	m_ctrlList.InsertColumn(0,_T("項目"),LVCFMT_RIGHT,100); 
	m_ctrlList.InsertColumn(1,_T("値")  ,LVCFMT_LEFT ,200); 

	CStringArray strItemInt;	//レジストリ値項目一覧
	CollectItemInt(strItemInt);
	CStringArray strItemString; //レジストリ文字列項目一覧
	CollectItemString(strItemString);

    // データ行の追加
	CString strValue;
	for(int i=0;i<strItemInt.GetSize();i++){
		m_ctrlList.InsertItem(i,strItemInt[i]);
		int nValue = pApp->GetProfileInt(strItemInt[i]);
		strValue.Format(L"%d",nValue);
		m_ctrlList.SetItemText(i,1,strValue);
	}
	int nAdd = strItemInt.GetSize() + 1; //追加位置
	for(int i=0;i<strItemString.GetSize();i++){
		m_ctrlList.InsertItem(nAdd+i,strItemString[i]);
		CString strValue = pApp->GetProfileString(strItemInt[i]);
		m_ctrlList.SetItemText(nAdd+i,1,strValue);
	}
	return TRUE;	
}

//レジストリ値項目一覧収集
void RegistryDlg::CollectItemInt(CStringArray &strItem)
{
	//レジストリ取得
	HKEY hKey;
	CString strEntry;
	CString strValue;
	DWORD dwType;
	TCHAR szEntry[MAX_PATH];
	BYTE byVal[MAX_PATH];
	if(RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\KIBO\\KIBO\\INT", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
		long lResult = ERROR_SUCCESS;
		for(int i=0;lResult == ERROR_SUCCESS;i++){
			ZeroMemory(szEntry,sizeof(szEntry));//バッファクリア
			DWORD dwLen = sizeof(szEntry); //バッファリセット
			ZeroMemory(byVal,sizeof(byVal));
			DWORD dwVal = sizeof(byVal); //バッファリセット
            lResult= RegEnumValue(
                hKey,               //キーのハンドル
                i,					//インデックス番号
                szEntry,            //エントリ名が格納される領域
                &dwLen,             //buf のサイズ
                NULL,               //予約済み
                &dwType,            //データのタイプ
                (BYTE *)byVal,      //受け取る領域
                &dwVal);           //領域のバイト数(受け取ったバイト数
			if( lResult == ERROR_SUCCESS ){
				strEntry = szEntry;
				strItem.Add(strEntry);
			}
		}
		RegCloseKey(hKey);
	}
}

//レジストリ文字列項目一覧収集
void RegistryDlg::CollectItemString(CStringArray &strItem)
{
	HKEY hKey;
	CString strEntry;
	CString strValue;
	DWORD dwType;
	TCHAR szEntry[MAX_PATH];
	BYTE byVal[MAX_PATH];
	if(RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\KIBO\\KIBO\\STRING", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
		long lResult = ERROR_SUCCESS;
		for(int i=0;lResult == ERROR_SUCCESS;i++){
			ZeroMemory(szEntry,sizeof(szEntry));//バッファクリア
			DWORD dwLen = sizeof(szEntry); //バッファリセット
			ZeroMemory(byVal,sizeof(byVal));
			DWORD dwVal = sizeof(byVal); //バッファリセット
            lResult= RegEnumValue(
                hKey,               //キーのハンドル
                i,					//インデックス番号
                szEntry,            //エントリ名が格納される領域
                &dwLen,             //buf のサイズ
                NULL,               //予約済み
                &dwType,            //データのタイプ
                (BYTE *)byVal,      //受け取る領域
                &dwVal);           //領域のバイト数(受け取ったバイト数

			if( lResult == ERROR_SUCCESS ){
				strEntry = szEntry;
				strItem.Add(strEntry);
			}
		}
		RegCloseKey(hKey);
	}
}

void RegistryDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nRow = 0;
	int nCol = 0;
	GetHitCell(nRow,nCol);//Click座標を取得
//	m_ctrlList.
	m_edit.ShowWindow(SW_SHOW);	
	*pResult = 0;
}
////セル指定領域取得
//CRect ListCtrlPlus::GetCellRect(int nRow,int nCol)
//{
//	CRect rect(0,0,0,0);
//	if(nRow < GetItemCount() && nCol < GetColCount() ){ //引数チェック
//		if( GetItemRect(nRow, &rect, LVIR_BOUNDS) ){	//領域取得
//			int nLeft = 0;
//			for(int i=0;i<nCol;i++){ //左端座標取得
//				nLeft += GetColumnWidth(i);
//			}
//			int nRight = nLeft + GetColumnWidth(i);//右端座標取得
//			rect.left  = nLeft;
//			rect.right = nRight;
//		}
//	}
//	CRect rcClient;//リストコントロール全体
//	GetClientRect(&rcClient);
//	return rect & rcClient;
//}


//Click座標を取得
int RegistryDlg::GetHitCell(int& nRow,int& nCol)
{
	CPoint ptMouse;
	::GetCursorPos(&ptMouse);
	m_ctrlList.ScreenToClient(&ptMouse);
	UINT unFlag = LVHT_ONITEM;
	nRow = m_ctrlList.HitTest(ptMouse,&unFlag);
	nCol = -1; //Default Value
	int nWidth = 0;
	for(int n=0;n<2;n++){
		nWidth += m_ctrlList.GetColumnWidth(n);
		if( ptMouse.x < nWidth ){ //見つかった
			nCol = n;
			break;
		}
	}
	return nRow;
}
