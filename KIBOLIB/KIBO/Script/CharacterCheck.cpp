#include "../stdafx.h"
#include "../LIB/typedef.h"
#include "../LIB/functions.h"
#include "../LIB/macro.h"
#include "../LIB/PointDbl.h"
#include "../KIBOView.h"
#include "../Commandid.h"
#include "CharRecog.h"

#ifdef USE_CHARRECOG 

//Line Pointの表示
void CheckResult::ShowLine(LPWSTR szName,CPoint &ptTL,CLine& line,Color color)
{	
	CLine lineDraw(line.m_ptS+ptTL,line.m_ptE+ptTL);
	CString strName;
	strName.Format(L"%s%d%d",szName,ptTL.x,ptTL.y);	
	GetView()->m_mgrVO.CreateLineObject(strName,lineDraw,color.ToCOLORREF());
}

//最小ライン描画
void CheckResult::ShowLine()
{
	ShowLine(L"W",m_ptTL,m_line,Color::Magenta); //横幅描画
}
//最小ラインの表示
void CheckResult::ShowMinLine(SizeDbl &size)
{
	if(m_line.IsValid() && m_line.GetLength() < size.cx ){
		ShowLine();
	}
}
//X方向検査結果設定 Color::Magenta
void CheckResult::SetLine(CLine &line)
{
	if( m_line.IsValid() ){ //すでに値がセットされている時
		if( line.GetLength() < m_line.GetLength() ){ //より小さいラインの時だけ
			m_line = line; //上書きセット
		}
	}else{//値がセットされていない時
		//無条件でセット
		m_line=line;
	}
}	
void CheckResult::SetLine(CPoint &ptS,CPoint &ptE)
{
	CLine line(ptS,ptE); //ライン作成
	SetLine(line);
}

bool operator < (const CPoint &ptL,const CPoint &ptR)
{
	if( ptL.x != ptR.x ){
		return (ptL.x < ptR.x);
	}
	return (ptL.y < ptR.y);
}

//長さを決定する点クラス
class LengthPoint
{
public:
	CPoint m_ptMin1;
	CPoint m_ptMin2;
	double m_dbWmin; //最小距離間の距離
	CPoint m_ptMax1;
	CPoint m_ptMax2;
	double m_dbWmax; //最大距離間の距離

public:
	//デフォルトコンストラクタ
	LengthPoint()
	{
		m_dbWmin = 0;
		m_dbWmax = 0;
	}
	~LengthPoint(){}

	//外側境界線と内側境界線を作成する
	static void GetArrayPointInOut(ArrayPoint &aryPoint,ArrayPoint &aryPointOut,ArrayPoint &aryPointIn)
	{
		ArrayPoint *pAryPoint = &aryPointOut;
		for(int i=0;i<aryPoint.GetSize();i++){
			CPoint &pt = aryPoint[i];
			if( pt.x < 0 ){	
				if( aryPointOut.GetSize() == 0 ){
					pAryPoint = &aryPointOut;
				}else if( aryPointIn.GetSize() == 0 ){
					pAryPoint = &aryPointIn;
				}else{
					pAryPoint = &aryPointIn; //Out,In,In の順番に点を追加
				}
			}else if(pAryPoint != NULL){
				pAryPoint->Add(pt);
			}
		}
	}
	//外側境界線と内側境界線を作成する
	static void GetArrayPointInOut(ArrayPoint &aryPoint,ArrayPoint &aryPointOut,ArrayPoint &aryPointIn1,ArrayPoint &aryPointIn2)
	{
		ArrayPoint *pAryPoint = &aryPointOut;
		for(int i=0;i<aryPoint.GetSize();i++){
			CPoint &pt = aryPoint[i];
			if( pt.x < 0 ){	
				if( aryPointOut.GetSize() == 0 ){
					pAryPoint = &aryPointOut;//外側への書き込み
				}else if( aryPointIn1.GetSize() == 0 ){
					pAryPoint = &aryPointIn1; //内側境界線1への書き込み
				}else if( aryPointIn2.GetSize() == 0 ){
					pAryPoint = &aryPointIn2; //内側境界線2への書き込み
				}else{
					pAryPoint = &aryPointOut; //Out,In1,In2,Out の順番に点を追加
				}
			}else if(pAryPoint != NULL){
				pAryPoint->Add(pt);
			}
		}
	}
};

//指定x座標で頂点から最初にぶつかるY座標を求める
static int ToBtm(Label &label,int x) 
{
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	BYTE *pFind = pBits + x; //検索開始アドレス
	for(int y=0;y<nCy;y++,pFind+=nCx){
		if( *pFind == MAXBYTE){ //ラインにぶつかった
			return y; //y座標取得
		}
	}
	return -1;
}
//指定x座標で底から最初にぶつかるY座標を求める
static int ToTop(Label &label,int x) 
{
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	int nYs = nCy-1;
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	BYTE *pFind = pBits +nYs*nCx+ x; //検索開始アドレス
	for(int y=nYs;y>=0;y--,pFind-=nCx){
		if( *pFind  == MAXBYTE){ //ラインにぶつかった
			return y; //y座標取得
		}
	}
	return -1;
}

