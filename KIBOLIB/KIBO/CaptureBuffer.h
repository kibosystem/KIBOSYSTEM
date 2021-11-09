#pragma once

#include "./LIB/typedef.h"
#include "./LIB/ArrayBitmap.h"

//�}���`�X���b�h�L���v�`���o�b�t�@
class CaptureBuffer
{
public:
	enum{
		 BITMAP_VALID = 0	//�L��
		,BITAMP_INVALID		//����
	};
private:
	bool m_isEnd; //�I���t���O
public:
	ArrayBitmap m_aryBmp; //�r�b�g�}�b�v�z��
	AryByte		m_aryFlag; //�t���O�z��
public:
	CaptureBuffer(void);
	~CaptureBuffer(void);

	int  Add(CDIB &dib);//�r�b�g�}�b�v�ǉ�
	bool IsEnd(){return m_isEnd;}	//�I���ł����H
	void SetEnd(bool b){m_isEnd=b;}	//�I�������ݒ�
	CDIB* GetDibPtr(); //�g�p�\�ȃr�b�g�}�b�v���擾

	int GetSize(int nType); //�r�b�g�}�b�v���擾
};
