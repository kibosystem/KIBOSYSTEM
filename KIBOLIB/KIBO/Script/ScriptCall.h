#pragma once

#include "MsgBox.h"
#include "ScriptBase.h"

#pragma once


//�X�N���v�g���s�N���X
class ScriptCall : public IActiveScriptSite 
{
private:
	IMPL_IUNKNOWN(IActiveScriptSite)

public: //�����o�ϐ�
	IActiveScript		*m_pAs;
	IActiveScriptParse	*m_pAsp;
	CString				m_strScript; //�X�N���v�g���s������
	BOOL				m_isRuning; //�X�N���v�g�͎��s���ł����H
public:
	//�R���X�g���N�^
	ScriptCall();
	//�f�X�g���N�^
	virtual ~ScriptCall();
	// IActiveScriptSite
	STDMETHODIMP GetLCID(LCID*pLcid);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersionString);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	//�G���[����
	STDMETHODIMP OnScriptError(IActiveScriptError *pAse);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);

	//�N���X�쐬�v��
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppunkItem,ITypeInfo **ppTypeInfo);

	// ScriptCall
	void Prepare();
	void Run(CString &strScript);// �X�N���v�g���s
	void Close();//��n��
};
