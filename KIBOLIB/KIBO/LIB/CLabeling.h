// CLabeling.h

#ifndef	__LABELING_H__
#define	__LABELING_H__

#include	<iostream>
#include	<algorithm>
#include	<list>
#include	<queue>

#include <afxtempl.h>
#include "typedef.h"

class RasterSegment{
private:
	int	m_nXL;	//�Z�O�����g�J�n�����W
	int	m_nXR;	//�Z�O�����g�I�������W
	int	m_nY;	//y���W
	int	m_nValue; //�Z�x�l
public:
	RasterSegment()
	{
		m_nXL = 0;
		m_nXR = 0;
		m_nY  = 0;
		m_nValue = 0;
	}

	RasterSegment( const int nXL, const int nXR,const int y, const int n_source_value )
		: m_nXL( nXL ), m_nXR( nXR ), m_nY( y ),m_nValue( n_source_value )
	{
	}

	~RasterSegment()
	{
	}

	// get
	inline int GetLeftX( void ) const
	{
		return m_nXL;
	}
	inline void SetLeftX(int n)
	{
		m_nXL = n;
	}
	
	inline	int	GetRightX( void )	const
	{
		return m_nXR;
	}
	inline	void SetRightX(int n)
	{
		m_nXR = n;
	}

	inline int GetY( void )	const
	{
		return m_nY;
	}
	inline void SetY(int n)
	{
		m_nY = n;
	}

	inline int GetSourceValue(void) const
	{
		return m_nValue;
	}
	inline void SetSourceValue(int n)
	{
		m_nValue = n;
	}
};

class RasArray : public CPtrArray
{
private:
	int m_nNum; //���ݎg�p����Ă���f�[�^��
public:
	RasArray(){
		m_nNum=0;
	}
	~RasArray(){
		for(int i=0,imax=(int)GetSize();i<imax;i++){
			RasterSegment *p = (RasterSegment *)GetAt(i);
			if( p != NULL ){
				delete p;
			}
		}		
	}

	void Back(){
		if( m_nNum > 0 ){
			m_nNum--; //�g�p�������炷
		}
	}
	void Clear(){
		m_nNum = 0; //�擪����g�p�\�ɂ���
	}

	//�g�p����Ă��Ȃ�Rastersegment���擾
	RasterSegment* GetBack(const int lx, const int rx,const int  y, const int src_value )
	{		
		RasterSegment *pRas = NULL;
		m_nNum++; //�g�p���v���X
		if( GetSize() > m_nNum ){ 
			pRas = (RasterSegment *)GetAt(m_nNum-1);
			pRas->SetLeftX(lx);
			pRas->SetRightX(rx);
			pRas->SetY(y);
			pRas->SetSourceValue(src_value);
		}else{
			//�ǉ�����K�v�����鎞			
			pRas = new RasterSegment(lx,rx,y,src_value);
			Add(pRas); //�V�K�ǉ�
		}
		return pRas;
	}
};

typedef	std::list<RasterSegment*> RSPList;
typedef	std::list<RasterSegment*>::iterator	RSPIterator;
typedef	std::queue<RasterSegment*> RSPQueue;

// information about region ///////////////////////////////////////////////

//���x�����O�v�f
class	Element 
{
private:
	int		m_pixels; //��f��
	float	center_x, center_y; //���S�ʒu
	int		size_x, size_y;
	int		min_x, min_y;	//�ŏ����W
	int		max_x, max_y;	//�ő���W
	int		m_source_value;
	int		m_result;//�v�f�ԍ�
	RSPList	m_SegmentList;	//�Z�O�����g���X�g
	float	gravity_x, gravity_y;	//�d�S
public:
	// constructor and destructor
	Element()
	{
		Clear();
	}	
	~Element()
	{
		m_SegmentList.clear();
	}
	void Clear()
	{
		m_pixels = 0;
		center_x = 0;
		center_y = 0;
		size_x = 0;
		size_y = 0;
		min_x = min_y = 0;
		max_x = max_y = 0;
		m_source_value = 0;
		m_result = 0;
		gravity_x = gravity_y = 0;
		m_SegmentList.clear();
	}
	void Copy(Element *pElem)
	{
		this->m_pixels = pElem->m_pixels; //��f��
		this->center_x		  = pElem->center_x;
		this->center_y		  = pElem->center_y; //���S�ʒu
		this->size_x		  = pElem->size_x;
		this->size_y		  = pElem->size_y;
		this->min_x			  = pElem->min_x;
		this->min_y			  = pElem->min_y;	//�ŏ����W
		this->max_x			  = pElem->max_x;
		this->max_y			  = pElem->max_y;	//�ő���W
		this->m_source_value  = pElem->m_source_value;
		this->m_result		  = pElem->m_result;
		this->m_SegmentList   = pElem->m_SegmentList;//�Z�O�����g���X�g
		this->gravity_x       = pElem->gravity_x;
		this->gravity_y		  = pElem->gravity_y;//�d�S
	}

