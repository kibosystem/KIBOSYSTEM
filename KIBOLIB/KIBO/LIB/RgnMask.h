#pragma once

#include "../Dialog/SelectDlg.h"

//���[�W�������
class RgnMask
{
private:
	bool m_isMoveOK; //�ړ��֎~�t���O
	CDialogBar *m_pBar; //�_�C�A���O�o�[ 
	int m_nID;			//�}�X�NID�ԍ�
	CRect m_rcImage; //�C���[�W���W
public:
	SelectDlg m_dlgLT; //����
	SelectDlg m_dlgRT; //�E��
	SelectDlg m_dlgLB; //����
	SelectDlg m_dlgRB; //�E��
public:
	RgnMask(void);
	~RgnMask(void);
public:
	int GetID(){return m_nID;} //�}�X�NID�ԍ��擾
	CRect GetImageRect(){return m_rcImage;} //���W�擾
	CRect GetClientRect(); //���W�擾
	void SetClientRect(CRect &rect); //�N���C�A���g���W�ݒ�
	void SetClientRect(int nLeft,int nTop,int nRight,int nBottom);
	void SetImageRect(int nLeft,int nTop,int nRight,int nBottom);
	void Show(); //�}�X�N�\��
	void Hide(); //�}�X�N��\��
	BOOL IsVisible(); //�\�����ł����H
	void Draw(CDC* pDC); //�}�X�N�`��
	//�I���_�C�A���O�̘A��
	void MoveSelectDlg(int nType);
	void SetWindow(CDialogBar *p){m_pBar=p;} //DialogBar�ݒ�
	void SetID(int n){m_nID=n;} //
private:

};
