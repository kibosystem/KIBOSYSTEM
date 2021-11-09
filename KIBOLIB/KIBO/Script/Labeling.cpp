#include "../stdafx.h"
#include "../KIBO.h"
#include "../LIB/functions.h"
#include "../LIB/ArrayPoint.h"
#include "../KIBOView.h"
#include "../OutputWnd.h"
#include "Labeling.h"
#include "App.h"
#include "Bitmap.h"

static CommandTable s_table[] =
{
	{Labeling::FUNC_SETBITMAP		,1,L"SetBitmap"}		//BITMAPクラスで初期化
	,{Labeling::FUNC_GETBITMAPSRC	,0,L"GetBitmapSrc"}		//ラベリングソース画像取得
	,{Labeling::FUNC_GETBITMAPDST	,0,L"GetBitmapDst"}		//ラベリング結果画像取得
	,{Labeling::FUNC_EXEC			,-1,L"Exec"}			//ラベリング実行
	,{Labeling::FUNC_ELEMENTEXEC	, 3,L"ElementLabeling"}  //エレメント内部のラベリング
	,{Labeling::FUNC_SORT			, 2,L"Sort"}			//ラベリング結果ソート
	,{Labeling::FUNC_OUTPUTRESULT	, 0,L"OutputResult"}		//ラベリング結果の出力
	,{Labeling::FUNC_DRAWRECT		,-1,L"DrawElementRect"}	//ラベリング領域描画
	,{Labeling::FUNC_PAINTELEMENT	, 4,L"PaintElement"}	//ラベリング領域描画
	,{Labeling::FUNC_LOADELEMENT	, 1,L"LoadElement"}		//画像ファイル読み込み
	,{Labeling::FUNC_REMOVE_ALL_ELEM,0,L"RemoveAllElement"}	//ラベリング要素削除
	,{Labeling::FUNC_INVERT			,0,L"Invert"}			//白黒反転
	,{Labeling::FUNC_EXPAND			,0,L"Expand"}			//膨張
	,{Labeling::FUNC_GETBORDER		,1,L"GetBorder"}		//境界線抽出
	,{Labeling::FUNC_DRAWBORDER		,4,L"DrawBorder"}		//境界線描画
	,{Labeling::FUNC_SELECTBORDER	,3,L"SelectBorder"}		//境界線一部抽出
	,{Labeling::FUNC_SETWIDTHRANGE , 2,L"SetWidthRange" }	//要素の幅指定
	,{Labeling::FUNC_SETHEIGHTRANGE ,2,L"SetHeightRange" }	//要素の高さ指定
	,{Labeling::FUNC_COUNT			,0,L"Count"}			//ラベリング要素数取得
	,{Labeling::FUNC_MAXELEMENT		,0,L"MaxElement"}		//最大要素番号取得
	,{Labeling::FUNC_INELEMENT		,3,L"InElement"}		//要素内の点ですか？	
	,{Labeling::FUNC_GX				,1,L"Gx"}				//重心ｘ座標
	,{Labeling::FUNC_GY				,1,L"Gy"}				//重心ｙ座標
	,{Labeling::FUNC_MIN_X			,1,L"Xmin"}				//最小ｘ座標
	,{Labeling::FUNC_MIN_Y			,1,L"Ymin"}				//最小ｙ座標
	,{Labeling::FUNC_MAX_X			,1,L"Xmax"}				//最大ｘ座標
	,{Labeling::FUNC_MAX_Y			,1,L"Ymax"}				//最大ｙ座標
	,{Labeling::FUNC_MIN_L			,3,L"Lmin"}				//指定地点からの最短距離
	,{Labeling::FUNC_MAX_L			,3,L"Lmax"}				//指定地点からの最長距離
	,{Labeling::FUNC_WIDTH			,1,L"Width"}			//幅
	,{Labeling::FUNC_HEIGHT			,1,L"Height"}			//高さ
	,{Labeling::FUNC_PIXELS			,1,L"Pixels"}			//画素数
	,{Labeling::FUNC_BORDER			,1,L"Border"}			//周囲長
	,{Labeling::FUNC_R_AVR			,1,L"Ravr"}				//平均半径
	,{Labeling::FUNC_R_MAX			,1,L"Rmax"}				//最大半径
	,{Labeling::FUNC_CIRCLE			,1,L"Circle"}			//円形度
	,{Labeling::FUNC_MAX			,0,NULL}				//コマンド数
};