	// set
	void SetNumOfPixels(int n )
	{
		m_pixels = n;
	}

	void SetCenter(float x,float y )
	{
		center_x = x;
		center_y = y;
	}

	void SetSize(int x,int y)
	{
		size_x = x;
		size_y = y;
	}

	void SetMin(int x,int y )
	{
		min_x = x;
		min_y = y;
	}

	void SetMax(int x,int y)
	{
		max_x = x;
		max_y = y;
	}

	void SetMinMax(int n_min_x, int n_min_y,int n_max_x,int n_max_y )
	{
		SetMin( n_min_x, n_min_y );
		SetMax( n_max_x, n_max_y );
		SetCenter(( n_min_x + n_max_x ) / 2.0f,
					( n_min_y + n_max_y ) / 2.0f );
		SetSize( n_max_x - n_min_x + 1, n_max_y - n_min_y + 1 );
	}

	void SetGravity(float x,float y )
	{
		gravity_x = x;
		gravity_y = y;
	}

	void SetSourceValue( const int n_source_value )
	{
		m_source_value = n_source_value;
	}

	void SetResult(int n_result )
	{		
		m_result = n_result;
	}

	// get
	void GetCenter( float& x, float& y )
	{
		x = center_x;
		y = center_y;
	}
	float GetCenterX()
	{
		return center_x;
	}
	float GetCenterY()
	{
		return center_y;
	}	
	int GetCenterXInt()
	{
		return (int)(center_x+0.5);
	}
	int GetCenterYInt()
	{
		return (int)(center_y+0.5);
	}	
	void GetSize( int& x, int& y )
	{
		x = size_x;
		y = size_y;
	}
	
	int GetMinX(){return min_x;}
	int GetMinY(){return min_y;}
	int GetMaxX(){return max_x;}
	int GetMaxY(){return max_y;}
	CRect GetRect(){return CRect(min_x,min_y,max_x,max_y);}
	int GetWidth() { return (max_x - min_x + 1); }
	int GetHeight(){ return (max_y - min_y + 1); }

	//�d�S�擾
	float GetGx()
	{
		return gravity_x;
	}
	float GetGy()
	{
		return gravity_y;
	}
	int GetSourceValue()
	{
		return m_source_value;
	}
	int GetResult()
	{
		return m_result;
	}

	// list
	RSPList& GetRasterSegmentList()
	{
		return m_SegmentList;
	}

	//�擪�ɓo�^
	void Push( RasterSegment* pRs )
	{
		m_SegmentList.push_front( pRs );
	}

	//�Z�O�����g�T�C�Y�擾
	int	GetNumOfRasterSegments( void )
	{
		return (int)m_SegmentList.size();
	}
	//��f�����擾
	int GetPixels()	const
	{
		return m_pixels;
	}
	//�~�`�x�擾
	double GetRoundness(int nBorder);	

	//�w��̈���̉�f�����擾
	int GetPixels(int nLeft,int nTop,int nRight,int nBottom);
	double GetPixelsLeft(); //�������̖ʐς��擾
	double GetPixelsRight();//�E�����̖ʐς��擾
	double GetPixelsTop();	 //�㔼���̖ʐς��擾
	double GetPixelsBottom();//�������̖ʐς��擾	
	double GetLMin(double dbX,double dbY); //�ŏ������擾
	double GetLMax(double dbX,double dbY); //�ő勗���擾
	double GetRoundness(); //�~�`�x�擾
	double GetArea(double dbCx,double dbCy){//�ʐώ擾
		return (dbCx*dbCy*GetPixels());
	}

	//�w����W���܂܂�Ă��邩�ǂ���
	BOOL IsContain(int x,int y);

	double AverageTop();
	double AverageBottom();

