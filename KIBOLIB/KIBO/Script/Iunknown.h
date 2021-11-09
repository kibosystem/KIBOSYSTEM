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

// IUnknown �C���^�[�t�F�C�X�̎���
// �N���X�̃R���X�g���N�^�� refCount(1) �Ƃ��ď��������邱�ƁB
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


