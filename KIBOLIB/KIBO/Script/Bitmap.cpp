#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOView.h"
#include "../LIB/macro.h"
#include "App.h"
#include "Bitmap.h"
#include "ImageDevice.h"
#include "Labeling.h"

MapPDib::~MapPDib()
{
	Clear();
}
//全ビットマップ削除
void MapPDib::Clear()
{
	for (POSITION pos = GetStartPosition(); pos != NULL;){
		int nKey;
		CDIB *pPtr = NULL; //ビットマップポインタ
		GetNextAssoc(pos, nKey, pPtr);
		if( pPtr != NULL ){
			delete pPtr;
		}
	}
	RemoveAll();
}

//ビットマップクラス取得
CDIB* MapPDib::GetDibPtr(int nKey) 
{
	CDIB *pPtr = NULL;
	if( Lookup(nKey,pPtr) ){ //すでに登録済み
		if( pPtr != NULL ){
			return pPtr;
		}
	}
	pPtr = new CDIB;//新規作成
	try{
		//bitmap作成
		SetAt(nKey,pPtr);
	}catch( CException* e ){
		::ErrorMsg(L"Memory Error");
		e->Delete();
	}
	return pPtr;
}

static CommandTable s_table[] =
{
	{Bitmap24::FUNC_LOAD		, 1,L"Load"}		//読み込み
	,{Bitmap24::FUNC_SAVE		,-1,L"Save"}		//書き込み
	,{Bitmap24::FUNC_SHOW		,0,L"Show"}		//表示
	,{Bitmap24::FUNC_SETTHRESHOLD,1,L"SetThreshold"}//しきい値設定
	,{Bitmap24::FUNC_GETTHRESHOLD,-1,L"GetThreshold" }	//2値化しきい値取得
	,{ Bitmap24::FUNC_SETLINECOLOR,3,L"SetLineColor"}	//ライン色設定
	,{Bitmap24::FUNC_FROMIMAGEDEVICE,0,L"FromImageDevice"}	//カメラ画像取得
	,{Bitmap24::FUNC_IMAGEDEVICEDIFF,1,L"ImageDeviceDiff"}	//カメラ画像比較
	,{Bitmap24::FUNC_TOIMAGEDEVICE,0,L"ToImageDevice"}	//イメージデバイスへ画像送信
	,{Bitmap24::FUNC_COLORLENGTH, 3,L"ColorLength"}	//色距離変換
	,{Bitmap24::FUNC_CHANGECOLOR, 1,L"ChangeColor"}	//色変換	
	,{Bitmap24::FUNC_GRADIENT	,-1,L"Gradient"}	//微分
	,{Bitmap24::FUNC_SOBEL		,-1,L"Sobel"}		//輪郭抽出 縦、横
	,{Bitmap24::FUNC_LAPLACIAN	,-1,L"Laplacian"}	//ラプラシアン
	,{Bitmap24::FUNC_PREWITT	,-1,L"Prewitt"}		//輪郭抽出
	,{Bitmap24::FUNC_GAMMA		, 1,L"Gamma"}		//ガンマ補正
	,{Bitmap24::FUNC_INVERT		,0,L"Invert"}		//各色反転
	,{Bitmap24::FUNC_CHANGERANGE,-1, L"ChangeRange" }		//コントラスト最大化
	,{Bitmap24::FUNC_EXPAND		,1,L"Expand"}		//膨張
	,{Bitmap24::FUNC_SHRINK		,1,L"Shrink"}		//収縮
	,{Bitmap24::FUNC_COLLECTBITMAP, 1, L"CollectBitmap" }//ビットマップを連想配列に連続登録
	,{Bitmap24::FUNC_SETBITMAP	,1,L"SetBitmap"}		//ビットマップを連想配列に保存
	,{Bitmap24::FUNC_GETBITMAP	,1,L"GetBitmap"}		//連想配列からビットマップ取得
	,{Bitmap24::FUNC_GETBITMAPCOUNT,0,L"GetBitmapCount" }//連想配列カウント取得
	,{Bitmap24::FUNC_DELTADIB	,2,L"DeltaDib"}		//ビットマップ差分
	,{Bitmap24::FUNC_DIVDIB		,2,L"DivDib"}		//ビットマップ割り算
	,{Bitmap24::FUNC_AVERAGE	,0,L"Average"}		//平均画像の作成
	,{Bitmap24::FUNC_DIBSETCLEAR,0,L"DibSetClear"}	//ビットマップ連想配列クリア
	,{Bitmap24::FUNC_HISTGRAM	,0,L"Histgram"}		//ヒストグラムの表示
	,{Bitmap24::FUNC_GETBINARYLEVEL,1,L"GetBinaryLevel"}//指定面積割合になる２値化レベルの取得
	,{Bitmap24::FUNC_GETBRIGHTNESS,5,L"GetBrightness"}	//明るさ変化右位置取得
	,{Bitmap24::FUNC_MEDIAN		,0,L"Median"}		//メディアンフィルタ
	,{Bitmap24::FUNC_MAPDELTA	,1,L"MapDelta"}		//連想配列比較画像作成
	,{Bitmap24::FUNC_EDIT		,2,L"Edit"}			//ビットマップ編集
	,{Bitmap24::FUNC_CALCTHRESHOOLD,0,L"CalcThreshold"} //しきい値自動計算
	,{Bitmap24::FUNC_SELECTCOLOR,4,L"SelectColor"}//色抽出
	,{Bitmap24::FUNC_SELECT,2,L"Select"}//上限と下限選択
	,{Bitmap24::FUNC_FINDEDGE,6,L"FindEdge"} //エッジ座標検出
	,{Bitmap24::FUNC_DRAWLINE,8,L"DrawLine"} //ライン描画
	,{Bitmap24::FUNC_DRAWBITMAPLINE,-1,L"DrawBitmapLine"} //ビットマップライン描画
	,{Bitmap24::FUNC_MASKBORDER,1,L"MaskBorder"} //ラベリング要素マスク
	,{Bitmap24::FUNC_CALCWIDTH,4,L"CalcWidth" }		//幅を計算
	,{Bitmap24::FUNC_CALCHEIGHT,4,L"CalcHeight" }	//高さを計算
	,{ Bitmap24::FUNC_CALCWIDTHDIRECT,3,L"CalcWidthDirect" }		//最小幅を計算
	,{ Bitmap24::FUNC_CALCHEIGHTDIRECT,3,L"CalcHeightDirect" }	//最小高さを計算	
	,{Bitmap24::FUNC_RMAX,-1,L"Rmax"}	//R要素最大値
	,{Bitmap24::FUNC_RMIN,-1,L"Rmin"}	//R要素最小値
	,{Bitmap24::FUNC_GMAX,-1,L"Gmax"}	//G要素最大値
	,{Bitmap24::FUNC_GMIN,-1,L"Gmin"}	//G要素最小値
	,{Bitmap24::FUNC_BMAX,-1,L"Bmax"}	//B要素最大値
	,{Bitmap24::FUNC_BMIN,-1,L"Bmin"}	//B要素最小値
	,{Bitmap24::FUNC_DEFECTTEST,3,L"DefectTest"}//欠陥検査
	,{Bitmap24::FUNC_LENSCHECK,4,L"LensCheck"}//レンズチェック
	,{Bitmap24::FUNC_BINARYMATCHING, 5, L"BinaryMatching" }//2値化マッチング
	,{Bitmap24::FUNC_OUTPUTFILEPATH, -1, L"OutputFilePath" }//FilePath出力
	,{Bitmap24::FUNC_OUTPUTFILENAME, -1, L"OutputFileName" }//FileName出力
	,{Bitmap24::FUNC_MAX  ,0,NULL}	//コマンド数
};

static Bitmap24 *s_pInstance = NULL; //唯一のクラス実体
Bitmap24* Bitmap24::GetInstance()//実体取得
{
	return s_pInstance;
}

Bitmap24::Bitmap24(void)
{
	s_pInstance = this;
	m_byTh = 128;	//しきい値設定
}

Bitmap24::~Bitmap24(void)
{
}

