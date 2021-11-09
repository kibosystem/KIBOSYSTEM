#pragma once

#include <math.h>

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

   PointDbl(IN double xr,IN double yr)
   {
       this->x = xr;
       this->y = yr;
   }

   PointDbl operator=(IN const PointDbl& point)
   {
	   x = point.x;
	   y = point.y;
       return *this;
   }

   PointDbl operator+(IN const PointDbl& point) const
   {
       return PointDbl(x + point.x,y + point.y);
   }

   PointDbl operator-(IN const PointDbl& point) const
   {
       return PointDbl(x - point.x,y - point.y);
   }
   BOOL operator==(IN const PointDbl& point)
   {
		return (x == point.x) && (y == point.y);
   }
   BOOL Equals(IN const PointDbl& point)
   {
       return (x == point.x) && (y == point.y);
   }
   double GetLen(const PointDbl &pt){
		double dbX = (this->x - pt.x);
		double dbY = (this->y - pt.y);
		return sqrt(dbX*dbX + dbY*dbY);
   }
   int GetIntX(){ return int(x+0.5);}
   int GetIntY(){ return int(y+0.5);}
public:

    double x;
    double y;
};

class ArrayPointDbl : public CArray<PointDbl> 
{
public:
	ArrayPointDbl(void){}
	~ArrayPointDbl(void){}
	
	ArrayPointDbl& operator=(const ArrayPointDbl &t)
	{
		this->Copy(t);
		return *this;
	}
	int Find(PointDbl &pt){
		for(int i=0;i<GetSize();i++){
			if( GetAt(i) == pt ){//Œ©‚Â‚©‚Á‚½
				return i;
			}
		}
		//Œ©‚Â‚©‚ç‚È‚©‚Á‚½
		return -1;
	}
};
