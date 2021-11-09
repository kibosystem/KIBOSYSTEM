#include "../stdafx.h"
#include <math.h>
#include "DibEx.h"
#include "../LIB/PointDbl.h"

//距離を求める
double GetLength(const PointDbl &ptD1,const PointDbl &ptD2)
{
	double dbDeltaX = ptD1.x-ptD2.x;
	double dbDeltaY = ptD1.y-ptD2.y;
	return sqrt(dbDeltaX*dbDeltaX+dbDeltaY*dbDeltaY);
}

CDIBEx::CDIBEx(void)
{
}

CDIBEx::~CDIBEx(void)
{
}

//256色モードでの輝度値を取得
double CDIBEx::Get256Val(int x,int y)
{
	if( x < GetWidth() && y < GetHeight() ){
		BYTE *pBits = GetPBitsB(x,y);

		BYTE byB = *pBits++; //画素値取得 B
		BYTE byG = *pBits++; //画素値取得 G
		BYTE byR = *pBits;	//画素値取得 R
		return (0.299*byR + 0.587*byG + 0.114*byB);
	}
	return 0;
}

//256色モードでの輝度値を取得
double CDIBEx::Get256Val(double x,double y)
{
	int nLeft = (int)x;
	int nTop  = (int)y;
	int nRight= nLeft+1;
	int nBtm  = nTop+1;
	
	double dbLT = Get256Val(nLeft ,nTop);
	double dbRT = Get256Val(nRight,nTop);
	double dbLB = Get256Val(nLeft ,nBtm);
	double dbRB = Get256Val(nRight,nBtm);
	
	//値計算
	double dbTop = dbLT * (nRight-x) + dbRT * (x-nLeft);
	double dbBtm = dbLB * (nRight-x) + dbRB * (x-nLeft);
	return ( dbTop * (nBtm-y) + dbBtm * (nBtm-y) );
}

//nTh以上になる座標を取得
PointDbl CDIBEx::GetLinePointIn(CPoint &ptS,CPoint &ptE,int nTh)
{
	PointDbl ptRet(-1,-1);

	//方向ベクトル計算	
	PointDbl ptVec;
	ptVec.x = ptE.x - ptS.x; //ベクトル取得
	ptVec.y = ptE.y - ptS.y;
	double dbSize = sqrt(ptVec.x * ptVec.x + ptVec.y * ptVec.y);
	if( dbSize > 0 ){
		//単位方向ベクトル取得
		ptVec.x /= dbSize; 
		ptVec.y /= dbSize;
		PointDbl pt(ptS.x,ptS.y);	//探索開始地点
		PointDbl ptEnd(ptE.x,ptE.y);//探索終了地点
		double dbLength = GetLength(pt,ptEnd); //探索距離
		int nTimes = (int)dbLength; //探索回数

		double dbPrev = 0;
		double dbVal = Get256Val(pt.x,pt.y);
		while( dbVal >= nTh ){ //Start PointがnThより下になる座標まで移動
			nTimes--;
			if(nTimes <= 0 ){
				return ptRet; //終了
			}
			dbPrev = dbVal;
			pt += ptVec;
			dbVal = Get256Val(pt.x,pt.y);
		}	
		//ここでは必ずdbVal < nTh
		for(int i=0;i<nTimes;i++){			
			dbVal = Get256Val(pt.x,pt.y);
			if( dbVal >= nTh ){ //見つかった
				pt.x -= ptVec.x * (dbVal-nTh) / (dbVal-dbPrev);
				pt.y -= ptVec.y * (dbVal-nTh) / (dbVal-dbPrev);
				return pt;
			}
			pt += ptVec; //次の探索ポイントへ			
		}
	}
	return ptRet; //検索なし
}

//nTh以下になる座標を取得
PointDbl CDIBEx::GetLinePointOut(CPoint &ptS,CPoint &ptE,int nTh)
{
	PointDbl ptRet(-1,-1);

	//方向ベクトル計算	
	PointDbl ptVec;
	ptVec.x = ptE.x - ptS.x; //ベクトル取得
	ptVec.y = ptE.y - ptS.y;
	double dbSize = sqrt(ptVec.x * ptVec.x + ptVec.y * ptVec.y);
	if( dbSize > 0 ){
		//単位方向ベクトル取得
		ptVec.x /= dbSize; 
		ptVec.y /= dbSize;
		PointDbl pt(ptS.x,ptS.y);	//探索開始地点
		PointDbl ptEnd(ptE.x,ptE.y);//探索終了地点
		double dbLength = GetLength(pt,ptEnd); //探索距離
		int nTimes = (int)dbLength; //探索回数

		double dbPrev = 0;
		double dbVal = Get256Val(pt.x,pt.y);
		while( dbVal <= nTh ){ //Start PointがnThより下になる座標まで移動
			nTimes--;
			if(nTimes <= 0 ){
				return ptRet; //終了
			}
			dbPrev = dbVal;
			pt += ptVec;
			dbVal = Get256Val(pt.x,pt.y);
		}
		//ここでは必ずdbVal > nTh
		for(int i=0;i<nTimes;i++){			
			dbVal  = Get256Val(pt.x,pt.y);
			if( dbVal <= nTh ){ //見つかった
				pt.x -= ptVec.x * (dbVal-dbPrev) / (dbVal-nTh); //  dbPrev > nTh 
				pt.y -= ptVec.y * (dbVal-dbPrev) / (dbVal-nTh); // (dbVal-dbPrev) < (dbVal-nTh)
				return pt;
			}
			pt += ptVec; //次の探索ポイントへ			
		}
	}
	return ptRet; //検索なし
}
