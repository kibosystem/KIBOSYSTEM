#pragma once

#include "globalmgr.h"
#include "dib.h"

class ArrayPoint : public CArray<CPoint> 
{
public:
	ArrayPoint(void){}
	~ArrayPoint(void){}
	
	ArrayPoint& operator=(const ArrayPoint &t);
	int Find(CPoint pt);			//�v�f����
	INT_PTR AddPoint(int x,int y);	//���W�ǉ�

	CPoint GetLeft();	//�ł����̓_���擾
	CPoint GetRight();	//�ł��E�̓_���擾
	CPoint GetTop();	//�ł���̓_���擾
	CPoint GetBottom(); //�ł����̓_���擾		
	CRect GetRect();//�_�z�񂩂��`���擾

	double GetLength(CPoint& pt); //�w����W�Ƃ̍ŒZ���������߂�
	CPoint GetLengthPoint(CPoint& pt,double &dbLmin);//�w����W�Ƃ̍ŒZ�����ƍŒZ�����ƂȂ�Point���擾
	int CollectPointX(int x,ArrayPoint &ptCollect);//�w��x���W�̓_�擾
	int CollectPointY(int y,ArrayPoint &ptCollect);//�w��y���W�̓_�擾
	CPoint FindX(int x); //�w��x���W����
	CPoint FindX(int nIndex,int x); //�w��x���W����	
	CPoint FindY(int y); //�w��y���W����	
	CPoint FindY(int nIndex,int y) ;//�w��y���W����

	void SetX(int x,ArrayPoint &ptCollect); //�w��Point��X���W���w��l�ɐݒ�
	void SortX(); //x���W�Ń\�[�g
	void SortY(); //y���W�Ń\�[�g
	double GetLength(ArrayPoint &ptAry,CPoint &ptS,CPoint &ptE); //�ŒZ�����擾
	void SplitX(int nX,ArrayPoint &aryL,ArrayPoint &aryR); //x���W�ɂ����2�ɕ�����

	BOOL IsConnect(CPoint &pt); //�ڑ�Point�ł����H
	void CollectConnectPoint(CPoint &pt,ArrayPoint &aryConnect); //���_���W�Ƃ̗א�Point�̂�
	void CollectExceptPoint(ArrayPoint &aryCollect,ArrayPoint &aryExcept); //�א�Point�ȊO�����W

	void RangeX(double dbXmin,double dbXmax); //�w��x�͈͓��̍��W�������c��
	void RangeY(double dbYmin,double dbYmax);//�w��y�͈͓��̍��W�������c��

	//�w�萔�ڑ�Point�݂̂�I��
	void SelectConnectPoint(int nDelete);	
	void DrawPolyline(CDIB &dib,COLORREF color);//���`��	
	void DrawPoints(int nNo);//�_�`��
};