static Labeling *s_pInstance = NULL; //唯一のクラス実体
Labeling* Labeling::GetInstance()//実体取得
{
	return s_pInstance;
}

Labeling::Labeling(void)
{
	s_pInstance = this;
}

Labeling::~Labeling(void)
{
	m_isInitOK = false; //初期化しましたか？
}

//関数名チェック
STDMETHODIMP Labeling::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId)
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//実行
STDMETHODIMP Labeling::Invoke(DISPID dispIdMember	//Function ID 番号
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script文字列
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr)
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //引数チェック
		return S_FALSE;
	}			
	//関数の時
	switch(dispIdMember){
		case FUNC_SETBITMAP: //BITMAPクラスで初期化
			{
				int nTh = pDispParams->rgvarg[0].ulVal; //しきい値
				SetBitmap(nTh);
			}
			break;
		case FUNC_GETBITMAPSRC: //ラベリング画像取得
			{
				Bitmap24 *pBitmap = Bitmap24::GetInstance();
				m_label.FillBitmapSrc(pBitmap->GetDib()); //ラベリング結果画像をビットマップへ転送
			}
			break;
		case FUNC_GETBITMAPDST: //ラベリング結果ビットマップ取得
			{
				Bitmap24 *pBitmap = Bitmap24::GetInstance();
				m_label.FillBitmapDst(pBitmap->GetDib()); //ラベリング結果画像をビットマップへ転送
			}
			break;
		case FUNC_EXEC: //ラベリング実行
			{
				int nRet = 0;
				if( pDispParams->cArgs >= 6 ){						
					nRet = Exec(
						 pDispParams->rgvarg[5].ulVal //最小画素数
						,pDispParams->rgvarg[4].ulVal //最大画素数
						,GetValientInt(&pDispParams->rgvarg[3]) //左
						,GetValientInt(&pDispParams->rgvarg[2]) //上
						,GetValientInt(&pDispParams->rgvarg[1]) //右
						,GetValientInt(&pDispParams->rgvarg[0]) //下
					);
				}else if(pDispParams->cArgs >= 2 ){
					nRet = Exec(
						 pDispParams->rgvarg[1].ulVal	//最小画素数
						,pDispParams->rgvarg[0].ulVal	 //最大画素数
						);
				}
				ReturnInt(wFlags,pVarResult,nRet); //ラベリング要素数を戻す
			}
			break;
		case FUNC_ELEMENTEXEC://エレメント内部のラベリング
			{
				int nEnum = pDispParams->rgvarg[2].ulVal;//要素番号
				int nMin = pDispParams->rgvarg[1].ulVal;//最小
				int nMax = pDispParams->rgvarg[0].ulVal;//最大				
				int nRet = ElementExec(nEnum,nMin,nMax); //エレメント内部のラベリング
				ReturnInt(wFlags,pVarResult,nRet); //ラベリング要素数を戻す
			}
			break;
		case FUNC_SORT: //ラベリング結果ソート
			{
				int nSnum = GetValientInt(&pDispParams->rgvarg[1]);
				BOOL isAscending = pDispParams->rgvarg[0].boolVal;
				Sort( nSnum,isAscending );
			}
			break;
		case FUNC_OUTPUTRESULT:	//ラベリング結果の出力
			OutputResult();
			break;
		case FUNC_DRAWRECT: //ラベリング領域描画
			if( pDispParams->cArgs >= 6 ){
				DrawElementRect(
					 pDispParams->rgvarg[5].ulVal	//要素番号取得
					,pDispParams->rgvarg[4].ulVal	//R
					,pDispParams->rgvarg[3].ulVal	//G
					,pDispParams->rgvarg[2].ulVal	//B
					,pDispParams->rgvarg[1].ulVal	//Dx
					,pDispParams->rgvarg[0].ulVal	//Dy
					);
			}else{
				DrawElementRect(
					 pDispParams->rgvarg[3].ulVal	//要素番号取得
					,pDispParams->rgvarg[2].ulVal	//R
					,pDispParams->rgvarg[1].ulVal	//G
					,pDispParams->rgvarg[0].ulVal	//B
					);
			}
			break;
		case FUNC_PAINTELEMENT: //ラベリング領域描画	
			{
				PaintElement(
					 pDispParams->rgvarg[3].ulVal	//要素番号取得
					,pDispParams->rgvarg[2].ulVal	//R
					,pDispParams->rgvarg[1].ulVal	//G
					,pDispParams->rgvarg[0].ulVal	//B
					);				
			}
			break;
		case FUNC_LOADELEMENT:	//画像ファイル読み込み
			{
				CString strDir = pDispParams->rgvarg[0].bstrVal; //ディレクトリPath
				CString strExt = strDir + L"\\*.bmp";
				int nNum = m_label.LoadCheckImage(strExt); //比較用の画像を読み込み
				ReturnInt(wFlags,pVarResult,nNum);
			}
			break;
		case FUNC_REMOVE_ALL_ELEM: //ラベリング要素全消去
			{
				CString strName;
				strName.Format(_T("Element"));
				GetView()->m_mgrVO.RemoveByName(strName.GetBuffer());
			}
			break;
		case FUNC_INVERT: //白黒反転処理
			Invert();
			break;
		case FUNC_EXPAND: //膨張
			m_label.ExpandSrc(); //ラベリング膨張
			break;
		case FUNC_GETBORDER://境界線抽出
			{
				int nEnum = pDispParams->rgvarg[0].ulVal;	//要素番号
				ArrayPoint *pAryPoint = GetAryPoint(nEnum);
				int nBorder = m_label.FindBorder(nEnum, *pAryPoint);
				ReturnInt(wFlags,pVarResult,nBorder); //Borderポイント数を戻す
			}			
			break;
		case FUNC_DRAWBORDER:		//境界線描画
			{
				int nEnum = pDispParams->rgvarg[3].ulVal;	//要素番号
				int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
				ArrayPoint *pAryPoint = GetAryPoint(nEnum);
				if( pAryPoint != NULL ){
					CString strName;
					strName.Format(L"Border%d",nEnum);
					GetView()->m_mgrVO.CreatePolylineObject( strName,*pAryPoint,RGB(nR,nG,nB) );
				}
			}
			break;
		case FUNC_SELECTBORDER:		//境界線一部抽出
			{
				int nEnum = pDispParams->rgvarg[2].ulVal;	//要素番号
				CString strType = pDispParams->rgvarg[1].bstrVal; //タイプ名称				
				int nW = pDispParams->rgvarg[0].ulVal;	//幅
				SelectBorder(nEnum,strType,nW); //TypeBorder抽出
			}
			break;
		case FUNC_SETWIDTHRANGE:	//要素の幅指定
			{
				int nWmin = pDispParams->rgvarg[1].ulVal;	//最小幅
				int nWmax = pDispParams->rgvarg[0].ulVal;	//最大幅
				SetWidthRange(nWmin,nWmax);
			}
			break;
		case FUNC_SETHEIGHTRANGE: //要素の高さ指定
		{
				int nHmin = pDispParams->rgvarg[1].ulVal;	//最小幅
				int nHmax = pDispParams->rgvarg[0].ulVal;	//最大幅
				SetHeightRange(nHmin, nHmax);
		}
			break;
		//プロパティ
		case FUNC_COUNT: //ラベリング要素数取得
			{
				int nCount = GetCount();
				ReturnInt(wFlags,pVarResult,nCount); //ラベリング要素数を戻す
			}
			break;
		case FUNC_MAXELEMENT: //最大要素番号取得
			{
				int nEnum = GetMaxElement(); //最大要素番号
				ReturnInt(wFlags,pVarResult,nEnum); //ラベリング要素数を戻す
			}
			break;
		case FUNC_INELEMENT:	//要素内の点ですか？	
			{
				int nEnum= GetValientInt(&pDispParams->rgvarg[2]); //要素番号
				int nX  = GetValientInt(&pDispParams->rgvarg[1]); //x座標
				int nY  = GetValientInt(&pDispParams->rgvarg[0]); //y座標
				BOOL isIn = FALSE;
				Element *pElement = m_label.GetElement(nEnum); //最大要素取得
				if( pElement != NULL ){
					isIn = pElement->IsContain(nX,nY); //要素内の点ですか？
				}
				ReturnBool(wFlags,pVarResult,isIn);
			}
			break;
		case FUNC_GX://重心ｘ座標
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				double dbGx = Gx(nEnum);
				ReturnDbl(wFlags,pVarResult,dbGx);
			}
			break;
		case FUNC_GY: //重心ｙ座標
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				double dbGy = Gy(nEnum);
				ReturnDbl(wFlags,pVarResult,dbGy);
			}
			break;
		case FUNC_MIN_L:	//指定地点からの最短距離
			{
				int nEnum = pDispParams->rgvarg[2].ulVal; //要素番号
				double dbX = pDispParams->rgvarg[1].dblVal; //x座標
				double dbY = pDispParams->rgvarg[0].dblVal; //y座標
				Element *pElement = m_label.GetElement(nEnum); //最大要素取得
				if(pElement != NULL){
					double dbMin = pElement->GetLMin(dbX,dbY); 
					ReturnDbl(wFlags,pVarResult,dbMin); //最小距離を戻す
				}
			}
			break;
		case FUNC_MAX_L:	//指定地点からの最長距離
			{
				int nEnum = pDispParams->rgvarg[2].ulVal; //要素番号
				double dbX = pDispParams->rgvarg[1].dblVal; //x座標
				double dbY = pDispParams->rgvarg[0].dblVal; //y座標
				Element *pElement = m_label.GetElement(nEnum); //最大要素取得
				if(pElement != NULL){
					double dbMax = pElement->GetLMax(dbX,dbY); 
					ReturnDbl(wFlags,pVarResult,dbMax); //最大距離を戻す
				}
			}
			break;
		case FUNC_PIXELS: //画素数
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				int nRet = Pixels(nEnum);				
				ReturnInt(wFlags,pVarResult,nRet); 
			}
			break;
		case FUNC_MIN_X: //最小ｘ座標
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				int nRet = Xmin(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;					 
		case FUNC_MIN_Y: //最小ｙ座標 
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				int nRet = Ymin(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_MAX_X: //最大ｘ座標
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				int nRet = Xmax(nEnum);				
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_MAX_Y: //最大ｙ座標
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				int nRet = Ymax(nEnum);
				ReturnInt(wFlags,pVarResult,nRet); 
			}						 
			break;
		case FUNC_WIDTH: //幅
		case FUNC_HEIGHT://高さ
		case FUNC_BORDER: //Border
		case FUNC_R_AVR://平均半径
		case FUNC_R_MAX: //最大半径取得
		case FUNC_CIRCLE://円形度
			{
				int nEnum = pDispParams->rgvarg[0].ulVal; //要素番号
				Element *pElement = m_label.GetElement(nEnum); //最大要素取得
				pVarResult->vt = VT_INT;
				if( pElement != NULL ){
					switch( dispIdMember){
						case FUNC_WIDTH: //幅
							pVarResult->intVal = pElement->GetWidth();
							break;
						case FUNC_HEIGHT://高さ
							pVarResult->intVal = pElement->GetHeight();
							break;
						case FUNC_BORDER: //周囲長
							{
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								pVarResult->intVal = GetBorderPixels(0,aryPoint); //周囲長のPixel数を取得
							}
							break;
						case FUNC_R_AVR://平均半径
							{
								int nPixels = pElement->GetPixels(); //面積
								double dbGx = pElement->GetGx(); //重心x
								double dbGy = pElement->GetGy(); //重心y
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								double dbLen = 0;
								int i=0;
								for(int imax=(int)aryPoint.GetSize();i<imax;i++){
									if(aryPoint[i].x < 0){
										break;
									}
									dbLen += ::GetLength(dbGx,dbGy,aryPoint[i].x,aryPoint[i].y); //半径の合計
								}
								if( i > 0 ){
									dbLen /= i;
								}
								ReturnDbl(wFlags,pVarResult,dbLen);	
							}
							break;
						case FUNC_R_MAX: //最大半径取得
							{
								int nPixels = pElement->GetPixels(); //面積
								double dbGx = pElement->GetGx(); //重心x
								double dbGy = pElement->GetGy(); //重心y
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								double dbMax = 0;
								int i=0;
								for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
									if(aryPoint[i].x < 0){
										break;
									}
									double dbLen = ::GetLength(dbGx,dbGy,aryPoint[i].x,aryPoint[i].y); //半径の合計
									if( dbLen > dbMax ){ //最大値に差替
										dbMax = dbLen;
									}
								}
								ReturnDbl(wFlags,pVarResult,dbMax);
							}
							break;
						case FUNC_CIRCLE://円形度
							{
								ArrayPoint aryPoint;
								m_label.FindBorder(nEnum, aryPoint);
								int nBorder = GetBorderPixels(0,aryPoint); //周囲長のPixel数を取得
								double dbCircle = pElement->GetRoundness(nBorder);
								ReturnDbl(wFlags,pVarResult,dbCircle);
							}
							break;

						default:
							pVarResult->intVal = -1;
							break;
					}
				}
			}
			break;
	}
	return S_OK;
}

