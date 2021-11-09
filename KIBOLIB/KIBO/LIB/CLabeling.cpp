// CLabeling.cpp

#include "../stdafx.h"
#include "../LIB/functions.h"
#include "CLabeling.h"

//指定領域内の画素数を取得
int Element::GetPixels(int nLeft,int nTop,int nRight,int nBottom)
{		
	int nCnt = 0;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			if( nTop <= pRS->GetY() && pRS->GetY() <= nBottom ){
				if( pRS->GetRightX() < nLeft || nRight < pRS->GetLeftX()){ 
					continue ;//何もしない
				}
				if( nLeft <= pRS->GetLeftX() ){ //左端が領域内に存在する時
					if(pRS->GetRightX() <= nRight ){//右端が領域内
						nCnt += int(pRS->GetRightX() - pRS->GetLeftX() + 1);
					}else{//右端が領域外
						nCnt += int(nRight - pRS->GetLeftX() + 1);
					}
				}else if(pRS->GetRightX()<=nRight){ //右端が領域内
					//この時必ず左端は領域外
					nCnt += int(pRS->GetRightX() - nLeft + 1);
				}else{ //右端も左端も領域外
					nCnt += int(nRight-nLeft+1);
				}
			}
		}
	}
	return nCnt ;
}

//小数点以下の数値のみ取得
double GetDecimal(double dbNum)
{
	int nNum = (int)dbNum; //小数点以下切捨て
	return (dbNum-nNum); //小数点以下の数値のみ取得
}

//左半分の面積を取得
double Element::GetPixelsLeft() 
{
	double dbArea = 0; //面積
	int nLeft = GetMinX(); //左
	double dbCenter = GetCenterX(); //中心点
	int nCenter = (int)dbCenter;

	if( GetDecimal(dbCenter) > 0){ //中心点が小数
		//中心点を除く面積で左半分となる
		dbArea = GetPixels(nLeft,GetMinY(),nCenter,GetMaxY()); //面積取得
	}else{ 
		int nRight = nCenter-1;
		if(nLeft <= nRight){
			dbArea = GetPixels(nLeft,GetMinY(),nRight,GetMaxY()); //左半分の面積
		}
		dbArea += GetPixels(nCenter,GetMinY(),nCenter,GetMaxY()) / 2.0; //中心点の面積を1/2で計算
	}
	return dbArea; //面積取得
}

//右半分の面積を取得
double Element::GetPixelsRight() 
{
	double dbArea = 0; //面積
	int nRight = GetMaxX(); //右
	double dbCenter = GetCenterX(); //中心点
	int nCenter = (int)dbCenter;

	if( GetDecimal(dbCenter) > 0 ){ //中心点が小数
		//中心点を除く面積で左半分となる
		dbArea = GetPixels(nCenter+1,GetMinY(),nRight,GetMaxY()); //面積取得
	}else{
		int nLeft = nCenter+1;
		if( nLeft <= nRight ){
			dbArea = GetPixels(nLeft,GetMinY(),nRight,GetMaxY()); //右半分の面積
		}
		dbArea += GetPixels(nCenter,GetMinY(),nCenter,GetMaxY()) / 2.0; //中心点の面積を1/2で計算
	}
	return dbArea; //面積取得
}

//上半分の面積を取得
double Element::GetPixelsTop()	 
{
	double dbArea = 0; //面積
	int nTop = GetMinY(); //上
	double dbCenter = GetCenterY(); //中心点
	int nCenter = (int)dbCenter;
	if( GetDecimal(dbCenter) > 0 ){ //中心点が小数
		//中心点を除く面積で左半分となる
		dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nCenter); //面積取得
	}else{
		int nBottom = nCenter-1;
		if( nTop <= nBottom ){
			dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nBottom); //面積取得
		}
		dbArea += GetPixels(GetMinX(),nCenter,GetMaxX(),nCenter) / 2.0; //面積取得
	}
	return dbArea; //面積取得
}

