#include "../stdafx.h"
#include "../KIBO.h"
#include "CharElement.h"

//文字特徴
CharElement::CharElement()
{
	//save data
	m_tChar			= 0;	//検査文字
	m_wRnum			= 0;	//領域番号
	m_nAreaA		= 0;	//面積All				（要素チェックメンバ）
	m_dbAreaT		= 0;	//面積上
	m_dbAreaB		= 0;	//面積下
	m_dbAreaL		= 0;	//面積左
	m_dbAreaR		= 0;	//面積右
	m_dbGx			= 0;	//重心x
	m_dbGy			= 0;	//重心y
	m_wContourNum	= 0;	//境界線の数
	m_dbContourOut	= 0;	//外部境界線の長さ
	m_dbContourIn	= 0;	//内部境界線の長さ
	m_dbCircle		= 0;	//円形度
	m_byEdgeLR		= 0;	//左から右方向のエッジ数
	m_byEdgeRL		= 0;	//右から左方向のエッジ数
	m_byEdgeTB		= 0;	//上から下方向のエッジ数
	m_byEdgeBT		= 0;	//下から上方向のエッジ数
	m_rect.left     = 0;	//文字領域				（要素チェックメンバ）
	m_rect.top      = 0;	//文字領域				（要素チェックメンバ）
	m_rect.right    = 0;	//文字領域				（要素チェックメンバ）
	m_rect.bottom   = 0;	//文字領域				（要素チェックメンバ）
	for(int i=0;i<POINT_NUM;i++){
		m_rLeft[i]  = 0;	//左特徴点からの距離
		m_rTop[i]	= 0;	//上特徴点からの距離
		m_rRight[i]	= 0;	//右特徴点からの距離
		m_rBottom[i]= 0;	//下特徴点からの距離
	}
}

CharElement::~CharElement()
{
}

CharElement& CharElement::operator=(IN const CharElement &e)
{
	m_tChar			= e.m_tChar			;	//検査文字
	m_wRnum			= e.m_wRnum			;	//領域番号
	m_nAreaA		= e.m_nAreaA		;	//面積All				（要素チェックメンバ）
	m_dbAreaT		= e.m_dbAreaT		;	//面積上
	m_dbAreaB		= e.m_dbAreaB		;	//面積下
	m_dbAreaL		= e.m_dbAreaL		;	//面積左
	m_dbAreaR		= e.m_dbAreaR		;	//面積右
	m_dbGx			= e.m_dbGx			;	//重心x
	m_dbGy			= e.m_dbGy			;	//重心y
	m_wContourNum	= e.m_wContourNum	;	//境界線の数
	m_dbContourOut	= e.m_dbContourOut	;	//外部境界線の長さ
	m_dbContourIn	= e.m_dbContourIn	;	//内部境界線の長さ
	m_dbCircle		= e.m_dbCircle		;	//円形度
	m_byEdgeLR		= e.m_byEdgeLR		;	//左から右方向のエッジ数
	m_byEdgeRL		= e.m_byEdgeRL		;	//右から左方向のエッジ数
	m_byEdgeTB		= e.m_byEdgeTB		;	//上から下方向のエッジ数
	m_byEdgeBT		= e.m_byEdgeBT		;	//下から上方向のエッジ数
	m_rect.left     = e.m_rect.left     ;	//文字領域（要素チェックメンバ）
	m_rect.top      = e.m_rect.top      ;	//文字領域（要素チェックメンバ）
	m_rect.right    = e.m_rect.right    ;	//文字領域（要素チェックメンバ）
	m_rect.bottom   = e.m_rect.bottom   ;	//文字領域（要素チェックメンバ）
	for(int i=0;i<POINT_NUM;i++){
		m_rLeft[i]  = e.m_rLeft[i];		//左特徴点からの距離（要素チェックメンバ）
		m_rTop[i]	= e.m_rTop[i];		//上特徴点からの距離（要素チェックメンバ）
		m_rRight[i]	= e.m_rRight[i];	//右特徴点からの距離（要素チェックメンバ）
		m_rBottom[i]= e.m_rBottom[i];	//下特徴点からの距離（要素チェックメンバ）
	}
	return *this;
}

//距離の総和
//左特徴点からの距離総和
REAL CharElement::GetLengthLeft()
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rLeft[i];
	}
	return rRet;
}

//上特徴点からの距離総和
REAL CharElement::GetLengthTop()	
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rTop[i];
	}
	return rRet;
}

//右特徴点からの距離総和
REAL CharElement::GetLengthRight()	
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rRight[i];
	}
	return rRet;
}

//下特徴点からの距離総和
REAL CharElement::GetLengthBottom()
{
	REAL rRet = 0;
	for(int i=0;i<POINT_NUM;i++){
		rRet += m_rBottom[i];
	}
	return rRet;
}
//左特徴点からの距離
CString CharElement::GetLeftCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rLeft[0]); //初期値
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rLeft[i]);
		strRet += strTmp;
	}
	return strRet;
}
//上特徴点からの距離
CString CharElement::GetTopCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rTop[0]); //初期値
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rTop[i]);
		strRet += strTmp;
	}
	return strRet;
}
//右特徴点からの距離
CString CharElement::GetRightCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rRight[0]); //初期値
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rRight[i]);
		strRet += strTmp;
	}
	return strRet;
}
//下特徴点からの距離
CString CharElement::GetBottomCsv()
{
	CString strRet;
	strRet.Format(L"%.3f",m_rBottom[0]); //初期値
	CString strTmp;
	for(int i=1;i<POINT_NUM;i++){
		strTmp.Format(L",%.3f",m_rBottom[i]);
		strRet += strTmp;
	}
	return strRet;
}