//周囲長のPixel数を取得
int Labeling::GetBorderPixels(int nBnum,ArrayPoint &aryPoint)
{
	int nBno = 0; //ボーダー番号
	int nCnt = 0; //周囲長
	for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
		if( aryPoint[i].x < 0 ){
			if( nBno == nBnum ){
				return nCnt;
			}
			nCnt = 0;//reset
			nBno++; //次のボーダー番号検査へ			
		}
		nCnt++;
	}
	return -1; //Error
}

//Bitmapクラスで初期化
BOOL Labeling::Init(CDIB &dib,int nTh) 
{
	m_label.Init(dib.GetWidth(), dib.GetHeight());//ラベリングバッファ作成						
	if (!m_label.CreateSrcBuf(dib, nTh)){//ラベリングバッファ（2値化画像作成)
		::ErrorMsg(_T("画像の取得が出来ませんでした。カメラとの接続を確認してください。"));
		return FALSE;
	}
	return TRUE;
}

//白黒反転処理
void Labeling::Invert()
{
	Labeling::GetInstance()->m_label.InvertSrcBuf(); //ラベリングクラス取得
}

//ラベリング実行
int Labeling::Exec(int nMin,int nMax,int nLeft,int nTop,int nRight,int nBottom)
{
	CRect rect;
	rect.left  = nLeft; //左
	rect.top   = nTop; //上
	rect.right = nRight; //右
	rect.bottom= nBottom; //下
	return m_label.Exec(nMin,nMax,rect);//ラベリング実行
}
//ラベリング実行
int Labeling::Exec(int nMin,int nMax)
{	
	return m_label.Exec(nMin,nMax);//ラベリング実行
}			
//エレメント内部のラベリング
int Labeling::ElementExec(int nEnum,int nMin,int nMax) 
{
	return m_label.ElementExec(nEnum, nMin, nMax);
}
//ソート
void Labeling::Sort(int nSortNum,BOOL isAscending)
{
	m_label.Sort(nSortNum, isAscending); //ソートの実行
}

