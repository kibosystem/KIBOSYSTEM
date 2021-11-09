#pragma once
#include "Iunknown.h"
#include "../COMMANDID.h"

//コマンドテーブル
struct CommandTable
{
	int		m_nID;		//コマンドID
	int		m_nArgs;	//引数の数
	LPWSTR	m_lpszName; //コマンド名称
};

//JScriptから呼び出されるクラス
class ScriptBase : public IDispatch 
{
public:
	IMPL_IUNKNOWN(IDispatch)
private:
	//static CArray<BSTR> s_aryBstr; //Script文字列
public:
	//コンストラクタ
	ScriptBase();
	//デストラクタ
	virtual ~ScriptBase();
	static ScriptBase* GetInstance();	
	static ScriptBase* CreateInstance(CString &strName);	
	//static void FreeBstr();

	// IDispatch
	virtual STDMETHODIMP GetTypeInfoCount(UINT __RPC_FAR *pctinfo) {
		return E_NOTIMPL;
	}
	virtual STDMETHODIMP GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo) {
		return E_NOTIMPL;
	}
	//必要な関数
	//関数名チェック
	int GetIDFromName(CString strName,CommandTable table[]);

	//関数名チェック
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames,UINT cNames ,LCID lcid,DISPID __RPC_FAR *rgDispId) = 0;
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID 番号
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script文字列
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr) = 0;

	BOOL IsPropertyGet(WORD wFlags){ //戻り値はありますか？
		return (wFlags & DISPATCH_PROPERTYGET); 
	}
	BOOL IsPropertyPut(WORD wFlags){ //値をセットする必要はありますか？
		return (wFlags & DISPATCH_PROPERTYPUT); 			
	}

	//BOOL値を返します
	BOOL ReturnBool(WORD wFlags,VARIANT *pVarResult,BOOL isOK);

	//int値を返します
	BOOL ReturnInt(WORD wFlags,VARIANT *pVarResult,int nVal);

	//UINT値を返します
	BOOL ReturnUint(WORD wFlags,VARIANT *pVarResult,UINT unVal);

	//Double値を返します
	BOOL ReturnDbl(WORD wFlags,VARIANT *pVarResult,double dbVal);

	//文字列を戻します
	BOOL ReturnString(WORD wFlags,VARIANT *pVarResult,CString &strRecv);

	//引数の数チェック
	BOOL IsArgOK(DISPPARAMS *pDispParams,CommandTable *pCmd);

	//指定ID番号のコマンドを取得
	CommandTable* GetTableFromID(CommandTable table[],int nID);
};

extern int GetValientInt(LPVARIANT psrcVal);	//Variant型を強制的にint変換
extern double GetValientDbl(LPVARIANT psrcVal); //Variant型を強制的にdoubleに変換