//左特徴点からの距離設定
void CharElement::SetLeft(ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptLB)
{
	CPoint pt = ptLT;//初期値
	m_rLeft[0] = (REAL)aryPoint.GetLength(pt);//左上の頂点からの距離

	REAL rDelta = (REAL)(ptLB.y - ptLT.y) / POINT_NUM; //差分取得	
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLT.y + rDelta*i); //バンド開始座標
		int nEnd   = (int)(ptLT.y + rDelta*(i+1)); //バンド終了座標
		for(int y=nStart;y<nEnd;y++){ //バンド距離作成
			pt.y = y;
			rSum += (REAL)aryPoint.GetLength(pt);//距離設定
		}
		m_rLeft[i] = rSum / (nEnd-nStart); //バンド距離の平均を取得
	}
}
//上特徴点からの距離設定
void CharElement::SetTop(ArrayPoint &aryPoint,const CPoint &ptLT,const CPoint &ptRT)
{
	CPoint pt = ptRT;//初期値
	m_rTop[0] = (REAL)aryPoint.GetLength(pt);//右上頂点からの距離

	pt = ptLT;//初期値
	REAL rDelta = (REAL)(ptRT.x - ptLT.x) / POINT_NUM; //差分取得
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLT.x + rDelta*i);  //バンド開始座標
		int nEnd   = (int)(ptLT.x + rDelta*(i+1));//バンド終了座標
		for(int x=nStart;x<nEnd;x++){ //バンド距離作成
			pt.x = x;
			rSum += (REAL)aryPoint.GetLength(pt);//距離設定
		}
		m_rTop[i] = rSum / (nEnd-nStart); //バンド距離の平均を取得
	}
}
//右特徴点からの距離設定
void CharElement::SetRight(ArrayPoint &aryPoint,const CPoint &ptRT,const CPoint &ptRB)
{
	CPoint pt = ptRB;//初期値
	m_rRight[0] = (REAL)aryPoint.GetLength(pt);//右下頂点からの距離

	pt = ptRT;//初期値
	REAL rDelta = (REAL)(ptRB.y - ptRT.y) / POINT_NUM; //差分取得	
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart  = (int)(ptRT.y + rDelta*i); //バンド開始座標
		int nEnd	= (int)(ptRT.y + rDelta*(i+1));   //バンド終了座標
		for(int y=nStart;y<nEnd;y++){ //バンド距離作成
			pt.y = y;
			rSum += (REAL)aryPoint.GetLength(pt);//距離設定
		}
		m_rRight[i] = rSum / (nEnd-nStart); //バンド距離の平均を取得
	}	
}
//下特徴点からの距離設定
void CharElement::SetBottom(ArrayPoint &aryPoint,const CPoint &ptLB,const CPoint &ptRB)
{
	CPoint pt = ptLB;//初期値
	m_rBottom[0] = (REAL)aryPoint.GetLength(pt);//左下頂点からの距離

	REAL rDelta = (REAL)(ptRB.x - ptLB.x) / POINT_NUM; //差分取得
	for(int i=1;i<POINT_NUM;i++){
		REAL rSum = 0;
		int nStart = (int)(ptLB.x + rDelta*i);  //バンド開始座標
		int nEnd   = (int)(ptLB.x + rDelta*(i+1));//バンド終了座標
		for(int x=nStart;x<nEnd;x++){ //バンド距離作成
			pt.x = x;
			rSum += (REAL)aryPoint.GetLength(pt);//距離設定
		}
		m_rBottom[i] = rSum / (nEnd-nStart); //バンド距離の平均を取得
	}
}

//左距離はOKですか？
BOOL CharElement::IsLeftOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rLeft[i] < dbLenMin[i]){ //最小値より小さい
			strErrType = L"左端距離不足";
			return FALSE;
		}else if(m_rLeft[i] > dbLenMax[i]){ //最大値より大きい
			strErrType = L"左端距離超過";
			return FALSE;
		}
	}
	return TRUE;
}
//上距離はOKですか？
BOOL CharElement::IsTopOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rTop[i] < dbLenMin[i]){//最小値より小さい
			strErrType = L"上端距離不足";
			return FALSE;
		}else if(m_rTop[i] > dbLenMax[i]){//最大値より大きい
			strErrType = L"上端距離超過";
			return FALSE;
		}
	}
	return TRUE;
}
//右距離はOKですか？
BOOL CharElement::IsRightOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rRight[i] < dbLenMin[i]){//最小値より小さい
			strErrType = L"右端距離不足";
			return FALSE;
		}else if(m_rRight[i] > dbLenMax[i]){//最大値より大きい
			strErrType = L"右端距離超過";
			return FALSE;
		}
	}
	return TRUE;
}
//下距離はOKですか？
BOOL CharElement::IsBottomOK(const double dbLenMin[],const double dbLenMax[],CString &strErrType)
{
	for(int i=0;i<POINT_NUM;i++){
		if(m_rBottom[i] < dbLenMin[i]){//最小値より小さい
			strErrType = L"右端距離不足";
			return FALSE;
		}else if(m_rBottom[i] > dbLenMax[i]){//最大値より大きい
			strErrType = L"右端距離超過";
			return FALSE;
		}
	}
	return TRUE;
}