//下半分の面積を取得
double Element::GetPixelsBottom()
{
	double dbArea = 0; //面積
	int nBottom = GetMaxY(); //下
	double dbCenter = GetCenterY(); //中心点
	int nCenter = (int)dbCenter;
	if( GetDecimal(dbCenter) > 0 ){ //中心点が小数
		//中心点を除く面積で左半分となる
		dbArea = GetPixels(GetMinX(),nCenter+1,GetMaxX(),nBottom); //面積取得
	}else{
		int nTop = nCenter+1;
		if( nTop <= nBottom ){
			dbArea = GetPixels(GetMinX(),nTop,GetMaxX(),nBottom); //面積取得
		}
		dbArea += GetPixels(GetMinX(),nCenter,GetMaxX(),nCenter) / 2.0; //面積取得
	}
	return dbArea; //面積取得
}

//最小距離取得
double Element::GetLMin(double dbX,double dbY) 
{
	double dbMin = DBL_MAX;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;		
		if( pRS != NULL){
			int y = pRS->GetY();
			for(int x = pRS->GetLeftX();x<=pRS->GetRightX();x++){
				double dbLen = ::GetLength(x,y,dbX,dbY);
				if( dbLen < dbMin){ //より小さいほうで置き換え
					dbMin = dbLen;
				}
			}
		}
	}
	return dbMin;
}
//最大距離取得
double Element::GetLMax(double dbX,double dbY) 
{
	double dbMax = 0;
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			int y = pRS->GetY();
			for(int x = pRS->GetLeftX();x<=pRS->GetRightX();x++){
				double dbLen = ::GetLength(x,y,dbX,dbY);
				if( dbMax < dbLen ){ //より大きいほうで置き換え
					dbMax = dbLen;
				}
			}
		}
	}
	return dbMax;
}

//円形度取得
double Element::GetRoundness(int nBorder) 
{
	int nArea = GetPixels();
	return (double)(4*PI*nArea /(nBorder * nBorder));//円形度
}

