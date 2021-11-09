// KIBODoc.cpp : CKIBODoc �N���X�̎���
//

#include "stdafx.h"
#include "enum.h"
#include "KIBO.h"
#include "KIBODoc.h"
#include "./LIB/fileplus.h"
#include "./Script/ImageDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static TCHAR s_lpwszInclude[] = _T("#include");

// CKIBODoc

IMPLEMENT_DYNCREATE(CKIBODoc, CDocument)

BEGIN_MESSAGE_MAP(CKIBODoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKIBODoc, CDocument)
END_DISPATCH_MAP()

// ����: VBA ����^�C�v �Z�[�t�ȃo�C���h���T�|�[�g���邽�߂ɁAIID_IKIBO �̃T�|�[�g��ǉ����܂��B
//  ���� IID �́A.IDL �t�@�C���̃f�B�X�p�b�` �C���^�[�t�F�C�X�փA�^�b�`����� 
//  GUID �ƈ�v���Ȃ���΂Ȃ�܂���B

// {415E9623-5D0C-4C33-BA7A-4B83B246CA24}
static const IID IID_IKIBO =
{ 0x415E9623, 0x5D0C, 0x4C33, { 0xBA, 0x7A, 0x4B, 0x83, 0xB2, 0x46, 0xCA, 0x24 } };

BEGIN_INTERFACE_MAP(CKIBODoc, CDocument)
	INTERFACE_PART(CKIBODoc, IID_IKIBO, Dispatch)
END_INTERFACE_MAP()

static CKIBODoc* s_pCKIBODoc = NULL;
CKIBODoc* GetDoc(){return s_pCKIBODoc;}

// CKIBODoc �R���X�g���N�V����/�f�X�g���N�V����

CKIBODoc::CKIBODoc()
{
	s_pCKIBODoc = this;

	EnableAutomation();
	AfxOleLockApp();
	m_Script.Prepare(); //�R�}���h���s
}

CKIBODoc::~CKIBODoc()
{	
	AfxOleUnlockApp();
}

BOOL CKIBODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()){
		return FALSE;
	}
	return TRUE;
}

void CKIBODoc::OnCloseDocument()
{
	m_Script.Close();//�ȑO�̃R�}���h���~
	CDocument::OnCloseDocument();
}

//�X�N���v�g�͎��s���ł����H
BOOL CKIBODoc::IsScriptRuning()
{ 
	return m_Script.m_isRuning;
}

//�X�N���v�g�̎��s
void CKIBODoc::Execute()
{ 
	m_Script.Run( m_strScript );	
}

//�X�N���v�g�����I��
void CKIBODoc::ScriptClose(){ 
	m_Script.Close(); 
}


//�R�}���h�z��ɃX�N���v�g�R�}���h����������W
BOOL CKIBODoc::CollectScriptCommand(CString& strFilePath,CStringArray &strAryCommand)
{
	strAryCommand.RemoveAll();

	CFileFind find;
	if( !find.FindFile(strFilePath) ){ //�t�@�C���̑��݊m�F
		return FALSE;
	}

	//�X�N���v�g�t�@�C���ǂݍ���
	FilePlus file;
	if( file.Open(strFilePath, FilePlus::modeUnicodeIn) ){
		BOOL isStartOK = FALSE;
		CString strLine;
		while( file.ReadString(strLine) ){
			if( strLine.Find(L"START:") >= 0){//�X�N���v�g�J�n
				isStartOK = TRUE;
				break;
			}
		}
		if( !isStartOK ){ //�X�N���v�g���݊m�F
			//::ErrorMsg(_T("START:�^�O���m�F���Ă�������\n%s"),strFilePath);
			::ErrorMsg(_T("�X�N���v�g�t�@�C��\n%s\n���������Ă��������B"),strFilePath);
		}
		m_strScript.Empty();
		//�X�N���v�g�t�@�C���ǂݍ���
		while( file.ReadString(strLine) ){			
			if(strLine.Find(L"END:") >= 0){ //�X�N���v�g�I��
				break;
			}
			CString strCommand;
			if( DeleteComment( strLine,strCommand ) ){ //�R�����g�폜������̂ݒ��o
				strAryCommand.Add(strCommand);
			}
		}
		return TRUE;
	}	
	return FALSE;
}

//Include�t�@�C�������擾
CString CKIBODoc::GetIncludeFile(CString &strLine)
{
	int nStart = -1;	//�t�@�C�����J�n�ʒu
	int nEnd = -1;		//�t�@�C�����I���ʒu
	for(int i=0;i<strLine.GetLength();i++){
		TCHAR tc = strLine.GetAt(i); //1�����擾
		if( tc == '"' ){
			if( nStart < 0 ){ 
				nStart = i+1; //�t�@�C�����J�n�ʒu�擾
			}else{
				nEnd = i-1;
				break;
			}
		}
	}
	if( nStart > 0 && nEnd > 0 ){
		return strLine.Mid(nStart,nEnd-nStart+1);
	}
	return L"";
}

//Script File ���s
BOOL CKIBODoc::ScriptFileOpen(CString& strFilePath)
{
	if( m_strPrevFilePath != strFilePath ){ //�ȑO�ǂݍ��񂾃t�@�C���ƈႤ������
		//��m_strScript�̍쐬
		CStringArray strAryCommand; //�R�}���h�z��
		if( !CollectScriptCommand(strFilePath,strAryCommand) ){ //�R�}���h�z��ɃX�N���v�g�R�}���h����������W
			::ErrorMsg(_T("�X�N���v�g�t�@�C��%s�̓ǂݍ��݂Ɏ��s���܂����B"),strFilePath);
			return FALSE;
		}
		CString strLine;
		//include����
		CStringArray strAryInclude; //Include�t�@�C���R�}���h
		for(int i=0;i<strAryCommand.GetSize();i++){
			strLine = strAryCommand[i];
			if( strLine.Find(s_lpwszInclude) >= 0){ //�����񍷂��ւ��̕K�v����
				strFilePath = GetIncludeFile(strLine); //Include�t�@�C�������擾
				if( !CollectScriptCommand(strFilePath,strAryInclude) ){ //�R�}���h�z��ɃX�N���v�g�R�}���h����������W
					::ErrorMsg(_T("�X�N���v�g�t�@�C��%s�̓ǂݍ��݂Ɏ��s���܂����B"),strFilePath);
					return FALSE;
				}
				//include�t�@�C���ǂݍ��ݐ���
				strAryCommand.InsertAt(i+1,&strAryInclude); //�R�}���h������}��
				i += (int)strAryInclude.GetSize();
			}
		}	
		//�X�N���v�g������̍쐬
		for(int i=0;i<strAryCommand.GetSize();i++){
			//TRACE(_T("�ŏI������:%s\n"),strAryCommand[i]);
			if( strAryCommand[i].Find(s_lpwszInclude) >= 0){ //��x�ڂ�include�͍폜
				continue; //�R�����g����
			}
			if( DeleteComment( strAryCommand[i],strLine ) ){ //�R�����g�폜������̂ݒ��o
				m_strScript += strLine;
			}
		}
		m_strPrevFilePath = strFilePath; //�ȑO�̃t�@�C���p�X���L��
	}
	//�X�N���v�g���s
	Execute(); //���s
	return TRUE;
}

//�R�����g�폜�������n��
BOOL CKIBODoc::DeleteComment( CString &strIn,CString &strOut )
{
	strIn = strIn.Trim(); //�󔒍폜
	int nCmt = strIn.Find(L"//"); //�R�����g����
	if( nCmt > 0 ){ //�R�����g���폜
		strIn = strIn.Left(nCmt);
	}else if( nCmt == 0 ){
		return FALSE; //�R�����g�s
	}
	strOut = strIn; //�R�����g�폜������쐬
	return TRUE;
}
