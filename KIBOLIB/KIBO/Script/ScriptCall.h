#pragma once

#include "MsgBox.h"
#include "ScriptBase.h"

#pragma once


//スクリプト実行クラス
class ScriptCall : public IActiveScriptSite 
{
private:
	IMPL_IUNKNOWN(IActiveScriptSite)

public: //メンバ変数
	IActiveScript		*m_pAs;
	IActiveScriptParse	*m_pAsp;
	CString				m_strScript; //スクリプト実行文字列
	BOOL				m_isRuning; //スクリプトは実行中ですか？
public:
	//コンストラクタ
	ScriptCall();
	//デストラクタ
	virtual ~ScriptCall();
	// IActiveScriptSite
	STDMETHODIMP GetLCID(LCID*pLcid);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersionString);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	//エラー処理
	STDMETHODIMP OnScriptError(IActiveScriptError *pAse);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);

	//クラス作成要求
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppunkItem,ITypeInfo **ppTypeInfo);

	// ScriptCall
	void Prepare();
	void Run(CString &strScript);// スクリプト実行
	void Close();//後始末
};
