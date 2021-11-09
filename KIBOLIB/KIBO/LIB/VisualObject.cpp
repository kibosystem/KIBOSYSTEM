#include "../stdafx.h"
#include "VisualObject.h"
#include "Object.h"
#include "../KIBOView.h"

//コンストラクタ
VisualObject::VisualObject(void)
{
	m_isVisible = true; //表示ON
	m_nX = 0;	//x座標
	m_nY = 0;	//y座標
	m_byType = TYPE_NONE; //指定なし
}

//デストラクタ
VisualObject::~VisualObject(void)
{
}

//コンストラクタ
AryVO::AryVO(void)
{
}
//デストラクタ
AryVO::~AryVO(void)
{
}
int AryVO::Find(VisualObject *pVO)
{
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		if( pVO == GetAt(i) ){ //見つかった
			return i;
		}
	}
	//見つからなかった
	return -1;
}

//コンストラクタ
VisualObjectMgr::VisualObjectMgr()
{
	m_isVisible = true; //Default表示
}
//デストラクタ
VisualObjectMgr::~VisualObjectMgr()
{
	//有効配列
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		//TRACE(L"Delete:%s\n",m_aryValid[i]->GetName());
		delete m_aryValid[i]; 
	}
	//無効配列
	for(int i=0,imax=(int)m_aryInvalid.GetSize();i<imax;i++){
		//TRACE(L"Delete:%s\n",m_aryInvalid[i]->GetName());
		delete m_aryInvalid[i]; 
	}

	m_aryValid.RemoveAll();	
	m_aryInvalid.RemoveAll();
}

//Objectの作成
VisualObject *VisualObjectMgr::CreateVisualObject(int nType,CString &strName)
{
	VisualObject *pVO = NULL;

	//すでに存在する場合
	for(int i=0;i<m_aryValid.GetSize();i++){
		VisualObject *pCheck = m_aryValid[i];
		if( pCheck->GetName() == strName ){ //発見
			m_aryValid.RemoveAt(i); //有効配列から外す
			pVO = pCheck;
			break;
		}
	}
	if( pVO == NULL ){
		//無効配列から取得	
		for(int i=0,imax=(int)m_aryInvalid.GetSize();i<imax;i++){
			VisualObject *pCheck = m_aryInvalid[i];
			if( pCheck->GetType() == nType ){ //見つかった
				m_aryInvalid.RemoveAt(i); //無効配列から外す
				pVO = pCheck; //戻り値
				break;
			}
		}
	}
	//新規に作成
	if( pVO == NULL ){
		switch(nType){
			case VisualObject::TYPE_BITMAP://Bitmap
				break;
			case VisualObject::TYPE_LINE://線
				{
					LineObject *pLO = new LineObject();
					pVO = (VisualObject *)pLO;
				}
				break;
			case VisualObject::TYPE_RECT://矩形
				{
					RectObject *pRO = new RectObject();
					pVO = (VisualObject *)pRO;
				}
				break;
			case VisualObject::TYPE_CIRCLE://円
				{
					CircleObject *pCO = new CircleObject();
					pVO = (VisualObject *)pCO;
				}
				break;
			case VisualObject::TYPE_STRING://文字列
				{
					StringObject *pSO = new StringObject();
					pVO = (VisualObject *)pSO;
				}
				break;
			case VisualObject::TYPE_POLYLINE://ポリゴン枠線
				{
					PolylineObject *pPO = new PolylineObject();
					pVO = (VisualObject *)pPO;
				}
				break;
		}
		//TRACE(L"Create:%s\n",strName);
	}
	if( pVO != NULL ){
		pVO->SetType(nType);
		pVO->SetName(strName);
		m_aryValid.Add(pVO); //有効配列へ登録
	}
	return pVO;
}

//指定VisualObjectを最前面に移動
void VisualObjectMgr::SetFront(VisualObject *pVO)
{
	//有効配列
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		if( m_aryValid[i] == pVO ){
			m_aryValid.RemoveAt(i); //有効配列から削除
			m_aryValid.Add(pVO); //最前面に追加
			break ;
		}
	}	
}

//指定VisualObjectを無効化
void VisualObjectMgr::Remove(VisualObject *pVO) 
{
	//有効配列
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		if( m_aryValid[i] == pVO ){
			m_aryValid.RemoveAt(i); //有効配列から
			m_aryInvalid.Add(pVO);	//無効配列へ移動
			break ;
		}
	}	
}
//全VisualObjectを無効化
void VisualObjectMgr::RemoveAll()
{
	//有効配列 -> 無効配列
	while(m_aryValid.GetSize() > 0){ //有効配列が存在する間
		VisualObject *pVO = m_aryValid[0]; //先頭要素を取得
		m_aryValid.RemoveAt(0); //有効配列から先頭要素を外す
		m_aryInvalid.Add(pVO);	//無効配列へ移動
	}
}
//指定名称を含むオブジェクトを無効化します
void VisualObjectMgr::RemoveByName(const CString &strName) 
{
	//有効配列
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( pVO->GetName().Find(strName) >= 0 ){ //名前を含んでいる場合
			m_aryValid.RemoveAt(i); //有効配列から
			m_aryInvalid.Add(pVO);	//無効配列へ移動
		}
	}	
}
//指定オブジェクト以外を削除
void VisualObjectMgr::RemoveExcept(const CString &strName) 
{
	//有効配列
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( pVO->GetName().Find(strName) < 0 ){ //名前を含んでない場合のみ
			m_aryValid.RemoveAt(i); //有効配列から
			m_aryInvalid.Add(pVO);	//無効配列へ移動
		}
	}
}