//ラベリング結果表示
void Labeling::OutputResult()
{
	OutputWnd *pOutput = GetOutputWnd();
	pOutput->Clear();

	CKIBOView* pView = GetView();
	CString str1; //表示文字列
	for (int i = 0, imax = m_label.GetEmax(); i<imax; i++){//全要素取得
		Element *pElem = m_label.GetElement(i);
		str1.Format(L"領域(%d,%d,%d,%d) 面積(%d) 重心1(%.2f,%.2f) 重心2(%.2f,%.2f)"
			,pElem->GetMinX(),pElem->GetMinY(),pElem->GetMaxX(),pElem->GetMaxY()
			,pElem->GetPixels()
			,pElem->GetGx(),pElem->GetGy()
			,pElem->GetGx()-pElem->GetMinX(),pElem->GetGy()-pElem->GetMinY()
			);
		pOutput->OutputText(str1);
	}
	pOutput->RedrawWindow();
}

//ラベリング要素数を戻す
int Labeling::GetCount()
{	
	return m_label.GetEmax(); //ラベリング要素数を戻す
}

//ラベリング領域描画
void Labeling::DrawElementRect(int nEnum,int nR,int nG,int nB,int nDx,int nDy)
{
	CString strName;
	strName.Format(_T("Element%d"),nEnum);
	Color color(nR,nG,nB);
	GetView()->SetElementRect(nEnum,strName,color,nDx,nDy);
}
void Labeling::DrawElementRect(int nEnum,int nR,int nG,int nB)
{
	CString strName;
	strName.Format(_T("Element%d"),nEnum);
	Color color(nR,nG,nB);
	GetView()->SetElementRect(nEnum,strName,color);	
}

