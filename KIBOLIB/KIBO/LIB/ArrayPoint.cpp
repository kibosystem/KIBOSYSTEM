#include "../stdafx.h"
#include "ArrayPoint.h"
#include "typedef.h"
#include "functions.h"
#include "GlobalMgr.h"
#include "../Script/App.h"

ArrayPoint& ArrayPoint::operator=(const ArrayPoint &t)
{
	this->Copy(t);
	return *this;
}

int ArrayPoint::Find(CPoint pt)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == pt ){
			return i;
		}
	}	
	return -1;
}
INT_PTR ArrayPoint::AddPoint(int x,int y)
{
	CPoint pt(x,y);
	return this->Add(pt);
}

//最も左の点を取得
CPoint ArrayPoint::GetLeft()	
{
	CPoint ptRet(0,0); //Default戻り値
	int imax=(int)GetSize(); //サイズチェック
	if( imax > 0 ){
		ptRet.x = INT_MAX;//最小値
		for(int i=0;i<imax;i++){ 
			CPoint &pt = GetAt(i);
			if( pt.x < ptRet.x && pt.x >= 0){ //より小さい座標を求める
				ptRet = pt;
			}
		}
	}
	return ptRet;
}
//最も右の点を取得
CPoint ArrayPoint::GetRight()
{
	CPoint ptRet(0,0); //Default戻り値
	int imax=(int)GetSize(); //サイズチェック
	if( imax > 0 ){
		for(int i=0;i<imax;i++){ 
			CPoint &pt = GetAt(i);
			if( ptRet.x <= pt.x ){
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//最も上の点を取得
CPoint ArrayPoint::GetTop()	
{
	CPoint ptRet(0,0); //Default戻り値
	int imax=(int)GetSize(); //サイズチェック
	if( imax > 0 ){
		ptRet.y = INT_MAX;
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( pt.y < ptRet.y && pt.y >= 0){ //より小さい値で置き換える
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//最も下の点を取得	
CPoint ArrayPoint::GetBottom() 
{
	CPoint ptRet(0,0); //Default戻り値
	int imax=(int)GetSize(); //サイズチェック
	if( imax > 0 ){
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( ptRet.y <= pt.y ){
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//点配列から矩形を取得
CRect ArrayPoint::GetRect()
{
	CRect rcRet(INT_MAX,INT_MAX,INT_MIN,INT_MIN);
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if( 0 <= pt.x && pt.x < rcRet.left ){
			rcRet.left = pt.x;
		}
		if(rcRet.right < pt.x){
			rcRet.right = pt.x;
		}
		if(0 <= pt.y && pt.y < rcRet.top){
			rcRet.top = pt.y;
		}
		if(rcRet.bottom < pt.y){
			rcRet.bottom = pt.y;
		}
	}
	return rcRet;
}
//指定座標との最短距離と最短距離となるPointを取得
CPoint ArrayPoint::GetLengthPoint(CPoint& pt,double &dbLmin)
{
	CPoint ptRet(0,0);
	dbLmin = DBL_MAX; //最小距離
	int nImin = -1;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		double dbLen = ::GetLength(pt,GetAt(i))+1; //線の両端を含む
		if( dbLen < dbLmin ){
			dbLmin = dbLen;
			nImin = i;
		}
	}
	if( nImin >= 0 ){
		ptRet = GetAt(nImin);
	}
	return ptRet;
}
//指定座標との最短距離を求める
double ArrayPoint::GetLength(CPoint& pt) 
{
	double dbLen;
	GetLengthPoint(pt,dbLen);
	return dbLen;
}

//最短距離取得
double ArrayPoint::GetLength(ArrayPoint &ptAry,CPoint &ptSmin,CPoint &ptEmin)
{
	double dbLmin = DBL_MAX;
	double dbLen;
	CPoint ptS,ptE;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		ptS = GetAt(i);
		ptE = ptAry.GetLengthPoint(ptS,dbLen);
		if( dbLen < dbLmin ){//最小距離
			ptSmin = ptS; //位置記録
			ptEmin = ptE;
			dbLmin = dbLen; //距離記録
		}
	}
	return dbLmin; //最短距離
}

//指定x座標検索
CPoint ArrayPoint::FindX(int x) 
{	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == x){
			return pt;
		}
	}
	return CPoint(-1,-1);
}
CPoint ArrayPoint::FindX(int nIndex,int x) 
{	
	for(int i=nIndex,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == x){
			return pt;
		}
	}
	return CPoint(-1,-1);
}

//指定y座標検索
CPoint ArrayPoint::FindY(int y) 
{
	CPoint ptRet(-1,-1);
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CPoint &pt = GetAt(i);
		if(pt.y == y){
			return pt;
		}
	}
	return CPoint(-1,-1);
}
//指定y座標検索
CPoint ArrayPoint::FindY(int nIndex,int y) 
{
	CPoint ptRet(-1,-1);
	for(int i=nIndex,imax=(int)GetSize();i<imax;i++){
		CPoint &pt = GetAt(i);
		if(pt.y == y){
			return pt;
		}
	}
	return CPoint(-1,-1);
}

//指定x座標の点取得
int ArrayPoint::CollectPointX(int nX,ArrayPoint &ptCollect)
{
	int nCnt = 0;	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == nX){
			ptCollect.Add(pt);
			nCnt++;
		}
	}
	return nCnt;
}
//指定y座標の点取得
int ArrayPoint::CollectPointY(int nY,ArrayPoint &ptCollect)
{
	int nCnt = 0;	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.y == nY){
			ptCollect.Add(pt);
			nCnt++;
		}
	}
	return nCnt;
}