//含まれていますか？
BOOL Element::IsContain(int x,int y)
{
	for(RSPList::iterator it = m_SegmentList.begin();it != m_SegmentList.end();it++){
		RasterSegment *pRS = *it;
		if( pRS != NULL){
			if(  pRS->GetY()==y && pRS->GetLeftX() <= x && x <= pRS->GetRightX() ){//見つかった
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
			for(int x= pRS->GetLeftX(); x<=pRS->GetRightX();x++ ){//見つかった
				int y = pRS->GetY() ;
				mapIntInt::iterator itmap = mapY.find(x);
				if( itmap == mapY.end() ){ //初期登録
					mapY[x] = y;
				}else{//比較
					if( itmap->second > y){ //より小さい方へ
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
	if( nCnt > 0 ){ //平均
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
			for(int x= pRS->GetLeftX(); x<=pRS->GetRightX();x++ ){//見つかった
				int y = pRS->GetY() ;
				mapIntInt::iterator itmap = mapY.find(x);
				if( itmap == mapY.end() ){ //初期登録
					mapY[x] = y;
				}else{ //比較
					if( itmap->second < y){ //より大きい方へ
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
	if( nCnt > 0 ){ //平均
		dbY /= nCnt;
	}
	return dbY;
}
///////////////////////////////////////////////////////////////////////////
//	class CLabeling
///////////////////////////////////////////////////////////////////////////

CLabeling::CLabeling()
{
	//初期化
	m_pSrc_frame = NULL;
	m_nWidth = 0;
	m_nHeight= 0;
	m_nWmin = 0; //要素最小幅
	m_nWmax = MAXINT; //要素最大幅
	m_nHmin = 0; //要素最小高さ
	m_nHmax = MAXINT; //要素最大高さ
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
	//ラベリング結果ベクトル
	m_result_region_info.clear();
	m_seed_queue.empty();
}

//行ごとのセグメントの登録
void CLabeling::RegisterSegment(const int lx, const int rx,const int  y, const int src_value )
{
	RasterSegment *pRs = m_aryRas.GetBack(lx, rx, y, src_value);

	m_pRaster_segment_list[ y ].push_back( pRs );
	m_num_of_raster_segments++;
}

//隣接セグメント検索
void CLabeling::SearchNeighboringSegment( RasterSegment *pRsSeed, const int dy )
{
	RSPList	*rspl_p = &m_pRaster_segment_list[ pRsSeed->GetY() + dy ];
	int	rs_seed_lx = pRsSeed->GetLeftX();	//接続元左端
	int	rs_seed_rx = pRsSeed->GetRightX();	//接続元右端
	int	rs_seed_source_value = pRsSeed->GetSourceValue();//接続値
	
	if( rspl_p->size() <= 0 ){
		return;
	}
	RSPIterator	rspi = rspl_p->begin();//接続先セグメント
	while(( *rspi )->GetRightX() < (rs_seed_lx-1) ){//左端以下のセグメントをスキップ
		rspi++;
		if(rspi == rspl_p->end()){
			return;
		}
	}
	RasterSegment *pRs;
	while( (pRs = *rspi)->GetLeftX() <= (rs_seed_rx+1) ){//右端を超えるまでループ
		if(rs_seed_source_value == pRs->GetSourceValue()){//同じ値が見つかった
			rspi = rspl_p->erase( rspi );//セグメントリストから外す
			m_seed_queue.push( pRs );//セグメントキューへ登録
		}else{
			rspi++;//次のセグメントへ
		}
		if( rspi == rspl_p->end()){
			return;
		}
	}
}

//セグメントの連結
Element* CLabeling::ConnectRasterSegment(RasterSegment *pRsSeed,const int region_num)
{
	Element *pRi = m_aryRip.GetBack(); //使用されていない新規領域情報
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
			m_seed_queue.pop();//キューのセグメントを取り出す
			pRi->Push( rs ); //領域のリストにセグメントを登録

			int	n = rs->GetRightX() - rs->GetLeftX() + 1;//セグメントの画素数
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
			sum_x += ( rs->GetLeftX() + rs->GetRightX()) * n; //重心計算x
			sum_y += rs->GetY() * n;//重心計算y

			if( rs->GetY() > 0 ){//1ライン上のセグメントと連結
				SearchNeighboringSegment( rs, -1 );
			}
			if( rs->GetY() < m_nHeight - 1 ){//1ライン下のセグメントと連結
				SearchNeighboringSegment( rs, 1 );
			}
		}
		pRi->SetNumOfPixels( num_of_pixels );
		pRi->SetMinMax( min_x, min_y, max_x, max_y );
		pRi->SetSourceValue( source_value );
		pRi->SetResult( region_num );
		float gx = static_cast<float>( sum_x ) / ( 2 * num_of_pixels ); //重心計算x
		float gy = static_cast<float>( sum_y ) / num_of_pixels;			//重心計算y
		pRi->SetGravity( gx, gy );
	}
	return pRi;
}

//ラベリング結果の取得(int)
void CLabeling::FillFrame(int *pDst, Element *pElem, const int fill_value )
{
	if( pDst != NULL ){
		RSPList& rspl = pElem->GetRasterSegmentList();
		for ( RSPIterator rspi = rspl.begin(); rspi != rspl.end(); rspi++ ) {//全セグメントリスト検索
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

//ラベリング結果の取得(BYTE)
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

//ラベリング実行
int	CLabeling::Exec( int *pSrcBuf,int nWidth, int nHeight,int nMinSize,int nMaxSize,CRect &rect)
{
	//引数チェック
	if( pSrcBuf == NULL || nMinSize >= nMaxSize){
		return -1;
	}
	if(rect.left < 0 || rect.top < 0 || rect.right > nWidth || rect.bottom > nHeight){
		return -2;
	}

	// phase pre1
	//全ラインを1セグメントリストとする
	if( m_pRaster_segment_list != NULL && m_nHeight < nHeight ){ //以前作成したセグメントリストが小さい時
		delete [] m_pRaster_segment_list;
		m_pRaster_segment_list = NULL; //clear
	}
	if( m_pRaster_segment_list == NULL ){//未だ作成されていないときだけ
		m_pRaster_segment_list = new RSPList[ nHeight ];
	}
	m_aryRas.Clear(); //ラスタセグメントクリア
	m_num_of_raster_segments = 0;

	m_pSrc_frame = pSrcBuf;
	m_nWidth     = nWidth;
	m_nHeight    = nHeight;
	m_nTotal_num = m_nWidth * m_nHeight;

	// phase 1		
	for( int y = rect.top; y < rect.bottom; y++ ){ //検査領域指定
		int	lx = 0;
		int	current_src_value = 0;
		int	*pSrc = m_pSrc_frame + nWidth * y + rect.left; //検査開始座標指定
		int x = rect.left;
		for( ; x < rect.right; x++ ){//検査領域指定
			if( *pSrc != current_src_value ){//ラベリング点発見
				if( current_src_value > 0 ){	// raster segment
					RegisterSegment( lx, x-1, y, current_src_value ); //右隣までをセグメントに登録 
				}
				current_src_value = *pSrc;
				lx = x;//左端登録
			}
			pSrc++;
		}
		if( current_src_value > 0 ){//セグメント登録
			RegisterSegment( lx,  rect.right-1, y, current_src_value );
		}
	}

	// phase pre2
	m_result_region_info.clear();
	m_aryRip.Clear();

	m_nEmax = 0;
	// phase 2: connect
	for( int y = 0; y < m_nHeight; y++ ){//各ラインごとに
		RSPList	*rspl_p = &m_pRaster_segment_list[ y ];
		while( rspl_p->size() > 0 ){//全ラスタセグメントループ
			RSPIterator	rspi = rspl_p->begin();
			RasterSegment *pRs = *rspi;	// get 1 raster segment
			rspl_p->erase( rspi );		// remove from list ラスターセグメントリストはここで削除される
			Element	*pElem = ConnectRasterSegment( pRs,m_nEmax+1 );			
			if (nMinSize < pElem->GetPixels() && pElem->GetPixels() < nMaxSize && 
				m_nWmin <	pElem->GetWidth() && pElem->GetWidth() < m_nWmax &&
				m_nHmin <	pElem->GetHeight() && pElem->GetHeight() < m_nHmax){//ラベリング要素発見
				m_result_region_info.push_back(pElem);
				m_nEmax++;//ラベリング要素を増やす
			}else{
				m_aryRip.Back(); //末尾を減らす
			}
		}
	}

	// phase 3
	// reorder by size
	//ラベリング番号のセット
	for ( int i = 0; i < m_nEmax; i++ ) {
		m_result_region_info[i]->SetResult(i+1);
	}
	return m_nEmax;
}

//最大要素取得
Element* CLabeling::GetMaxElement(int *pEno/*=NULL*/)
{
	Element *pInfoMax = NULL; //最大要素
	int nEmax = 0; //最大要素数
	for(int i=0,imax=(int)m_result_region_info.size();i<imax;i++){
		Element *pInfo =	m_result_region_info[i];
		if( pInfo->GetPixels() > nEmax ){
			pInfoMax = pInfo; //最大要素記憶
			nEmax = pInfo->GetPixels();//最大値差し替え
			if(pEno != NULL){ //最大要素番号
				*pEno = i; //要素番号セット
			}
		}
	}
	return pInfoMax;
}
//最小要素取得
Element* CLabeling::GetMinElement(int *pEno/*=NULL*/)
{
	Element *pInfoMin = NULL; //最小要素
	int nEmin = INT_MAX; //最小要素数
	for(int i=0,imax=(int)m_result_region_info.size();i<imax;i++){
		Element *pInfo = m_result_region_info[i];
		if( pInfo->GetPixels() < nEmin ){
			pInfoMin = pInfo; //最大要素記憶
			nEmin = pInfo->GetPixels();//最大値差し替え
			if(pEno != NULL){ //最大要素番号
				*pEno = i; //要素番号セット
			}
		}
	}
	return pInfoMin;
}

//指定番号要素削除
void CLabeling::RemoveElement(int nEnum)
{
	if( nEnum < (int)m_result_region_info.size() ){
		int nCnt=0;
		RIPVector::iterator it = m_result_region_info.begin();
		for( ; it!=m_result_region_info.end();it++,nCnt++){
			if( nCnt == nEnum){ //見つかった			
				break;
			}
		}
		m_result_region_info.erase(it);
		m_nEmax--;
	}		
}

//全要素を含む領域を取得
CRect CLabeling::GetAllRect()
{
	CRect rcAll(0,0,0,0);//全要素領域
	Element *pElem = GetElement(0);
	if( pElem != NULL ){
		rcAll = pElem->GetRect();//領域初期化
		for(int i=1,imax=(int)m_result_region_info.size();i<imax;i++){//全領域検査
			Element *pInfo = m_result_region_info[i];
			rcAll |= pInfo->GetRect();//領域追加
		}
	}
	return rcAll;
}

