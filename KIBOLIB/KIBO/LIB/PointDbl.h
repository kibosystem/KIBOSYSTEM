#pragma once

#include <math.h>
#include <float.h>

class PointDbl
{
public:
	PointDbl()
	{
	   x = y = 0.0f;
	}

	PointDbl(IN const PointDbl &point)
	{
	   x = point.x;
	   y = point.y;
	}

	PointDbl(IN double x,IN double y)
	{
	   this->x = x;
	   this->y = y;
	}

	PointDbl& operator=(IN const PointDbl& point)
	{
	   x = point.x;
	   y = point.y;
	   return *this;
	}

	PointDbl operator+(IN const PointDbl& point) const
	{
	   return PointDbl(x + point.x,y + point.y);
	}
	void operator+=(IN const PointDbl& point)
	{
	   x += point.x;
	   y += point.y;
	}
	PointDbl operator-(IN const PointDbl& point) const
	{
	   return PointDbl(x - point.x,y - point.y);
	}
	void operator-=(IN const PointDbl& point)
	{
	   x -= point.x;
	   y -= point.y;
	}
	BOOL operator==(IN const PointDbl& point)
	{
		return (x == point.x) && (y == point.y);
	}
	BOOL operator!=(IN const PointDbl& point)
	{
		return (x != point.x) || (y != point.y);
	}
	BOOL Equals(IN const PointDbl& point)
	{
	   return (x == point.x) && (y == point.y);
	}
	double GetLength(){ //原点からの距離取得
		return sqrt(x*x+y*y);
	}
	double GetLength(double dbX,double dbY){ //点間の距離取得
		dbX -= x;
		dbY -= y;
		return sqrt(dbX*dbX+dbY*dbY);
	}
	int GetIntX(){ return int(x+0.5);}
	int GetIntY(){ return int(y+0.5);}
public:
    double x;
    double y;
};

//double 型サイズ
class SizeDbl
{
public:
	double cx;
	double cy;
public:
	SizeDbl()
	{
		cx = cy =0.0f;
	}
	SizeDbl(IN double cx,IN double cy)
	{
	   this->cx = cx;
	   this->cy = cy;
	}
	SizeDbl(IN const SizeDbl &size)
	{
	   cx = size.cx;
	   cy = size.cy;
	}
	void Clear()
	{
		cx = cy =0.0f;
	}
	SizeDbl& operator=(IN const SizeDbl& size)
	{
	   cx = size.cx;
	   cy = size.cy;
	   return *this;
	}
	SizeDbl operator+(IN const SizeDbl& size) const
	{
	   return SizeDbl(cx + size.cx,cy + size.cy);
	}
	void operator+=(IN const SizeDbl& size)
	{
	   cx += size.cx;
	   cy += size.cy;
	}
	SizeDbl operator-(IN const SizeDbl& size) const
	{
	   return SizeDbl(cx - size.cx,cy - size.cy);
	}
	void operator-=(IN const SizeDbl& size)
	{
	   cx -= size.cx;
	   cy -= size.cy;
	}
	BOOL operator==(IN const SizeDbl& size)
	{
		return (cx == size.cx) && (cy == size.cy);
	}
	BOOL operator!=(IN const SizeDbl& size)
	{
		return (cx != size.cx) || (cy != size.cy);
	}
	BOOL Equals(IN const SizeDbl& size)
	{
	   return (cx == size.cx) && (cy == size.cy);
	}
	int GetIntX(){ return int(cx+0.5);}
	int GetIntY(){ return int(cy+0.5);}
};


class ArrayPointDbl : public CArray<PointDbl> 
{
public:
	ArrayPointDbl(void){}
	~ArrayPointDbl(void){}

	INT_PTR AddPoint(double x,double y)	//座標追加
	{
		PointDbl pt(x,y);
		return this->Add(pt);
	}

	int LengthMin(PointDbl &pt)
	{
		int nIndex = -1;
		double dbMin = DBL_MAX;
		for(int i=0;i<GetSize();i++){
			double dbDx = GetAt(i).x - pt.x;
			double dbDy = GetAt(i).y - pt.y;
			double dbLen = sqrt(dbDx*dbDx + dbDy*dbDy);
			if( dbLen < dbMin ){//最小値取得
				nIndex = i;
				dbMin = dbLen;
			}
		}
		return nIndex;
	}
	int LengthMax(PointDbl &pt)
	{
		int nIndex = -1;
		double dbMax = 0;
		for(int i=0;i<GetSize();i++){
			double dbDx = GetAt(i).x - pt.x;
			double dbDy = GetAt(i).y - pt.y;
			double dbLen = sqrt(dbDx*dbDx + dbDy*dbDy);
			if( dbLen > dbMax){//最大値取得
				nIndex = i;
				dbMax = dbLen;
			}
		}
		return nIndex;
	}

};