//ラベリング要素塗りつぶし
void Labeling::PaintElement(int nEnum,int nR,int nG,int nB)
{
	GetView()->PaintElement(nEnum,nR,nG,nB);
}

//点配列取得
ArrayPoint* Labeling::GetAryPoint(CString &str)
{
	ArrayPoint *pRet = m_mapAryPointPtr[str];
	if( pRet == NULL ){ //無いときは作成
		pRet = new ArrayPoint;
		m_mapAryPointPtr[str] = pRet;
	}
	return pRet;
} 
ArrayPoint* Labeling::GetAryPoint(int n)
{
	CString str; //Key文字列作成
	str.Format(L"%d",n); 
	return GetAryPoint(str);
}

//Type別Border抽出
void Labeling::SelectBorder(int nEnum,CString strType,int nW)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){
		CString strName;
		strName.Format(L"%s%d",strType,nEnum);
		ArrayPoint *pAryCpy = GetAryPoint(strName); //一部のみの領域取得
		pAryCpy->Copy(*pAryPoint); //全体コピー
		if( strType == "LEFT" ){			
			CPoint pt = pAryPoint->GetLeft(); //左端取得
			pAryCpy->RangeX(pt.x,pt.x+nW); //左端のみ抽出
			pAryCpy->SortY();
		}else if(strType == "RIGHT"){
			CPoint pt = pAryPoint->GetRight(); //右端取得
			pAryCpy->RangeX(pt.x-nW,pt.x); //右端のみ抽出
			pAryCpy->SortY();
		}else if(strType == "TOP"){
			CPoint pt = pAryPoint->GetTop();
			pAryCpy->RangeY(pt.y,pt.y+nW); //上端のみ抽出
			pAryCpy->SortX();
		}else if(strType == "BOTTOM"){
			CPoint pt = pAryPoint->GetBottom(); //下端取得
			pAryCpy->RangeX(pt.y-nW,pt.y); //下端のみ抽出
			pAryCpy->SortX();
		}else if(strType == "ALL"){
			pAryCpy->RangeX(0,nW); //プラス領域のみ抽出
		}
	}
}

