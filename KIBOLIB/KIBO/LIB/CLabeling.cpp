// CLabeling.cpp

#include "../stdafx.h"
#include "../LIB/functions.h"
#include "CLabeling.h"

//�w��̈���̉�f�����擾
int Element::GetPixels(int nLeft,int nTop,int nRight,int nBottom)
{		
	int nCnt = 0;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			if( nTop <= pRS->GetY() && pRS->GetY() <= nBottom ){
				if( pRS->GetRightX() < nLeft || nRight < pRS->GetLeftX()){ 
					continue ;//�������Ȃ�
				}
				if( nLeft <= pRS->GetLeftX() ){ //���[���̈���ɑ��݂��鎞
					if(pRS->GetRightX() <= nRight ){//�E�[���̈��
						nCnt += int(pRS->GetRightX() - pRS->GetLeftX() + 1);
					}else{//�E�[���̈�O
						nCnt += int(nRight - pRS->GetLeftX() + 1);
					}
				}else if(pRS->GetRightX()<=nRight){ //�E�[���̈��
					//���̎��K�����[�͗̈�O
					nCnt += int(pRS->GetRightX() - nLeft + 1);
				}else{ //�E�[�����[���̈�O
					nCnt += int(nRight-nLeft+1);
				}
			}
		}
	}
	return nCnt ;
}

//�����_�ȉ��̐��l�̂ݎ擾
double GetDecimal(double dbNum)
{
	int nNum = (int)dbNum; //�����_�ȉ��؎̂�
	return (dbNum-nNum); //�����_�ȉ��̐��l�̂ݎ擾
}

//�������̖ʐς��擾
double Element::GetPixelsLeft() 
{
	double dbArea = 0; //�ʐ�
	int nLeft = GetMinX(); //��
	double dbCenter = GetCenterX(); //���S�_
	int nCenter = (int)dbCenter;

	if( GetDecimal(dbCenter) > 0){ //���S�_������
		//���S�_�������ʐςō������ƂȂ�
		dbArea = GetPixels(nLeft,GetMinY(),nCenter,GetMaxY()); //�ʐώ擾
	}else{ 
		int nRight = nCenter-1;
		if(nLeft <= nRight){
			dbArea = GetPixels(nLeft,GetMinY(),nRight,GetMaxY()); //�������̖ʐ�
		}
		dbArea += GetPixels(nCenter,GetMinY(),nCenter,GetMaxY()) / 2.0; //���S�_�̖ʐς�1/2�Ōv�Z
	}
	return dbArea; //�ʐώ擾
}

//�E�����̖ʐς��擾
double Element::GetPixelsRight() 
{
	double dbArea = 0; //�ʐ�
	int nRight = GetMaxX(); //�E
	double dbCenter = GetCenterX(); //���S�_
	int nCenter = (int)dbCenter;

	if( GetDecimal(dbCenter) > 0 ){ //���S�_������
		//���S�_�������ʐςō������ƂȂ�
		dbArea = GetPixels(nCenter+1,GetMinY(),nRight,GetMaxY()); //�ʐώ擾
	}else{
		int nLeft = nCenter+1;
		if( nLeft <= nRight ){
			dbArea = GetPixels(nLeft,GetMinY(),nRight,GetMaxY()); //�E�����̖ʐ�
		}
		dbArea += GetPixels(nCenter,GetMinY(),nCenter,GetMaxY()) / 2.0; //���S�_�̖ʐς�1/2�Ōv�Z
	}
	return dbArea; //�ʐώ擾
}

//�㔼���̖ʐς��擾
double Element::GetPixelsTop()	 
{
	double dbArea = 0; //�ʐ�
	int nTop = GetMinY(); //��
	double dbCenter = GetCenterY(); //���S�_
	int nCenter = (int)dbCenter;
	if( GetDecimal(dbCenter) > 0 ){ //���S�_������
		//���S�_�������ʐςō������ƂȂ�
		dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nCenter); //�ʐώ擾
	}else{
		int nBottom = nCenter-1;
		if( nTop <= nBottom ){
			dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nBottom); //�ʐώ擾
		}
		dbArea += GetPixels(GetMinX(),nCenter,GetMaxX(),nCenter) / 2.0; //�ʐώ擾
	}
	return dbArea; //�ʐώ擾
}