//上から下方向の線の太さチェック
double CharRecog::TopToBtm(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);

	//左右を狭める
	int nLeft  = aryPoint.GetLeft().x;	//最も左の点を取得
	int nRight = aryPoint.GetRight().x;	//最も右の点を取得

	aryPoint.RangeX(nLeft+dbDelete,nRight-dbDelete); //指定範囲のみのPointに変更 線幅の半分を検査不可能領域とする

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptTop = aryPoint.GetTop(); //頂点座標取得
	aryPoint.CollectConnectPoint( ptTop ,aryConnect); //頂点座標との隣接Pointのみ
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //隣接Point以外を収集	
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //指定数接続Pointのみを選択

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin){ //検査結果
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;
}
//下から上方向の線の太さチェック
double CharRecog::BtmToTop(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);

	//左右を狭める
	int nLeft  = aryPoint.GetLeft().x;	//最も左の点を取得
	int nRight = aryPoint.GetRight().x;	//最も右の点を取得
	aryPoint.RangeX(nLeft+dbDelete,nRight-dbDelete); //指定範囲のみのPointに変更 線幅の半分を検査不可能領域とする

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptBtm = aryPoint.GetBottom(); //頂点座標取得
	aryPoint.CollectConnectPoint( ptBtm,aryConnect); //頂点座標との隣接Pointのみ
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //隣接Point以外を収集
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //指定数接続Pointのみを選択

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //検査結果
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;
}

//左から右方向の線の幅チェック
double CharRecog::LeftToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);

	//上下を狭める
	int nTop = aryPoint.GetTop().y;	//最も上の点を取得
	int nBtm = aryPoint.GetBottom().y;	//最も下の点を取得

	aryPoint.RangeY(nTop+dbDelete,nBtm-dbDelete); //指定範囲のみのPointに変更 線幅の半分を検査不可能領域とする

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptLeft = aryPoint.GetLeft(); //左端座標取得
	aryPoint.CollectConnectPoint( ptLeft ,aryConnect); //左端座標との隣接Pointのみ
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //隣接Point以外を収集
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //指定数接続Pointのみを選択

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //検査結果
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;	
}

//右から左方向の線の幅チェック
double CharRecog::RightToLeft(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);

	//上下を狭める
	int nTop = aryPoint.GetTop().y;	//最も左の点を取得
	int nBtm = aryPoint.GetBottom().y;	//最も右の点を取得
	aryPoint.RangeY(nTop+dbDelete,nBtm-dbDelete); //指定範囲のみのPointに変更 線幅の半分を検査不可能領域とする

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptRight = aryPoint.GetRight(); //左端座標取得
	aryPoint.CollectConnectPoint( ptRight,aryConnect); //左端座標との隣接Pointのみ
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //隣接Point以外を収集
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //指定数接続Pointのみを選択

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //検査結果
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;	
}
 //中央から左
double CharRecog::CenterToLeft(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);
	//左半分だけにする
	CRect rect = aryPoint.GetRect();
	aryPoint.RangeY(0,rect.CenterPoint().x+dbDelete);
	return RightToLeft(aryPoint,dbDelete,dbLenMin);
}
//中央から右
double CharRecog::CenterToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//境界線のコピー作成
	aryPoint.Copy(aryPointSrc);
	//右半分だけにする
	CRect rect = aryPoint.GetRect();
	aryPoint.RangeY(rect.CenterPoint().x-dbDelete,rect.right);
	return RightToLeft(aryPoint,dbDelete,dbLenMin);
}

//内側境界線と外側境界線間の距離検査
SizeDbl CharRecog::CheckInOut(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint)
{
	ArrayPoint aryPointOut;	//外側境界線
	ArrayPoint aryPointIn;	//内側境界線

	LengthPoint lenPt;//内側と外側の境界線間の距離を求める
	lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //外側境界線と内側境界線を作成する

	CPoint ptS,ptE;
	double dbLen = aryPointOut.GetLength(aryPointIn,ptS,ptE);
	SizeDbl size;
	if( dbLen < DBL_MAX ){
		size.cx = dbLen;
		size.cy = dbLen;
		m_ckResult.SetLine(ptS,ptE);
	}
	return size;
}

//内側境界線と外側境界線間の距離検査
SizeDbl CharRecog::CheckIn2Out1(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint)
{
	ArrayPoint aryPointOut;	//外側境界線
	ArrayPoint aryPointIn1;	//内側境界線
	ArrayPoint aryPointIn2;	//内側境界線

	LengthPoint lenPt;//内側と外側の境界線間の距離を求める
	lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn1,aryPointIn2); //外側境界線と内側境界線を作成する

	CPoint ptS1,ptE1;
	double dbLen1 = aryPointOut.GetLength(aryPointIn1,ptS1,ptE1);
	CPoint ptS2,ptE2;
	double dbLen2 = aryPointOut.GetLength(aryPointIn2,ptS2,ptE2);
	SizeDbl size;
	if( dbLen1 < dbLen2 ){
		size.cx = dbLen1;
		size.cy = dbLen1;
		m_ckResult.SetLine(ptS1,ptE1);
	}else if(dbLen2 <= dbLen1){
		size.cx = dbLen2;
		size.cy = dbLen2;
		m_ckResult.SetLine(ptS2,ptE2);
	}
	return size;
}

