#include "../stdafx.h"
#include "../KIBO.h"
#include "CharElement.h"

//��������
CharElement::CharElement()
{
	//save data
	m_tChar			= 0;	//��������
	m_wRnum			= 0;	//�̈�ԍ�
	m_nAreaA		= 0;	//�ʐ�All				�i�v�f�`�F�b�N�����o�j
	m_dbAreaT		= 0;	//�ʐϏ�
	m_dbAreaB		= 0;	//�ʐω�
	m_dbAreaL		= 0;	//�ʐύ�
	m_dbAreaR		= 0;	//�ʐωE
	m_dbGx			= 0;	//�d�Sx
	m_dbGy			= 0;	//�d�Sy
	m_wContourNum	= 0;	//���E���̐�
	m_dbContourOut	= 0;	//�O�����E���̒���
	m_dbContourIn	= 0;	//�������E���̒���
	m_dbCircle		= 0;	//�~�`�x
	m_byEdgeLR		= 0;	//������E�����̃G�b�W��
	m_byEdgeRL		= 0;	//�E���獶�����̃G�b�W��
	m_byEdgeTB		= 0;	//�ォ�牺�����̃G�b�W��
	m_byEdgeBT		= 0;	//�����������̃G�b�W��
	m_rect.left     = 0;	//�����̈�				�i�v�f�`�F�b�N�����o�j
	m_rect.top      = 0;	//�����̈�				�i�v�f�`�F�b�N�����o�j
	m_rect.right    = 0;	//�����̈�				�i�v�f�`�F�b�N�����o�j
	m_rect.bottom   = 0;	//�����̈�				�i�v�f�`�F�b�N�����o�j
	for(int i=0;i<POINT_NUM;i++){
		m_rLeft[i]  = 0;	//�������_����̋���
		m_rTop[i]	= 0;	//������_����̋���
		m_rRight[i]	= 0;	//�E�����_����̋���
		m_rBottom[i]= 0;	//�������_����̋���
	}
}

CharElement::~CharElement()
{
}

CharElement& CharElement::operator=(IN const CharElement &e)
{
	m_tChar			= e.m_tChar			;	//��������
	m_wRnum			= e.m_wRnum			;	//�̈�ԍ�
	m_nAreaA		= e.m_nAreaA		;	//�ʐ�All				�i�v�f�`�F�b�N�����o�j
	m_dbAreaT		= e.m_dbAreaT		;	//�ʐϏ�
	m_dbAreaB		= e.m_dbAreaB		;	//�ʐω�
	m_dbAreaL		= e.m_dbAreaL		;	//�ʐύ�
	m_dbAreaR		= e.m_dbAreaR		;	//�ʐωE
	m_dbGx			= e.m_dbGx			;	//�d�Sx
	m_dbGy			= e.m_dbGy			;	//�d�Sy
	m_wContourNum	= e.m_wContourNum	;	//���E���̐�
	m_dbContourOut	= e.m_dbContourOut	;	//�O�����E���̒���
	m_dbContourIn	= e.m_dbContourIn	;	//�������E���̒���
	m_dbCircle		= e.m_dbCircle		;	//�~�`�x
	m_byEdgeLR		= e.m_byEdgeLR		;	//������E�����̃G�b�W��
	m_byEdgeRL		= e.m_byEdgeRL		;	//�E���獶�����̃G�b�W��
	m_byEdgeTB		= e.m_byEdgeTB		;	//�ォ�牺�����̃G�b�W��
	m_byEdgeBT		= e.m_byEdgeBT		;	//�����������̃G�b�W��
	m_rect.left     = e.m_rect.left     ;	//�����̈�i�v�f�`�F�b�N�����o�j
	m_rect.top      = e.m_rect.top      ;	//�����̈�i�v�f�`�F�b�N�����o�j
	m_rect.right    = e.m_rect.right    ;	//�����̈�i�v�f�`�F�b�N�����o�j
	m_rect.bottom   = e.m_rect.bottom   ;	//�����̈�i�v�f�`�F�b�N�����o�j
	for(int i=0;i<POINT_NUM;i++){
		m_rLeft[i]  = e.m_rLeft[i];		//�������_����̋����i�v�f�`�F�b�N�����o�j
		m_rTop[i]	= e.m_rTop[i];		//������_����̋����i�v�f�`�F�b�N�����o�j
		m_rRight[i]	= e.m_rRight[i];	//�E�����_����̋����i�v�f�`�F�b�N�����o�j
		m_rBottom[i]= e.m_rBottom[i];	//�������_����̋����i�v�f�`�F�b�N�����o�j
	}
	return *this;
}

//�����̑��a
//�������_����̋������a
REAL CharElement::GetLengthLeft()
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rLeft[i];
	}
	return rRet;
}

//������_����̋������a
REAL CharElement::GetLengthTop()	
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rTop[i];
	}
	return rRet;
}

//�E�����_����̋������a
REAL CharElement::GetLengthRight()	
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rRight[i];
	}
	return rRet;
}

//�������_����̋������a
REAL CharElement::GetLengthBottom()
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rBottom[i];
	}
	return rRet;
}
//�������_����̋���
CString CharElement::GetLeftCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rLeft[0]); //�����l
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rLeft[i]);
		strRet += strTmp;
	}
	return strRet;
}
//������_����̋���
CString CharElement::GetTopCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rTop[0]); //�����l
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rTop[i]);
		strRet += strTmp;
	}
	return strRet;
}
//�E�����_����̋���
CString CharElement::GetRightCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rRight[0]); //�����l
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rRight[i]);
		strRet += strTmp;
	}
	return strRet;
}
//�������_����̋���
CString CharElement::GetBottomCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rBottom[0]); //�����l
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rBottom[i]);
		strRet += strTmp;
	}
	return strRet;
}