//関数名チェック
STDMETHODIMP Bitmap24::GetIDsOfNames(REFIID riid
						,LPOLESTR __RPC_FAR *rgszNames
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
STDMETHODIMP Bitmap24::Invoke(DISPID dispIdMember	//Function ID 番号
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
	switch(dispIdMember){
		case FUNC_LOAD: //Load			
			{
				//引数文字列取得
				CString strPath = pDispParams->rgvarg[0].bstrVal;// ビットマップファイルパス
				if( strPath[0] == L'.' ){ //カレントディレクトリ指定
					strPath = GetApp()->GetCurrentPath() + strPath.Mid(1);
				}
				BOOL isOK = m_dib.Load(strPath);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_SAVE:	//書き込み
			{
				::SetCurrentDirectory( GetApp()->GetAppPath() ); //カレントディレクトリ設定
				CString strPath = pDispParams->rgvarg[0].bstrVal;// ビットマップファイルパス
				CTime time = CTime::GetCurrentTime(); //現在時刻取得
				if( strPath.Find(_T("TODAY")) >= 0 ){
					CString strDate = time.Format(_T("%Y%m%d"));
					strPath.Replace(_T("TODAY"),strDate);
				}
				if( strPath.Find(_T("TIME")) >= 0 ){
					CString strTime = time.Format(_T("%H%M%S"));
					strPath.Replace(_T("TIME"),strTime);
				}
				//strPath.Replace(L"/",L"");
				//strPath.Replace(L"?",L"");
				CString strExt = strPath.Right(3);//拡張子取得
				m_dib.WriteImage(strPath,strExt);//画像ファイル保存
			}
			break;
		case FUNC_SHOW: //表示
			{
				ShowBitmap(); //ビットマップ表示仮想関数
			}
			break;
		case FUNC_SETTHRESHOLD: //しきい値設定
			{
				BYTE byTh = pDispParams->rgvarg[0].bVal;
				SetTh( byTh );
			}
			break;
		case FUNC_SETLINECOLOR:	//ライン色設定
		{
			BYTE byR = GetValientInt(&pDispParams->rgvarg[2]);//R
			BYTE byG = GetValientInt(&pDispParams->rgvarg[1]);//G
			BYTE byB = GetValientInt(&pDispParams->rgvarg[0]);//B
			SetLineColor(RGB(byR, byG, byB)); //
		}
			break;
		case FUNC_IMAGEDEVICEDIFF://カメラ画像比較
			{
				BOOL isDiff = ImageDeviceDiff(); //比較
				ReturnBool(wFlags,pVarResult,isDiff); //  != : true
			}
			break;
		case FUNC_FROMIMAGEDEVICE://カメラ画像取得
			{
				FromImageDevice();
			}
			break;
		case FUNC_TOIMAGEDEVICE://イメージデバイスへ画像送信
			{
				ToImageDevice();
			}
			break;
		case FUNC_COLORLENGTH: //色距離変換
			{
				int nR = pDispParams->rgvarg[2].ulVal; //R
				int nG = pDispParams->rgvarg[1].ulVal; //G
				int nB = pDispParams->rgvarg[0].ulVal; //B
				//指定色と近い色を計算
				m_dib.ColorLength(nR,nG,nB); //指定色との距離画像に変換
			}
			break;
		case FUNC_CHANGECOLOR:	//色変換
			{
				CString strMode = pDispParams->rgvarg[0].bstrVal; //色モード
				int nType = CDIB::TYPE_COLOR; //Default Type
				if(strMode== _T("RGRAY")){
					nType = CDIB::TYPE_RGRAY;
				}else if(strMode== _T("GGRAY")){
					nType = CDIB::TYPE_GGRAY;
				}else if(strMode== _T("BGRAY")){
					nType = CDIB::TYPE_BGRAY;
				}else if(strMode== _T("R")){
					nType = CDIB::TYPE_R;
				}else if(strMode== _T("G")){
					nType = CDIB::TYPE_G;
				}else if(strMode== _T("B")){
					nType = CDIB::TYPE_B;
				}else if(strMode== _T("GRAY")){
					nType = CDIB::TYPE_GRAY;
				}else if(strMode== _T("Y")){
					nType = CDIB::TYPE_Y;
				}else if(strMode== _T("H")){
					nType = CDIB::TYPE_H;
				}else if(strMode== _T("S")){
					nType = CDIB::TYPE_S;
				}else if(strMode== _T("2")){//2値化
					m_dib.ChangeBinary( GetTh() );
				}else{ //clear
					m_dib.PaintBlack();
					return S_OK;
				}
				m_dib.ChangeColor(nType); //色変換
			}
			break;
		case FUNC_GRADIENT://微分
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//引数が存在する場合
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Gradient(dbAmp); //微分
			}
			break;
		case FUNC_SOBEL:	//輪郭抽出 縦、横
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//引数が存在する場合
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Sobel(dbAmp); //微分
			}
			break;
		case FUNC_LAPLACIAN://ラプラシアン
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//引数が存在する場合
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Laplacian(dbAmp);
			}
			break;
		case FUNC_GAMMA: //ガンマ補正
			{
				double dbExp = GetValientDbl(&pDispParams->rgvarg[0]);
				Gamma(dbExp);
			}
			break;
		case FUNC_PREWITT:	//輪郭抽出
			{
				double dbAmp = 1.0;
				if( pDispParams->cArgs > 0 ){//引数が存在する場合
					dbAmp = GetValientDbl(&pDispParams->rgvarg[0]);
				}
				Prewitt(dbAmp); //輪郭抽出
			}
			break;
		case FUNC_INVERT:	//各色反転
			m_dib.Invert(); //各色反転
			break;
		case FUNC_CHANGERANGE:	//コントラスト最大化
			{
				if( pDispParams->cArgs == 0 ){
					BYTE byMin,byMax;
					if( m_dib.GetRange(byMin,byMax) ){ //最大値と最小値の取得
						m_dib.ChangeRange(byMin, byMax);//Rangeを最小値->0 ... 最大値->255へ広げる
					}
				}else if(pDispParams->cArgs== 2){ //最小値と最大値の指定あり
					BYTE byMin = (BYTE)pDispParams->rgvarg[1].ulVal; //最小値取得
					BYTE byMax = (BYTE)pDispParams->rgvarg[0].ulVal; //最大値取得
					m_dib.ChangeRange(byMin, byMax);//Rangeを最小値->0 ... 最大値->255へ広げる
				}
			}
			break;
		case FUNC_EXPAND://膨張
			{
				int nNum = pDispParams->rgvarg[0].ulVal; //膨張回数
				Expand(nNum);
			}
			break;
		case FUNC_SHRINK://収縮
			{
				int nNum = pDispParams->rgvarg[0].ulVal; //膨張回数
				Shrink(nNum);
			}
			break;
		case FUNC_COLLECTBITMAP://ビットマップを連想配列に連続登録
		{
			CString strPath = pDispParams->rgvarg[0].bstrVal; //Path
			CollectBitmap(strPath); //指定フォルダ内ビットマップ収集

		}
			break;
		case FUNC_SETBITMAP: //ビットマップを連想配列に保存
			{
				int nNo = pDispParams->rgvarg[0].ulVal; //ビットマップ番号
				SetDib(nNo);
			}
			break;
		case FUNC_GETBITMAP://連想配列からビットマップ取得
			{
				int nNo = pDispParams->rgvarg[0].ulVal; //ビットマップ番号
				CDIB *pDib = m_mapDibPtr.GetDibPtr(nNo); //ビットマップクラス取得
				if(pDib != NULL){
					pDib->Copy(m_dib); //pDib - > m_dib
				}								
			}
			break;
		case FUNC_GETBITMAPCOUNT:
			{
				int nCnt = GetMapCount();
				ReturnInt(wFlags, pVarResult, nCnt);
			}
			break;
		case FUNC_DELTADIB:	//ビットマップ差分
			{
				int nNo1 = pDispParams->rgvarg[1].ulVal; //ビットマップ番号
				int nNo2 = pDispParams->rgvarg[0].ulVal; //ビットマップ番号
				CDIB *pDib1 = m_mapDibPtr.GetDibPtr(nNo1); //ビットマップクラス取得
				CDIB *pDib2 = m_mapDibPtr.GetDibPtr(nNo2); //ビットマップクラス取得
				DeltaDib(pDib1,pDib2); //ビットマップの引き算
			}
			break;
		case FUNC_DIVDIB://ビットマップ割り算
			{
				int nNo1 = pDispParams->rgvarg[1].ulVal; //ビットマップ番号
				int nNo2 = pDispParams->rgvarg[0].ulVal; //ビットマップ番号
				CDIB *pDib1 = m_mapDibPtr.GetDibPtr(nNo1); //ビットマップクラス取得
				CDIB *pDib2 = m_mapDibPtr.GetDibPtr(nNo2); //ビットマップクラス取得
				DivDib(pDib1,pDib2); //ビットマップの引き算		
			}
			break;
		case FUNC_AVERAGE: //平均画像の作成
			AverageFilter();
			break;
		case FUNC_DIBSETCLEAR:	//ビットマップ連想配列クリア
			m_mapDibPtr.Clear();
			break;
		case FUNC_HISTGRAM: //ヒストグラムの表示
			ShowHistgram();
			break;
		case FUNC_GETBINARYLEVEL: //指定面積割合になる２値化レベルの取得
			{
				double dbPer = GetValientDbl(&pDispParams->rgvarg[0]);	//指定割合
				ReturnInt(wFlags,pVarResult,GetBinaryLevel(dbPer));
			}
			break;
		case FUNC_GETTHRESHOLD:  //2値化しきい値取得
			{
				int nTh  = this->GetTh(); //設定されたしきい値
				if( pDispParams->cArgs == 2 ){
					CString strColor = pDispParams->rgvarg[1].bstrVal;//白？黒？
					int nPixels = pDispParams->rgvarg[0].ulVal; //指定画素数
					nTh = CalcThreshold(strColor,nPixels);
				}else if(pDispParams->cArgs == 1){
					CString strCommand = pDispParams->rgvarg[0].bstrVal;
					if( strCommand == "auto" || strCommand == "AUTO" ){
						nTh = CalcThreshold();						
					}
				}
				ReturnInt(wFlags,pVarResult,nTh);	
			}			
			break;
		case FUNC_GETBRIGHTNESS: //明るさの範囲を求める
			{
				CString strType = pDispParams->rgvarg[4].bstrVal;//MIN？MAX？
				CRect rect;
				rect.left   = GetValientInt(&pDispParams->rgvarg[3]);//左
				rect.top    = GetValientInt(&pDispParams->rgvarg[2]);//上
				rect.right  = GetValientInt(&pDispParams->rgvarg[1]);//右
				rect.bottom = GetValientInt(&pDispParams->rgvarg[0]);//下
				int nVal = -1;
				if( strType == L"MIN"){
					nVal = GetBrightnessMin(rect);
				}else if(strType == L"MAX"){
					nVal = GetBrightnessMax(rect);
				}else if(strType == L"RANGE"){
					nVal = GetBrightnessRange(rect);
				}else{ //strType == L"DIFF"
					nVal = GetBrightnessDiff(rect);
				}
				ReturnInt(wFlags,pVarResult,nVal);	
			}
			break;
		case FUNC_MEDIAN:	//メディアンフィルタ
			{
				m_dib.MedianFilter();
			}
			break;
		case FUNC_MAPDELTA:	//連想配列最小値画像作成
			{
				double dbAmp = GetValientDbl(&pDispParams->rgvarg[0]); //倍率
				MapDelta(dbAmp);
			}
			break;
		case FUNC_EDIT:		//ビットマップ編集
			{
				CString strEdit = pDispParams->rgvarg[1].bstrVal; //ビットマップ編集アプリケーション
				CString strFile = pDispParams->rgvarg[0].bstrVal; //ビットマップファイル名称
				BOOL isOK = Exec(SW_HIDE,strEdit.GetBuffer(),strFile.GetBuffer());
				ReturnBool(wFlags,pVarResult,isOK);
			}			
			break;
		case FUNC_CALCTHRESHOOLD://しきい値自動計算
			{
				int nRet = CalcThreshold();
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_SELECTCOLOR: //色抽出
			{
				double dbLength = GetValientDbl(&pDispParams->rgvarg[3]);//距離
				BYTE byR = GetValientInt(&pDispParams->rgvarg[2]);//R
				BYTE byG = GetValientInt(&pDispParams->rgvarg[1]);//G
				BYTE byB =GetValientInt(&pDispParams->rgvarg[0]);//B
				SelectColor(dbLength,byR,byG,byB);
			}
			break;
		case FUNC_SELECT://上限と下限選択
			{
				double dbLow = GetValientDbl(&pDispParams->rgvarg[1]);
				double dbHigh= GetValientDbl(&pDispParams->rgvarg[0]);
				SelectLH(dbLow,dbHigh);
			}
			break;
		case FUNC_FINDEDGE: //エッジ座標検出
			{
				CRect rect;
				rect.left   = GetValientInt(&pDispParams->rgvarg[5]);//左
				rect.top    = GetValientInt(&pDispParams->rgvarg[4]);//上
				rect.right  = GetValientInt(&pDispParams->rgvarg[3]);//右
				rect.bottom = GetValientInt(&pDispParams->rgvarg[2]);//下
				int nDirection = GetValientInt(&pDispParams->rgvarg[1]);//方向
				int nColor = GetValientInt(&pDispParams->rgvarg[0]);//白 or 黒
				double dbEdge = FindEdge(rect,nDirection,nColor); //Point取得
				ReturnDbl(wFlags,pVarResult,dbEdge);
			}
			break;
		case FUNC_DRAWLINE:		//ライン描画
			{
				int nX1 = GetValientInt(&pDispParams->rgvarg[7]);//x1
				int nY1 = GetValientInt(&pDispParams->rgvarg[6]);//y1
				int nX2 = GetValientInt(&pDispParams->rgvarg[5]);//x2
				int nY2 = GetValientInt(&pDispParams->rgvarg[4]);//y2
				BYTE byR = pDispParams->rgvarg[3].intVal;//R
				BYTE byG = pDispParams->rgvarg[2].intVal;//G
				BYTE byB = pDispParams->rgvarg[1].intVal;//B
				int nWidth=pDispParams->rgvarg[0].intVal;//Line width
				DrawLine(nX1,nY1,nX2,nY2,RGB(byR,byG,byB),nWidth);
			}
			break;
		case FUNC_DRAWBITMAPLINE: //ビットマップライン描画
			{
				if (pDispParams->cArgs  > 0) {
					App *pApp = App::GetInstance();
					CString strMsg;
					strMsg.Format(L"%.2f,%.2f-%.2f,%.2f",m_ptS.X,m_ptS.Y,m_ptE.X,m_ptE.Y);
					pApp->TextOutput(strMsg);
				}
				DrawBitmapLine();
			}
			break;
		case FUNC_MASKBORDER: //ラベリング要素マスク
			{
				int nEnum = pDispParams->rgvarg[0].intVal;//Element No
				MaskBorder(nEnum);
			}
			break;
		case FUNC_CALCWIDTH://幅を計算
		{
			int nX = GetValientInt(&pDispParams->rgvarg[3]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[2]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[1]);//範囲指定
			CString strType = pDispParams->rgvarg[0].bstrVal; //画像計測種類
			double dbWidth = CalcWidth(nX, nY, nRange, strType);
			ReturnDbl(wFlags, pVarResult, dbWidth);
		}
			break;
		case FUNC_CALCHEIGHT:	//高さを計算
		{
			int nX = GetValientInt(&pDispParams->rgvarg[3]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[2]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[1]);//範囲指定
			CString strType = pDispParams->rgvarg[0].bstrVal; //画像計測種類
			double dbHeight = CalcHeight(nX, nY, nRange, strType);
			ReturnDbl(wFlags, pVarResult, dbHeight);
		}
			break;
		case FUNC_CALCWIDTHDIRECT:		//最小幅を計算
		{
			int nX = GetValientInt(&pDispParams->rgvarg[2]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[1]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[0]);//範囲指定
			double dbWidth = CalcWidthDirect(nX,nY,nRange, m_ptS,m_ptE);
			ReturnDbl(wFlags, pVarResult, dbWidth);
		}
			break;
		case FUNC_CALCHEIGHTDIRECT:		//最小高さを計算
		{
			int nX = GetValientInt(&pDispParams->rgvarg[2]);//x
			int nY = GetValientInt(&pDispParams->rgvarg[1]);//y
			int nRange = GetValientInt(&pDispParams->rgvarg[0]);//範囲指定
			double dbHeight = CalcHeightDirect(nX, nY, nRange, m_ptS, m_ptE);
			ReturnDbl(wFlags, pVarResult, dbHeight);
		}
		break;

		case FUNC_RMAX://R要素最大値
			{
				int nRet = 0;
				if(pDispParams->cArgs==0){
					nRet = GetMaxElement(2);
					ReturnInt(wFlags,pVarResult,nRet);
				}else if( pDispParams->cArgs == 4 ){//領域指定
					int nXmin = GetValientInt(&pDispParams->rgvarg[3]);
					int nYmin = GetValientInt(&pDispParams->rgvarg[2]);
					int nXmax = GetValientInt(&pDispParams->rgvarg[1]);
					int nYmax = GetValientInt(&pDispParams->rgvarg[0]);
					nRet = GetMaxElement(2,nXmin,nYmin,nXmax,nYmax);					
				}
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_RMIN:	//R要素最小値
			{
				int nRet = 0;
				if( pDispParams->cArgs == 0 ){
					nRet = GetMinElement(2);					
				}else if( pDispParams->cArgs == 4 ){ //領域指定
					int nXmin = GetValientInt(&pDispParams->rgvarg[3]);
					int nYmin = GetValientInt(&pDispParams->rgvarg[2]);
					int nXmax = GetValientInt(&pDispParams->rgvarg[1]);
					int nYmax = GetValientInt(&pDispParams->rgvarg[0]);
					nRet = GetMinElement(2,nXmin,nYmin,nXmax,nYmax);
				}
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_GMAX:	//G要素最大値
			ReturnInt(wFlags,pVarResult,GetMaxElement(1));
			break;
		case FUNC_GMIN:	//G要素最小値
			ReturnInt(wFlags,pVarResult,GetMinElement(1));
			break;
		case FUNC_BMAX:	//B要素最大値
			ReturnInt(wFlags,pVarResult,GetMaxElement(0));
			break;
		case FUNC_BMIN:	//B要素最小値
			ReturnInt(wFlags,pVarResult,GetMinElement(0));
			break;
		case FUNC_DEFECTTEST:	//欠陥検査
			{
				int nTh    = pDispParams->rgvarg[2].intVal;//しきい値
				int nRange = pDispParams->rgvarg[1].intVal;//しきい値幅
				int nMin   =  pDispParams->rgvarg[0].intVal;//最小画素数
				DefectTest(nTh,nRange,nMin);//欠陥検査
			}
			break;
		case FUNC_LENSCHECK:	//レンズチェック
			{
				int nTh   = pDispParams->rgvarg[3].intVal;//しきい値
				int nRange= pDispParams->rgvarg[2].intVal;//しきい値幅
				int nMin   =  pDispParams->rgvarg[1].intVal;//最小画素数
				int nEdge = pDispParams->rgvarg[0].intVal;//周囲画素数
				DefectTest(nTh,nRange,nMin);//欠陥検査
				PaintBorder(nEdge);
			}
			break;
		case FUNC_BINARYMATCHING: //2値化マッチング
		{
			int nTh = pDispParams->rgvarg[4].intVal;//しきい値
			double dbX = GetValientDbl(&pDispParams->rgvarg[3]);
			double dbY = GetValientDbl(&pDispParams->rgvarg[2]);
			int nMin = pDispParams->rgvarg[1].intVal;//最小画素数
			int nMax = pDispParams->rgvarg[0].intVal;//最大画素数
			double dbVal = BinaryMatching(nTh,dbX,dbY,nMin,nMax); //マッチング%
			ReturnDbl(wFlags, pVarResult, dbVal);
		}
			break;
		case FUNC_OUTPUTFILEPATH://FilePath出力
		
		if (pDispParams->cArgs == 3){
			CString strID = pDispParams->rgvarg[2].bstrVal;//
			int nX = pDispParams->rgvarg[1].intVal;//
			int nY = pDispParams->rgvarg[0].intVal;//			
			App::GetInstance()->DrawText(strID, m_dib.GetFilePath(), nX, nY);
		}
		else{
			App::GetInstance()->TextOutput(m_dib.GetFilePath());
		}
			break;
		case FUNC_OUTPUTFILENAME: //FileName出力
			if (pDispParams->cArgs == 3){
				CString strID = pDispParams->rgvarg[2].bstrVal;//
				int nX = pDispParams->rgvarg[1].intVal;//
				int nY = pDispParams->rgvarg[0].intVal;//			
				App::GetInstance()->DrawText(strID, m_dib.GetFileName(), nX, nY);
			}
			else{
				App::GetInstance()->TextOutput(m_dib.GetFileName());
			}
			break;
	}
	return S_OK;
}

//指定ビットマップの平均を計算
void Bitmap24::Average(CStringArray &strAryPath)
{
	int nBnum = (int)strAryPath.GetSize();
	int nSmax = 0; //ビットマップサイズ最大
	for(int i=0;i<nBnum;i++){//ファイルの数だけループ
		CDIB *pDib = m_mapDibPtr.GetDibPtr(i); //ビットマップクラス取得
		if( pDib != NULL ){
			pDib->Load( strAryPath[i] ); //読み込み
			
			if( nSmax < pDib->GetSize() ){
				nSmax = pDib->GetSize();
			}
		}
	}
	if( nSmax > 0 ){
		//合計値を計算
		m_bufCalc.Alloc(nSmax); //計算用バッファの作成		
		for(int i=0;i<nBnum;i++){
			CDIB *pDib = m_mapDibPtr.GetDibPtr(i);
			if(pDib == NULL){
				continue ;
			}
			int nW = pDib->GetWidth();
			int nH = pDib->GetHeight();
			BYTE *pSrc = pDib->GetBits();
			int *pDst = m_bufCalc.m_pBuf; //転送先
			int nSize = pDib->GetSize();
			for(int n=0;n<nSize;n++){
				*pDst++ += *pSrc++;
			}			
			if( nW != m_dib.GetWidth() || nH != m_dib.GetHeight() ){ 
				m_dib.Create(nW,nH);//ビットマップ作成
			}		

		}
		//結果をメンバビットマップm_dibにセット
		int *pSrc  = m_bufCalc.m_pBuf; //転送先
		BYTE *pDst = m_dib.GetBits();
		for(int n=0;n<nSmax;n++){
			*pDst++ = (BYTE)(*pSrc++ / nBnum);
		}
	}
}

//膨張
void Bitmap24::Expand(int nNum) 
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 + 3; //パディングサイズ //x==nW-1分も考慮
	if( m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		for(int i=0;i<nNum;i++){
			Expand(nW,nH,nLine,nPad);
		}
	}
}
void Bitmap24::Expand(int nW,int nH,int nLine,int nPad)
{
	BYTE *pSrc = m_dib.GetBits();
	BYTE *pDst = m_dibTmp.GetBits(); //先頭アドレス取得		

	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	//８方向を検査してもっとも大きな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	BYTE byVal; //Src Value
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			byVal = *pSrc;
			if( byVal < *(pSrc-3)){//左
				byVal = *(pSrc-3);
			}
			if( byVal < *(pSrc+3)){//右
				byVal = *(pSrc+3);
			}				
			if( byVal < *(pSrc-nLine-3)){//左上
				byVal = *(pSrc-nLine-3);
			}
			if( byVal < *(pSrc-nLine)){//上
				byVal = *(pSrc-nLine);
			}
			if( byVal < *(pSrc-nLine+3)){//右上
				byVal = *(pSrc-nLine+3);
			}
			if( byVal < *(pSrc+nLine-3)){//左下
				byVal = *(pSrc+nLine-3);
			}
			if( byVal < *(pSrc+nLine)){//下
				byVal = *(pSrc+nLine);
			}
			if( byVal < *(pSrc+nLine+3)){//右下
				byVal = *(pSrc+nLine+3);
			}
			//次へ
			pSrc++;
			*pDst++ = byVal; //最大値Set
		}
		pSrc += nPad;
		pDst += nPad;
	}
	m_dibTmp.Copy(m_dib); //m_dibTmp -> m_dib
}