//最短距離の配列番号を取得
int CharRecog::GetMinLengthIndex(CPoint &pt)
{
	int i=0;
	int imax=m_aryPoint.GetSize();
	for(;i<imax;i++){
		if(m_aryPoint[i]==pt){ //同一座標が存在する時
			return i;
		}
	}
	//同一座標が存在しない時
	int nRet = 0;
	double dbMin = INT_MAX;
	for(i=0;i<imax;i++){
		double dbLen = ::GetLength(pt,m_aryPoint[i]); //線の両端を含む
		if( dbLen < dbMin ){ //より小さい値に変更
			dbMin = dbLen;
			nRet = i;
		}
	}
	return nRet;
}

//境界線上の配列内距離を取得
int CharRecog::GetIndexLen(CPoint &pt1,CPoint &pt2)
{
	int nIndex1 = GetMinLengthIndex(pt1);
	int nIndex2 = GetMinLengthIndex(pt2);

	int nDelta1 = abs( nIndex1-nIndex2 );
	int nDelta2 = abs(nDelta1 - m_aryPoint.GetSize());
	return min(nDelta1,nDelta2); //index間距離を取得
}

//左から右方向へのエッジの数を求める
int CharRecog::GetEdgeNumLeftToRight(int nRnum,int nStartX,AryLine *pOutput, AryLine &aryLineH,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//今のIn Point	
	CPoint ptPrev(-1,-1);//前のIn Point	
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	int nLastY = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //エッジですか？

	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cx,MIN_EDGE_LEN); //エッジ判定距離

	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx + nStartX; //検索開始アドレス
		ptNow.y = y;
		for(int x=nStartX,xmax=nCx-1;x<xmax;x++,pFind++){
			ptNow.x = x;
			if( *pFind==MAXBYTE && *(pFind+1)==MAXBYTE){ //In Pointが見つかった
				isEdge = FALSE;
				lineNow.Clear(); //ライン上の点をクリア
				if( ptPrev.x >= 0 && abs(nLastY-y) >= 2){ //検査可能 && エッジの連続不可					
					if( aryLineH.GetLine(linePrev,ptPrev)  //一つ前の座標を取得
					&&  aryLineH.GetLine(lineNow ,ptNow) ){ //指定座標の水平線を取得
						if(  abs(ptNow.x-ptPrev.x) > dbEdgeLen){ //ラインの開始点が離れているとき
							nEdgeNum++;
							nLastY = y;
							isEdge = TRUE;
							if( pOutput != NULL ){
								pOutput->RemoveY(y-1); //エッジ近辺削除
								pOutput->RemoveY(y-2); //エッジ近辺削除
							}
						}
					}
				}else if(ptPrev.x < 0){ //開始ポイント
					nEdgeNum++;
					nLastY = y;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastY-y) >= 2){ //エッジではない&& エッジ近辺でもない時だけ
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //ライン追加
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.y-nLastY) >= 2 ){
		nEdgeNum++; //エッジの数
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//右から左方向へのエッジの数を求める
int CharRecog::GetEdgeNumRightToLeft(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//今のIn Point	
	CPoint ptPrev(-1,-1);//前のIn Point	
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	int nLastY = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //エッジですか？

	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cx,MIN_EDGE_LEN); //エッジ判定距離

	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx + nStartX; //検索開始アドレス
		ptNow.y = y;
		for(int x=nStartX;x>0;x--,pFind--){
			ptNow.x = x;
			if( *pFind==MAXBYTE && *(pFind-1)==MAXBYTE ){ //In Pointが見つかった
				isEdge = FALSE;
				lineNow.Clear(); //ライン上の点をクリア
				if( ptPrev.x >= 0 && abs(nLastY-y) >= 2){ //検査可能 && エッジの連続不可					
					if( aryLineH.GetLine(linePrev,ptPrev)  //一つ前の座標を取得
					&&  aryLineH.GetLine(lineNow ,ptNow) ){ //指定座標の水平線を取得
						if(  abs(ptNow.x-ptPrev.x) > dbEdgeLen ){ //ラインの開始点が離れているとき
							nEdgeNum++;
							nLastY = y;
							isEdge = TRUE;
							if( pOutput != NULL ){
								pOutput->RemoveY(y-1); //エッジ近辺削除
								pOutput->RemoveY(y-2); //エッジ近辺削除
							}
						}
					}
				}else if(ptPrev.x < 0){ //開始ポイント
					nEdgeNum++;
					nLastY = y;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastY-y) >= 2){ //エッジではない&& エッジ近辺でもない時だけ
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //ライン追加
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.y-nLastY) >= 2 ){
		nEdgeNum++; //エッジの数
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//上から下方向のエッジ数
int CharRecog::GetEdgeNumTopToBtm(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs/*=0*/)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//今のIn Point	
	CPoint ptPrev(-1,-1);//前のIn Point	
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	int nLastX = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //エッジですか？
	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cy,MIN_EDGE_LEN); //エッジ判定距離
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + nYs*nCx + x; //検索開始アドレス
		ptNow.x = x;
		for(int y=nYs,ymax=nCy-1;y<ymax;y++,pFind+=nCx){
			ptNow.y = y;
			if( *pFind==MAXBYTE && *(pFind+nCx)==MAXBYTE ){ //In Pointが見つかった
				isEdge = FALSE;
				lineNow.Clear(); //ライン上の点をクリア
				if( ptPrev.x >= 0 ){ //検査可能 && エッジの連続不可
					if( aryLineV.GetLine(linePrev,ptPrev)   //一つ前の座標を取得
					&&  aryLineV.GetLine(lineNow ,ptNow) ){ //指定座標の高さを取得						
						if(  abs(ptNow.y-ptPrev.y) > dbEdgeLen){ //ラインの開始点が離れているとき
							nEdgeNum++;
							nLastX = x;
							isEdge = TRUE;
							////////////////////////////////////////////FOR DEBUG/////////////////////
							//CPoint ptEdge = ptNow;
							//CRect rect(ptEdge.x+ptTL.x,ptEdge.y+ptTL.y,ptEdge.x+1+ptTL.x,ptEdge.y+1+ptTL.y);
							//GetView()->m_mgrVO.CreateRectObject(L"",rect,COLOR_RED,TRUE);					
							////////////////////////////////////////////FOR DEBUG/////////////////////
							if( pOutput != NULL ){
								pOutput->RemoveX(x-1); //エッジ近辺削除
								pOutput->RemoveX(x-2); //エッジ近辺削除
							}
						}
					}
				}else if(ptPrev.x < 0){ //開始ポイント
					nEdgeNum++;
					nLastX = x;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastX-x) >= 2){ //エッジではないときだけ
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //ライン追加
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.x-nLastX) >= 2 ){
		nEdgeNum++; //エッジの数
		if( pOutput != NULL ){
			int nXmax = pOutput->GetMaxX();
			pOutput->RemoveX(nXmax);
			pOutput->RemoveX(nXmax-1);
		}
	}
	return nEdgeNum;
}

