#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBODoc.h"
#include "App.h"
#include "ScriptCall.h"
#include "Bitmap.h"
#include "UsbIO.h"
#include "Labeling.h"
#include "SerialPort.h"
#include "ImageDevice.h"
#include "CharRecog.h"

//�R���X�g���N�^
ScriptCall::ScriptCall()
: refCount(1)
{
	m_pAs  = NULL;
	m_pAsp = NULL;
	m_isRuning = FALSE;
}

//�f�X�g���N�^
ScriptCall::~ScriptCall() 
{
	Close();
}

// IActiveScriptSite
STDMETHODIMP ScriptCall::GetLCID(LCID*pLcid) 
{
	return E_NOTIMPL;
}

STDMETHODIMP ScriptCall::GetDocVersionString(BSTR *pbstrVersionString) 
{
	return E_NOTIMPL;
}

STDMETHODIMP ScriptCall::OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo) 
{
	return S_OK;
}

STDMETHODIMP ScriptCall::OnStateChange(SCRIPTSTATE ssScriptState) 
{
	return S_OK;
}

//�G���[����
STDMETHODIMP ScriptCall::OnScriptError(IActiveScriptError *pAse) 
{	
	if(pAse==NULL){
		return E_POINTER;
	}
	EXCEPINFO ei;
    HRESULT hr=pAse->GetExceptionInfo(&ei);
	if(SUCCEEDED(hr)){
		MessageBox(NULL, ei.bstrDescription, ei.bstrSource, MB_OK | MB_ICONSTOP);
	}
	return hr;
}
STDMETHODIMP ScriptCall::OnEnterScript(void) 
{
	return S_OK;
}
STDMETHODIMP ScriptCall::OnLeaveScript(void) 
{
	return S_OK;
}
//�N���X�쐬�v��
STDMETHODIMP ScriptCall::GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppunkItem,ITypeInfo **ppTypeInfo) 
{	
	if((dwReturnMask & SCRIPTINFO_IUNKNOWN)) {
		CString strName = pstrName;
		if(strName  == L"MsgBox"){
			*ppunkItem=(IUnknown*)new MsgBox();
			return S_OK;
		}
		else if(strName == L"ColorBitmap"){
			*ppunkItem = Bitmap24::CreateInstance(strName);
			return S_OK;
		}
		else if(strName == L"App"){
			*ppunkItem = App::CreateInstance(strName);
			if( *ppunkItem != NULL ){
				return S_OK;
			}
		}
		//else if(strName == L"Camera"){
		//	*ppunkItem=(IUnknown*)new Camera();
		//	return S_OK;
		//}
		else if(strName == L"Labeling"){
			*ppunkItem=(IUnknown*)new Labeling();
			return S_OK;
		}
		else if(strName == L"SerialPort"){ //RS232C
			*ppunkItem=(IUnknown*)new SerialPort();
			return S_OK;
		}
		else if(strName == L"ImageDevice"){ //�摜���͋@��
			*ppunkItem = ImageDevice::CreateInstance(strName);
			if( *ppunkItem != NULL ){
				return S_OK;
			}
		}
#ifdef USE_CHARRECOG
		else if(strName == L"CharRecog"){ //�����F���N���X
			*ppunkItem=(IUnknown*)new CharRecog();
			return S_OK;
		}
#endif
#ifdef USE_USBIO
		else if(strName == L"UsbIO"){ //UsbIO�N���X
			*ppunkItem=(IUnknown*)new UsbIO();
			return S_OK;
		}
#endif
	}
	return TYPE_E_ELEMENTNOTFOUND;
}

// �X�N���v�g�Ăяo���O����
void ScriptCall::Prepare()
{
	CLSID clsid;
	CLSIDFromProgID(L"JScript", &clsid );
	CoCreateInstance(clsid, 0, CLSCTX_ALL, IID_IActiveScript, (void **)&m_pAs);
	if( m_pAs != NULL ){
		m_pAs->SetScriptSite(this);
		m_pAs->QueryInterface(IID_IActiveScriptParse, (void **)&m_pAsp);
		if( m_pAsp != NULL ){
			m_pAsp->InitNew();
			//�X�N���v�g����Ăяo���\�ȃN���X�̍쐬
			m_pAs->AddNamedItem(L"MsgBox"	 ,SCRIPTITEM_ALL_FLAGS);
			m_pAs->AddNamedItem(L"App"		 ,SCRIPTITEM_ALL_FLAGS);
			m_pAs->AddNamedItem(L"Labeling"	 ,SCRIPTITEM_ALL_FLAGS);
			m_pAs->AddNamedItem(L"SerialPort",SCRIPTITEM_ALL_FLAGS);
			m_pAs->AddNamedItem(L"ImageDevice",SCRIPTITEM_ALL_FLAGS);
			m_pAs->AddNamedItem(L"ColorBitmap",SCRIPTITEM_ALL_FLAGS);

#ifdef USE_CHARRECOG
			m_pAs->AddNamedItem(L"CharRecog" ,SCRIPTITEM_ALL_FLAGS);
#endif
#ifdef USE_USBIO
			m_pAs->AddNamedItem(L"UsbIO"	 ,SCRIPTITEM_ALL_FLAGS);
#endif
		}
	}
}

// �X�N���v�g���s
void ScriptCall::Run(CString &strScript) 
{
	m_isRuning = TRUE;
	if( m_pAsp != NULL ){
		m_pAsp->ParseScriptText(strScript,NULL,NULL,NULL,0,0,SCRIPTTEXT_ISPERSISTENT,NULL,NULL);
	}
	if( m_pAs != NULL ){
		m_pAs->SetScriptState(SCRIPTSTATE_CONNECTED);
	}
	m_isRuning = FALSE;
	if( m_pAs != NULL ){
		m_pAs->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	}
}

//��n��
void ScriptCall::Close() 
{
	if( m_pAs != NULL ){
		//m_pAs->SetScriptState(SCRIPTSTATE_CLOSED);
		m_pAs->Close();
		m_pAs->Release();
		m_pAs = NULL;
	}
	if( m_pAsp != NULL ){
		m_pAsp->Release();
		m_pAsp = NULL;
	}
}