//収縮
void Bitmap24::Shrink(int nNum)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 + 3; //パディングサイズ //x==nW-1分も考慮
	if( m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		for(int i=0;i<nNum;i++){
			Shrink(nW,nH,nLine,nPad);
		}
	}
}
//収縮
void Bitmap24::Shrink(int nW,int nH,int nLine,int nPad)
{
	BYTE *pSrc = m_dib.GetBits();
	BYTE *pDst = m_dibTmp.GetBits(); //先頭アドレス取得		
	//８方向を検査してもっとも小さな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	BYTE byVal; //Src Value
	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			byVal = *pSrc;
			if( byVal > *(pSrc-3)){//左
				byVal = *(pSrc-3);
			}
			if( byVal > *(pSrc+3)){//右
				byVal = *(pSrc+3);
			}				
			if( byVal > *(pSrc-nLine-3)){//左上
				byVal = *(pSrc-nLine-3);
			}
			if( byVal > *(pSrc-nLine)){//上
				byVal = *(pSrc-nLine);
			}
			if( byVal > *(pSrc-nLine+3)){//右上
				byVal = *(pSrc-nLine+3);
			}
			if( byVal > *(pSrc+nLine-3)){//左下
				byVal = *(pSrc+nLine-3);
			}
			if( byVal > *(pSrc+nLine)){//下
				byVal = *(pSrc+nLine);
			}
			if( byVal > *(pSrc+nLine+3)){//右下
				byVal = *(pSrc+nLine+3);
			}
			//次へ
			pSrc++;
			*pDst++ = byVal; //最大値Set
		}
		pSrc += nPad;
		pDst += nPad;
	}
	m_dibTmp.Copy(m_dib); //m_dibTmp -> m_dib
}

//近似色抽出
void Bitmap24::SelectColor(double dbLength,BYTE byR,BYTE byG,BYTE byB)
{
	int nSize = m_dib.GetSize();
	BYTE *pBits = m_dib.GetBits();
	for(int i=0;i<nSize;i+=3){
		double dbB = *pBits     - byB;
		double dbG = *(pBits+1) - byG;
		double dbR = *(pBits+2) - byR;
		double dbLen = sqrt(dbB*dbB+dbG*dbG+dbR*dbR);
		if( dbLen < dbLength){
			*pBits++ = 255;
			*pBits++ = 255;
			*pBits++ = 255;
		}else{
			*pBits++ = 0;
			*pBits++ = 0;
			*pBits++ = 0;			
		}
	}
}

//微分処理
void Bitmap24::Gradient(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //パディングサイズ //x==nW-1分も考慮
	if( !m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //先頭アドレス取得		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//オペレーター
	static const int fx[9]={
		0,0, 0,
		0,1,-1,
		0,0, 0,
	};
	static const int fy[9]={
		0, 0,0,
		0, 1,0,
		0,-1,0,
	};
	int d[9]; //計算結果
	double dbDx,dbDy;
	//８方向を検査してもっとも小さな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDx = dbDy = 0;
			d[0] = *(pSrc+nLine-3);	//左上
			d[1] = *(pSrc+nLine);	//上	
			d[2] = *(pSrc+nLine+3);	//右上	
			d[3] = *(pSrc-3);		//左
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//右
			d[6] = *(pSrc-nLine-3);	//左下
			d[7] = *(pSrc-nLine);	//下
			d[8] = *(pSrc-nLine+3);	//右下
			for(int i=0;i<9;i++){
				dbDx += d[i]*fx[i];
				dbDy += d[i]*fy[i];
			}
			*pDst++ = (BYTE)(dbAmp*sqrt(dbDx*dbDx+dbDy*dbDy));
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Srcを変更
}

//輪郭線抽出用微分
void Bitmap24::Sobel(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //パディングサイズ //x==nW-1分も考慮
	if( !m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //先頭アドレス取得		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//オペレーター
	static const int fx[9]={
		-1,0,1,
		-2,0,2,
		-1,0,1,
	};
	static const int fy[9]={
		-1,-2,-1,
		 0, 0, 0,
		 1, 2, 1,
	};

	int d[9]; //計算結果
	double dbDx,dbDy;
	//８方向を検査してもっとも小さな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDx = dbDy = 0;
			d[0] = *(pSrc+nLine-3);	//左上
			d[1] = *(pSrc+nLine);	//上	
			d[2] = *(pSrc+nLine+3);	//右上	
			d[3] = *(pSrc-3);		//左
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//右
			d[6] = *(pSrc-nLine-3);	//左下
			d[7] = *(pSrc-nLine);	//下
			d[8] = *(pSrc-nLine+3);	//右下
			for(int i=0;i<9;i++){
				dbDx += d[i]*fx[i]; //x方向変動値 ///////////////////////////////////////////////////////////////////////////
				dbDy += d[i]*fy[i]; //y方向変動値
			}
			*pDst++ = (BYTE)(dbAmp*sqrt(dbDx*dbDx+dbDy*dbDy));
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //微分画像転送

	//////半分だけ変更//////////////////////////////////////FOR DEBUG
	////int nHalf = nLine / 2;
	////for(int y=0;y<nH;y++){
	////	BYTE *pDst = m_dib.GetPBitsB(0,y);
	////	BYTE *pSrc = m_dibTmp.GetPBitsB(0,y);
	////	for(int i=0;i<nHalf;i++,pDst++,pSrc++){
	////		*pDst = MIN(MAXBYTE,*pDst+*pSrc);
	////	}
	////}
	////
	////合成
	//for(int y=0;y<nH;y++){
	//	BYTE *pDst = m_dib.GetPBitsB(0,y);
	//	BYTE *pSrc = m_dibTmp.GetPBitsB(0,y);
	//	for(int i=0;i<nLine;i++,pDst++,pSrc++){
	//		*pDst = MIN(MAXBYTE,*pDst+*pSrc);
	//	}
	//}
}

//ラプラシアン
void Bitmap24::Laplacian(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //パディングサイズ //x==nW-1分も考慮
	if( !m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //先頭アドレス取得		
	BYTE *pDst = m_dibTmp.GetBits(); 

	//オペレーター
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	int d[9]; //計算結果
	double dbDelta;
	//８方向を検査してもっとも小さな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			dbDelta = 0;
			d[0] = *(pSrc+nLine-3);	//左上
			d[1] = *(pSrc+nLine);	//上	
			d[2] = *(pSrc+nLine+3);	//右上	
			d[3] = *(pSrc-3);		//左
			d[4] = *pSrc;			//
			d[5] = *(pSrc+3);		//右
			d[6] = *(pSrc-nLine-3);	//左下
			d[7] = *(pSrc-nLine);	//下
			d[8] = *(pSrc-nLine+3);	//右下
			for(int i=0;i<9;i++){
				dbDelta += d[i]*c[i];
			}
			*pDst++ = (BYTE)abs(dbAmp*dbDelta);
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Srcを変更
}
//ガンマ補正
void Bitmap24::Gamma(double dbExp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nSize = m_dib.GetSize();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //パディングサイズ //x==nW-1分も考慮

	//最大値を求める
	double dbMax = pow(MAXBYTE,dbExp);
	double dbAlpha = MAXBYTE / dbMax ; //比率

	BYTE *pSrc = m_dib.GetBits();   //先頭アドレス取得		
	for(int i=0;i<nSize;i++){
		double dbTmp = pow( *pSrc,dbExp);
		*pSrc++ = (BYTE)(dbTmp * dbAlpha);
	}
}
//輪郭抽出
void Bitmap24::Prewitt(double dbAmp)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nLine = m_dib.GetLineSize();
	int nPad = nLine - nW*3 ; //パディングサイズ //x==nW-1分も考慮
	if( !m_dibTmp.Create(nW,nH) ){ //計算用共通バッファ取得
		return ;
	}
	BYTE *pSrc = m_dib.GetBits();   //先頭アドレス取得		
	m_dibTmp.PaintBlack();
	BYTE *pDst = m_dibTmp.GetBits();

	int d0,d1,d2,d3,d4,d5,d6,d7,d8; //計算結果1
	int m[9]; //計算結果2
	int nMax = 0;
	//８方向を検査してもっとも小さな値に変換します
	//y==0
	pSrc += nLine;
	pDst += nLine;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc+=3;
		pDst+=3;
		for(int x=3,xmax=3*(nW-1);x<xmax;x++){
			nMax = 0;
			d0 = *(pSrc+nLine-3);//左上
			d1 = *(pSrc+nLine);	 //上	
			d2 = *(pSrc+nLine+3);//右上	
			d3 = *(pSrc-3);		 //左
			d4 = *pSrc * 2;		 //
			d5 = *(pSrc+3);		 //右
			d6 = *(pSrc-nLine-3);//左下
			d7 = *(pSrc-nLine);	 //下
			d8 = *(pSrc-nLine+3);//右下
			m[0] =  d0 + d1 + d2 + d3 - d4 + d5 - d6 - d7 - d8;
			m[1] =  d0 + d1 + d2 + d3 - d4 - d5 + d6 - d7 - d8;
			m[2] =  d0 + d1 - d2 + d3 - d4 - d5 + d6 + d7 - d8;
			m[3] =  d0 - d1 - d2 + d3 - d4 - d5 + d6 + d7 + d8;
			m[4] = -d0 - d1 - d2 + d3 - d4 + d5 + d6 + d7 + d8;
			m[5] = -d0 - d1 + d2 - d3 - d4 + d5 + d6 + d7 + d8;
			m[6] = -d0 + d1 + d2 - d3 - d4 + d5 - d6 + d7 + d8;
			m[7] =  d0 + d1 + d2 - d3 - d4 + d5 - d6 - d7 + d8;
			//int nNo = 0;
			for(int i=0;i<8;i++){//最大値決定
				if( nMax < m[i] ){
					nMax = m[i];
				}
			}
			*pDst++ = (BYTE)dbAmp*nMax;
			pSrc++;
		}
		//x==nW-1
		pSrc+=3;
		pDst+=3;
		pSrc += nPad;
		pDst += nPad;
	}
	memcpy(m_dib.GetBits(),m_dibTmp.GetBits(),m_dib.GetSize()); //Srcを変更	
}

//指定割合の白値で区分するためのしきい値を取得
int Bitmap24::GetBinaryLevel(double dbPer)
{
	int nR[MAXBYTE+1];
	int nG[MAXBYTE+1];
	int nB[MAXBYTE+1];
	m_dib.CollectHistgram(nR,nG,nB);//ヒストグラム収集
	int nAll[MAXBYTE+1]; //全要素の画素数
	for(int i=0;i<=MAXBYTE;i++){
		nAll[i] = nR[i] + nG[i] + nB[i];
	}

	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nPixels = (int)(nW*nH*3*dbPer+0.5); //指定画素数
	
	for(int i=MAXBYTE;i>=0;i--){
		nPixels -= nAll[i];
		if( nPixels <= 0 ){
			return i;
		}
	}
	return 0;
}

//RGB要素最大値取得
int Bitmap24::GetMaxElement(int nRGB)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	BYTE byMax = 0;	
	for(int y=0;y<nH;y++){
		BYTE *pBits = m_dib.GetPBitsB(0,y);
		for(int x=0;x<nW;x++,pBits+=3){
			if( byMax < *(pBits+nRGB) ){//R or G or B
				byMax = *(pBits+nRGB);
			}
		}
	}
	return byMax;
}
//RGB要素最大値取得 領域指定
int Bitmap24::GetMaxElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nXmin = MAX(0,nXmax);
	nYmin = MAX(0,nYmin);
	nXmax = MIN(nXmax,nW-1);
	nYmax = MIN(nYmax,nH-1);
	BYTE byMax = 0;	
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pBits = m_dib.GetPBitsB(nXmin,y);
		for(int x=nXmin;x<=nXmax;x++,pBits+=3){
			if( byMax < *(pBits+nRGB) ){//R or G or B
				byMax = *(pBits+nRGB);
			}
		}
	}
	return byMax;
}

//RGB要素最小値取得
int Bitmap24::GetMinElement(int nRGB)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	BYTE byMin = MAXBYTE;
	for(int y=0;y<nH;y++){
		BYTE *pBits = m_dib.GetPBitsB(0,y);
		for(int x=0;x<nW;x++,pBits+=3){
			if( byMin > *(pBits+nRGB) ){//R or G or B
				byMin = *(pBits+nRGB);
			}
		}
	}
	return byMin;
}
//RGB要素最大値取得 領域指定
int Bitmap24::GetMinElement(int nRGB,int nXmin,int nYmin,int nXmax,int nYmax)
{
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nXmin = MAX(0,nXmax);
	nYmin = MAX(0,nYmin);
	nXmax = MIN(nXmax,nW-1);
	nYmax = MIN(nYmax,nH-1);
	BYTE byMin = MAXBYTE;
	for(int y=nYmin;y<=nYmax;y++){
		BYTE *pBits = m_dib.GetPBitsB(nXmin,y);
		for(int x=nXmin;x<=nXmax;x++,pBits+=3){
			if( byMin > *(pBits+nRGB) ){//R or G or B
				byMin = *(pBits+nRGB);
			}
		}
	}
	return byMin;
}

//ヒストグラム表示
void Bitmap24::ShowHistgram()
{	
	if( !::IsWindow(m_dlgHistgram.GetSafeHwnd()) ){//Window作成の必要あり
		m_dlgHistgram.Create(IDD_HISTGRAM);		
	}
	if( ::IsWindow(m_dlgHistgram.GetSafeHwnd()) ){//Window作成に成功
		//m_dib.CollectHistgram(m_dlgHistgram.m_nR,m_dlgHistgram.m_nG,m_dlgHistgram.m_nB);//ヒストグラム収集
		m_dib.CollectHistgramGray(m_dlgHistgram.m_nGray);//ヒストグラム収集
		m_dlgHistgram.Init();//初期化
		m_dlgHistgram.ShowWindow(SW_SHOW);
	}
}

//面積比からしきい値を計算
int Bitmap24::CalcThreshold(CString &strColor,int nPixels)
{
	int nAll = 0;
	int nR[256];
	int nG[256];
	int nB[256];
	m_dib.CollectHistgram(nR,nG,nB);//ヒストグラム収集
	if(strColor == "B" || strColor == "BLACK"){
		//指定画素数は黒
		for(int i=0;i<=MAXBYTE;i++){
			nAll += nR[i];
			nAll += nG[i];
			nAll += nB[i];
			if( nPixels < nAll ){ //指定画素数を超えたとき
				return i;
			}
		}
	}else if(strColor == "W" || strColor == "WHITE"){
		//指定画素数は白
		for(int i=MAXBYTE;i>=0;i--){
			nAll += nR[i];
			nAll += nG[i];
			nAll += nB[i];
			if( nPixels < nAll ){ //指定画素数を超えたとき
				return i;
			}
		}
	}
	return -1;
}

//自動しきい値計算
int Bitmap24::CalcThreshold()
{
	int nTh = 128;
	int nGray[256];
	m_dib.CollectHistgramGray(nGray);//ヒストグラム収集

	//128より下の最大画素数を求める
	int nMaxL = 0;
	int nIndexL = 0;
	for(int i=1;i<128;i++){
		if( nMaxL < nGray[i] ){ //最大値の差し替え			
			nMaxL = nGray[i];
			nIndexL = i;
		}
	}
	//128以上の最大画素数を求める
	int nMaxH = 128;
	int nIndexH = 255;
	for(int i=128;i<255;i++){
		if( nMaxH < nGray[i] ){ //最大値の差し替え			
			nMaxH = nGray[i];
			nIndexH = i;
		}		
	}
	//最大値の中心を取得
	return (int)( (nIndexH+nIndexL+0.5)/2.0 );
}

//イメージデバイス画像取得
void Bitmap24::FromImageDevice()
{
	ImageDevice *pImageDevice = ImageDevice::GetInstance();
	if( pImageDevice != NULL ){
		CDIB &dib = pImageDevice->GetDib();
		dib.Copy(m_dib);
	}
}
//比較
BOOL Bitmap24::ImageDeviceDiff()
{
	ImageDevice *pImageDevice = ImageDevice::GetInstance();
	if( pImageDevice != NULL ){
		CDIB &dib = pImageDevice->GetDib();
		if( GetDib().IsEqual( dib ) ){ //DIBに変化なし
			return FALSE;
		}
		return TRUE; //DIBに変化あり
	}
	return FALSE;
}
//イメージデバイスへ画像送信
void Bitmap24::ToImageDevice()
{
	ImageDevice* pImageDevice = ImageDevice::GetInstance();
	pImageDevice->ToBitmap(m_dib);
}

//指定範囲明るさ最小値取得
int Bitmap24::GetBrightnessMin(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMin = INT_MAX;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start位置
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色				
			if( by256 < nMin ){ //より小さい値に差し替え
				nMin = by256;
			}
		}
	}
	return nMin;
}
//指定範囲明るさ最大値取得
int Bitmap24::GetBrightnessMax(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMax = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start位置
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色				
			if( nMax < by256 ){ //より大きい値に差し替え
				nMax = by256;
			}
		}
	}
	return nMax;
}

//指定範囲明るさ幅取得
int Bitmap24::GetBrightnessRange(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	int nTop  = MAX(0,rect.top);
	int nBtm  = MIN(nH-1,rect.bottom);
	int nLeft = MAX(0,rect.left);
	int nRight= MIN(nW-1,rect.right);

	int nMin = MAXBYTE;
	int nMax = 0;
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pData = m_dib.GetPBitsB(nLeft,y); //Start位置
		for(int x=nLeft;x<=nRight;x++){
			BYTE byB = *pData++;//B
			BYTE byG = *pData++;//G
			BYTE byR = *pData++;//R
			BYTE by256 = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB); //フルカラー -> 256色				
			if( nMax < by256 ){ //より大きい値に差し替え
				nMax = by256;
			}
			if( by256 < nMin  ){
				nMin = by256;
			}
		}
	}
	return (nMax-nMin);
}

//指定オペレーターの計算
static int CalcOperator(const BYTE *pSrc,const int c[],int nLine)
{
	//Bのラプラシアン
	int nSum = *(pSrc+nLine-3) * c[0];//左上
	nSum += *(pSrc+nLine)   * c[1];	//上
	nSum += *(pSrc+nLine+3) * c[2];	//右上	
	nSum += *(pSrc-3)	    * c[3];	//左
	nSum += *pSrc		    * c[4];	//
	nSum += *(pSrc+3)	    * c[5];	//右
	nSum += *(pSrc-nLine-3) * c[6];	//左下
	nSum += *(pSrc-nLine)   * c[7];	//下
	nSum += *(pSrc-nLine+3) * c[8];	//右下
	return nSum;
}

//指定範囲明るさ微分値取得
int Bitmap24::GetBrightnessDiff(const CRect &rect)
{
	BYTE *pBits = m_dib.GetBits();
	int nLine = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();	

	//計算領域を取得
	int nTop  = MAX(1,rect.top);
	int nBtm  = MIN(nH-2,rect.bottom);
	int nLeft = MAX(1,rect.left);
	int nRight= MIN(nW-2,rect.right);

	//オペレーター
	static const int c[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	//指定範囲の微分値計算
	int nSumDiff = 0; //微分値合計
	for(int y=nTop;y<=nBtm;y++){
		BYTE *pSrc = m_dib.GetPBitsB(nLeft,y);//Start位置
		for(int x=nLeft;x<=nRight;x++){
			for(int i=0;i<3;i++){//RGB
				int nTmp = CalcOperator(pSrc++,c,nLine); //オペレーター計算
				nSumDiff += abs(nTmp);
			}
		}
	}
	return (int)(nSumDiff/8.0+0.5);
}

//平均フィルタ
void Bitmap24::AverageFilter()
{
	BYTE *pSrcTop = m_dib.GetBits(); //先頭アドレス
	int nLineSize = m_dib.GetLineSize();
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	m_dib.Copy(m_dibTmp); //Copy先作成
	BYTE *pDstTop = m_dibTmp.GetBits();

	for(int y=1,ymax=nH-1;y<ymax;y++){
		BYTE *pSrc = pSrcTop + y*nLineSize + 3;
		BYTE *pDst = pDstTop + y*nLineSize + 3;
		for(int x=1,xmax=nW-1;x<xmax;x++){
			for(int i=0;i<3;i++){ //BGR
				*pDst++ = Average(pSrc++,nLineSize);//B
			}
		}		
	}
	m_dibTmp.Copy(m_dib);
}

//平均値取得
BYTE Bitmap24::Average(BYTE *pByte,int nLineSize)
{
	int nVal = *pByte;
	nVal += *(pByte-nLineSize-3);
	nVal += *(pByte-nLineSize);
	nVal += *(pByte-nLineSize+3);
	nVal += *(pByte-3);
	nVal += *(pByte+3);
	nVal += *(pByte+nLineSize-3);
	nVal += *(pByte+nLineSize);
	nVal += *(pByte+nLineSize+3);
	return (BYTE)(nVal/9+0.5); //平均
}

//連想配列画像差分作成
void Bitmap24::MapDelta(double dbAmp)
{
	BYTE *pDst = m_dib.GetBits(); //書き込み先アドレス
	int nSize = m_dib.GetSize();
	m_dib.Copy(m_dibTmp); //m_dib -> m_dibTmp
	m_dib.PaintBlack(); //0 Clear
	for(POSITION pos = m_mapDibPtr.GetStartPosition(); pos != NULL;){
		int nNo=0;
		CDIB *pDIB = NULL; //ビットマップポインタ
		m_mapDibPtr.GetNextAssoc(pos, nNo, pDIB);
		if( pDIB != NULL ){
			BYTE *pSrc1 = m_dibTmp.GetBits(); //元データアドレス
			BYTE *pSrc2 = pDIB->GetBits();
			for(int i=0;i<nSize;i++){
				BYTE byDelta = abs(*pSrc1++ - *pSrc2++);
				*pDst++ = (BYTE)(dbAmp * 100 * byDelta / 255 );
			}			
		}
	}
}

//R成分消去
void Bitmap24::ClearR() 
{
	BYTE *pSrc = m_dib.GetBits(); //先頭アドレス
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*(pSrc+2) = 0; //Clear R
		pSrc += 3;
	}
}
//G成分消去
void Bitmap24::ClearG() 
{
	BYTE *pSrc = m_dib.GetBits(); //先頭アドレス
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*(pSrc+1) = 0; //Clear G
		pSrc += 3;
	}
}
//B成分消去
void Bitmap24::ClearB() 
{
	BYTE *pSrc = m_dib.GetBits(); //先頭アドレス
	int nSize = m_dib.GetSize();
	for(int i=0;i<nSize;i+=3){
		*pSrc = 0; //Clear B
		pSrc += 3;
	}
}

//static ImageDlg s_dlg;
//
////ダイアログ表示
//void Bitmap24::ShowDialog(int nCx,int nCy)
//{
//	CRect rect;
//	GetView()->GetWindowRect(&rect); //表示位置
//	rect.right  = rect.left + nCx;
//	rect.bottom = rect.top  + nCy;
//
//	if( s_dlg.GetSafeHwnd() == NULL ){
//		s_dlg.Create(ImageDlg::IDD,GetView());
//	}
//	s_dlg.SetBitmap(m_dib);
//	s_dlg.CalcWindowRect(&rect);
//	s_dlg.MoveWindow(rect);
//	s_dlg.ShowWindow(SW_SHOW);
//}
//
////ダイアログ非表示
//void Bitmap24::HideDialog()
//{
//	if( s_dlg.GetSafeHwnd() != NULL ){
//		s_dlg.ShowWindow(SW_HIDE);
//	}
//}

//上限と下限の選択
void Bitmap24::SelectLH(double dbLow,double dbHigh)
{
	//前準備
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	int nGray[256];
	m_dib256.Copy(m_dib);	//256色ビットマップの作成
	m_dib256.CollectHistgram( nGray );//ヒストグラム収集
	BYTE *pDib	= m_dib.GetBits(); //24bit color Top
	int nPadF = m_dib.GetLineSize() - nW*3;
	BYTE *pGray = m_dib256.GetBits(); //Gray Top
	int nPadG = m_dib256.GetLineSize() - nW; //パディングサイズ
	int nSize = m_dib256.GetSize();
	int nLowSize  = (int)(nSize * dbLow  / 100);
	int nHighSize = (int)(nSize * dbHigh / 100);
	//High	
	int nHighCount = 0;
	int i=255;
	for(;i>=0;i--){
		nHighCount += nGray[i];
		if( nHighCount >= nHighSize ){
			break;
		}
	}
	BYTE byVal = (BYTE)i;
	for(int y=0;y<nH;y++){		
		for(int x=0;x<nW;x++){			
			if( *pGray++ >= byVal ){
				*pDib++ = 255;//B
				*pDib++ = 255;//G
				*pDib++ = 255;//R
			}else{
				pDib+=3; //skip
			}
		}
		 //1Line終了
		pGray += nPadG;
		pDib  += nPadF;
	}

	pDib  = m_dib.GetBits(); //24bit color Top
	pGray = m_dib256.GetBits(); //Gray Top
	//LOW
	int nLowCount  = 0;
	for(i=0;i<256;i++){
		nLowCount += nGray[i];
		if( nLowCount >= nLowSize ){
			break;
		}
	}
	byVal = (BYTE)i;
	for(int y=0;y<nH;y++){		
		for(int x=0;x<nW;x++){			
			if( *pGray++ <= byVal ){
				*pDib++ = 0;//B
				*pDib++ = 0;//G
				*pDib++ = 0;//R
			}else{
				pDib+=3;
			}
		}
		 //1Line終了
		pGray += nPadG;
		pDib  += nPadF;
	}		
}
//ビットマップの引き算
BOOL Bitmap24::DeltaDib(CDIB *pDib1,CDIB *pDib2)
{
	if( pDib1 != NULL && pDib2 != NULL ){		
		BYTE *pBits1 = pDib1->GetBits();
		BYTE *pBits2 = pDib2->GetBits();
		int nSize1 = pDib1->GetSize();
		int nSize2 = pDib2->GetSize();
		if( nSize1 == nSize2 ){
			int nW = pDib1->GetWidth();
			int nH = pDib1->GetHeight();
			m_dib.Create(nW,nH);
			BYTE *pDst = m_dib.GetBits();
			for(int i=0;i<nSize1;i++){
				int nDelata = *pBits1++ - *pBits2++;
				*pDst++ = mid(0,nDelata,MAXBYTE);
			}
			return TRUE;
		}
	}
	return FALSE;
}

//ビットマップの割り算
BOOL Bitmap24::DivDib(CDIB *pDib1,CDIB *pDib2)
{
	if( pDib1 != NULL && pDib2 != NULL ){	
		BYTE *pBits1 = pDib1->GetBits();
		BYTE *pBits2 = pDib2->GetBits();
		int nSize1 = pDib1->GetSize();
		int nSize2 = pDib2->GetSize();
		if( nSize1 == nSize2 ){
			int nW = pDib1->GetWidth();
			int nH = pDib1->GetHeight();
			double dbValMax = 0;
			for(int i=0;i<nSize1;i++,pBits1++,*pBits2++){
				if(*pBits2 > 0){
					double dbVal = (double)*pBits1 / *pBits2;
					if( dbValMax < dbVal ){
						dbValMax = dbVal; //最大値記憶
					}
				}
			}
			if( dbValMax > 0 ){
				pBits1 = pDib1->GetBits();
				pBits2 = pDib2->GetBits();
				m_dib.Create(nW,nH);
				BYTE *pDst = m_dib.GetBits();
				for(int i=0;i<nSize1;i++,pBits1++,*pBits2++,pDst++ ){
					if(*pBits2 > 0){
						*pDst = (BYTE)((*pBits1 * MAXBYTE ) / (*pBits2 * dbValMax)); //Rangeを広げる
					}
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

//ライン描画
void Bitmap24::DrawLine(int nX1,int nY1,int nX2,int nY2,COLORREF color,int nWidth)
{
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CPen pen(PS_SOLID,nWidth,color);
	CPen *pPenOld = dcMem.SelectObject(&pen);
	CBitmap *pBmpOld = dcMem.SelectObject(m_dib.GetBitmap());
	dcMem.MoveTo(nX1,nY1);
	dcMem.LineTo(nX2,nY2);
	//後始末
	dcMem.SelectObject(pBmpOld);
	dcMem.SelectObject(pPenOld);
	pDC->DeleteDC();
}
//ビットマップライン描画
void Bitmap24::DrawBitmapLine()
{
	this->DrawLine((int)(m_ptS.X+0.5), (int)(m_ptS.Y+0.5), (int)(m_ptE.X+0.5), (int)(m_ptE.Y+0.5), GetLineColor(),1);
}

//ラベリングマスク
void Bitmap24::MaskBorder(int nEnum)
{
	static CPoint s_ptAry[10000]; 
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	Label &label = Labeling::GetInstance()->m_label; //ラベリングクラス取得
	Element *pElem = label.GetElement(nEnum);
	if( pElem == NULL ){
		return ;
	}
	ArrayPoint aryPoint; //境界線
	if( label.FindBorder(pElem,aryPoint) > 0 ){	//境界線検出
		int nCount = 0;
		for(int i=0,imax=(int)aryPoint.GetSize();i<imax;i++){
			if( aryPoint[i].x < 0 ){ //終了
				break;
			}
			s_ptAry[i] = aryPoint[i];
			nCount++;
		}
		CRgn rgn;
		rgn.CreatePolygonRgn(s_ptAry,nCount,ALTERNATE);
		CRgn rgnRect;
		rgnRect.CreateRectRgn(0,0,nW-1,nH-1);
		rgn.CombineRgn(&rgnRect,&rgn,RGN_DIFF); //Paint領域を選択

		//Clear描画
		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL); 
		CBitmap *pOldBmp = (CBitmap*)dcMem.SelectObject( m_dib.GetBitmap() );
		CBrush br; //ブラシ
		br.CreateSolidBrush( COLOR_BLACK );
		CBrush *pOldBrush = dcMem.SelectObject( &br );
		dcMem.FillRgn(&rgn,&br); //マスク塗りつぶし
		dcMem.SelectObject( pOldBmp );
		dcMem.SelectObject( pOldBrush);
	}
}

//欠陥検査 
void Bitmap24::DefectTest(int nTh,int nRange,int nMin)
{
	m_dib.Copy(m_dibTmp);//退避
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();

	//10以上を残しそれ以外はすべて0にする
	int nSize = m_dib.GetSize();
	BYTE *pBits = m_dib.GetBits();
	for(int i=0;i<nSize;i++,pBits++){
		if( *pBits < 10 ){
			*pBits = 0;
		}
	}

	if( !m_dib256.Create(nW,nH) ){ //計算用共通バッファ取得
		return ;
	}

	m_dib256.Copy(m_dib); //256階調変換
	BYTE *pSrc = m_dib256.GetBits();   //先頭アドレス取得		
	BYTE *pDst = m_dib.GetBits();
	int nLine256 = m_dib256.GetLineSize();//モノクロラインサイズ
	int nLine = m_dib.GetLineSize();//フルカラー1ラインサイズ
	int nLen = 1; //周囲１画素分だけ検査固定
	int nSide = nLen*2+1; //1辺の長さ
	int nPoints = nSide * nSide; //検査エリア画素
	
	int ary[36];//計算結果
	ZeroMemory(ary,sizeof(ary));

	BYTE byDelta;	
	//８方向を検査してもっとも小さな値に変換します
	//y==0	
	pSrc += nLine256;
	pDst += nLine ;
	for(int y=1,ymax=nH-1;y<ymax;y++){
		//x==0
		pSrc++;
		pDst+=3;
		for(int x=1,xmax=nW-1;x<nW-1;x++){
			byDelta = 0;
			BYTE *pData = pSrc-nLine256*nLen-nLen;	//左上端
			for(int i=0;i<nSide;i++){
				for(int j=0;j<nSide;j++){
					if( *pData == 0 ){
						ZeroMemory(ary,sizeof(ary));
						i=nSide;
						break;
					}else if(*pSrc < *pData ){
						ary[i+j*nSide] = 0;
					}else{
						int nDelta = *pSrc - *pData;//差分計算
						//ary[i+j*nSide] = (255-*pData) * 10 / *pSrc; 
						ary[i+j*nSide] = *pSrc - *pData + (255-*pData)*(255-*pData)/900; //差分計算
					}
					pData++;
				}
				pData -= nSide;//左端へ
				pData += nLine256;//次の行へ
			}
			for(int i=0;i<nPoints;i++){
				if(byDelta < ary[i]){ //最大値検出
					byDelta =  ary[i];
				}
				//byDelta += ary[i];
			}
			if( byDelta > 0 ){
				BYTE byVal = *pSrc + byDelta;
				*pDst++ = byVal;
				*pDst++ = byVal;
				*pDst++ = byVal;
			}else{
				*pDst++ = 0;
				*pDst++ = 0;
				*pDst++ = 0;
			}
			pSrc++;
		}
		//x==nW-1
		pSrc++;
		pDst+=3;
	}
	//2値化
	m_dib256.Copy(m_dib); //256階調変換
	m_dib.ChangeBinary(nTh); //コア作成

	//m_dib256.SaveFile(L"F:\\tmp\\test.bmp");

	for(int n=0;n<nRange;n++){ //しきい値幅ループ
		pSrc = m_dib256.GetBits();   //先頭アドレス取得		
		pDst = m_dib.GetBits();// 
		for(int y=0;y<nH;y++){
			for(int x=0;x<nW;x++){
				if( *pSrc > nTh && x>0 && y>0 && x<nW-1 && y<nH-1){//コアの確認
					if( *pDst || *(pDst-3) || *(pDst+3) 
					|| *(pDst-nLine-3) || *(pDst-nLine+3) || *(pDst-nLine)
					|| *(pDst+nLine-3) || *(pDst+nLine+3) || *(pDst+nLine) ){
						*pDst = 255;
						*(pDst+1) = 255;
						*(pDst+2) = 255;
					}					
				}
				pSrc++;
				pDst+=3;
			}
		}
		nTh--;
	}

	//ラベリング 赤
	Label &label = Labeling::GetInstance()->m_label; //ラベリングクラス取得
	label.Init(nW,nH);
	label.CreateSrcBuf(m_dib,MAXBYTE-1);
	int nEnum = label.Exec(nMin,9999);////////////////////////////////////////////////////////////傷最小画素数
	for(int i=0;i<nEnum;i++){
		Element *pE = label.GetElement(i);
		label.PaintElementColor(pE,m_dib,255,0,0);
	}
}

//境界線塗りつぶし
void Bitmap24::PaintBorder(int nEdge)
{
	Label &label = Labeling::GetInstance()->m_label; //ラベリングクラス取得

	//外円抽出
	//label.CreateSrcBuf(m_dibTmp,nTh);
	//label.Exec(1000,1000000);
	Element *pE = label.GetMaxElement();
	if( pE == NULL ){
		return ;
	}
	int nLineCx = nEdge; //ライン幅
	CRect rcEllipse = pE->GetRect(); //要素矩形領域
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL); 
	CBitmap *pOldBmp = (CBitmap*)dcMem.SelectObject( m_dib.GetBitmap() );
	CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);
	//CPen pen(PS_SOLID,nLineCx,COLOR_GREEN);
	CPen pen(PS_SOLID,nLineCx,COLOR_BLACK);
	CPen *pOldPen = dcMem.SelectObject(&pen);
	dcMem.Ellipse(rcEllipse); 	//緑円描画
	dcMem.SelectObject( pOldBmp );
	dcMem.SelectObject( pOldBrush);
	dcMem.SelectObject( pOldPen);
}

//水平方向エッジ検出
double Bitmap24::FindEdgeH(int nTh,int nX,int nY,int nDelta) 
{
	BYTE *pTop = m_dib.GetBits() ;
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	nX = MIN(nX, nW - 1);
	if( pTop != NULL && nY < nH ){
		for(int x=nX;0<=x && x<nW;x+=nDelta){
			int nVal = m_dib.GetBitsB(x,nY);
			if( nVal >= nTh ){//白検出
				int nXPrev = x-nDelta;
				int nVPrev = m_dib.GetBitsB(nXPrev,nY);
				double dbDv = nVal-nVPrev;
				double dbX = nXPrev + (nTh-nVPrev) / dbDv; //x座標取得
				return dbX;				
			}
		}
	}
	return -1;
}
//垂直方向エッジ検出
double Bitmap24::FindEdgeV(int nTh,int nX,int nY,int nDelta) 
{
	BYTE *pTop = m_dib.GetBits() ;
	int nW = m_dib.GetWidth();
	int nH = m_dib.GetHeight();
	if( pTop != NULL && nY < nH ){
		for(int y=nY;0<=y && y<nH;y+=nDelta){
			int nVal = m_dib.GetBitsB(nX,y);
			if( nVal >= nTh ){//黒検出
				int nYPrev = y-nDelta;
				int nVPrev = m_dib.GetBitsB(nX,nYPrev);
				double dbDv = nVal-nVPrev;
				double dbY = nYPrev + (nTh-nVPrev) / dbDv; //x座標取得
				return dbY;				
			}
		}
	}
	return -1;
}

//指定フォルダ内ビットマップ収集
void Bitmap24::CollectBitmap(CString &strPath)
{
	int nCnt = 0;
	CFileFind find;
	BOOL isOK = find.FindFile(strPath + _T("\\*.*"));
	while (isOK){
		isOK = find.FindNextFile();
		if (!find.IsDirectory()){ //File
			CString strFilePath = find.GetFilePath();
			if (m_dib.Load(strFilePath)){
				SetDib(nCnt);
				nCnt++;
			}
		}
	}
}

//指定番号にビットマップ記憶
BOOL Bitmap24::SetDib(int nNo)
{
	CDIB *pDib = m_mapDibPtr.GetDibPtr(nNo); //ビットマップクラス取得
	if(pDib != NULL){
		m_dib.Copy(*pDib); //m_dib -> pDib
		return TRUE;
	}
	return FALSE;
}

//方向
enum{
	 D_FROM_TOP = 0
	,D_FROM_BTM
	,D_FROM_LEFT
	,D_FROM_RIGHT
	,D_NUM
};

//Point取得
double Bitmap24::FindEdge(CRect &rect,int nDirection,int nColor)
{
	double dbRet = 0;
	double dbX = 0;
	double dbY = 0;
	double dbYmax = 0;	
	int nCnt = 0;
	switch(nDirection){
		case D_FROM_TOP:

			break;
		case D_FROM_BTM:
			for(int x=rect.left;x<=rect.right;x++){
				dbY = FindEdgeV(m_byTh,x,rect.top,1) ;
				if( dbYmax < dbY ){
					dbYmax = dbY;
				}
				nCnt++;
			}
			dbRet = dbYmax; //最大値取得
			break;
		case D_FROM_LEFT:
			for(int y=rect.top;y<=rect.bottom;y++){
				dbX += FindEdgeH(m_byTh,rect.left,y,1) ;
				nCnt++;
			}
			dbRet = dbX / nCnt;
			break;
		case D_FROM_RIGHT:
			for(int y=rect.top;y<=rect.bottom;y++){
				dbX += FindEdgeH(m_byTh,rect.right,y,-1) ;
				nCnt++;
			}
			dbRet = dbX / nCnt;
			break;
	}
	return dbRet;
}

//2値化マッチング
double Bitmap24::BinaryMatching(int nTh, double dbX, double dbY,int nMin,int nMax)
{
	double dbPerMax = 0;
	double dbPer = 0; //
	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256色作成
	Labeling *pLabel = Labeling::GetInstance();
	Label&label =  pLabel->m_label;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nSize = m_dib256.GetSize();
	int nBnum = (int)m_mapDibPtr.GetCount(); //最大数
	Dib256 dibDic; //辞書データ
	for (int i = 0; i < nBnum; i++){//ファイルの数だけループ
		CDIB *pDib = m_mapDibPtr.GetDibPtr(i); //ビットマップクラス取得			
		if (pDib->GetWidth()==nW && pDib->GetHeight() == nH){ //サイズチェック
			label.Init(nW, nH);
			label.CreateSrcBuf(*pDib, nTh);			
			if (label.Exec(nMin,nMax) > 0){
				BYTE *pCamera = m_dib256.GetBits(); //カメラデータ取得
				int nEmax = pLabel->GetMaxElement(); //最大要素番号
				double dbGx = pLabel->Gx(nEmax); //重心取得
				double dbGy = pLabel->Gy(nEmax);
				pDib->Copy(m_dibTmp); //データ退避
				pDib->Shift(m_dibTmp,dbX-dbGx, dbY-dbGy); //xy方向移動				
				dibDic.Copy(*pDib); //256色変換
				BYTE *pDic = dibDic.GetBits();//先頭アドレス
				int nSame = 0;
				int nDiff = 0;
				for (int n = 0; n < nSize; n++, pCamera++, pDic++){
					if (*pDic >= nTh){
						if (*pCamera >= nTh){ //
							nSame++;
						}
						else{
							nDiff++;
						}
					}
					else{//*pDic < nTh
						if (*pCamera >= nTh ){ //
							nDiff++;
						}
					}
				}
				dbPer = 100.0 * nSame / (nSame + nDiff);
				if (dbPerMax < dbPer){ //最大値記録
					dbPerMax = dbPer;
					m_dib.SetFilePath(pDib->GetFilePath()); //ファイルパス記録
				}
				m_dibTmp.Copy(*pDib); //元に戻す
			}
		}
	}
	return dbPerMax;
}
//水平方向最大距離取得
double GetWidthMax(BYTE byTh,CDIB &dib,int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMax = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//指定座標の値を取得
	double dbRight=nW-1, dbLeft=0, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int y = ys; y <=ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);			
			for (int x = nX; x < nW; x++,pG+=3) { //＋方向探索				
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMax < dbLen) { //最大値に変換
				dbLenMax = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	else { //白の時
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbRight = x - 1 + (byPrev- byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbLeft = x + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMax < dbLen) { //最大値に変換
				dbLenMax = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	return dbLenMax;
}
//垂直方向最大距離取得
double GetHeightMax(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbHMax = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize(); 
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	//指定座標の値を取得
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int x = xs; x <= xe; x++) { 
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++,pG-=nLine) {//+方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG+=nLine) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbHMax < dbLen) { //最大値に変換
				dbHMax = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	else { //白の時
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) {//+方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev- *pG;
					dbBtm = y - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev  - *pG;
					dbTop = y + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbHMax < dbLen) { //最大値に変換
				dbHMax = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	return dbHMax;
}

double GetWidthMin(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMin = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//指定座標の値を取得
	double dbRight, dbLeft, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			dbRight = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			dbLeft = 0;
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMin > dbLen) { //最小値に変換
				dbLenMin = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	else { //白の時
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbRight = x - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbLeft = x + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			if (dbLenMin > dbLen) { //最小値に変換
				dbLenMin = dbLen;
				ptS.X = (REAL)dbLeft;
				ptE.X = (REAL)dbRight;
				ptS.Y = ptE.Y = (REAL)y;
			}
		}
	}
	return dbLenMin;
}
double GetHeightMin(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenMin = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	//指定座標の値を取得
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; x < nH; x++, pG -= nLine) { //＋方向探索				
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbLenMin > dbLen) { //最小値に変換
				dbLenMin = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	else { //白の時
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; x < nH; x++, pG -= nLine) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbBtm = y - 1 + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y >= 0; y--, pG += nLine) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					dbTop = y + (byPrev - byTh) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			if (dbLenMin > dbLen) { //最小値に変換
				dbLenMin = dbLen;
				ptS.Y = (REAL)dbTop;
				ptE.Y = (REAL)dbBtm;
				ptS.X = ptE.X = (REAL)x;
			}
		}
	}
	return dbLenMin;
}
//平均値取得：幅
double GetWidthAvg(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenAvg = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	int nCnt = 0;
	//指定座標の値を取得
	double dbRight, dbLeft, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;

	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	else { //白の時
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = *pG - byPrev;
					dbRight = x - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;

			}
			pG = dib.GetPBitsG(nX, y);
			for (int x = nX; x >= 0; x--, pG -= 3) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = *pG - byPrev;
					dbLeft = x + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbRight - dbLeft;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	ptS.X = (REAL)dbLeft;
	ptE.X = (REAL)dbRight;
	ptS.Y = ptE.Y = (REAL)nY;
	return dbLenAvg;
}
//平均値取得：高さ
double GetHeightAvg(BYTE byTh, CDIB &dib, int nX, int nY, int nRange, PointF &ptS, PointF &ptE)
{
	double dbLenAvg = 0;
	BYTE *pBits = dib.GetBits();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nH - 1);
	int nCnt = 0;
	//指定座標の値を取得
	double dbTop, dbBtm, dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;

	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) { //＋方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG += nLine) { //-方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	else { //白の時
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG -= nLine) { //＋方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = *pG - byPrev;
					dbBtm = y - 1 + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;

			}
			pG = dib.GetPBitsG(x, nY);
			for (int y = nY; y < nH; y++, pG += nLine) { //-方向探索
				if (*pG < byTh) { //黒発見
					dbDeltaV = *pG - byPrev;
					dbTop = y + (byTh - byPrev) / dbDeltaV;
					break;
				}
				byPrev = *pG;
			}
			double dbLen = dbBtm - dbTop;
			dbLenAvg += dbLen;
			nCnt++;
		}
		dbLenAvg /= nCnt;
	}
	ptS.Y = (REAL)dbTop;
	ptE.Y = (REAL)dbBtm;
	ptS.X = ptE.X = (REAL)nX;
	return dbLenAvg;
}

//幅を計算
double Bitmap24::CalcWidth(int nX, int  nY, int  nRange, CString& strType)
{
	CDIB &dib = GetDib();

	//タイプ
	BYTE byTh = GetTh();
	double dbLength = 0;
	CString strTypeLow = strType.MakeLower();
	if (strTypeLow = L"max") {
		dbLength = GetWidthMax(byTh,dib,nX, nY, nRange, m_ptS, m_ptE);
	}
	else if (strTypeLow = L"min") {
		dbLength = GetWidthMin(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else { //strTypeLow = L"avg" 
		dbLength = GetWidthAvg(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	return dbLength;
}

//高さを計算
double Bitmap24::CalcHeight(int nX, int  nY, int  nRange, CString& strType)
{
	CDIB &dib = GetDib();

	//タイプ
	BYTE byTh = GetTh();
	double dbLength = 0;
	CString strTypeLow = strType.MakeLower();
	if (strTypeLow = L"max") {
		dbLength = GetHeightMax(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else if (strTypeLow = L"min") {
		dbLength = GetHeightMin(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	else { //strTypeLow = L"avg" 
		dbLength = GetHeightAvg(byTh, dib, nX, nY, nRange, m_ptS, m_ptE);
	}
	return dbLength;
}
//最小幅を計算
double Bitmap24::CalcWidthDirect(int nX, int  nY, int  nRange,PointF &ptL,PointF &ptR)
{
	BYTE byTh = GetTh();

	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256色ビットマップへ

	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int ys = MAX(0, nY - nRange);
	int ye = MIN(nY + nRange, nH - 1);
	//指定座標の値を取得
	double dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	CArray<PointF> aryPtRT; //右端上半分
	CArray<PointF> aryPtRB; //右端下半分
	
	double dbYC = (ys + ye) / 2.0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			ptR.X = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					ptR.X = (REAL)(x - 1 + (byTh - byPrev) / dbDeltaV);
					ptR.Y = (REAL)y;
					break;
				}
				byPrev = *pG;
			}
			if (ptR.X > 0) {
				if (y < dbYC) { //上半分
					aryPtRT.Add(ptR);
				}
				else {//下半分
					aryPtRB.Add(ptR);
				}
			}
		}
	}
	else { //白の時
		for (int y = ys; y <= ye; y++) {
			BYTE *pG = dib.GetPBitsG(nX, y);
			ptR.X = 0;
			for (int x = nX; x < nW; x++, pG += 3) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					ptR .X = (REAL)(x - 1 + (byPrev - byTh) / dbDeltaV);
					ptR.Y = (REAL)y;
					break;
				}
				byPrev = *pG;
			}
			if (ptR.X > 0) {
				if (y < dbYC) { //上半分
					aryPtRT.Add(ptR);
				}
				else {//下半分
					aryPtRB.Add(ptR);
				}
			}
		}
	}
	//
	PointF ptRTAvg; //重心
	for (int i = 0; i < aryPtRT.GetSize(); i++) {
		ptRTAvg = ptRTAvg + aryPtRT[i];
	}
	ptRTAvg.X /= aryPtRT.GetSize();
	ptRTAvg.Y /= aryPtRT.GetSize();

	PointF ptRBAvg; //重心
	for (int i = 0; i < aryPtRB.GetSize(); i++) {
		ptRBAvg = ptRBAvg + aryPtRB[i];
	}
	ptRBAvg.X /= aryPtRB.GetSize();
	ptRBAvg.Y /= aryPtRB.GetSize();

	int nX1 = (int)(ptRTAvg.X + 0.5);
	int nY1 = (int)(ptRTAvg.Y + 0.5);
	int nX2 = (int)(ptRBAvg.X + 0.5);
	int nY2 = (int)(ptRBAvg.Y + 0.5);
	this->DrawLine(nX1, nY1, nX2, nY2, COLOR_YELLOW, 3); //YELLOW LINE

	//探索ベクトルの始点
	PointF ptS((REAL)((ptRTAvg.X + ptRBAvg.X)*0.5), (REAL)((ptRTAvg.Y + ptRBAvg.Y)*0.5)); //中点
	PointF vecV(ptRBAvg.X - ptRTAvg.X, ptRBAvg.Y - ptRTAvg.Y);

	//単位ベクトルへ
	double dbSize = sqrt(vecV.X*vecV.X + vecV.Y * vecV.Y);
	vecV.X = (REAL)(vecV.X / dbSize);
	vecV.Y = (REAL)(vecV.Y / dbSize);

	PointF vecF = vecV; //探索ベクトル
	//90度回転 
	vecF.X = -vecV.Y;
	vecF.Y = vecV.X ;


	//探索
	double dbLenMin = 0;//最小値
	double dbPrev = 0; //以前の値
	PointF ptF = ptS + vecF; //探索座標
	if (byData < byTh) { //指定ポイントが黒のとき		
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (ptF.X > 0 &&  ptF.Y > 0 ){
			dbPrev = dbValue;
			ptF = ptF + vecF; //次の点へ
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
			if (byTh <= dbValue) { 
				break;
			}
		}
		//白発見
		double dbRate = abs( (byTh - dbValue) / (dbPrev - dbValue) ); //線形補間比率を求める
		ptF.X =(REAL)( ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y-(vecF.Y * dbRate));
	}
	else { //白のとき
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (dbValue  > byTh) { //白の間
			dbPrev = dbValue;
			ptF = ptF + vecF; //次の点へ
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		}
		//黒発見
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //線形補間比率を求める
		ptF.X = (REAL)(ptF.X-(vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y-(vecF.Y * dbRate));
	}
	ptR = ptS;
	ptL = ptF;
	double dbLength  = ::GetLength(ptF.X, ptF.Y, ptS.X, ptS.Y); //２点間距離	
	return dbLength;
}
double Bitmap24::CalcHeightDirect(int nX, int  nY, int  nRange, PointF &ptT, PointF &ptB)
{
	BYTE byTh = GetTh();
	CDIB &dib = GetDib();
	m_dib256.Copy(dib); //256色ビットマップへ

	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nLine = dib.GetLineSize();
	int xs = MAX(0, nX - nRange);
	int xe = MIN(nX + nRange, nW - 1);
	
//指定座標の値を取得
	double dbDeltaV;
	BYTE byData = dib.GetBitsG(nX, nY);
	BYTE byPrev = 0;
	CArray<PointF> aryPtBL; //下左半分
	CArray<PointF> aryPtBR; //下右半分
	double dbXC = (xs + xe) / 2.0;
	if (byData < byTh) { //指定ポイントが黒のとき		
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			ptB.Y = 0;
			for (int y = nY; y < nH; y++, pG -= nLine) { //＋方向探索
				if (*pG >= byTh) { //白発見
					dbDeltaV = *pG - byPrev;
					ptB.X = (REAL)x;
					ptB.Y = (REAL)(y - 1 + (byTh - byPrev) / dbDeltaV);
					break;
				}
				byPrev = *pG;
			}
			if (ptB.Y > 0) {
				if (x < dbXC) { //左半分
					aryPtBL.Add(ptB);
				}//右半分
				else {
					aryPtBR.Add(ptB);
				}
			}
		}
	}
	else { //白の時
		for (int x = xs; x <= xe; x++) {
			BYTE *pG = dib.GetPBitsG(x, nY);
			ptB.Y = 0;
			for (int y = nY; y < nH; y++, pG -= nLine) { //＋方向探索				
				if (*pG < byTh) { //黒発見
					dbDeltaV = byPrev - *pG;
					ptB.X = (REAL)x;
					ptB.Y = (REAL)(y - 1 + (byPrev - byTh) / dbDeltaV);
					break;
				}
				byPrev = *pG;
			}
			if (ptB.Y > 0) {
				if (x < dbXC) { //左半分
					aryPtBL.Add(ptB);
				}//右半分
				else {
					aryPtBR.Add(ptB);
				}
			}
		}
	}
	//
	PointF ptBLAvg; //重心
	for (int i = 0; i < aryPtBL.GetSize(); i++) {
		ptBLAvg = ptBLAvg + aryPtBL[i];
	}
	ptBLAvg.X /= aryPtBL.GetSize();
	ptBLAvg.Y /= aryPtBL.GetSize();

	PointF ptBRAvg; //重心
	for (int i = 0; i < aryPtBR.GetSize(); i++) {
		ptBRAvg = ptBRAvg + aryPtBR[i];
	}
	ptBRAvg.X /= aryPtBR.GetSize();
	ptBRAvg.Y /= aryPtBR.GetSize();

	int nX1 = (int)(ptBLAvg.X + 0.5);
	int nY1 = (int)(ptBLAvg.Y + 0.5);
	int nX2 = (int)(ptBRAvg.X + 0.5);
	int nY2 = (int)(ptBRAvg.Y + 0.5);
	this->DrawLine(nX1, nY1, nX2, nY2, COLOR_YELLOW, 3); //YELLOW LINE

														 //探索ベクトルの始点
	PointF ptS((REAL)((ptBLAvg.X + ptBRAvg.X)*0.5), (REAL)((ptBLAvg.Y + ptBRAvg.Y)*0.5)); //中点
	PointF vecV(ptBLAvg.X - ptBRAvg.X, ptBLAvg.Y - ptBRAvg.Y);

	//単位ベクトルへ
	double dbSize = sqrt(vecV.X*vecV.X + vecV.Y * vecV.Y);
	vecV.X = (REAL)(vecV.X / dbSize);
	vecV.Y = (REAL)(vecV.Y / dbSize);

	PointF vecF = vecV; //探索ベクトル
						//90度回転 
	vecF.X = -vecV.Y;
	vecF.Y = vecV.X;


	//探索
	double dbLenMin = 0;//最小値
	double dbPrev = 0; //以前の値
	PointF ptF = ptS + vecF; //探索座標
	if (byData < byTh) { //指定ポイントが黒のとき		
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (ptF.X > 0 && ptF.Y > 0) {
			dbPrev = dbValue;
			ptF = ptF + vecF; //次の点へ
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
			if (byTh <= dbValue) {
				break;
			}
		}
		//白発見
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //線形補間比率を求める
		ptF.X = (REAL)(ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y - (vecF.Y * dbRate));
	}
	else { //白のとき
		double dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		while (dbValue  > byTh) { //白の間
			dbPrev = dbValue;
			ptF = ptF + vecF; //次の点へ
			dbValue = m_dib256.GetAprxVal(ptF.X, ptF.Y);
		}
		//黒発見
		double dbRate = abs((byTh - dbValue) / (dbPrev - dbValue)); //線形補間比率を求める
		ptF.X = (REAL)(ptF.X - (vecF.X * dbRate));
		ptF.Y = (REAL)(ptF.Y - (vecF.Y * dbRate));
	}
	ptB = ptS;
	ptT = ptF;
	double dbLength = ::GetLength(ptF.X, ptF.Y, ptS.X, ptS.Y); //２点間距離	
	return dbLength;

}