//下から上方向のエッジ数
int CharRecog::GetEdgeNumBtmToTop(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//今のIn Point	
	CPoint ptPrev(-1,-1);//前のIn Point	
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	int nLastX = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //エッジですか？
	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cy,MIN_EDGE_LEN); //エッジ判定距離
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + (nCy-1)*nCx + x; //検索開始アドレス
		ptNow.x = x;
		for(int y=nCy-1;y>0;y--,pFind-=nCx){
			ptNow.y = y;
			if( *pFind==MAXBYTE && *(pFind-nCx)==MAXBYTE ){ //In Pointが見つかった
				isEdge = FALSE;
				lineNow.Clear(); //ライン上の点をクリア
				if( ptPrev.x >= 0 && abs(nLastX-x) >= 2 ){ //検査可能 && エッジの連続不可
					if( aryLineV.GetLine(linePrev,ptPrev)   //一つ前の座標を取得
					&&  aryLineV.GetLine(lineNow ,ptNow) ){ //指定座標の高さを取得
						SizeDbl size = GetDefLineWidth(nRnum);
						if(  abs(ptNow.y-ptPrev.y) > dbEdgeLen ){ //ラインの開始点が離れているとき
							nEdgeNum++;
							nLastX = x;
							isEdge = TRUE;
							////////////////////////////////////////////FOR DEBUG/////////////////////
							//CPoint ptEdge = ptNow;
							//CRect rect(ptEdge.x+ptTL.x,ptEdge.y+ptTL.y,ptEdge.x+1+ptTL.x,ptEdge.y+1+ptTL.y);
							//GetView()->m_mgrVO.CreateRectObject(L"",rect,COLOR_RED,TRUE);					
							////////////////////////////////////////////FOR DEBUG/////////////////////
							if( pOutput != NULL ){
								pOutput->RemoveX(x-1); //エッジ近辺削除
								pOutput->RemoveX(x-2); //エッジ近辺削除
							}
						}
					}
				}else if(ptPrev.x < 0){ //開始ポイント
					nEdgeNum++;
					nLastX = x;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastX-x) >= 2){ //エッジではないときだけ
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //ライン追加
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.x-nLastX) >= 2 ){
		nEdgeNum++; //エッジの数
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//縦方向センターラインを横切る線の数をカウントします
BOOL CharRecog::CenterLineVCheck(int nRnum,TCHAR tch,Label &label,int nCheck)
{
	if( !IsInclination(nRnum) ){ //斜め文字以外のときだけ
		int nRet = 0;
		BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
		int nCx = label.GetWidth();
		int nCy = label.GetHeight();
		int x = nCx / 2; //センターライン
		BYTE *pFind = pBits + x; //検索開始アドレス
		for(int y=0;y<nCy;y++,pFind+=nCx){
			if( *pFind==MAXBYTE ){ //In Pointが見つかった			
				BOOL isFindOK = FALSE;
				for(;y<nCy;y++,pFind+=nCx){//Out Point へ
					if(!*pFind){ //Out Pointが見つかった
						isFindOK = TRUE;
						nRet++; //線の数インクリメント
						break;
					}
				}
				if(!isFindOK){//Out Pointが見つからなかった
					nRet++; //端まで到達したために線の数インクリメント
				}
			}
		}
		if( nRet!=nCheck){
			GetOutputView()->Output(false,L"%c: CenterLine %d本",tch,nRet);
			return FALSE;
		}
	}
	return TRUE; 
}

//文字の線幅チェック
bool CharRecog::CheckSize(TCHAR tch,SizeDbl &size,double dbWmin,double dbLen)
{
	if(dbWmin <= 0 ){
		return false;
	}
	bool isOK = ((size.cx-0.0001) <= dbWmin); //文字の線幅は基準値以上ですか？(0.0001の違いまで許可）

	CString strLen;
	strLen.Format(L"文字認識:%3.1f",dbLen);
	while( strLen.GetLength() < 10 ){
		strLen += ' '; //半角空白追加
	}

	CString strOut; //出力メッセージ
	if(dbWmin >= 0 ){
		strOut.Format(L"%c: %s 幅:%.2f",tch,strLen,dbWmin);
	}else{
		strOut.Format(L"%c: %s",tch,strLen);
	}
	GetOutputView()->Output(isOK,strOut.GetBuffer());
	return isOK;
}

//指定配列中の最小幅取得
int CharRecog::GetWminIndex(AryLine *pAryLine,double &dbWmin)
{
	dbWmin = 0; //初期値 
	int nIndex = -1;
	double dbMin = INT_MAX; //線幅
	for(int i=0,imax=pAryLine->GetSize();i<imax;i++){ //先頭と末尾をエッジとして外す
		double dbW = pAryLine->GetAt(i).GetLength();
		//TRACE("dbW %.1f\n",dbW);
		if( dbW < dbMin){//より小さい値が見つかった
			nIndex = i;
			dbMin = dbW; 
		}
	}
	if( nIndex >= 0 ){
		dbWmin = dbMin; //最小幅Set
	}
	return nIndex; //Index番号取得
}

//削除線幅取得
double CharRecog::GetDeleteWidth( const CRect &rect,double dbLen) 
{
	int nWidth = rect.Width();
	int nHeight= rect.Height();
	return min3(nWidth,nHeight,dbLen); 
}

//Pointの表示
void CharRecog::ShowPoint(LPWSTR szName,CPoint &pt,Color color)
{
	CString strName;
	strName.Format(L"%s%d%d",szName,pt.x,pt.y);
	
	CRect rect(pt.x,pt.y,pt.x+1,pt.y+1);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,color,true); //矩形描画
}