//指定Visual Object取得
VisualObject* VisualObjectMgr::GetVO(CString strName)
{
	//有効配列
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( strName == pVO->GetName() ){ //見つかった
			return pVO;
		}
	}
	//見つからなかった
	return NULL;
}

//Visual Objectの描画
void VisualObjectMgr::Draw(CDC *pDC)
{
	if( m_isVisible ){ //Visual Object全体の表示・非表示切り替え
		for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
			if( m_aryValid[i]->IsVisible() ){
				m_aryValid[i]->Draw(pDC); 
			}
		}
		//TRACE(L"imax:%d\n",m_aryValid.GetSize());
	}
}
//ライン追加
void VisualObjectMgr::CreateLineObject(CString strName,const CLine &line,Color color)
{
	//CKIBOView *pView = GetView();
	//double dbRateCx = pView->GetRateCx();
	//double dbRateCy = pView->GetRateCy();
	LineObject *pLO = (LineObject *)CreateVisualObject(VisualObject::TYPE_LINE,strName);
	pLO->SetPointS(line.m_ptS.x,line.m_ptS.y);
	pLO->SetPointE(line.m_ptE.x,line.m_ptE.y);
	pLO->SetColor(color);
	pLO->SetVisible(true);
}
//矩形追加
void VisualObjectMgr::CreateRectObject(CString strName,const CRect &rect,Color color,BOOL isPaint)
{
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CRect rc((int)(rect.left   * dbRateCx)
			,(int)(rect.top    * dbRateCy)
			,(int)(rect.right  * dbRateCx)
			,(int)(rect.bottom * dbRateCy));

	RectObject *pRO = (RectObject *)CreateVisualObject(VisualObject::TYPE_RECT,strName);
	pRO->SetRect(rc);
	pRO->SetColor(color);
	pRO->SetPaint(isPaint ? true : false);
	pRO->SetVisible(true);
}

//円追加
void VisualObjectMgr::CreateCircleObject(CString strName,const CRect &rect,Color color,BOOL isPaint)
{
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CRect rc((int)(rect.left   * dbRateCx)
			,(int)(rect.top    * dbRateCy)
			,(int)(rect.right  * dbRateCx)
			,(int)(rect.bottom * dbRateCy));
	CircleObject *pCO = (CircleObject *)CreateVisualObject(VisualObject::TYPE_CIRCLE,strName);
	pCO->SetRect(rc);
	pCO->SetColor(color);
	pCO->SetPaint(isPaint ? true : false);	
	pCO->SetVisible(true);
}

//ポリゴン枠線追加
void VisualObjectMgr::CreatePolylineObject(CString strName,ArrayPoint &ptAry,COLORREF color,bool isClosed /*=true*/)
{
	ArrayPoint ary;
	ary.Copy( ptAry );
	if( ary.GetSize() > 0 ){
		CKIBOView *pView = GetView();
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();

		PolylineObject *pPO = (PolylineObject *)CreateVisualObject(VisualObject::TYPE_POLYLINE,strName);
		for(int i=0,imax=(int)ary.GetSize();i<imax;i++){
			if(ptAry[i].x < 0){
				break;
			}
			ary[i].x = (int)(ary[i].x * dbRateCx);
			ary[i].y = (int)(ary[i].y * dbRateCy);
		}
		pPO->SetPoints(ary);
		pPO->SetColor(color);
		pPO->SetClosed(isClosed);
		pPO->SetVisible(true);
	}
}
//ポリゴン枠線追加
void VisualObjectMgr::CreatePolylineObject(CString strName,const LPPOINT lpPoints,int nCnt,COLORREF color,bool isClosed /*=true*/)
{
	if( nCnt > 0 ){
		CKIBOView *pView = GetView();
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();

		PolylineObject *pPO = (PolylineObject *)CreateVisualObject(VisualObject::TYPE_POLYLINE,strName);
		ArrayPoint ptAry;
		CPoint point;
		for(int i=0;i<nCnt;i++){
			point.x = (int)(lpPoints[i].x * dbRateCx + 0.5);
			point.y = (int)(lpPoints[i].y * dbRateCy + 0.5);
			ptAry.Add(point);
		}
		pPO->SetPoints(ptAry);
		pPO->SetColor(color);
		pPO->SetClosed(isClosed);
		pPO->SetVisible(true);
	}
}

//文字列追加
void VisualObjectMgr::CreateStringObject(CString strName,int nX,int nY,CString strText,COLORREF color,LOGFONT &lf)
{
	StringObject *pSO = (StringObject *)CreateVisualObject(VisualObject::TYPE_STRING,strName);
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();

	HFONT hFont = g_GlobalMgr.GetFontHandle(lf);
	nX = (int)(nX * dbRateCx + 0.5);
	nY = (int)(nY * dbRateCy + 0.5);
	pSO->SetFont(hFont);
	pSO->SetX(nX);
	pSO->SetY(nY);
	pSO->SetText(strText);
	pSO->SetColor(color);
	pSO->InitRect();
	pSO->SetVisible(true);
}