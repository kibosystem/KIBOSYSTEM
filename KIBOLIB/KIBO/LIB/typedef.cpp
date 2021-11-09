#include "../stdafx.h"
#include <float.h>
#include "typedef.h"
#include "functions.h"

AryByte& AryByte::operator=(const AryByte &ary)
{
	this->Copy(ary);
	return *this;
}
AryInt& AryInt::operator=(const AryInt &ary)
{
	this->Copy(ary);
	return *this;
}
AryDouble& AryDouble::operator=(const AryDouble &ary)
{
	this->Copy(ary);
	return *this;
}

AryWord& AryWord::operator=(const AryWord &ary)
{
	this->Copy(ary);
	return *this;
}

AryDWord& AryDWord::operator=(const AryDWord &ary)
{
	this->Copy(ary);
	return *this;
}

AryLongLong& AryLongLong::operator=(const AryLongLong &t)
{
	this->Copy(t);
	return *this;
}

StringArrayPlus& StringArrayPlus::operator=(const StringArrayPlus &t)
{
	this->Copy(t);
	return *this;
}
int AryByte::Find(BYTE by)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == by ){
			return i;
		}
	}
	return -1;
}
int AryInt::Find(int n)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == n ){
			return i;
		}
	}
	return -1;	
}
int AryDouble::Find(double db)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == db ){
			return i;
		}
	}
	return -1;		
}
int AryWord::Find(WORD w)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == w ){
			return i;
		}
	}
	return -1;
}
int AryDWord::Find(DWORD dw)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == dw ){
			return i;
		}
	}
	return -1;
}
int AryParam::Find(LPARAM lParam)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == lParam ){
			return i;
		}
	}
	return -1;
}
int AryLongLong::Find(LONGLONG ll)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == ll ){
			return i;
		}
	}
	return -1;
}
int StringArrayPlus::Find(LPCTSTR lpsz)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == lpsz ){
			return i;
		}
	}
	return -1;
}	

//����
double AryInt::GetAverage() 
{
	double dbSam = 0;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		dbSam += GetAt(i);
	}
	return (imax > 0) ? (dbSam / imax) : 0;
}

//�����폜
void AryInt::RemoveLast()
{
	int nSize = (int)GetSize();
	if( nSize > 0 ){
		RemoveAt(nSize-1); //�����폜
	}
}

double AryDouble::GetAverage() 
{
	double dbSam = 0;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		dbSam += GetAt(i);
	}
	return (imax > 0) ? (dbSam / imax) : 0;
}
//�ŏ��l
double AryDouble::GetMin(int *pIndex/*=NULL*/)
{
	int nIndex = -1;
	double dbMin = DBL_MAX;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		double db = GetAt(i);
		if( db < dbMin ){
			nIndex = i;
			dbMin = db;
		}
	}
	if( pIndex != NULL ){
		*pIndex = nIndex;
	}
	return dbMin;
}
//�ő�l
double AryDouble::GetMax(int *pIndex/*=NULL*/)
{
	int nIndex = -1;
	double dbMax = DBL_MIN;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		double db = GetAt(i);
		if( dbMax < db){
			nIndex = i;
			dbMax = db;
		}
	}
	if( pIndex != NULL ){
		*pIndex = nIndex;
	}
	return dbMax;
}

AryLine& AryLine::operator=(const AryLine &t)
{
	this->Copy(t);
	return *this;	
}
//�����폜
void AryLine::RemoveLast()
{
	int nSize = (int)GetSize();
	if( nSize > 0 ){
		RemoveAt(nSize-1); //�����폜
	}
}
//�w��x���W�v�f�폜
void AryLine::RemoveX(int x) 
{
	for(int i=0;i<GetSize();i++){
		CLine& line = GetAt(i); //���C�����o��
		if( line.m_ptS.x == x && line.m_ptE.x == x){
			RemoveAt(i);
			break;
		}
	}	
}
//�w��x���W�v�f�폜
void AryLine::RemoveY(int y) 
{
	for(int i=0;i<GetSize();i++){
		CLine& line = GetAt(i); //���C�����o��
		//TRACE("S(%d,%d) E(%d,%d)\n",line.m_ptS.x,line.m_ptS.y,line.m_ptE.x,line.m_ptE.y);
		if( line.m_ptS.y == y && line.m_ptE.y == y){
			RemoveAt(i);
			break;
		}
	}	
}
//���̒���
double CLine::GetLength() 
{
	return ::GetLength(m_ptS,m_ptE)+1; //���̑����͒[�_���܂�
}
//�P�ʎw����̒���
double CLine::GetLength(double dbCx,double dbCy) 
{
	double dbDx = dbCx*(m_ptS.x-m_ptE.x);
	double dbDy = dbCy*(m_ptS.x-m_ptE.x);
	double dbLen = sqrt(dbDx*dbDx + dbDy*dbDy);
	return dbLen+1; //���̑����͒[�_���܂�
}