//文字欠け検査
CheckNickData CharRecog::CheckNick(int nIndex,Label &label,CharElement &celem,CRect &rect,ArrayPoint& aryPoint,int nRnum,double dbLen,int nTh)
{
	CheckNickData data;
	data.m_tChar = celem.m_tChar;

	CPoint pt = rect.TopLeft();//相対座標
	TCHAR tch = celem.m_tChar;
	SizeDbl size = GetLineWidthAll(nIndex,tch,nRnum); //総合的な判断による文字幅取得

	m_ckResult.Clear(); //検査結果クリア
	m_ckResult.SetTL(pt); //左上設定

	label.FillDstBuf(0,MAXBYTE); //ラベリング結果に255
	//label.ContractDst(); //縮小 (細い線を消去）	
	//label.ExpandDst();   //膨張 	

	COutputView *pOutput = GetOutputView();	
	double dbWmin = -1;//幅の最小値
	double dbDelete = GetDeleteWidth( rect,size.cx ); //削除線幅
	if( size.cx > 0 ){ //幅指定はありますか？
		switch(tch){
			case '-': //LR
				dbWmin = TopToBtm(aryPoint,3,INT_MAX); //上から下
				break;
			case 'i': 
			case 'I':
			case 'l': //LR
				dbWmin = LeftToRight(aryPoint,3,INT_MAX); //左から右方向
				break;
			case '/': //LR + TB	Delete 線幅
				dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //上から下
				dbWmin = LeftToRight(aryPoint,2,dbWmin ); //左から右方向
				break;
				break;
			case 'r'://LR+TB
			case '1':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右方向
				dbWmin = TopToBtm   (aryPoint,       2,dbWmin ); //上から下
				break;
			case 'X': //LR + RL
			case 'x':		
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //右から左
				break;
			case 'L': //LR + BT
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin ); //下から上
				break;
			case '7'://RL + TB
				dbWmin = RightToLeft(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = TopToBtm   (aryPoint,dbDelete,dbWmin ); //上から上
				break;
			case 'C': //LR + TB + BT(線幅削除）
			case 'c':
				{
					dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //上から下
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin); //下から上
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); //左から右方向
				}
				break;
			case 'f': //LR + TB + BT
			case 'F':
				{
					dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //上から下
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin ); //下から上
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); //左から右方向
				}
				break;
			case 'W':// HCenter
			case 'M':
			case 'm':
			case 'w':
				{
					dbWmin = LeftToRight  (aryPoint,dbDelete,INT_MAX); //左から右方向
					dbWmin = RightToLeft  (aryPoint,dbDelete,dbWmin); //左から右方向
					dbWmin = CenterToLeft (aryPoint,dbDelete,dbWmin); //中央から左					
					dbWmin = CenterToRight(aryPoint,dbDelete,dbWmin); //中央から右
				}
				break;
			
			case 'E':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //縦方向センターライン３本					
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterY = rect.CenterPoint().y;
					//上下半分に分割
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//下半分
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					
					//上半分
					dbWmin = LeftToRight(aryPointTop,dbDelete,INT_MAX); //左から右方向
					dbWmin = TopToBtm(aryPointTop,   dbDelete,dbWmin); //上から下

					//下半分
					dbWmin = LeftToRight(aryPointBtm,dbDelete,dbWmin); //左から右方向
					dbWmin = BtmToTop   (aryPointBtm,dbDelete,dbWmin); //下から上
					
					//中央のみ
					ArrayPoint aryPointC;//中央
					aryPointC.Copy(aryPoint);
					aryPointC.RangeY(nCenterY-size.cy*1.5,nCenterY+size.cy*1.5);
					dbWmin = TopToBtm(aryPointC,2,dbWmin); //上から下
				}
				break;
			case '3':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //縦方向センターライン３本					
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterY = rect.CenterPoint().y;
					//上下半分に分割
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//下半分
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					
					//上半分
					dbWmin = RightToLeft(aryPointTop,dbDelete,INT_MAX); //左から右方向
					dbWmin = TopToBtm   (aryPointTop,       2,dbWmin); //上から下

					//下半分
					dbWmin = RightToLeft(aryPointBtm,dbDelete,dbWmin); //左から右方向
					dbWmin = BtmToTop   (aryPointBtm,       2,dbWmin); //下から上

					//中央のみ
					ArrayPoint aryPointC;//中央
					aryPointC.Copy(aryPoint);
					aryPointC.RangeY(nCenterY-size.cy*1.5,nCenterY+size.cy*1.5);
					dbWmin = TopToBtm(aryPointC,2,dbWmin); //上から下
				}
				break;
			case 'a'://IO + RL
			case 'd':
			case 'q':
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = RightToLeft(aryPoint,dbDelete,sizeIO.cx); //左から右方向
				}
				break;
			case 'P':
			case 'p'://IO + LR
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //左から右方向
				}
				break;
						
			case 'Q': //IO + BT
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = BtmToTop(aryPoint,dbDelete,sizeIO.cy); //下から上
				}
				break;
			case 'A': //IO + LR + RL
			case 'R':
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //左から右方向
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin   ); //左から右方向
				}
				break;			
			case 'e': //IO + LR + BT
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //左から右方向
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin); //下から上
				}
				break;
			case '4': //IO + RL + BT
				if(CenterLineVCheck(nRnum,tch,label,2)){ //縦方向センターライン2本
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = RightToLeft(aryPoint,dbDelete,sizeIO.cx); //左から右方向
					dbWmin = TopToBtm   (aryPoint,       2,dbWmin); //上から下
				}
				break;
			case '6': //IO + LR + TB
				{
					//内側と外側
					ArrayPoint aryPointOut;	//外側境界線
					ArrayPoint aryPointIn;	//内側境界線
					LengthPoint lenPt;//内側と外側の境界線間の距離を求める
					lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //外側境界線と内側境界線を作成する
					SizeDbl sizeIO;
					CPoint ptS,ptE; //最小距離Point
					dbWmin = aryPointIn.GetLength(aryPointOut,ptS,ptE); //In Out間の最小距離を取得
					m_ckResult.SetLine(ptS,ptE);

					//左側検査
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); 

					//上側検査
					//内側境界線部分を削除
					int nRangeY = aryPointIn.GetTop().y; //内側境界線上限
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nRangeY);
					dbWmin = TopToBtm(aryPointTop,dbDelete,dbWmin); //上から下
				}
				break;
			case '9'://IO + RL + BT
				if(CenterLineVCheck(nRnum,tch,label,3)){ //縦方向センターライン3本
					//内側と外側
					ArrayPoint aryPointOut;	//外側境界線
					ArrayPoint aryPointIn;	//内側境界線
					LengthPoint lenPt;//内側と外側の境界線間の距離を求める
					lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //外側境界線と内側境界線を作成する

					CPoint ptS,ptE; //x方向最小距離Point
					dbWmin = aryPointIn.GetLength(aryPointOut,ptS,ptE); //In Out間のx方向の最小値を取得
					m_ckResult.SetLine(ptS,ptE);

					//右側検査
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); 

					//上側検査
					//内側境界線部分を削除
					int nBtm = aryPointOut.GetBottom().y; //外側境界線下限
					int nTop = aryPointIn.GetBottom().y; //内側境界線下限
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(nTop,nBtm);
					dbWmin = BtmToTop(aryPointTop,dbWmin,dbWmin); //下から上
				}
				break;
			case '0': //境界線間検査
			case 'D': //IO
			case 'O':
			case 'o':
				if( CenterLineVCheck(nRnum,tch,label,2) ){ //縦方向センターライン2本
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //線幅検査
					dbWmin = sizeIO.cx;
				}
				break;
			case 'B': //IO 2回
			case '8':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //縦方向センターライン3本
					SizeDbl sizeIO = CheckIn2Out1(tch,pt,aryPoint);
					dbWmin = sizeIO.cx;
				}
				break;
			case '2': //TB + BT
			case 'Z': //上半分 RL
			case 'z': //下半分 LR
				{
					//上下半分に分割
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//下半分
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					dbWmin = LeftToRight(aryPointBtm,3,INT_MAX); //下半分左から右
					dbWmin = BtmToTop(aryPointBtm,   3,dbWmin);  //下半分下から上
					dbWmin = RightToLeft(aryPointTop,3,dbWmin);	 //上半分右から左
					dbWmin = TopToBtm(aryPointTop,dbDelete,dbWmin);//上半分上から下
				}
				break;
			case '5':
				{										
					//上下半分に分割
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointTop;//上半分
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//下半分
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					dbWmin = LeftToRight(aryPointTop,dbDelete,INT_MAX);	//上半分左から右
					dbWmin = TopToBtm(aryPointTop,3,dbWmin );			//上半分上から下
					dbWmin = RightToLeft(aryPointBtm,dbDelete,dbWmin);	//下半分右から左
					dbWmin = BtmToTop(aryPointBtm,3,dbWmin);			//下半分下から上
				}
				break;
			case 'S':
			case 's':
				{
					//3分割
					CRect rect = aryPoint.GetRect();//周囲長領域
					int n3_1 = rect.Height() / 3; //3分の1の高さ

					ArrayPoint aryPoint1_3;//3分の1上
					ArrayPoint aryPoint2_3;//中心
					ArrayPoint aryPoint3_3;//3分の1下
					aryPoint1_3.Copy(aryPoint);
					aryPoint2_3.Copy(aryPoint);
					aryPoint3_3.Copy(aryPoint);
					aryPoint1_3.RangeY(rect.top,rect.top+n3_1);
					aryPoint2_3.RangeY(rect.top+n3_1,rect.top+n3_1*2);
					aryPoint3_3.RangeY(rect.top+n3_1*2,rect.bottom);

					dbWmin = TopToBtm(aryPoint1_3,dbDelete,INT_MAX);	//上から下
					dbWmin = LeftToRight(aryPoint2_3,dbDelete,dbWmin);	//左から右
					dbWmin = BtmToTop(aryPoint3_3,dbDelete,dbWmin);		//下から上

					//Sの左右つながっている部分横
					dbWmin = LeftToRight(aryPoint1_3,dbDelete,dbWmin);	//左から右
					dbWmin = RightToLeft(aryPoint3_3,dbDelete,dbWmin);	//右から左

					//Sの左右かけている部分横
					CPoint ptRight  = aryPoint1_3.GetRight();	//最も右のポイント
					CPoint ptBtm1_3 = aryPoint1_3.GetBottom();
					int nDelete = ptBtm1_3.y - ptRight.y;
					if( nDelete > 0 ){
						dbWmin = RightToLeft(aryPoint1_3,nDelete,dbWmin);	//右から左						
					}
					CPoint ptLeft  = aryPoint3_3.GetLeft();	//最も右のポイント
					CPoint ptBtm3_3 = aryPoint3_3.GetBottom();					
					nDelete = ptBtm3_3.y - ptLeft.y;
					if( nDelete > 0 ){
						dbWmin = LeftToRight(aryPoint3_3,nDelete,dbWmin);	//左から右						
					}
				}
				break;
			case 'N': 
				{
					dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //右から左

					//左右半分に分割
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterX = rect.CenterPoint().x;
					ArrayPoint aryPointLeft;//左半分
					aryPointLeft.Copy(aryPoint);
					aryPointLeft.RangeX(0,nCenterX+size.cx);
					ArrayPoint aryPointRight;//右半分
					aryPointRight.Copy(aryPoint);
					aryPointRight.RangeX(nCenterX-size.cx,rect.right);
					dbWmin = TopToBtm(aryPointLeft ,dbDelete,INT_MAX); //左半分上から下
					dbWmin = BtmToTop(aryPointRight,dbDelete,dbWmin ); //右半分下から上
				}
				break;
			case 'h': //LR + RL + TB
			case 'n':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //右から左
				dbWmin = TopToBtm(aryPoint,dbDelete,dbWmin); //上から下
				break;
			case 'J':
			case 'j':
				{
					dbWmin = TopToBtm(aryPoint,dbDelete,dbWmin);			//上半分上から下
					dbWmin = BtmToTop(aryPoint,dbDelete,dbWmin);			//下半分下から上

					//左右半分に分割
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterX = rect.CenterPoint().x;
					
					ArrayPoint aryPointLeft;//左半分
					aryPointLeft.Copy(aryPoint);
					aryPointLeft.RangeX(0,nCenterX);
					ArrayPoint aryPointRight;//右半分
					aryPointRight.Copy(aryPoint);
					aryPointRight.RangeX(nCenterX,rect.right);
					dbWmin = LeftToRight(aryPointLeft ,dbDelete,INT_MAX);	//上半分左から右
					dbWmin = RightToLeft(aryPointRight,dbDelete,dbWmin);	//下半分右から左
				}
				break;
			case 'U': //LR + RL + BT
			case 'u':
			case 'V':
			case 'v':
			case 'y':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //右から左
				dbWmin = BtmToTop(aryPoint,dbDelete,dbWmin); //下から上
				break;
			case 'T':
				{
					dbWmin = TopToBtm(aryPoint,dbDelete,INT_MAX); //上から下

					//下半分取得
					CRect rect = aryPoint.GetRect();//周囲長領域
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointBtm;//下半分
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					CPoint ptLeft = aryPointBtm.GetLeft(); //下半分の中での左端取得

					ArrayPoint aryPointCpy;//境界線のコピー作成
					aryPointCpy.Copy(aryPoint);

					//上下を狭める
					int nTop = aryPointCpy.GetTop().y;	  //最も上の点を取得
					int nBtm = aryPointCpy.GetBottom().y; //最も下の点を取得
					aryPoint.RangeY(nTop+size.cy,nBtm-size.cy); //指定範囲のみのPointに変更 線幅の半分を検査不可能領域とする

					ArrayPoint aryConnect;
					ArrayPoint aryNotConnect;
					aryPoint.CollectConnectPoint( ptLeft ,aryConnect); //左端座標との隣接Pointのみ
					aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //隣接Point以外を収集
					aryNotConnect.SelectConnectPoint( (int)size.cy ); //指定数接続Pointのみを選択

					CRect rcLeft = aryConnect.GetRect(); //隣接領域					
					//隣接領域の右側だけにする
					aryNotConnect.RangeX( rcLeft.left,rect.right);
					CPoint ptS,ptE;
					double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
					if( dbLen < dbWmin){ //検査結果
						m_ckResult.SetLine(ptS,ptE);
						dbWmin = dbLen;
					}
				}
				break;
			//Default 文字欠けチェック
			case 'K':
			case 'k':
			case 'G':
			case 'H':
			case 'Y':
			case 't': 
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //左から右
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //右から左
				dbWmin = TopToBtm(aryPoint   ,dbDelete,dbWmin); //上から下
				dbWmin = BtmToTop(aryPoint   ,dbDelete,dbWmin); //下から上
				break;
			case '★':
			case L'★':
				{
					data.m_isOK = true;	
					return data;
				}
				break;
		}
	}
	//m_dibGrayとm_ckResultの計算追加
	CPoint &ptS = m_ckResult.m_line.m_ptS;
	CPoint &ptE = m_ckResult.m_line.m_ptE;	
	if(dbWmin > 0){
		//CString strFilename;
		//strFilename.Format(L"c:\\tmp\\%d_%d_%d.bmp",nRnum,ptS.x,ptS.y);
		//m_dibGray.SaveFile(strFilename);

		PointDbl vec; ;//方向ベクトル
		vec.x = (ptS.x-ptE.x)/dbWmin;
		vec.y = (ptS.y-ptE.y)/dbWmin;
		BYTE byDataS  = MAXBYTE - m_dibGray.GetValue(ptS); //指定座標の値取得
		double dbDataS= MAXBYTE - m_dibGray.GetAprxVal(ptS.x+vec.x,ptS.y+vec.y);
		if( byDataS > dbDataS && byDataS > nTh && nTh > dbDataS ){ //しきい値による線幅拡張
			double dbS1 = byDataS-nTh; //しきい値余分
			double dbS2 = nTh-dbDataS; //外部余分
			dbWmin += dbS1/(dbS1+dbS2);			
		}
		BYTE byDataE  = MAXBYTE - m_dibGray.GetValue(ptE); //指定座標の値取得
		double dbDataE= MAXBYTE - m_dibGray.GetAprxVal(ptE.x-vec.x,ptE.y-vec.y);		
		if( byDataE > dbDataE && byDataE > nTh && nTh > dbDataE ){ //しきい値による線幅拡張
			double dbE1 = byDataE-nTh;//しきい値余分
			double dbE2 = nTh-dbDataE;//外部余分
			dbWmin += dbE1/(dbE1+dbE2);			
		}
	}

	data.m_isOK = CheckSize(tch,size,dbWmin,dbLen);		
	if( data.m_isOK ){ //線幅が正常値の時
		SetMinWidth(tch,nRnum,dbWmin); //最小文字幅結果登録
	}else{	//線幅異常値の時
		m_ckResult.ShowMinLine(size); //最小ラインの表示	
	}
	return data;
}

#endif