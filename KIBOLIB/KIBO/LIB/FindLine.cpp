#include "../stdafx.h"
#include "FindLine.h"
#include "../LIB/color.h"

//FindLine::FindLine(void)
//{
//	ZeroMemory(m_sn,sizeof(m_sn));	//三角関数テーブル（サイン）	
//	ZeroMemory(m_cs,sizeof(m_cs));	//三角関数テーブル（コサイン）	
//}
//
//FindLine::~FindLine(void)
//{
//}
//
////初期化
//BOOL FindLine::Init(CString strPath)
//{
//	CDIB dib;
//	if( m_dib.Load(strPath) ){
//		m_YMAX = m_dib.GetHeight();
//		m_XMAX = m_dib.GetWidth();
//	
//		//三角関数テーブルを作成
//		double PIK = PI/THETA_MAX;
//		for(int i=0;i<THETA_MAX;i++){
//			m_sn[i] = (float)sin(PIK*i);
//			m_cs[i] = (float)cos(PIK*i);
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
////初期化
//BOOL FindLine::Init(CDIB &dib)
//{
//	dib.Copy( m_dib );
//	m_YMAX = m_dib.GetHeight();
//	m_XMAX = m_dib.GetWidth();
//	
//	//三角関数テーブルを作成
//	double PIK = PI/THETA_MAX;
//	for(int i=0;i<THETA_MAX;i++){
//		m_sn[i] = (float)sin(PIK*i);
//		m_cs[i] = (float)cos(PIK*i);
//	}
//	return TRUE;
//}
//
////カウンターデータ
//struct CountData
//{
//	int m_nCnt; //カウンター
//};
//
//
////直線検出
//BOOL FindLine::Find(BYTE byLine,int nPixels,int nCount,int nCheckLen)
//{
//	int end_flag=0;   //繰り返しを終了させるフラグ
//	int count=0;      //検出された直線または円の個数カウンタ
//	int x,y;
//	// ---------------------- Hough変換 --------------------------
//	//直線の場合 -------------------------------------------------	
//	static CountData counter[THETA_MAX][2*RHO_MAX];//直線検出用頻度カウンタ
//	ZeroMemory( counter,sizeof(counter) );
//	for(y=0;y<m_YMAX;y++){
//		for(x=0;x<m_XMAX;x++){
//			BYTE *pBits = m_dib.GetPBitsB(x,y);
//			if( *pBits == byLine && *(pBits+1) == byLine ){ //ライン色(RGB同一値）
//				for(int theta=0;theta<THETA_MAX;theta++){
//					int rho=(int)(x*m_cs[theta]+y*m_sn[theta]+0.5); //x,y -> θρ変換
//					CountData *pCData = &counter[theta][rho+RHO_MAX];
//					pCData->m_nCnt++; //カウンター
//				}
//				//色塗りします
//				*pBits = 255;
//			}
//		}
//	}
//
//	// --------------------- Hough逆変換 -------------------------
//	//直線の場合 -------------------------------------------------
//	int counter_max;
//	int theta_max = 0;
//	int rho_max = -RHO_MAX;
//	COutputView *pOutput = GetOutputView();
//	while(end_flag == 0 && count < nCount){ //長さがnPixelsピクセル以下か、直線が5本検出されたら終了
//		count++;
//		counter_max=0;         
//		//counterが最大になるtheta_maxとrho_maxを求める
//		for(int theta=0;theta<THETA_MAX;theta++){//全θループ
//			for(int rho=-RHO_MAX;rho<RHO_MAX;rho++){//全ρループ
//				CountData *pCData = &counter[theta][rho+RHO_MAX];
//				if(pCData->m_nCnt > counter_max){
//					counter_max = pCData->m_nCnt;//最大値の差し替え
//					//nPixelsピクセル以下の直線になれば検出を終了
//					if(counter_max<=nPixels){ end_flag=1;}
//					else{end_flag=0;}
//					theta_max = theta;
//					rho_max   = rho;
//				}
//			}
//		}
//		//rho_maxとtheta_maxの描画
//		double dbX = rho_max / m_cs[theta_max]; //y==0
//		double dbY = rho_max / m_sn[theta_max]; //x==0
//		// (dbX,0) - (0,dbY)  を通る直線と　矩形(0,0,nW,nH)との交点を求める
//		// t *  (dbX,-dbY) + (0,dbY)
//
//		PointDbl pt[4];
//		//x==0との交点　(0,dbY)
//		pt[0].x = 0;
//		pt[0].y = dbY;			
//		//x==m_XMAXとの交点 t = m_XMAX / dbX
//		pt[1].x = m_XMAX;
//		pt[1].y = (dbX != 0) ? (dbY - m_XMAX * dbY / dbX) : -1;
//		//y==0との交点 (dbX,0)
//		pt[2].x = dbX;
//		pt[2].y = 0;
//		//y==m_YMAXとの交点 t = (dbY-m_YMAX)/dbY
//		pt[3].x = (dbY != 0) ? (dbX*(dbY-m_YMAX) / dbY) : -1;
//		pt[3].y = m_YMAX;
//
//		//矩形と直線との交点を２つに絞る
//		PointDbl pt1(0,0);
//		PointDbl pt2(0,0);
//		for(int i=0;i<4;i++){
//			if( 0 <= pt[i].x && pt[i].x <= m_XMAX  && 0 <= pt[i].y && pt[i].y <= m_YMAX ){
//				if(pt1.x <= 0 && pt1.y <= 0){
//					pt1 = pt[i];
//				}else{
//					pt2 = pt[i];
//					break;
//				}
//			}
//		}
//		//Start:pt1 End:pt2 
//		DrawLine(pt1,pt2,byLine,nPixels,nCheckLen); //CHECK_LENごとに直線検出検査
//
//		PointDbl vec = pt2 - pt1; //方向ベクトル
//		double dbLen = vec.GetLength();
//		vec.x /= dbLen; //単位ベクトル化
//		vec.y /= dbLen;
//
//		pt1.x = (int)(nCheckLen/2 * vec.x + pt1.x + 0.5); //半分の距離づらす
//		pt1.y = (int)(nCheckLen/2 * vec.y + pt1.y + 0.5);
//		DrawLine(pt1,pt2,byLine,nPixels,nCheckLen); //CHECK_LENごとに直線検出検査
//
//		//近傍の直線を消す
//		int nImin = -THETA_MAX / 2;
//		int nImax = THETA_MAX / 2;
//		for(int j=-10;j<=10;j++){
//			for(int i=nImin;i<=nImax;i++){
//				if(theta_max+i<0){//マイナス角度
//					theta_max+=THETA_MAX;
//					rho_max=-rho_max;
//				}
//				if(theta_max+i>=THETA_MAX){
//					theta_max-=THETA_MAX;
//					rho_max=-rho_max;
//				}
//				if(rho_max+j<-RHO_MAX || RHO_MAX <= rho_max+j){
//					continue;
//				}
//				counter[theta_max+i][rho_max+RHO_MAX+j].m_nCnt = 0;
//			}
//		}
//	}
//	return TRUE;
//}
//
////CHECK_LENごとに直線検出検査
//void FindLine::DrawLine(PointDbl pt1,PointDbl pt2,BYTE byLine,int nPixels,int nCheckLen)
//{
//	//pt1 - pt2間の距離
//	PointDbl vec = pt2 - pt1; //方向ベクトル
//	double dbLen = vec.GetLength();
//	vec.x /= dbLen; //単位ベクトル化
//	vec.y /= dbLen;
//	int tmax = (int)dbLen;
//
//	//(x,y) = t * vec + pt1
//	CPoint ptS((int)pt1.x,(int)pt1.y); //ライン端点
//	CPoint ptE(0,0); //ライン端点
//	int nCnt = 0;
//	int nCheckPoint = 0;
//	int nArea = 0; //検査エリア番号
//	for(int t=0;t<tmax;t++){
//		int nX = min(m_XMAX-1,(int)(t * vec.x + pt1.x + 0.5));
//		int nY = min(m_YMAX-1,(int)(t * vec.y + pt1.y + 0.5));
//		nArea = t / nCheckLen; //現在のエリア番号
//		if( nArea > nCheckPoint ){ //次のエリアに移った時
//			ptE.x = nX;
//			ptE.y = nY;
//			nCheckPoint = nArea; //新しいエリアに移動
//			if( nCnt >= nPixels ){ //エラーライン
//				m_dib.Line(ptS.x,ptS.y,ptE.x,ptE.y,COLOR_RED);
//			}else{
//				m_dib.Line(ptS.x,ptS.y,ptE.x,ptE.y,COLOR_GREEN);
//			}
//			ptS = ptE; //次の開始点へ
//			nCnt = 0; //クリアポイント
//		}							
//		//カウント
//		BYTE byB = m_dib.GetBitsB(nX,nY);
//		BYTE byG = m_dib.GetBitsG(nX,nY);
//		BYTE byR = m_dib.GetBitsR(nX,nY);
//		if( byB == 255 && byG <= 0 ){//&& byR <= 0){ 
//			nCnt++; //カウント
//		}
//	}
//}
//
//void FindLine::Draw(CDC &dc)
//{
//	//m_dib.Draw(&dc,0,0,m_dib.GetWidth(),m_dib.GetHeight(),0,0);
//}