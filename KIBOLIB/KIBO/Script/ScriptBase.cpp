#include "../stdafx.h"
#include "ScriptBase.h"
#include "../LIB/functions.h"
#include "../LIB/WinAppPlus.h"

//Variant�^�������I��int�ϊ�
int GetValientInt(LPVARIANT psrcVal)
{
	int nRet = 0;
	VARIANT dstVariant;
	VariantInit(&dstVariant);
	VariantChangeType(&dstVariant,psrcVal,0,VT_INT);
	nRet = dstVariant.intVal;
	VariantClear(&dstVariant); 
	return nRet;
}

//Variant�^�������I��double�ɕϊ�
double GetValientDbl(LPVARIANT psrcVal)
{
	double dbRet = 0;
	VARIANT dstVariant;
	VariantInit(&dstVariant);
	VariantChangeType(&dstVariant,psrcVal,0,VT_R8);
	dbRet = dstVariant.dblVal;
	VariantClear(&dstVariant); 
	return dbRet;
}

//�R���X�g���N�^
ScriptBase::ScriptBase()
:refCount(1)
{
}

//�f�X�g���N�^
ScriptBase::~ScriptBase()
{

}
//�w�肳�ꂽ�N���X�̎��̂��쐬
ScriptBase* ScriptBase::CreateInstance(CString& strName)
{
	ScriptBase *pInstance = NULL;
	WinAppPlus *pWinApp = GetApp();
	if( pWinApp != NULL ){
		pInstance = pWinApp->CreateInstanceScript(strName);
	}
	return pInstance;	
}

//�֐����`�F�b�N
int ScriptBase::GetIDFromName(CString strName,CommandTable table[])
{
	for(int i=0;table[i].m_lpszName != NULL;i++){ //�I�[�܂Ń��[�v
		if( strName == table[i].m_lpszName ){ //���ꖼ�̔���
			return table[i].m_nID;		//ID�ԍ��w��
		}
	}
	return 0;
}

//BOOL�l��Ԃ��܂�
BOOL ScriptBase::ReturnBool(WORD wFlags,VARIANT *pVarResult,BOOL isOK)
{
	if( IsPropertyGet(wFlags) ){ //�߂�l������Ƃ�����
		pVarResult->vt = VT_BOOL;
		pVarResult->boolVal = isOK;
		return TRUE;
	}
	return FALSE;
}

//Int�l��Ԃ��܂�
BOOL ScriptBase::ReturnInt(WORD wFlags,VARIANT *pVarResult,int nVal)
{
	if( IsPropertyGet(wFlags) ){ //�߂�l������Ƃ�����
		pVarResult->vt = VT_INT;
		pVarResult->intVal = nVal;
		return TRUE;
	}
	return FALSE;
}

//UINT�l��Ԃ��܂�
BOOL ScriptBase::ReturnUint(WORD wFlags,VARIANT *pVarResult,UINT unVal)
{
	if( IsPropertyGet(wFlags) ){ //�߂�l������Ƃ�����
		pVarResult->vt = VT_UINT;
		pVarResult->intVal = unVal;
		return TRUE;
	}
	return FALSE;
}

//Double�l��Ԃ��܂�
BOOL ScriptBase::ReturnDbl(WORD wFlags,VARIANT *pVarResult,double dbVal)
{
	if( IsPropertyGet(wFlags) ){ //�߂�l������Ƃ�����
		pVarResult->vt = VT_R8;
		pVarResult->dblVal = dbVal;
		return TRUE;
	}
	return FALSE;
}

//�������߂��܂�
BOOL ScriptBase::ReturnString(WORD wFlags,VARIANT *pVarResult,CString &strRecv)
{
	if( IsPropertyGet(wFlags) ){ //�߂�l������Ƃ�����		
		pVarResult->vt = VT_BSTR;
		pVarResult->bstrVal = strRecv.AllocSysString();
		TRACE("AllocSysString�����s����܂����B\n");
		return TRUE;
	}
	return FALSE;
}

//�����̐��`�F�b�N
BOOL ScriptBase::IsArgOK(DISPPARAMS *pDispParams,CommandTable *pCmd)
{
	if( pCmd != NULL && pCmd->m_nArgs >= 0  && pDispParams->cArgs != pCmd->m_nArgs ){  //�����̐������肳��Ă��鎞����
		//::ErrorMsg(_T("�����̐��ɊԈႢ������܂�,%s:�K�v��%d->�w�萔%d"),pCmd->m_lpszName,pCmd->m_nArgs,pDispParams->cArgs);
		::ErrorMsg(L"�X�N���v�g�t�@�C���ɊԈႢ������܂��B\n�R�}���h(%s)",pCmd->m_lpszName);
		return FALSE;
	}
	return TRUE;
}

//�w��ID�ԍ��̃R�}���h���擾
CommandTable* ScriptBase::GetTableFromID(CommandTable table[],int nID)
{
	for(int i=0;table[i].m_lpszName != NULL ;i++){ 
		if( table[i].m_nID == nID ){
			return &table[i];
		}
	}
	return NULL;
}