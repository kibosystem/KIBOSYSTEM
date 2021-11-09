#include "../stdafx.h"
#include "../KIBO.h"
#include "../SCript/App.h"
#include "RegistryDlg.h"

// RegistryDlg �_�C�A���O

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


// RegistryDlg ���b�Z�[�W �n���h��

BOOL RegistryDlg::OnInitDialog()
{
	App *pApp = App::GetInstance(); //�A�v���P�[�V�����X�N���v�g�N���X

	CDialog::OnInitDialog();

	m_ctrlList.InsertColumn(0,_T("����"),LVCFMT_RIGHT,100); 
	m_ctrlList.InsertColumn(1,_T("�l")  ,LVCFMT_LEFT ,200); 

	CStringArray strItemInt;	//���W�X�g���l���ڈꗗ
	CollectItemInt(strItemInt);
	CStringArray strItemString; //���W�X�g�������񍀖ڈꗗ
	CollectItemString(strItemString);

    // �f�[�^�s�̒ǉ�
	CString strValue;
	for(int i=0;i<strItemInt.GetSize();i++){
		m_ctrlList.InsertItem(i,strItemInt[i]);
		int nValue = pApp->GetProfileInt(strItemInt[i]);
		strValue.Format(L"%d",nValue);
		m_ctrlList.SetItemText(i,1,strValue);
	}
	int nAdd = strItemInt.GetSize() + 1; //�ǉ��ʒu
	for(int i=0;i<strItemString.GetSize();i++){
		m_ctrlList.InsertItem(nAdd+i,strItemString[i]);
		CString strValue = pApp->GetProfileString(strItemInt[i]);
		m_ctrlList.SetItemText(nAdd+i,1,strValue);
	}
	return TRUE;	
}

//���W�X�g���l���ڈꗗ���W
void RegistryDlg::CollectItemInt(CStringArray &strItem)
{
	//���W�X�g���擾
	HKEY hKey;
	CString strEntry;
	CString strValue;
	DWORD dwType;
	TCHAR szEntry[MAX_PATH];
	BYTE byVal[MAX_PATH];
	if(RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\KIBO\\KIBO\\INT", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
		long lResult = ERROR_SUCCESS;
		for(int i=0;lResult == ERROR_SUCCESS;i++){
			ZeroMemory(szEntry,sizeof(szEntry));//�o�b�t�@�N���A
			DWORD dwLen = sizeof(szEntry); //�o�b�t�@���Z�b�g
			ZeroMemory(byVal,sizeof(byVal));
			DWORD dwVal = sizeof(byVal); //�o�b�t�@���Z�b�g
            lResult= RegEnumValue(
                hKey,               //�L�[�̃n���h��
                i,					//�C���f�b�N�X�ԍ�
                szEntry,            //�G���g�������i�[�����̈�
                &dwLen,             //buf �̃T�C�Y
                NULL,               //�\��ς�
                &dwType,            //�f�[�^�̃^�C�v
                (BYTE *)byVal,      //�󂯎��̈�
                &dwVal);           //�̈�̃o�C�g��(�󂯎�����o�C�g��
			if( lResult == ERROR_SUCCESS ){
				strEntry = szEntry;
				strItem.Add(strEntry);
			}
		}
		RegCloseKey(hKey);
	}
}

//���W�X�g�������񍀖ڈꗗ���W
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
			ZeroMemory(szEntry,sizeof(szEntry));//�o�b�t�@�N���A
			DWORD dwLen = sizeof(szEntry); //�o�b�t�@���Z�b�g
			ZeroMemory(byVal,sizeof(byVal));
			DWORD dwVal = sizeof(byVal); //�o�b�t�@���Z�b�g
            lResult= RegEnumValue(
                hKey,               //�L�[�̃n���h��
                i,					//�C���f�b�N�X�ԍ�
                szEntry,            //�G���g�������i�[�����̈�
                &dwLen,             //buf �̃T�C�Y
                NULL,               //�\��ς�
                &dwType,            //�f�[�^�̃^�C�v
                (BYTE *)byVal,      //�󂯎��̈�
                &dwVal);           //�̈�̃o�C�g��(�󂯎�����o�C�g��

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
	GetHitCell(nRow,nCol);//Click���W���擾
//	m_ctrlList.
	m_edit.ShowWindow(SW_SHOW);	
	*pResult = 0;
}
////�Z���w��̈�擾
//CRect ListCtrlPlus::GetCellRect(int nRow,int nCol)
//{
//	CRect rect(0,0,0,0);
//	if(nRow < GetItemCount() && nCol < GetColCount() ){ //�����`�F�b�N
//		if( GetItemRect(nRow, &rect, LVIR_BOUNDS) ){	//�̈�擾
//			int nLeft = 0;
//			for(int i=0;i<nCol;i++){ //���[���W�擾
//				nLeft += GetColumnWidth(i);
//			}
//			int nRight = nLeft + GetColumnWidth(i);//�E�[���W�擾
//			rect.left  = nLeft;
//			rect.right = nRight;
//		}
//	}
//	CRect rcClient;//���X�g�R���g���[���S��
//	GetClientRect(&rcClient);
//	return rect & rcClient;
//}


//Click���W���擾
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
		if( ptMouse.x < nWidth ){ //��������
			nCol = n;
			break;
		}
	}
	return nRow;
}
