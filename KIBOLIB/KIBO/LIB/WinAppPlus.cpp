// WinAppPlus.cpp : �����t�@�C��
//

#include "../stdafx.h"
#include <afxsock.h>
#include <Iphlpapi.h>
#include "WinAppPlus.h"
#include "DataBuf.h"
#include "FilePlus.h"
#include "../MACADDRESS.h"
#include "functions.h"

#pragma comment(lib,"Iphlpapi.lib")

static DWORD GetVolumeID(TCHAR tcVolume)
{
	TCHAR szVolumeName[100];
	TCHAR szSystemName[100];
	DWORD dwSerialNumber = 0;
	DWORD dwFileNameLength = 0;
	DWORD dwFlags = 0;

	TCHAR szPath[4];
	szPath[0] = tcVolume;
	szPath[1] = ':';
	szPath[2] = '\\';
	szPath[3] = NULL;

	if( GetVolumeInformation(szPath,szVolumeName,100
							,&dwSerialNumber,&dwFileNameLength,&dwFlags
							,szSystemName,100) ){
		return dwSerialNumber;
	}
	return 0;
}


// WinAppPlus

IMPLEMENT_DYNCREATE(WinAppPlus, CWinAppEx)

WinAppPlus::WinAppPlus()
{
	m_hMutex = NULL;
	m_isEnd = false; //�I���t���O
}

WinAppPlus::~WinAppPlus()
{
}

//�J�����g�f�B���N�g���̐�΃p�X���擾
CString WinAppPlus::GetCurrentPath()
{
	TCHAR szBuf[MAX_PATH];
	GetCurrentDirectory(sizeof(szBuf),szBuf); //�J�����g�f�B���N�g���擾
	CString sPath(szBuf); //�ԋp�l
	return sPath; //�J�����g�f�B���N�g���ԋp
}

BOOL WinAppPlus::InitInstance()
{
	m_strAppPath = GetCurrentPath(); //�A�v���P�[�V�����N�����̃p�X�Z�b�g
	::AfxSocketInit(&m_wsaData);	//�\�P�b�g�ʐM�g�p�\
	::CoInitialize(NULL);			//COM�R���|�[�l���g�g�p�\

	srand( (unsigned)time( NULL ) ); //�����_�����l������	
	//���d�N������
	HANDLE hPrevMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,::AfxGetAppName()); 
	if( hPrevMutex ){ //�ȑO�̃A�v���P�[�V������Mutex���쐬���Ă���
		CloseHandle(hPrevMutex);
		return -1;
	}
	//Mutex�̍쐬
	m_hMutex = CreateMutex(FALSE,0,::AfxGetAppName());
	if (CWinAppEx::InitInstance()){
		return 1; //�m���ɂP�ɕϊ����Ė߂�
	}

	return 0;
}

int WinAppPlus::ExitInstance()
{
	::CoUninitialize();
	ReleaseMutex(m_hMutex);//Mutex�̉��
	return CWinAppEx::ExitInstance();
}

//PC��IP�A�h���X���擾���܂�
CString WinAppPlus::GetIPAddr()
{
	char szBuf[MAX_PATH];
	ZeroMemory(szBuf,sizeof(szBuf));
	gethostname(szBuf,sizeof(szBuf));
	hostent *pHostent = gethostbyname(szBuf);	
	CString sRet;
	if( pHostent->h_addr_list != NULL ){
		IN_ADDR inaddr;
		memcpy(&inaddr, pHostent->h_addr_list[0], 4);
		sRet = inet_ntoa(inaddr);
	}
	return sRet;
}

//Mac Address���擾���܂��B
BOOL WinAppPlus::SetMacAddress(CStringArray& astrMacAddress)
{
	astrMacAddress.RemoveAll();

	PMIB_IFTABLE	pIfTable;
	CString			strBuff;

	DWORD dwSize = 0;
	::GetIfTable(NULL,&dwSize,TRUE);			//�K�v�o�b�t�@�T�C�Y�擾

	BYTE *pBuff = new BYTE[dwSize];
	if( pBuff != NULL ){
		pIfTable = (PMIB_IFTABLE) pBuff;

		DWORD ret = ::GetIfTable(pIfTable,&dwSize,TRUE);
		if(ret == NO_ERROR)
		{
			for(UINT i = 0; i < pIfTable->dwNumEntries; i++)
			{
				strBuff.Format(L"%02X%02X%02X%02X%02X%02X"
						,pIfTable->table[i].bPhysAddr[0]
						,pIfTable->table[i].bPhysAddr[1]
						,pIfTable->table[i].bPhysAddr[2]
						,pIfTable->table[i].bPhysAddr[3]
						,pIfTable->table[i].bPhysAddr[4]
						,pIfTable->table[i].bPhysAddr[5]);
				astrMacAddress.Add(strBuff);
			}
		}
		delete	pBuff;
	}
	if(astrMacAddress.GetSize() > 0){
		return	true;
	}
	return	false;	
}

//�r�f�I�J�[�h�̊g��k���̋@�\���`�F�b�N���܂��B
BOOL WinAppPlus::CheckDevice()
{
	BOOL isOK = FALSE;
	HDC hdc = ::GetDC(NULL);
	isOK = ( ::GetDeviceCaps(hdc,RASTERCAPS) & RC_STRETCHBLT ); //�g��k���\�̓`�F�b�N
	ReleaseDC(NULL,hdc);
	return isOK;
}

//USB�������̃`�F�b�N�����܂��B
BOOL WinAppPlus::CheckUSBMemory()
{
	//if(IsEvaluateVersion()){
	//	::ErrorMsg(L"���̃A�v���P�[�V�����͕]���łł��B\n�]���ړI�ȊO�Ŏg�p���邱�Ƃ͏o���܂���B\n\n(C) 2009 KIBO Corporation.\nAll rights reserved.");
	//}
	//for(TCHAR tc='A';tc<='Z';tc++){//�S�h���C�u�`�F�b�N
	//	DWORD dwID = GetVolumeID(tc);
	//	int imax = sizeof(USBMEMORYID) / sizeof(DWORD); //�z���
	//	for(int i=0;i<imax;i++){
	//		if( dwID == USBMEMORYID[i]){ 
	//			return TRUE;//��v�����B
	//		}
	//	}
	//}
	return TRUE;
	//return FALSE;
}

BEGIN_MESSAGE_MAP(WinAppPlus, CWinAppEx)
END_MESSAGE_MAP()


