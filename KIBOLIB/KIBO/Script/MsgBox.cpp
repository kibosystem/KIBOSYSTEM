#include "../stdafx.h"
#include "MsgBox.h"

static MsgBox *s_pInstance = NULL; //唯一のクラス実体
MsgBox* MsgBox::GetInstance()//実体取得
{
	return s_pInstance;
}

//コンストラクタ
MsgBox::MsgBox()
{
	s_pInstance = this;
}

//デストラクタ
MsgBox::~MsgBox() 
{
}

static CommandTable s_table[] =
{
	 {MsgBox::FUNC_SHOW,-1,L"Show"}			//表示作成
	,{MsgBox::FUNC_MAX , 0,NULL}			//コマンド数
};

//関数名チェック
STDMETHODIMP MsgBox::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId)
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//処理実行
STDMETHODIMP MsgBox::Invoke(DISPID dispIdMember	//Function ID 番号
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script文字列
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr) 
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //引数チェック
		return S_FALSE;
	}		
	switch(dispIdMember){
		case FUNC_SHOW: //メッセージボックス表示
			if(pDispParams->cArgs==1) {
				_variant_t str(pDispParams->rgvarg);
				str.ChangeType(VT_BSTR);
				MessageBox(NULL,str.bstrVal,TEXT(""),MB_OK);
				return S_OK;
			}else if(pDispParams->cArgs==2) {
				_variant_t str(pDispParams->rgvarg[1]);
				_variant_t subject(pDispParams->rgvarg[0]);
				str.ChangeType(VT_BSTR);
				subject.ChangeType(VT_BSTR);
				MessageBox(NULL,str.bstrVal,subject.bstrVal,MB_OK);
				return S_OK;
			}
			break;
	}

	return S_FALSE;
}