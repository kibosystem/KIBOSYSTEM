#include "../stdafx.h"
#include <math.h>
#include "DibEx.h"
#include "../LIB/PointDbl.h"

//���������߂�
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

//256�F���[�h�ł̋P�x�l���擾
double CDIBEx::Get256Val(int x,int y)
{
	if( x < GetWidth() && y < GetHeight() ){
		BYTE *pBits = GetPBitsB(x,y);

		BYTE byB = *pBits++; //��f�l�擾 B
		BYTE byG = *pBits++; //��f�l�擾 G
		BYTE byR = *pBits;	//��f�l�擾 R
		return (0.299*byR + 0.587*byG + 0.114*byB);
	}
	return 0;
}

//256�F���[�h�ł̋P�x�l���擾
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
	
	//�l�v�Z
	double dbTop = dbLT * (nRight-x) + dbRT * (x-nLeft);
	double dbBtm = dbLB * (nRight-x) + dbRB * (x-nLeft);
	return ( dbTop * (nBtm-y) + dbBtm * (nBtm-y) );
}

//nTh�ȏ�ɂȂ���W���擾
PointDbl CDIBEx::GetLinePointIn(CPoint &ptS,CPoint &ptE,int nTh)
{
	PointDbl ptRet(-1,-1);

	//�����x�N�g���v�Z	
	PointDbl ptVec;
	ptVec.x = ptE.x - ptS.x; //�x�N�g���擾
	ptVec.y = ptE.y - ptS.y;
	double dbSize = sqrt(ptVec.x * ptVec.x + ptVec.y * ptVec.y);
	if( dbSize > 0 ){
		//�P�ʕ����x�N�g���擾
		ptVec.x /= dbSize; 
		ptVec.y /= dbSize;
		PointDbl pt(ptS.x,ptS.y);	//�T���J�n�n�_
		PointDbl ptEnd(ptE.x,ptE.y);//�T���I���n�_
		double dbLength = GetLength(pt,ptEnd); //�T������
		int nTimes = (int)dbLength; //�T����

		double dbPrev = 0;
		double dbVal = Get256Val(pt.x,pt.y);
		while( dbVal >= nTh ){ //Start Point��nTh��艺�ɂȂ���W�܂ňړ�
			nTimes--;
			if(nTimes <= 0 ){
				return ptRet; //�I��
			}
			dbPrev = dbVal;
			pt += ptVec;
			dbVal = Get256Val(pt.x,pt.y);
		}	
		//�����ł͕K��dbVal < nTh
		for(int i=0;i<nTimes;i++){			
			dbVal = Get256Val(pt.x,pt.y);
			if( dbVal >= nTh ){ //��������
				pt.x -= ptVec.x * (dbVal-nTh) / (dbVal-dbPrev);
				pt.y -= ptVec.y * (dbVal-nTh) / (dbVal-dbPrev);
				return pt;
			}
			pt += ptVec; //���̒T���|�C���g��			
		}
	}
	return ptRet; //�����Ȃ�
}

//nTh�ȉ��ɂȂ���W���擾
PointDbl CDIBEx::GetLinePointOut(CPoint &ptS,CPoint &ptE,int nTh)
{
	PointDbl ptRet(-1,-1);

	//�����x�N�g���v�Z	
	PointDbl ptVec;
	ptVec.x = ptE.x - ptS.x; //�x�N�g���擾
	ptVec.y = ptE.y - ptS.y;
	double dbSize = sqrt(ptVec.x * ptVec.x + ptVec.y * ptVec.y);
	if( dbSize > 0 ){
		//�P�ʕ����x�N�g���擾
		ptVec.x /= dbSize; 
		ptVec.y /= dbSize;
		PointDbl pt(ptS.x,ptS.y);	//�T���J�n�n�_
		PointDbl ptEnd(ptE.x,ptE.y);//�T���I���n�_
		double dbLength = GetLength(pt,ptEnd); //�T������
		int nTimes = (int)dbLength; //�T����

		double dbPrev = 0;
		double dbVal = Get256Val(pt.x,pt.y);
		while( dbVal <= nTh ){ //Start Point��nTh��艺�ɂȂ���W�܂ňړ�
			nTimes--;
			if(nTimes <= 0 ){
				return ptRet; //�I��
			}
			dbPrev = dbVal;
			pt += ptVec;
			dbVal = Get256Val(pt.x,pt.y);
		}
		//�����ł͕K��dbVal > nTh
		for(int i=0;i<nTimes;i++){			
			dbVal  = Get256Val(pt.x,pt.y);
			if( dbVal <= nTh ){ //��������
				pt.x -= ptVec.x * (dbVal-dbPrev) / (dbVal-nTh); //  dbPrev > nTh 
				pt.y -= ptVec.y * (dbVal-dbPrev) / (dbVal-nTh); // (dbVal-dbPrev) < (dbVal-nTh)
				return pt;
			}
			pt += ptVec; //���̒T���|�C���g��			
		}
	}
	return ptRet; //�����Ȃ�
}
