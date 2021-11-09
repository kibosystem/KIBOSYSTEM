#pragma once

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"comsupp.lib")
#pragma comment(lib,"ole32.lib")
#include <tchar.h>
#include <windows.h>
#include <objbase.h>
#include <comutil.h>
#include <dispex.h>
#include <activscp.h>

// IUnknown インターフェイスの実装
// クラスのコンストラクタで refCount(1) として初期化すること。
#define IMPL_IUNKNOWN(IF_NAME) \
public:\
	STDMETHODIMP_(ULONG) AddRef() {\
		return ++refCount;\
	}\
	STDMETHODIMP_(ULONG) Release() {\
		--refCount;\
		if (refCount <= 0) {\
			delete this;\
			return 0;\
		}\
		return refCount;\
	}\
	STDMETHODIMP QueryInterface(const IID & riid,void **ppvObj) {\
		if (!ppvObj)\
			return E_POINTER;\
		if (IsEqualIID(riid, __uuidof(IF_NAME)) ||\
			IsEqualIID(riid, IID_IUnknown)) {\
			AddRef();\
			*ppvObj=this;\
			return S_OK;\
		}\
		return E_NOINTERFACE;\
	}\
private:\
	ULONG refCount;