//指定PointのX座標を指定値に設定
void ArrayPoint::SetX(int x,ArrayPoint &ptCollect)
{
	for(int i=0;i<ptCollect.GetSize();i++){ //収集した点を削除
		int nIndex = Find( ptCollect[i] );
		if( nIndex >= 0 ){
			GetAt(nIndex).x = -1; //終点に切り替え
		}
	}
}

//x座標でソート
void ArrayPoint::SortX() 
{
	vector<CPoint> vPoint;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		vPoint.push_back( GetAt(i) );
	}
	::sort(vPoint.begin(),vPoint.end(),SortPointX());
	
	RemoveAll();//clear
	for(int i=0;i<imax;i++){//並び替え
		Add( vPoint[i] );
	}
}

//y座標でソート
void ArrayPoint::SortY() 
{
	vector<CPoint> vPoint;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		vPoint.push_back( GetAt(i) );
	}
	::sort(vPoint.begin(),vPoint.end(),SortPointY());
	
	RemoveAll();//clear
	for(int i=0;i<imax;i++){//並び替え
		Add( vPoint[i] );
	}
}

//x座標によって2つに分ける
void ArrayPoint::SplitX(int nX,ArrayPoint &aryL,ArrayPoint &aryR)
{	
	for(int i=0;i<GetSize();i++){
		CPoint &pt = GetAt(i);
		if( nX < pt.x ){ //右側作成
			aryR.Add(pt);
		}else{ //左側作成
			aryL.Add(pt);
		}
	}		
}

//頂点座標との隣接Pointのみ収集
void ArrayPoint::CollectConnectPoint(CPoint &pt,ArrayPoint &aryCollect) 
{
	aryCollect.RemoveAll();
	int imax = (int)GetSize();
	aryCollect.Add(pt);
	for(int i=0;i<imax;i++){ //順方向に接続検査
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 && aryCollect.IsConnect(ptCheck) ){ //接続確認
			aryCollect.Add(ptCheck);
		}
	}
	for(int i=imax-1;i>=0;i--){ //逆方向に接続検査
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 && aryCollect.IsConnect(ptCheck) ){ //接続確認
			aryCollect.Add(ptCheck);
		}
	}
}

//隣接Point以外を収集
void ArrayPoint::CollectExceptPoint(ArrayPoint &aryCollect,ArrayPoint &aryExcept)
{
	aryExcept.RemoveAll();
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){ //順方向に接続検査
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 ){ //aryCollect以外の点のみ追加
			aryExcept.Add(ptCheck);
		}
	}	
}

