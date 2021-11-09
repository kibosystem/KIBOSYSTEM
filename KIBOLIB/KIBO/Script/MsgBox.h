#pragma once

#include "ScriptBase.h"

//JScript����Ăяo�����N���X
class MsgBox : public ScriptBase 
{
public:

	enum{ //Function ID �ԍ�
		 FUNC_SHOW = 1	//���b�Z�[�W�{�b�N�X�\��
		,FUNC_MAX		//�R�}���h��
	};

public:
	//�R���X�g���N�^
	MsgBox();

	//�f�X�g���N�^
	virtual ~MsgBox();

	static MsgBox* GetInstance();	

	//�֐����`�F�b�N
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//���s
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID �ԍ�
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script������
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);
};