//�����Ԃ̒���
double CLine::GetLength(CLine &line) 
{
	PointDbl vec; //�P�ʕ����x�N�g��
	vec.x = m_ptE.x - m_ptS.x;
	vec.y = m_ptE.y - m_ptS.y;
	double dbLen = vec.GetLength(); //�x�N�g���̃T�C�Y���擾
	vec.x /= dbLen;
	vec.y /= dbLen;
	PointDbl ptCheck(m_ptS.x,m_ptS.y); //�����|�C���g

	double dbMin = INT_MAX; //�ŏ��l�v�Z
	while( ptCheck.GetLength(m_ptS.x,m_ptS.y) < dbLen){ //�I�_�𒴂���܂�
		double dbSpace = line.CalcLength(ptCheck.x,ptCheck.y);
		if( dbSpace < dbMin ){
			dbMin = dbSpace;
		}
		ptCheck.x += vec.x; //�ړ�
		ptCheck.y += vec.y;
	}
	return dbMin;
}

//�����Ɠ_�Ԃ̒���
double CLine::CalcLength(double dbX,double dbY)
{
	PointDbl vec; //�P�ʕ����x�N�g��
	vec.x = m_ptE.x - m_ptS.x;
	vec.y = m_ptE.y - m_ptS.y;
	double dbSize = vec.GetLength(); //�x�N�g���̃T�C�Y���擾
	vec.x /= dbSize;
	vec.y /= dbSize;
	PointDbl ptCheck(m_ptS.x,m_ptS.y); //�����|�C���g

	double dbMin = INT_MAX; //�ŏ��l�v�Z
	while( ptCheck.GetLength(m_ptS.x,m_ptS.y) < dbSize ){ //�I�_�𒴂���܂�
		double dbLen = ptCheck.GetLength(dbX,dbY);
		if( dbLen < dbMin ){
			dbMin = dbLen;
		}		
		ptCheck.x += vec.x; //�ړ�
		ptCheck.y += vec.y;
	}
	return dbMin;
}

//�w����W���܂ރ��C�����擾
BOOL AryLine::GetLine(CLine &line,CPoint &pt)
{
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		if( GetAt(i).IsIn(pt) ){ //���C�����w����W���܂݂܂����H
			line = GetAt(i);
			return TRUE;
		}
	}
	return FALSE;
}
//�w�胉�C������
int  AryLine::Find(CLine &line)
{
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		if( GetAt(i) == line ){
			return i; //��������
		}
	}
	return -1;//������Ȃ�����
}
//�̈�ŏ��_�擾
int AryLine::GetMinX() 
{
	int nMinX = INT_MAX;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CLine &line = GetAt(i);
		int nX = min(line.m_ptS.x,line.m_ptE.x); //x���W�擾
		if( nX < nMinX ){//��菬�����l����������
			nMinX = nX;
		}
	}
	return nMinX;
}
//�̈�ŏ��_�擾
int AryLine::GetMinY() 
{
	int nMinY = INT_MAX;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CLine &line = GetAt(i);
		int nY = min(line.m_ptS.y,line.m_ptE.y); //y���W�擾
		if( nY < nMinY ){//��菬�����l����������
			nMinY = nY;
		}
	}
	return nMinY;
}
//�̈�ő�_�擾
int AryLine::GetMaxX() 
{
	int nMaxX = 0;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CLine &line = GetAt(i);
		int nX = max(line.m_ptS.x,line.m_ptE.x); //x���W�擾
		if( nMaxX < nX ){//���傫���l����������
			nMaxX = nX;
		}
	}
	return nMaxX;
}
//�̈�ő�_�擾
int AryLine::GetMaxY() 
{
	int nMaxY = 0;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CLine &line = GetAt(i);
		int nY = max(line.m_ptS.y,line.m_ptE.y); //y���W�擾
		if( nMaxY < nY ){//���傫���l����������
			nMaxY = nY;
		}
	}
	return nMaxY;
}

//�w����W���܂݂܂����H
BOOL CLine::IsIn(CPoint& pt)
{
	int nXmin = min(m_ptS.x,m_ptE.x);
	int nXmax = max(m_ptS.x,m_ptE.x);
	int nYmin = min(m_ptS.y,m_ptE.y);
	int nYmax = max(m_ptS.y,m_ptE.y);
	if(nXmin <= pt.x && pt.x <= nXmax){
		return (nYmin <= pt.y && pt.y <= nYmax);
	}
	return FALSE;
}

void CSortStringArray::Sort()
{
   BOOL bNotDone = TRUE;

	while (bNotDone){
		bNotDone = FALSE;
		for(int pos = 0;pos < GetUpperBound();pos++){
			bNotDone |= CompareAndSwap(pos);
		}
	}
}
BOOL CSortStringArray::CompareAndSwap(int pos)
{
	CString temp;
	int posFirst = pos;
	int posNext = pos + 1;
	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0){
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return TRUE;
   }
   return FALSE;
}