#pragma once

#include "ScriptBase.h"

//JScriptから呼び出されるクラス
class MsgBox : public ScriptBase 
{
public:

	enum{ //Function ID 番号
		 FUNC_SHOW = 1	//メッセージボックス表示
		,FUNC_MAX		//コマンド数
	};

public:
	//コンストラクタ
	MsgBox();

	//デストラクタ
	virtual ~MsgBox();

	static MsgBox* GetInstance();	

	//関数名チェック
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//実行
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID 番号
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script文字列
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);
};