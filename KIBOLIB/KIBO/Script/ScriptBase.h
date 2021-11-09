#pragma once
#include "Iunknown.h"
#include "../COMMANDID.h"

//�R�}���h�e�[�u��
struct CommandTable
{
	int		m_nID;		//�R�}���hID
	int		m_nArgs;	//�����̐�
	LPWSTR	m_lpszName; //�R�}���h����
};

//JScript����Ăяo�����N���X
class ScriptBase : public IDispatch 
{
public:
	IMPL_IUNKNOWN(IDispatch)
private:
	//static CArray<BSTR> s_aryBstr; //Script������
public:
	//�R���X�g���N�^
	ScriptBase();
	//�f�X�g���N�^
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
	//�K�v�Ȋ֐�
	//�֐����`�F�b�N
	int GetIDFromName(CString strName,CommandTable table[]);

	//�֐����`�F�b�N
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames,UINT cNames ,LCID lcid,DISPID __RPC_FAR *rgDispId) = 0;
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID �ԍ�
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script������
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr) = 0;

	BOOL IsPropertyGet(WORD wFlags){ //�߂�l�͂���܂����H
		return (wFlags & DISPATCH_PROPERTYGET); 
	}
	BOOL IsPropertyPut(WORD wFlags){ //�l���Z�b�g����K�v�͂���܂����H
		return (wFlags & DISPATCH_PROPERTYPUT); 			
	}

	//BOOL�l��Ԃ��܂�
	BOOL ReturnBool(WORD wFlags,VARIANT *pVarResult,BOOL isOK);

	//int�l��Ԃ��܂�
	BOOL ReturnInt(WORD wFlags,VARIANT *pVarResult,int nVal);

	//UINT�l��Ԃ��܂�
	BOOL ReturnUint(WORD wFlags,VARIANT *pVarResult,UINT unVal);

	//Double�l��Ԃ��܂�
	BOOL ReturnDbl(WORD wFlags,VARIANT *pVarResult,double dbVal);

	//�������߂��܂�
	BOOL ReturnString(WORD wFlags,VARIANT *pVarResult,CString &strRecv);

	//�����̐��`�F�b�N
	BOOL IsArgOK(DISPPARAMS *pDispParams,CommandTable *pCmd);

	//�w��ID�ԍ��̃R�}���h���擾
	CommandTable* GetTableFromID(CommandTable table[],int nID);
};

extern int GetValientInt(LPVARIANT psrcVal);	//Variant�^�������I��int�ϊ�
extern double GetValientDbl(LPVARIANT psrcVal); //Variant�^�������I��double�ɕϊ�
