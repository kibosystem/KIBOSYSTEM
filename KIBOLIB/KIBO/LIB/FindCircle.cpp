#include "../stdafx.h"
#include "FindCircle.h"
//
//FindCircle::FindCircle()
//{
//	m_nRmin = 0;	//半径最小
//	m_nRmax = 0;	//半径最大
//	m_nPixelsMin = 0; //指定画素数黒
//	m_nPixelsMax = 0; //指定画素数白
//	ZeroMemory(m_sn,sizeof(m_sn));	//三角関数テーブル（サイン）	
//	ZeroMemory(m_cs,sizeof(m_cs));	//三角関数テーブル（コサイン）
//}
//FindCircle::~FindCircle()
//{
//
//}
//BOOL FindCircle::Init(CDIB &dib,int nRmin,int nRmax,int nPixelsMin,int nPixelsMax)
//{
//	m_aryCircle.clear();
//	//画像ファイルを読み込む
//
//	int nCx = 640;
//	int nCy = 480;
//	CDC dcMem;
//	dcMem.CreateCompatibleDC(NULL);
//	CDIB dibSrc;
//	if( dibSrc.Create(nCx,nCy) ){
//		CBitmap *pOld = dcMem.SelectObject( dibSrc.GetBitmap() );	
//		dib.StretchDraw(&dcMem,0,0,nCx,nCy);
//		dcMem.SelectObject( pOld );
//
//		dibSrc.WriteBitmap(L"F:\\tmp\\tmp.bmp");
//	}
//
//	if( m_dibGray.Copy(dibSrc) ){ //フルカラー → Gray
//		int nW = m_dibGray.GetWidth();
//		int nH = m_dibGray.GetHeight();
//		if( nW > 0 && nH > 0 ){//変化があった時だけ
//			if( m_nRmax != nRmax || m_nRmin != nRmin || m_nPixelsMin != nPixelsMin || m_nPixelsMax != nPixelsMax){
//				m_nPixelsMin = nPixelsMin; //黒画素数
//				m_nPixelsMax = nPixelsMax; //白画素数
//				m_nRmin = nRmin; //半径
//				m_nRmax = nRmax;
//				//三角関数テーブルを作成
//				double PIK = PI/THETA_MAX_C;
//				for(int i=0;i<THETA_MAX_C;i++){
//					m_sn[i] = (float)sin(PIK*i);
//					m_cs[i] = (float)cos(PIK*i);
//				}
//				//斜線長テーブルを作成
//				m_diagonal.resize(nW*nH);
//				for(int y=0;y<nH;y++){
//					for(int x=0;x<nW;x++){
//						m_diagonal[y*nW+x]=(short)(sqrt((float)(y*y+x*x))+0.5);
//					}
//				}
//			}
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
////ハフ変換円検出
//void FindCircle::Find()
//{
//	// ---------------------- Hough変換 --------------------------
//	int x,y;
//	int centerX,centerY,radius;
//    //円検出用頻度カウンタ
//	vector<short> v_counterMax; //Max値
//	vector<short> v_counterMin; //Min値
//
//	BYTE *pGrayT = m_dibGray.GetBits();
//	int nW = m_dibGray.GetWidth();
//	int nH = m_dibGray.GetHeight();
//	int nXRmax = nW * m_nRmax;
//	v_counterMax.resize(nH*nXRmax);
//	v_counterMin.resize(nH*nXRmax);
//	int nLineSize = m_dibGray.GetLineSize();	
//	for(y=0;y<nH;y++){
//		BYTE *pGray = pGrayT + (nH-1-y)*nLineSize;
//		for(x=0;x<nW;x++){
//			BYTE byVal = *pGray++;
//			//if( byVal == 255){ //ライン値Max
//			//	//x,yを通過する全ての円テーブルをインクリメント
//			//	for(centerY=0;centerY<nH;centerY++){
//			//		int distY=abs(y-centerY);
//			//		if(distY<m_nRmax){
//			//			for(centerX=0;centerX<nW;centerX++){
//			//				int distX = abs(x-centerX);
//			//				radius = m_diagonal[distY*nW+distX];
//			//				if(radius<m_nRmax){
//			//					v_counterMax[centerY * nXRmax + centerX * m_nRmax + radius] += 1;
//			//				}							
//			//			}
//			//		}
//			//	}
//   //         }
//			if( byVal == 0 ){ //ライン値Min
//				//x,yを通過する全ての円テーブルをインクリメント
//				for(centerY=0;centerY<nH;centerY++){
//					int distY=abs(y-centerY);
//					if(distY<m_nRmax){
//						for(centerX=0;centerX<nW;centerX++){
//							int distX = abs(x-centerX);
//							radius = m_diagonal[distY*nW+distX];
//							if(radius<m_nRmax){
//								v_counterMin[centerY * nXRmax + centerX * m_nRmax + radius] += 1;
//							}
//						}
//					}
//				}
//            }
//		}
//	}
//
//	// --------------------- Hough逆変換 -------------------------
//	CIRCLE result; //検出結果
//	int end_flag=0;   //繰り返しを終了させるフラグ
//	int count=0;      //検出された直線または円の個数カウンタ
//	int counter_max;
//	while(count<5){
//		count++;
//		counter_max = 0;
//        //counterが最大になるcenterX_max、
//		//centerY_maxとradius_maxを求める
//		for(centerY=0;centerY<nH;centerY++){
//			for(centerX=0;centerX<nW;centerX++){
//				for(radius=m_nRmin;radius<m_nRmax;radius++){
//					int nIndex = centerY*nXRmax+centerX*m_nRmax+radius;
//					int nPixMin = v_counterMin[nIndex]; //0  の画素数
//					int nPixMax = v_counterMax[nIndex]; //255の画素数
//					if(m_nPixelsMin <= nPixMin && m_nPixelsMax <= nPixMax){//どちらも基準を超える必要あり
//						int nCnt = nPixMin + nPixMax;
//						if(nCnt > counter_max){ //円検出用頻度カウンタ最大値を求める
//							counter_max = nCnt;
//							result.x=centerX;
//							result.y=centerY;
//							result.r=radius;
//							result.val0 = nPixMin;
//							result.val255 = nPixMax;
//						}
//					}
//				}
//			}
//		}
//		if(result.val0 < m_nPixelsMin && result.val255 < m_nPixelsMax ){//指定画素数より下の円になれば検出を終了
//			break;
//		}
//		//近傍の円を消す
//		for(int k=-10;k<=10;k++){
//			if(result.y+k>=nH || result.y+k<0){ continue;}
//			for(int j=-10;j<=10;j++){
//				if(result.x+j>=nW || result.x+j<0){ continue;}
//				for(int i=-20;i<=20;i++){
//					int nR = result.r+i;
//					if( 0 < nR && nR <= m_nRmax ){
//						int nIndex = (result.y+k)*nXRmax+(result.x+j)*m_nRmax+nR;
//						v_counterMax[nIndex]=0;
//						v_counterMin[nIndex]=0;
//					}
//			   }
//			}
//		}          
//		//検出した円
//		if( counter_max > 0 ){
//			m_aryCircle.push_back(result);
//		}
//	}
//}
////結果出力
//void FindCircle::Output()
//{
//	COutputView *pOutput = GetOutputView();
//	pOutput->Clear();
//	for(int i=0,imax=(int)m_aryCircle.size();i<imax;i++){
//		CIRCLE &result = m_aryCircle[i];
//		pOutput->Output(true,L"座標:(%d,%d) 半径:%d 黒:%d 白:%d",result.x,result.y,result.r,result.val0,result.val255);
//	}
//}