//要素幅指定
void Labeling::SetWidthRange(int nWmin, int nWmax)
{
	m_label.SetWmin(nWmin);
	m_label.SetWmax(nWmax);
}
//要素高さ指定
void Labeling::SetHeightRange(int nHmin, int nHmax)
{
	m_label.SetHmin(nHmin);
	m_label.SetHmax(nHmax);
}

//指定Y座標の左端点を収集
void Labeling::CollectPointXmin(int nEnum,ArrayPoint &aryPtXmin,int nYS,int nYE)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){
		int imax = (int)pAryPoint->GetSize();//全境界線数
		for(int y=nYS;y<=nYE;y++){//全y座標ごとのx座標を取得
			int nXmin = INT_MAX;
			for(int i=0;i<imax;i++){//全境界線検索
				if( pAryPoint->GetAt(i).y == y ){ //y座標発見
					int nX = pAryPoint->GetAt(i).x;
					if( nX < nXmin ){
						nXmin = nX;
					}
				}
			}
			if( nXmin < INT_MAX ){
				aryPtXmin.AddPoint(nXmin,y); //y座標登録			
			}
		}
	}
}

//指定Y座標の右端点を収集
void Labeling::CollectPointXmax(int nEnum,ArrayPoint &aryPtXmax,int nYS,int nYE)
{
	ArrayPoint *pAryPoint = GetAryPoint(nEnum);
	if( pAryPoint != NULL ){		
		int imax = (int)pAryPoint->GetSize();//全境界線数
		for(int y=nYS;y<=nYE;y++){//全y座標ごとのx座標を取得
			int nXmax = -1;
			for(int i=0;i<imax;i++){//全境界線検索
				if( pAryPoint->GetAt(i).y == y ){ //y座標発見
					int nX = pAryPoint->GetAt(i).x;
					if( nXmax < nX ){
						nXmax = nX;
					}
				}
			}
			if( nXmax > 0 ){
				aryPtXmax.AddPoint(nXmax,y); //y座標登録			
			}
		}
	}
}

//BITMAPクラスで初期化
BOOL Labeling::SetBitmap(int nTh)
{
	Bitmap24 *pBitmap = Bitmap24::GetInstance();
	if( pBitmap != NULL ){
		return Init(pBitmap->GetDib(),nTh);
	}
	return FALSE;
}

//重心x座標
double Labeling::Gx(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetGx(); 
	}
	return 0;	
}
//重心y座標
double Labeling::Gy(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetGy(); 
	}
	return 0;		
}
//画素数
int Labeling::Pixels(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetPixels();
	}
	return 0;			
}
//最小ｘ座標	
int Labeling::Xmin(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetMinX();
	}
	return 0;			
}
//最小ｙ座標
int Labeling::Ymin(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetMinY();
	}
	return 0;			
}
//最大ｘ座標
int Labeling::Xmax(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetMaxX();
	}
	return 0;			
}
//最大ｙ座標
int Labeling::Ymax(int nEnum)
{
	Element *pElement = m_label.GetElement(nEnum); //最大要素取得
	if( pElement != NULL ){
		return pElement->GetMaxY();
	}
	return 0;			
}
//最大要素番号取得
int Labeling::GetMaxElement()
{
	int nEnum = - 1; //最大要素番号
	m_label.GetMaxElement(&nEnum); //最大要素番号セット
	return nEnum;
}