//指定数接続Pointのみを選択
void ArrayPoint::SelectConnectPoint(int nCnum)
{
	int imax = (int)GetSize();
	if( imax > 0 ){
		ArrayPoint aryConnect; //接続ポイント配列
		ArrayPoint aryTmp;
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( aryTmp.GetSize() == 0 || aryTmp.IsConnect(pt) ){	//接続確認
				aryTmp.Add(pt); 
			}else{ //接続が途切れた時
				if( aryTmp.GetSize() >= nCnum ){ //十分な数の接続要素があるときだけ
					aryConnect.Append( aryTmp ); //新配列に追加
				}
				aryTmp.RemoveAll();
			}
		}
		//2周目
		CPoint ptStart = GetAt(0);
		if( aryTmp.IsConnect(ptStart) ){
			for(int i=0;i<imax;i++){
				CPoint &pt = GetAt(i);
				if( aryTmp.GetSize() == 0 || (aryTmp.Find(pt)<0 && aryTmp.IsConnect(pt)) ){	//接続確認
					aryTmp.Add(pt); 
				}else{ //接続が途切れた時
					break;
				}
			}			
		}
		if( aryTmp.GetSize() >= nCnum ){ //十分な数の接続要素があるときだけ
			aryConnect.Append( aryTmp ); //新配列に追加
		}
		this->RemoveAll();
		this->Copy( aryConnect ); //指定数接続配列と差し替える
	}
}
//接続Pointですか？
BOOL ArrayPoint::IsConnect(CPoint &pt) 
{
	for(int i=0;i<GetSize();i++){
		CPoint &pt1 = GetAt(i);
		if( abs(pt1.x-pt.x) <=1 && abs(pt1.y-pt.y)<=1 ){ 
			return TRUE;//接続ポイントが存在する
		}
	}
	return FALSE; //接続ポイントは存在しなかった
}

//指定x範囲内の座標だけを残す
void ArrayPoint::RangeX(double dbXmin,double dbXmax)
{
	int imax = (int)GetSize(); //末尾から検出
	for(int i=imax-1;i>=0;i--){
		int nX = GetAt(i).x;
		if( nX < dbXmin || dbXmax < nX ){ //Range外の座標をもと点は削除
			RemoveAt(i);
		}
	}
}
//指定y範囲内の座標だけを残す
void ArrayPoint::RangeY(double dbYmin,double dbYmax)
{
	int imax = (int)GetSize(); //末尾から検出
	for(int i=imax-1;i>=0;i--){
		int nY = GetAt(i).y;
		if( nY < dbYmin || dbYmax < nY ){ //Range外の座標をもと点は削除
			RemoveAt(i);
		}
	}
}

//線描画
void ArrayPoint::DrawPolyline(CDIB &dib,COLORREF color)
{
	//メモリデバイスコンテキストの作成
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	LOGPEN lp; //指定色のペン
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 3;
	lp.lopnColor = color; //色指定
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen取得
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBitmap *pBmpOld = dcMem.SelectObject(dib.GetBitmap());
	int imax = (int)GetSize(); //末尾から検出
	if( imax>0){
		CPoint pt = GetAt(0);
		dcMem.MoveTo(pt);
		for(int i=1;i<imax;i++){
			CPoint pt = GetAt(i);
			dcMem.LineTo(pt);			
		}
	}
	//後始末
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}
//点描画
void ArrayPoint::DrawPoints(int nNo)
{
	App *pApp = App::GetInstance();
	CString strNameT;
	int imax = (int)GetCount();
	for (int i = 0; i < imax; i++){
		CPoint &pt = GetAt(i);
		strNameT.Format(L"R%d_%d", nNo, i);
		pApp->DrawRect(strNameT, pt.x - 1, pt.y - 1, pt.x + 1, pt.y + 1, 255, 255, 255, 0, TRUE);
	}
}