//�������_����̋����ݒ�
void CharElement::SetLeft(ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptLB)
{
	CPoint pt = ptLT;//�����l
	m_rLeft[0] = (REAL)aryPoint.GetLength(pt);//����̒��_����̋���

	REAL rDelta = (REAL)(ptLB.y - ptLT.y) / POINT_NUM; //�����擾	
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLT.y + rDelta*i); //�o���h�J�n���W
		int nEnd   = (int)(ptLT.y + rDelta*(i+1)); //�o���h�I�����W
		for(int y=nStart;y<nEnd;y++){ //�o���h�����쐬
			pt.y = y;
			rSum += (REAL)aryPoint.GetLength(pt);//�����ݒ�
		}
		m_rLeft[i] = rSum / (nEnd-nStart); //�o���h�����̕��ς��擾
	}
}
//������_����̋����ݒ�
void CharElement::SetTop(ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptRT)
{
	CPoint pt = ptRT;//�����l
	m_rTop[0] = (REAL)aryPoint.GetLength(pt);//�E�㒸�_����̋���

	pt = ptLT;//�����l
	REAL rDelta = (REAL)(ptRT.x - ptLT.x) / POINT_NUM; //�����擾
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLT.x + rDelta*i);  //�o���h�J�n���W
		int nEnd   = (int)(ptLT.x + rDelta*(i+1));//�o���h�I�����W
		for(int x=nStart;x<nEnd;x++){ //�o���h�����쐬
			pt.x = x;
			rSum += (REAL)aryPoint.GetLength(pt);//�����ݒ�
		}
		m_rTop[i] = rSum / (nEnd-nStart); //�o���h�����̕��ς��擾
	}
}
//�E�����_����̋����ݒ�
void CharElement::SetRight(ArrayPoint &aryPoint,const CPoint &ptRT,const CPoint &ptRB)
{
	CPoint pt = ptRB;//�����l
	m_rRight[0] = (REAL)aryPoint.GetLength(pt);//�E�����_����̋���

	pt = ptRT;//�����l
	REAL rDelta = (REAL)(ptRB.y - ptRT.y) / POINT_NUM; //�����擾	
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart  = (int)(ptRT.y + rDelta*i); //�o���h�J�n���W
		int nEnd	= (int)(ptRT.y + rDelta*(i+1));   //�o���h�I�����W
		for(int y=nStart;y<nEnd;y++){ //�o���h�����쐬
			pt.y = y;
			rSum += (REAL)aryPoint.GetLength(pt);//�����ݒ�
		}
		m_rRight[i] = rSum / (nEnd-nStart); //�o���h�����̕��ς��擾
	}	
}
//�������_����̋����ݒ�
void CharElement::SetBottom(ArrayPoint &aryPoint,const CPoint &ptLB,const CPoint &ptRB)
{
	CPoint pt = ptLB;//�����l
	m_rBottom[0] = (REAL)aryPoint.GetLength(pt);//�������_����̋���

	REAL rDelta = (REAL)(ptRB.x - ptLB.x) / POINT_NUM; //�����擾
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLB.x + rDelta*i);  //�o���h�J�n���W
		int nEnd   = (int)(ptLB.x + rDelta*(i+1));//�o���h�I�����W
		for(int x=nStart;x<nEnd;x++){ //�o���h�����쐬
			pt.x = x;
			rSum += (REAL)aryPoint.GetLength(pt);//�����ݒ�
		}
		m_rBottom[i] = rSum / (nEnd-nStart); //�o���h�����̕��ς��擾
	}
}

//��������OK�ł����H
BOOL CharElement::IsLeftOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rLeft[i] < dbLenMin[i]){ //�ŏ��l��菬����
			strErrType = L"���[�����s��";
			return FALSE;
		}else if(m_rLeft[i] > dbLenMax[i]){ //�ő�l���傫��
			strErrType = L"���[��������";
			return FALSE;
		}
	}
	return TRUE;
}
//�㋗����OK�ł����H
BOOL CharElement::IsTopOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rTop[i] < dbLenMin[i]){//�ŏ��l��菬����
			strErrType = L"��[�����s��";
			return FALSE;
		}else if(m_rTop[i] > dbLenMax[i]){//�ő�l���傫��
			strErrType = L"��[��������";
			return FALSE;
		}
	}
	return TRUE;
}
//�E������OK�ł����H
BOOL CharElement::IsRightOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rRight[i] < dbLenMin[i]){//�ŏ��l��菬����
			strErrType = L"�E�[�����s��";
			return FALSE;
		}else if(m_rRight[i] > dbLenMax[i]){//�ő�l���傫��
			strErrType = L"�E�[��������";
			return FALSE;
		}
	}
	return TRUE;
}
//��������OK�ł����H
BOOL CharElement::IsBottomOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rBottom[i] < dbLenMin[i]){//�ŏ��l��菬����
			strErrType = L"�E�[�����s��";
			return FALSE;
		}else if(m_rBottom[i] > dbLenMax[i]){//�ő�l���傫��
			strErrType = L"�E�[��������";
			return FALSE;
		}
	}
	return TRUE;
}