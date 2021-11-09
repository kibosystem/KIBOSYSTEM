#include "stdafx.h"
#include "KIBO.h"
#include "CaptureBuffer.h"


//�R���X�g���N�^
CaptureBuffer::CaptureBuffer(void)
{
	m_isEnd = false; //�I���t���O
}
//�f�X�g���N�^
CaptureBuffer::~CaptureBuffer(void)
{
}

//�r�b�g�}�b�v�ǉ�
int CaptureBuffer::Add(CDIB &dib)
{
	//���g�p�̃r�b�g�}�b�v��T��
	for(int i=0,imax=(int)m_aryFlag.GetSize();i<imax;i++){
		if( m_aryFlag[i] == BITAMP_INVALID){ //�����ȃr�b�g�}�b�v��ύX
			m_aryBmp.Set(i,dib); //�w��C���f�b�N�X�ɃR�s�[
			m_aryFlag.SetAt(i,BITMAP_VALID); //�r�b�g�}�b�v�t���O�ύX
			return imax; //�z��T�C�Y
		}
	}
	m_aryBmp.Add(dib); //�r�b�g�}�b�v�z��ǉ�
	m_aryFlag.Add(BITMAP_VALID);//�r�b�g�}�b�v�t���O�ǉ�
	return (int)m_aryBmp.GetSize(); //�z��T�C�Y
}

//�g�p�\�ȃr�b�g�}�b�v���擾
CDIB* CaptureBuffer::GetDibPtr()
{
	//�g�p���̂̃r�b�g�}�b�v��T��
	for(int i=0;i<m_aryFlag.GetSize();i++){
		if( m_aryFlag[i] == BITMAP_VALID){ //�t���O�z��
			m_aryFlag.SetAt(i,BITAMP_INVALID); //���o�����r�b�g�}�b�v�͖����ɂ���
			return m_aryBmp.GetDibPtr(i); //���o��
		}
	}
	return NULL;
}

//�r�b�g�}�b�v���擾
int CaptureBuffer::GetSize(int nType) 
{
	int nCnt = 0;
	for(int i=0,imax=(int)m_aryFlag.GetSize();i<imax;i++){
		if( m_aryFlag[i] == nType ){
			nCnt++;
		}
	}
	return nCnt; //�w��t���O�r�b�g�}�b�v��
}