	// operators
	friend	bool operator<( const Element& l, const Element& r )
	{
		return ( l.GetPixels() < r.GetPixels());
	}
};

class RIPArray : public CPtrArray
{
	private:
		int m_nNum; //���ݎg�p����Ă���f�[�^��
	public:
		RIPArray(){
			m_nNum=0;
		}
		~RIPArray(){
			for(int i=0,imax=(int)GetSize();i<imax;i++){
				Element *p = (Element *)GetAt(i);
				if( p != NULL ){
					delete p;
				}
			}
		}
		void Back(){
			if( m_nNum > 0 ){
				m_nNum--; //�g�p�������炷
			}
		}
		void Clear(){
			m_nNum = 0; //�擪����g�p�\�ɂ���
		}

		//�g�p����Ă��Ȃ��̈�����擾
		Element* GetBack()
		{
			Element *pRi = NULL;
			m_nNum++; //�g�p���v���X
			if( GetSize() >= m_nNum ){ //
				pRi = (Element*)GetAt(m_nNum-1);
				pRi->Clear();
			}else{
				pRi = new Element;
				Add(pRi); //�V�K�ǉ�
			}
			return pRi;
		}
};

typedef	std::list<Element *>			RIPList;
typedef	std::list<Element *>::iterator	RIPIterator;
typedef	std::vector<Element *>			RIPVector;

///////////////////////////////////////////////////////////////////////////
//	class CLabeling       
///////////////////////////////////////////////////////////////////////////

class CLabeling 
{

private:	
	int	*m_pSrc_frame;
	int	m_nWidth;
	int	m_nHeight;
	int m_nWmin; //�v�f�ŏ���
	int m_nWmax; //�v�f�ő啝
	int m_nHmin; //�v�f�ŏ�����
	int m_nHmax; //�v�f�ő卂��
	int	m_nTotal_num;

	RasArray	m_aryRas; //���X�^�[�Z�O�����g�z��
	RSPList		*m_pRaster_segment_list;  //���X�^�[�Z�O�����g��񃊃X�g
	int			m_num_of_raster_segments;
	RSPQueue	m_seed_queue;
protected:
	RIPArray	m_aryRip; //�̈�z��
	int			m_nEmax; //�v�f��
	RIPVector	m_result_region_info;	//���x�����O���ʃx�N�g��
public:
	CLabeling();
	virtual ~CLabeling();

	// private functions
private:

	void RegisterSegment(const int lx, const int rx,const int  y, const int src_value );

	//���X�^�Z�O�����g�̘A��
	void SearchNeighboringSegment( RasterSegment *pRsSeed, const int dy );
	Element* ConnectRasterSegment(RasterSegment *pRsSeed,const int region_num);

	static	bool RevCompRegionInfoPointer( const Element * const &l,const Element * const &r )
	{
		return ( l->GetPixels() > r->GetPixels());
	}

public:
	//���x�����O���ʂ̎擾(int)
	void FillFrame(int *pDst, Element *pElem, const int fill_value );

	//���x�����O���ʂ̎擾(BYTE)
	void FillFrameByte(BYTE *pDst, Element *pElem, const BYTE byVal);

	inline	int	GetEmax( void ) const //�v�f���擾
	{
		return m_nEmax;
	}
	inline Element *GetElement(int nEnum) const //�̈���擾
	{
		return (nEnum < (int)m_result_region_info.size()) ? m_result_region_info[ nEnum ] : NULL;
	}
	Element* GetMaxElement(int *pEno=NULL);//�ő�v�f�擾	
	Element* GetMinElement(int *pEno=NULL);//�ŏ��v�f�擾
	int	Exec( int *pSrcBuf,int nWidth, int nHeight,int nMinSize,int nMaxSize,CRect &rect);//���x�����O���s	
	void RemoveElement(int nEnum);//�w��ԍ��v�f�폜
	CRect GetAllRect(); //�S�v�f���܂ޗ̈���擾	
	void SetWmin(int n){ m_nWmin = n; } //�v�f�ŏ���
	void SetWmax(int n){ m_nWmax = n; } //�v�f�ő啝
	void SetHmin(int n){ m_nHmin = n; } //�v�f�ŏ�����
	void SetHmax(int n){ m_nHmax = n; } //�v�f�ő卂��
};


#endif	// __LABELING_H__
