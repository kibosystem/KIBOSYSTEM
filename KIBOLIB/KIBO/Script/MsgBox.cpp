#include "../stdafx.h"
#include "MsgBox.h"

static MsgBox *s_pInstance = NULL; //�B��̃N���X����
MsgBox* MsgBox::GetInstance()//���̎擾
{
	return s_pInstance;
}

//�R���X�g���N�^
MsgBox::MsgBox()
{
	s_pInstance = this;
}

//�f�X�g���N�^
MsgBox::~MsgBox() 
{
}

static CommandTable s_table[] =
{
	 {MsgBox::FUNC_SHOW,-1,L"Show"}			//�\���쐬
	,{MsgBox::FUNC_MAX , 0,NULL}			//�R�}���h��
};

//�֐����`�F�b�N
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

//�������s
STDMETHODIMP MsgBox::Invoke(DISPID dispIdMember	//Function ID �ԍ�
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script������
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr) 
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //�����`�F�b�N
		return S_FALSE;
	}		
	switch(dispIdMember){
		case FUNC_SHOW: //���b�Z�[�W�{�b�N�X�\��
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