//�������̖ʐς��擾
double Element::GetPixelsBottom()
{
	double dbArea = 0; //�ʐ�
	int nBottom = GetMaxY(); //��
	double dbCenter = GetCenterY(); //���S�_
	int nCenter = (int)dbCenter;
	if( GetDecimal(dbCenter) > 0 ){ //���S�_������
		//���S�_�������ʐςō������ƂȂ�
		dbArea = GetPixels(GetMinX(),nCenter+1,GetMaxX(),nBottom); //�ʐώ擾
	}else{
		int nTop = nCenter+1;
		if( nTop <= nBottom ){
			dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nBottom); //�ʐώ擾
		}
		dbArea += GetPixels(GetMinX(),nCenter,GetMaxX(),nCenter) / 2.0; //�ʐώ擾
	}
	return dbArea; //�ʐώ擾
}

//�ŏ������擾
double Element::GetLMin(double dbX,double dbY) 
{
	double dbMin = DBL_MAX;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;		
		if( pRS != NULL){
			int y = pRS->GetY();
			for(int x = pRS->GetLeftX();x<=pRS->GetRightX();x++){
				double dbLen = ::GetLength(x,y,dbX,dbY);
				if( dbLen < dbMin){ //��菬�����ق��Œu������
					dbMin = dbLen;
				}
			}
		}
	}
	return dbMin;
}
//�ő勗���擾
double Element::GetLMax(double dbX,double dbY) 
{
	double dbMax = 0;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			int y = pRS->GetY();
			for(int x = pRS->GetLeftX();x<=pRS->GetRightX();x++){
				double dbLen = ::GetLength(x,y,dbX,dbY);
				if( dbMax < dbLen ){ //���傫���ق��Œu������
					dbMax = dbLen;
				}
			}
		}
	}
	return dbMax;
}

//�~�`�x�擾
double Element::GetRoundness(int nBorder) 
{
	int nArea = GetPixels();
	return (double)(4*PI*nArea /(nBorder * nBorder));//�~�`�x
}

//�܂܂�Ă��܂����H
BOOL Element::IsContain(int x,int y)
{
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			if(  pRS->GetY()==y && pRS->GetLeftX() <= x && x <= pRS->GetRightX() ){//��������
				return TRUE;
			}
		}
	}	
	return FALSE;
}
double Element::AverageTop()
{
	mapIntInt mapY;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			for(int x= pRS->GetLeftX(); x<=pRS->GetRightX();x++ ){//��������
				int y = pRS->GetY() ;
				mapIntInt::iterator itmap = mapY.find(x);
				if( itmap == mapY.end() ){ //�����o�^
					mapY[x] = y;
				}else{//��r
					if( itmap->second > y){ //��菬��������
						mapY[x] = y;
					}
				}
			}
		}
	}
	int nCnt = 0;
	double dbY = 0;
	for(mapIntInt::iterator it=mapY.begin();it!=mapY.end();it++){
		dbY += it->second;
		nCnt++;
	}
	if( nCnt > 0 ){ //����
		dbY /= nCnt;
	}
	return dbY;
}
double Element::AverageBottom()
{
	mapIntInt mapY;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			for(int x= pRS->GetLeftX(); x<=pRS->GetRightX();x++ ){//��������
				int y = pRS->GetY() ;
				mapIntInt::iterator itmap = mapY.find(x);
				if( itmap == mapY.end() ){ //�����o�^
					mapY[x] = y;
				}else{ //��r
					if( itmap->second < y){ //���傫������
						mapY[x] = y;
					}
				}
			}
		}
	}
	int nCnt = 0;
	double dbY = 0;
	for(mapIntInt::iterator it=mapY.begin();it!=mapY.end();it++){
		dbY += it->second;
		nCnt++;
	}
	if( nCnt > 0 ){ //����
		dbY /= nCnt;
	}
	return dbY;
}
///////////////////////////////////////////////////////////////////////////
//	class CLabeling
///////////////////////////////////////////////////////////////////////////

CLabeling::CLabeling()
{
	//������
	m_pSrc_frame = NULL;
	m_nWidth = 0;
	m_nHeight= 0;
	m_nWmin = 0; //�v�f�ŏ���
	m_nWmax = MAXINT; //�v�f�ő啝
	m_nHmin = 0; //�v�f�ŏ�����
	m_nHmax = MAXINT; //�v�f�ő卂��
	m_nTotal_num = 0;

	m_num_of_raster_segments = 0;
	m_nEmax = 0;

	m_pRaster_segment_list = NULL;
}

CLabeling::~CLabeling()
{		
	// clear
	if( m_pRaster_segment_list != NULL ){
		delete [] m_pRaster_segment_list;
	}
	//���x�����O���ʃx�N�g��
	m_result_region_info.clear();
	m_seed_queue.empty();
}

//�s���Ƃ̃Z�O�����g�̓o�^
void CLabeling::RegisterSegment(const int lx, const int rx,const int  y, const int src_value )
{
	RasterSegment *pRs = m_aryRas.GetBack(lx, rx, y, src_value);

	m_pRaster_segment_list[ y ].push_back( pRs );
	m_num_of_raster_segments++;
}

//�אڃZ�O�����g����
void CLabeling::SearchNeighboringSegment( RasterSegment *pRsSeed, const int dy )
{
	RSPList	*rspl_p = &m_pRaster_segment_list[ pRsSeed->GetY() + dy ];
	int	rs_seed_lx = pRsSeed->GetLeftX();	//�ڑ������[
	int	rs_seed_rx = pRsSeed->GetRightX();	//�ڑ����E�[
	int	rs_seed_source_value = pRsSeed->GetSourceValue();//�ڑ��l
	
	if( rspl_p->size() <= 0 ){
		return;
	}
	RSPIterator	rspi = rspl_p->begin();//�ڑ���Z�O�����g
	while(( *rspi )->GetRightX() < (rs_seed_lx-1) ){//���[�ȉ��̃Z�O�����g���X�L�b�v
		rspi++;
		if(rspi == rspl_p->end()){
			return;
		}
	}
	RasterSegment *pRs;
	while( (pRs = *rspi)->GetLeftX() <= (rs_seed_rx+1) ){//�E�[�𒴂���܂Ń��[�v
		if(rs_seed_source_value == pRs->GetSourceValue()){//�����l����������
			rspi = rspl_p->erase( rspi );//�Z�O�����g���X�g����O��
			m_seed_queue.push( pRs );//�Z�O�����g�L���[�֓o�^
		}else{
			rspi++;//���̃Z�O�����g��
		}
		if( rspi == rspl_p->end()){
			return;
		}
	}
}

//�Z�O�����g�̘A��
Element* CLabeling::ConnectRasterSegment(RasterSegment *pRsSeed,const int region_num)
{
	Element *pRi = m_aryRip.GetBack(); //�g�p����Ă��Ȃ��V�K�̈���
	if( pRi != NULL ){
		int min_x = pRsSeed->GetLeftX();
		int max_x = pRsSeed->GetRightX();
		int min_y = pRsSeed->GetY();
		int max_y = pRsSeed->GetY();
		int source_value = pRsSeed->GetSourceValue();
		double sum_x = 0;
		double sum_y = 0;
	
		m_seed_queue.push( pRsSeed );

		int	num_of_pixels = 0;
		while( m_seed_queue.size() > 0 ){
			RasterSegment *rs = m_seed_queue.front();
			m_seed_queue.pop();//�L���[�̃Z�O�����g�����o��
			pRi->Push( rs ); //�̈�̃��X�g�ɃZ�O�����g��o�^

			int	n = rs->GetRightX() - rs->GetLeftX() + 1;//�Z�O�����g�̉�f��
			num_of_pixels += n;
			if ( rs->GetLeftX() < min_x ) {
				min_x = rs->GetLeftX();
			}
			if ( rs->GetRightX() > max_x ) {
				max_x = rs->GetRightX();
			}
			if ( rs->GetY() < min_y ) {
				min_y = rs->GetY();
			} else if ( rs->GetY() > max_y ) {
				max_y = rs->GetY();
			}
			sum_x += ( rs->GetLeftX() + rs->GetRightX()) * n; //�d�S�v�Zx
			sum_y += rs->GetY() * n;//�d�S�v�Zy

			if( rs->GetY() > 0 ){//1���C����̃Z�O�����g�ƘA��
				SearchNeighboringSegment( rs, -1 );
			}
			if( rs->GetY() < m_nHeight - 1 ){//1���C�����̃Z�O�����g�ƘA��
				SearchNeighboringSegment( rs, 1 );
			}
		}
		pRi->SetNumOfPixels( num_of_pixels );
		pRi->SetMinMax( min_x, min_y, max_x, max_y );
		pRi->SetSourceValue( source_value );
		pRi->SetResult( region_num );
		float gx = static_cast<float>( sum_x ) / ( 2 * num_of_pixels ); //�d�S�v�Zx
		float gy = static_cast<float>( sum_y ) / num_of_pixels;			//�d�S�v�Zy
		pRi->SetGravity( gx, gy );
	}
	return pRi;
}

//���x�����O���ʂ̎擾(int)
void CLabeling::FillFrame(int *pDst, Element *pElem, const int fill_value )
{
	if( pDst != NULL ){
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//�S�Z�O�����g���X�g����
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			int	*sp = pDst + lx + y * m_nWidth;
			for ( int i = 0, imax = ( rx - lx + 1 );i<imax; i++ ) {
				*sp++ = fill_value;
			}
		}
	}
}

//���x�����O���ʂ̎擾(BYTE)
void CLabeling::FillFrameByte(BYTE *pDst, Element *pElem, const BYTE byVal )
{
	if( pDst != NULL ){
		ZeroMemory(pDst,m_nWidth*m_nHeight); //Buffer Clear
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {
			RasterSegment *pRs = *rspi;
			int	lx = pRs->GetLeftX();
			int	rx = pRs->GetRightX();
			int	y = pRs->GetY();
			BYTE *sp = pDst + lx + y * m_nWidth;
			memset(sp,byVal,(rx-lx+1));
		}
	}
}

//���x�����O���s
int	CLabeling::Exec( int *pSrcBuf,int nWidth, int nHeight,int nMinSize,int nMaxSize,CRect &rect)
{
	//�����`�F�b�N
	if( pSrcBuf == NULL || nMinSize >= nMaxSize){
		return -1;
	}
	if(rect.left < 0 || rect.top < 0 || rect.right > nWidth || rect.bottom > nHeight){
		return -2;
	}

	// phase pre1
	//�S���C����1�Z�O�����g���X�g�Ƃ���
	if( m_pRaster_segment_list != NULL && m_nHeight < nHeight ){ //�ȑO�쐬�����Z�O�����g���X�g����������
		delete [] m_pRaster_segment_list;
		m_pRaster_segment_list = NULL; //clear
	}
	if( m_pRaster_segment_list == NULL ){//�����쐬����Ă��Ȃ��Ƃ�����
		m_pRaster_segment_list = new RSPList[ nHeight ];
	}
	m_aryRas.Clear(); //���X�^�Z�O�����g�N���A
	m_num_of_raster_segments = 0;

	m_pSrc_frame = pSrcBuf;
	m_nWidth     = nWidth;
	m_nHeight    = nHeight;
	m_nTotal_num = m_nWidth * m_nHeight;

	// phase 1		
	for( int y = rect.top; y < rect.bottom; y++ ){ //�����̈�w��
		int	lx = 0;
		int	current_src_value = 0;
		int	*pSrc = m_pSrc_frame + nWidth * y + rect.left; //�����J�n���W�w��
		int x = rect.left;
		for( ; x < rect.right; x++ ){//�����̈�w��
			if( *pSrc != current_src_value ){//���x�����O�_����
				if( current_src_value > 0 ){	// raster segment
					RegisterSegment( lx, x-1, y, current_src_value ); //�E�ׂ܂ł��Z�O�����g�ɓo�^ 
				}
				current_src_value = *pSrc;
				lx = x;//���[�o�^
			}
			pSrc++;
		}
		if( current_src_value > 0 ){//�Z�O�����g�o�^
			RegisterSegment( lx,  rect.right-1, y, current_src_value );
		}
	}

	// phase pre2
	m_result_region_info.clear();
	m_aryRip.Clear();

	m_nEmax = 0;
	// phase 2: connect
	for( int y = 0; y < m_nHeight; y++ ){//�e���C�����Ƃ�
		RSPList	*rspl_p = &m_pRaster_segment_list[ y ];
		while( rspl_p->size() > 0 ){//�S���X�^�Z�O�����g���[�v
			RSPIterator	rspi = rspl_p->begin();
			RasterSegment *pRs = *rspi;	// get 1 raster segment
			rspl_p->erase( rspi );		// remove from list ���X�^�[�Z�O�����g���X�g�͂����ō폜�����
			Element	*pElem = ConnectRasterSegment( pRs,m_nEmax+1 );			
			if (nMinSize < pElem->GetPixels() && pElem->GetPixels() < nMaxSize && 
				m_nWmin <	pElem->GetWidth() && pElem->GetWidth() < m_nWmax &&
				m_nHmin <	pElem->GetHeight() && pElem->GetHeight() < m_nHmax){//���x�����O�v�f����
				m_result_region_info.push_back(pElem);
				m_nEmax++;//���x�����O�v�f�𑝂₷
			}else{
				m_aryRip.Back(); //���������炷
			}
		}
	}

	// phase 3
	// reorder by size
	//���x�����O�ԍ��̃Z�b�g
	for ( int i = 0; i < m_nEmax; i++ ) {
		m_result_region_info[i]->SetResult(i+1);
	}
	return m_nEmax;
}

//�ő�v�f�擾
Element* CLabeling::GetMaxElement(int *pEno/*=NULL*/)
{
	Element *pInfoMax = NULL; //�ő�v�f
	int nEmax = 0; //�ő�v�f��
	for(int i=0,imax=(int)m_result_region_info.size();i<imax;i++){
		Element *pInfo =	m_result_region_info[i];
		if( pInfo->GetPixels() > nEmax ){
			pInfoMax = pInfo; //�ő�v�f�L��
			nEmax = pInfo->GetPixels();//�ő�l�����ւ�
			if(pEno != NULL){ //�ő�v�f�ԍ�
				*pEno = i; //�v�f�ԍ��Z�b�g
			}
		}
	}
	return pInfoMax;
}
//�ŏ��v�f�擾
Element* CLabeling::GetMinElement(int *pEno/*=NULL*/)
{
	Element *pInfoMin = NULL; //�ŏ��v�f
	int nEmin = INT_MAX; //�ŏ��v�f��
	for(int i=0,imax=(int)m_result_region_info.size();i<imax;i++){
		Element *pInfo = m_result_region_info[i];
		if( pInfo->GetPixels() < nEmin ){
			pInfoMin = pInfo; //�ő�v�f�L��
			nEmin = pInfo->GetPixels();//�ő�l�����ւ�
			if(pEno != NULL){ //�ő�v�f�ԍ�
				*pEno = i; //�v�f�ԍ��Z�b�g
			}
		}
	}
	return pInfoMin;
}

//�w��ԍ��v�f�폜
void CLabeling::RemoveElement(int nEnum)
{
	if( nEnum < (int)m_result_region_info.size() ){
		int nCnt=0;
		RIPVector::iterator it = m_result_region_info.begin();
		for( ; it!=m_result_region_info.end();it++,nCnt++){
			if( nCnt == nEnum){ //��������			
				break;
			}
		}
		m_result_region_info.erase(it);
		m_nEmax--;
	}		
}

//�S�v�f���܂ޗ̈���擾
CRect CLabeling::GetAllRect()
{
	CRect rcAll(0,0,0,0);//�S�v�f�̈�
	Element *pElem = GetElement(0);
	if( pElem != NULL ){
		rcAll = pElem->GetRect();//�̈揉����
		for(int i=1,imax=(int)m_result_region_info.size();i<imax;i++){//�S�̈挟��
			Element *pInfo = m_result_region_info[i];
			rcAll |= pInfo->GetRect();//�̈�ǉ�
		}
	}
	return rcAll;
}

