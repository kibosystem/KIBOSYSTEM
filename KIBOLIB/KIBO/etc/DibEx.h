#pragma once

#include "../LIB/dib.h"
#include "../LIB/PointDbl.h"

//画像処理専用ビットマップクラス
class CDIBEx : public CDIB
{

public:
	CDIBEx(void);
	~CDIBEx(void);

public:
	double Get256Val(int x,int y); //256色輝度値取得
	double Get256Val(double x,double y); //256色輝度値取得

	PointDbl GetLinePointIn (CPoint &ptS,CPoint &ptE,int nTh); //nTh以上になる座標を取得
	PointDbl GetLinePointOut(CPoint &ptS,CPoint &ptE,int nTh); //nTh以下になる座標を取得
};
