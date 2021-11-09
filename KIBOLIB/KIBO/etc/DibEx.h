#pragma once

#include "../LIB/dib.h"
#include "../LIB/PointDbl.h"

//�摜������p�r�b�g�}�b�v�N���X
class CDIBEx : public CDIB
{

public:
	CDIBEx(void);
	~CDIBEx(void);

public:
	double Get256Val(int x,int y); //256�F�P�x�l�擾
	double Get256Val(double x,double y); //256�F�P�x�l�擾

	PointDbl GetLinePointIn (CPoint &ptS,CPoint &ptE,int nTh); //nTh�ȏ�ɂȂ���W���擾
	PointDbl GetLinePointOut(CPoint &ptS,CPoint &ptE,int nTh); //nTh�ȉ��ɂȂ���W���擾
};
