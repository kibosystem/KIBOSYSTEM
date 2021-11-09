#include "../stdafx.h"
#include "ScriptBase.h"
#include "../LIB/functions.h"
#include "../LIB/WinAppPlus.h"

//Variant型を強制的にint変換
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

//Variant型を強制的にdoubleに変換
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

//コンストラクタ
ScriptBase::ScriptBase()
:refCount(1)
{
}

//デストラクタ
ScriptBase::~ScriptBase()
{

}
//指定されたクラスの実体を作成
ScriptBase* ScriptBase::CreateInstance(CString& strName)
{
	ScriptBase *pInstance = NULL;
	WinAppPlus *pWinApp = GetApp();
	if( pWinApp != NULL ){
		pInstance = pWinApp->CreateInstanceScript(strName);
	}
	return pInstance;	
}

//関数名チェック
int ScriptBase::GetIDFromName(CString strName,CommandTable table[])
{
	for(int i=0;table[i].m_lpszName != NULL;i++){ //終端までループ
		if( strName == table[i].m_lpszName ){ //同一名称発見
			return table[i].m_nID;		//ID番号指定
		}
	}
	return 0;
}

//BOOL値を返します
BOOL ScriptBase::ReturnBool(WORD wFlags,VARIANT *pVarResult,BOOL isOK)
{
	if( IsPropertyGet(wFlags) ){ //戻り値があるときだけ
		pVarResult->vt = VT_BOOL;
		pVarResult->boolVal = isOK;
		return TRUE;
	}
	return FALSE;
}

//Int値を返します
BOOL ScriptBase::ReturnInt(WORD wFlags,VARIANT *pVarResult,int nVal)
{
	if( IsPropertyGet(wFlags) ){ //戻り値があるときだけ
		pVarResult->vt = VT_INT;
		pVarResult->intVal = nVal;
		return TRUE;
	}
	return FALSE;
}

//UINT値を返します
BOOL ScriptBase::ReturnUint(WORD wFlags,VARIANT *pVarResult,UINT unVal)
{
	if( IsPropertyGet(wFlags) ){ //戻り値があるときだけ
		pVarResult->vt = VT_UINT;
		pVarResult->intVal = unVal;
		return TRUE;
	}
	return FALSE;
}

//Double値を返します
BOOL ScriptBase::ReturnDbl(WORD wFlags,VARIANT *pVarResult,double dbVal)
{
	if( IsPropertyGet(wFlags) ){ //戻り値があるときだけ
		pVarResult->vt = VT_R8;
		pVarResult->dblVal = dbVal;
		return TRUE;
	}
	return FALSE;
}

//文字列を戻します
BOOL ScriptBase::ReturnString(WORD wFlags,VARIANT *pVarResult,CString &strRecv)
{
	if( IsPropertyGet(wFlags) ){ //戻り値があるときだけ		
		pVarResult->vt = VT_BSTR;
		pVarResult->bstrVal = strRecv.AllocSysString();
		TRACE("AllocSysStringが実行されました。\n");
		return TRUE;
	}
	return FALSE;
}

//引数の数チェック
BOOL ScriptBase::IsArgOK(DISPPARAMS *pDispParams,CommandTable *pCmd)
{
	if( pCmd != NULL && pCmd->m_nArgs >= 0  && pDispParams->cArgs != pCmd->m_nArgs ){  //引数の数が決定されている時だけ
		//::ErrorMsg(_T("引数の数に間違いがあります,%s:必要数%d->指定数%d"),pCmd->m_lpszName,pCmd->m_nArgs,pDispParams->cArgs);
		::ErrorMsg(L"スクリプトファイルに間違いがあります。\nコマンド(%s)",pCmd->m_lpszName);
		return FALSE;
	}
	return TRUE;
}

//指定ID番号のコマンドを取得
CommandTable* ScriptBase::GetTableFromID(CommandTable table[],int nID)
{
	for(int i=0;table[i].m_lpszName != NULL ;i++){ 
		if( table[i].m_nID == nID ){
			return &table[i];
		}
	}
	return NULL;
}