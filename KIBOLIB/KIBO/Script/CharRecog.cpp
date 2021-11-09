#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOView.h"
#include "App.h"
#include "CharRecog.h"
#include "../LIB/typedef.h"
#include "../LIB/FilePlus.h"
#include "../LIB/Object.h"
#include "../LIB/macro.h"
#include "../LIB/Gdi.h"

#ifdef USE_CHARRECOG 

#define LABEL_MIN		  30	//領域内文字数を判定するときの文字の最小画素数 Default
#define LABEL_MAX		90000	//領域内文字数を判定するときの文字の最大画素数 Default
#define LABEL_MIN_CHAR	   5	//1文字分の領域で検索する汚れの大きさ
#define ADJUST_SIZE		   5	//微調整開始サイズ
#define ADJUST_NUM		  10	//微調整回数

//領域ごとの文字サイズ取得
CSize CharRecog::GetCharPixels(int nRnum)
{
	CSize size(LABEL_MIN,LABEL_MAX); //Default設定
	m_mapCharPixels.Lookup(nRnum,size);
	return size;
}
//領域ごとの文字サイズ設定

void CharRecog::SetCharPixels(int nRnum,int nMin,int nMax)
{
	CSize size(nMin,nMax); 
	m_mapCharPixels.SetAt(nRnum,size);	
}

#define CHAR_SPACE_X	   3 //文字余白横方向
#define CHAR_SPACE_Y	   5 //文字余白縦方向

static TCHAR s_tcCheckRectName[] = L"検査領域";
static TCHAR s_tcError = '?'; //エラー文字

//コマンド一覧
static CommandTable s_table[] =
{
	 {CharRecog::FUNC_RECOGNIZE_CHAR	, 2,L"RecognizeChar"}		//印字検査
	,{CharRecog::FUNC_WHITE				, 2,L"White"}				//白文字検査
	,{CharRecog::FUNC_SETERASEDOT		, 2,L"SetEraseDot"}			//ドット抜け数削除数
	,{CharRecog::FUNC_DIRTY_CHECK		, 2,L"DirtyCheck"}			//汚れ検査
	,{CharRecog::FUNC_INCLINATION		, 2,L"Inclination"}			//傾斜
	,{CharRecog::FUNC_SET_COMMON		, 2,L"SetCommon"}			//検査領域ごとの文字種を分けるかどうか
	,{CharRecog::FUNC_SET_CHECK_RECT	, 6,L"SetCheckRect"}		//検査領域の設定
	,{CharRecog::FUNC_READ_SAMPLE		,-1,L"ReadSample"}			//サンプル画像読み込み
	,{CharRecog::FUNC_DRAWRESULT		, 3,L"DrawResult"}			//認識文字列の描画
	,{CharRecog::FUNC_SETCHARWIDTH		,-1,L"SetCharWidth"}		//文字幅指定
	,{CharRecog::FUNC_SETCHARINDEXWIDTH	,-1,L"SetCharIndexWidth"}	//固定文字幅指定
	,{CharRecog::FUNC_DEFAULT_CHARWIDTH	,-1,L"SetDefaultCharWidth"} //標準文字幅指定 
	,{CharRecog::FUNC_SET_CHAR_SIZE		, 3,L"SetCharSize"}			//領域内文字文字画素数設定
	,{CharRecog::FUNC_SET_AVENUM		,-1,L"SetAverageNum"}		//平均用撮影回数取得
	,{CharRecog::FUNC_SET_CONTRAST		, 2,L"SetContrast"}			//コントラストの設定
	,{CharRecog::FUNC_SET_NUMBER		, 2,L"SetNumber"}			//数値設定
	,{CharRecog::FUNC_SET_ALPHA			, 2,L"SetAlpha"}			//アルファベット設定
	,{CharRecog::FUNC_SET_INIFILE		, 1,L"SetIniFile"}			//IniFileは使用しますか
	,{CharRecog::FUNC_GET_CHECKSTRING	, 2,L"GetCheckString"}		//指定検査領域の文字列を取得
	,{CharRecog::FUNC_GET_MIN_WIDTH		, 2,L"GetMinWidth"}			//指定検査領域の文字幅最小値を取得
	,{CharRecog::FUNC_TEXTOUT_CHARWIDTH	, 1,L"TextOutCharWidth"}	//文字幅設定出力
	,{CharRecog::FUNC_SET_LENGTH_TH		, 2,L"SetLengthTh"}			//文字認識しきい値設定
	,{CharRecog::FUNC_BIN_CHECK_RECT	, 3,L"BinaryCheckRect"}		//検査領域ごとに2値化
	,{CharRecog::FUNC_MAX,0,NULL}									//コマンド数
};

static CharRecog *s_pInstance = NULL; //唯一のOPENクラス実体
//CharRecog実体取得
CharRecog* CharRecog::GetInstance() 
{
	return s_pInstance;
}

//コンストラクタ
CharRecog::CharRecog(void)
{
	s_pInstance = this;	//実体を設定
	m_nAnum = 1; //平均取得用撮影回数
	m_dwWaitTime = 100;//キャプチャフレーム間隔タイム msec
	m_byContrastMin = MIN_CONVERT; //コントラスト最小値設定
	m_byContrastMax = MAX_CONVERT; //コントラスト最大値設定

	m_isIniFile = true; //IniFileは必要ですか？
}

//デストラクタ
CharRecog::~CharRecog(void)
{
}

//関数名チェック
STDMETHODIMP CharRecog::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
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

//処理実行
STDMETHODIMP CharRecog::Invoke(DISPID dispIdMember	//Function ID 番号
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
		case FUNC_RECOGNIZE_CHAR: //文字認識
			{
				int nCnum = GetValientInt(&pDispParams->rgvarg[1]);//カメラ番号
				int nTh = GetValientInt(&pDispParams->rgvarg[0]);//2値化しきい値
				CString strRet = RecognizeChar(nCnum,nTh);
				if(IsPropertyGet(wFlags)){ //戻り値あり
					ReturnString(wFlags,pVarResult,strRet); //文字列を渡す
				}
				return S_OK;
			}
			break;
		case FUNC_WHITE:	//白文字検査
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//領域番号
				m_mapIsWhite[nRnum] = pDispParams->rgvarg[0].boolVal ? true : false;//白文字ですか？
				return S_OK;
			}
			break;
		case FUNC_SETERASEDOT: //ドット抜け数削除
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//領域番号
				int nEnum = GetValientInt(&pDispParams->rgvarg[0]);//ドット抜け削除数
				m_mapEraseDotNum[nRnum] = nEnum;//ドット抜け削除数設定
				return S_OK;
			}
			break;
		case FUNC_DIRTY_CHECK:	//汚れ検査
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//領域番号
				bool isCheck = pDispParams->rgvarg[0].boolVal ? true : false; //汚れ検査を行うかどうか
				m_mapIsDitrtyCheck[nRnum] = isCheck;
				return S_OK;
			}
			break;
		case FUNC_INCLINATION:	//傾斜
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//領域番号
				bool isInclination = pDispParams->rgvarg[0].boolVal ? true : false; //汚れ検査を行うかどうか
				m_mapIsInclination[nRnum] = isInclination;
				return S_OK;
			}
			break;
		case FUNC_SET_COMMON:	//検査領域ごとの文字種を分けるかどうか
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);//領域番号
				BOOL isCommon = pDispParams->rgvarg[0].boolVal; //共通文字
				m_mapCommonChar[nRnum] = isCommon;
				return S_OK;
			}
			break;
		case FUNC_SET_CHECK_RECT: //検査領域の設定
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[5]);	//領域番号
				int nCharNum = GetValientInt(&pDispParams->rgvarg[4]); //文字数
				CheckRect crect; //検査領域
				crect.m_nCharNum    = nCharNum;//文字数
				crect.m_rect.left   = GetValientInt(&pDispParams->rgvarg[3]); //左
				crect.m_rect.top    = GetValientInt(&pDispParams->rgvarg[2]); //上
				crect.m_rect.right  = GetValientInt(&pDispParams->rgvarg[1]); //右
				crect.m_rect.bottom = GetValientInt(&pDispParams->rgvarg[0]); //下
				m_mapCheckRect[nRnum] = crect; //検査領域セット
				m_mapCheckRectOld[nRnum] = crect.m_rect;
				//ラベリング検査領域描画
				CString strName; //描画オブジェクト名称
				strName.Format(L"%s%d",s_tcCheckRectName,nRnum);
				CKIBOView *pView = GetView();
				pView->m_mgrVO.CreateRectObject(strName,crect.m_rect,Color::Blue,false); //矩形描画
				return S_OK;
			}
			break;
		case FUNC_READ_SAMPLE: //サンプルデータ読み込み
			{
				int nTh = GetValientInt(&pDispParams->rgvarg[2]); //しきい値
				CString strFile = pDispParams->rgvarg[1].bstrVal;	//ファイル名取得
				CString strNumber = pDispParams->rgvarg[0].bstrVal;	//データ文字列取				
				CString strFileBin = strFile; //バイナリデータファイル
				strFileBin.Replace(L".bmp",L".bin"); //ファイル名作成
				strFileBin.Replace(L".png",L".bin"); //ファイル名作成

				CStringArray sAry; //文字列配列
				int nRmax = SplitToken(' ',sAry,strNumber); //半角空白区切り
				for(int nRnum = 0;nRnum<nRmax;nRnum++){ //検査領域の数だけループ
					CString strNum = sAry[nRnum]; //検査領域ごとの文字
					if( m_mapCheckRect[nRnum].m_nCharNum == 0 ){ //固定文字数の場合
						if( m_mapCheckRect[nRnum].m_str != strNum ){
							m_mapCheckRect[nRnum].m_str = strNum; //固定文字セット
						}
					}
				}
				CFileFind find;
				if(find.FindFile(strFileBin)){ //ファイルが存在する時だけ
					find.FindNextFile();
					if( find.GetLength() > 0 ){//ファイルサイズが存在するときだけ
						ReadElement(strFileBin,sAry); //文字特徴読み込み　& m_aryCharElementへ追加
						return S_OK;
					}
					return S_FALSE;
				}else{ //未だ記録されていない or バージョン変更			
					if( !ReadSample(nTh,strFile,strFileBin,strNumber) ){
						//ファイルが壊れているとき
						::DeleteFile(strFileBin); //壊れたファイルは削除する
						return S_FALSE;
					}
				}
				return S_OK;
			}
			break;
		case FUNC_DRAWRESULT://認識文字列の描画
			{
				CString strNumbers = pDispParams->rgvarg[2].bstrVal; //認識文字列
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //文字描画開始x座標
				int nHeight = GetValientInt(&pDispParams->rgvarg[0]); //文字の高さ
				BOOL isOK = DrawResult(strNumbers,nX,nHeight);
				ReturnBool(wFlags,pVarResult,isOK); //Script戻り値 
				return S_OK;
			}
			break;
		case FUNC_SETCHARWIDTH: //文字幅指定
			{
				CString strChar; //指定文字
				int nRnum   = 0; //領域番号
				double dbCx = 0; //幅x 
				double dbCy = 0; //幅y
				if(pDispParams->cArgs == 4){ //幅x,幅y指定
					strChar = pDispParams->rgvarg[3].bstrVal; //指定文字
					nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //領域番号
					dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //幅x 
					dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //幅y
				}else if(pDispParams->cArgs == 3){ //幅指定
					strChar = pDispParams->rgvarg[2].bstrVal; //指定文字
					nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //領域番号
					dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //幅
					dbCy = 0; //予約
				}
				if( strChar.GetLength() > 0 ){
					TCHAR tch = strChar[0];
					SetLineWidth(tch,nRnum,dbCx,dbCy);
					return S_OK;
				}
			}
			break;
		case FUNC_SETCHARINDEXWIDTH: //固定文字幅指定
			{
				int nIndex  = -1;
				int nRnum   = 0; //領域番号
				double dbCx = 0; //幅x 
				double dbCy = 0; //幅y
				if(pDispParams->cArgs == 4){ //幅x,幅y指定
					nIndex  = GetValientInt(&pDispParams->rgvarg[3]); //文字並び順index
					nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //領域番号
					dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //幅x 
					dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //幅y
				}else if(pDispParams->cArgs == 3){ //幅指定
					nIndex  = GetValientInt(&pDispParams->rgvarg[2]); //文字並び順index
					nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //領域番号
					dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //幅x 
				}
				if( 0 <= nIndex ){
					SetLineWidthIndex(nIndex,nRnum,dbCx,dbCy);
					return S_OK;					
				}
			}
			break;
		case FUNC_DEFAULT_CHARWIDTH: //標準文字幅指定
			if(pDispParams->cArgs == 3){ //幅x,幅y指定
				int nRnum   = GetValientInt(&pDispParams->rgvarg[2]); //領域番号
				double dbCx = GetValientDbl(&pDispParams->rgvarg[1]); //幅x 
				double dbCy = GetValientDbl(&pDispParams->rgvarg[0]); //幅y				
				SetLineWidth(DEFAULT_CHAR_NUM,nRnum,dbCx,dbCy); //DEFAULT_CHAR_NUM(0) is Default Number
				return S_OK;
			}else if(pDispParams->cArgs == 2){ //幅xのみ指定
				int nRnum   = GetValientInt(&pDispParams->rgvarg[1]); //領域番号
				double dbCx = GetValientDbl(&pDispParams->rgvarg[0]); //幅x 
				double dbCy = 0;
				SetLineWidth(DEFAULT_CHAR_NUM,nRnum,dbCx,dbCy); //DEFAULT_CHAR_NUM(0) is Default Number
				return S_OK;
			}
			break;

		case FUNC_SET_CHAR_SIZE://領域内文字文字画素数設定
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[2]);	//領域番号
				int nMin = GetValientInt(&pDispParams->rgvarg[1]);	//最小画素数
				int nMax = GetValientInt(&pDispParams->rgvarg[0]);	//最大画素数
				SetCharPixels(nRnum,nMin,nMax);
				return S_OK;
			}
			break;
		case FUNC_SET_AVENUM://平均用撮影回数取得
			{
				if( pDispParams->cArgs == 1 ){
					int nAnum = GetValientInt(&pDispParams->rgvarg[0]);
					SetAverageNum(nAnum);//回数のみ設定
					return S_OK;
				}else if(pDispParams->cArgs == 2 ){
					int nAnum = GetValientInt(&pDispParams->rgvarg[1]);
					int nTime = GetValientInt(&pDispParams->rgvarg[0]);
					SetAverage(nAnum,nTime);//回数と撮影間隔設定
					return S_OK;
				}				
			}
			break;
		case FUNC_SET_CONTRAST: //コントラストの設定
			{
				BYTE byMin = (BYTE)pDispParams->rgvarg[1].ullVal; //最小値
				BYTE byMax = (BYTE)pDispParams->rgvarg[0].ullVal; //最大値
				SetContrast(byMin,byMax); //コントラストのレンジ設定
				return S_OK;
			}
			break;
		case FUNC_SET_NUMBER:	//数値設定
			{
				int nRnum = (int)pDispParams->rgvarg[1].ullVal; //領域番号
				int nCnum = (int)pDispParams->rgvarg[0].ullVal; //文字番号
				SetNumber(nRnum,nCnum);
				return S_OK;
			}
			break;
		case FUNC_SET_ALPHA:	//アルファベット設定
			{
				int nRnum = (int)pDispParams->rgvarg[1].ullVal; //領域番号
				int nCnum = (int)pDispParams->rgvarg[0].ullVal; //文字番号
				SetAlpha(nRnum,nCnum);
				return S_OK;
			}
			break;
		case FUNC_SET_INIFILE:	//IniFileは使用しますか
			m_isIniFile = pDispParams->rgvarg[0].boolVal ? true : false;
			return S_OK;
			break;
		case FUNC_GET_CHECKSTRING:	//指定検査領域の文字列を取得
			{
				CString strText = pDispParams->rgvarg[1].bstrVal;	//データ文字列取得
				int nRnum = (int)pDispParams->rgvarg[0].ullVal; //領域番号
				CStringArray sAry;
				if( ::SplitToken(' ',sAry,strText) > nRnum ){
					strText = sAry[nRnum]; //領域内文字列取得
				}
				ReturnString(wFlags,pVarResult,strText); //文字列を渡す
				return S_OK;
			}
			break;
		case FUNC_GET_MIN_WIDTH://指定検査領域の文字幅最小値を取得
			{
				CString strChar = pDispParams->rgvarg[1].bstrVal; //指定文字
				int nRnum   = GetValientInt(&pDispParams->rgvarg[0]); //領域番号
				if( strChar.GetLength() > 0 ){
					TCHAR tch = strChar[0];//文字取得
					double dbVal = GetMinWidth(tch,nRnum);	//指定文字幅取得
					ReturnDbl(wFlags,pVarResult,dbVal);	
					return S_OK;
				}				
			}
			break;
		case FUNC_TEXTOUT_CHARWIDTH: //文字幅設定出力
			{
				int nRnum  = GetValientInt(&pDispParams->rgvarg[0]); //領域番号
				TextOutMinWidth(nRnum); //検査領域内の存在する文字すべての最小線幅を出力
			}
			return S_OK;
		case FUNC_SET_LENGTH_TH://文字認識しきい値設定
			{
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);	//領域番号
				double dbLen = GetValientDbl(&pDispParams->rgvarg[0]); //距離
				m_mapLenTh[nRnum] = dbLen; //文字認識距離取得
			}
			return S_OK;
		case FUNC_BIN_CHECK_RECT: //検査領域ごとに2値化
			{
				int nCnum = GetValientInt(&pDispParams->rgvarg[2]);	//カメラ番号
				int nRnum = GetValientInt(&pDispParams->rgvarg[1]);	//領域番号
				int nTh   = GetValientInt(&pDispParams->rgvarg[0]);	//しきい値
				BinaryCheckRect(nCnum,nRnum,nTh);
			}
			break;
	}
	return S_FALSE;
}

//認識結果描画
BOOL CharRecog::DrawResult(CString& strNumbers,int nX,int nHeight)
{
	CKIBOView *pView = GetView();
	pView->m_mgrVO.RemoveByName(L"STRING"); //全描画オブジェクトを削除
	CStringArray strAry;	//認識文字列
	CStringArray strAryR;	//エラー文字列
	int nCnt = CollectRecognizeChar(strNumbers,strAry,strAryR); //認識文字列取得
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(LOGFONT));
	lf.lfHeight = nHeight;
	lf.lfCharSet = DEFAULT_CHARSET;
	wcscpy_s(lf.lfFaceName,_T("ＭＳ 明朝"));
	GetView()->m_mgrVO.CreateStringObject(_T("STRINGL"),nX, 0,_T("認識文字列:"),COLOR_WHITE,lf);
	BOOL isOK = TRUE;
	CString strName;
	for(int i=1;i<=nCnt;i++){
		strName.Format(_T("STRING%d"),i); //Green文字列Name
		isOK &= CreateStringResult(strName,nX,i*nHeight,strAry[i-1],COLOR_GREEN,lf); //文字描画結果表示
		strName.Format(_T("STRINR%d"),i); //Red文字列Name
		pView->m_mgrVO.CreateStringObject(strName,nX,i*nHeight,strAryR[i-1],COLOR_RED,lf);
	}
	for(int i=0;i<nCnt;i++){
		CString strRed = strAryR[i];
		for(int i=0;i<strRed.GetLength();i++){
			TCHAR tc = strRed[i];
			if( tc != ' ' && tc != ',' ){ //エラー文字が存在するかどうか
				isOK = FALSE; //NGラベル
			}
		}
	}
	//判定結果表示
	App *pApp = App::GetInstance();	

    COLORREF crResult = pApp->GetProfileInt(L"RESULT.OK.COLOR"); //OK文字色
	lf.lfHeight = pApp->GetProfileInt(L"RESULT.SIZE"); //文字サイズ指定
	strName.Format(_T("STRING%d"),nCnt+1); //結果文字列Name
	if( strNumbers.Find(_T("?")) < 0 && isOK){
		pView->m_mgrVO.CreateStringObject(strName,nX,(nCnt+1)*nHeight,pApp->GetProfileString(L"RESULT.OK"),crResult,lf);
		return TRUE;
	}
	crResult = pApp->GetProfileInt(L"RESULT.NG.COLOR"); //NG文字色    
	pView->m_mgrVO.CreateStringObject(strName,nX,(nCnt+1)*nHeight,pApp->GetProfileString(L"RESULT.NG"),crResult,lf);
	return FALSE;
}

//認識文字列の取得
int CharRecog::CollectRecognizeChar(CString strNumber,CStringArray &strAry,CStringArray &strAryR)
{
	CString strRed = GetRight(strNumber,L","); //赤文字部分を取得
	strNumber = GetLeft(strNumber,L","); //表示文字部分のみを取得
	int nCnt  = ::SplitToken(' ',strAry,strNumber);//空白文字で分割
	int nCntR = ::SplitToken(',',strAryR,strRed);//カンマ区切り
	return min(nCnt,nCntR);
}

//線幅の指定
void CharRecog::SetLineWidth(TCHAR tChar,int nRnum,double dbCx,double dbCy)
{
	SizeDbl sizeDbl(dbCx,dbCy);
	LONG lKey = MAKELONG(tChar,nRnum); //キーワード作成
	m_mapAryLineWidth.SetAt(lKey,sizeDbl);
}
//固定文字線幅の指定
void CharRecog::SetLineWidthIndex(int nIndex,int nRnum,double dbCx,double dbCy)
{
	SizeDbl sizeDbl(dbCx,dbCy);
	LONG lKey = MAKELONG(nIndex,nRnum); //キーワード作成
	m_mapAryIndexWidth.SetAt(lKey,sizeDbl);
}

//最小線幅登録
void CharRecog::SetMinWidth(TCHAR tChar,int nRnum,double dbWmin)
{
	LONG lKey = MAKELONG(tChar,nRnum); //キーワード作成
	double dbWidth;
	if( m_mapAryMinWidth.Lookup(lKey,dbWidth) ){ //すでに登録されている場合
		if(dbWmin < dbWidth){//より小さい値ならば
			m_mapAryMinWidth[lKey] = dbWmin;	//最小文字幅結果map情報
		}
	}else{//無条件で登録
		m_mapAryMinWidth[lKey] = dbWmin;	//最小文字幅結果map情報
	}
}

//最小線幅取得
double CharRecog::GetMinWidth(TCHAR tChar,int nRnum)
{
	LONG lKey = MAKELONG(tChar,nRnum); //キーワード作成
	double dbWidth = 0;
	if( m_mapAryMinWidth.Lookup(lKey,dbWidth) ){ //登録されている場合
		return dbWidth;
	}
	return 0;
}

//検査領域内の存在する文字すべての最小線幅を出力
void CharRecog::TextOutMinWidth(int nRnum)
{
	COutputView *pOutput = GetOutputView();
	CString strText; //出力メッセージ
	POSITION pos = m_mapAryMinWidth.GetStartPosition();
	LONG lKey=0;		//Key
	double dbVal=0;		//Value
	while (pos != NULL){
		m_mapAryMinWidth.GetNextAssoc(pos, lKey, dbVal);
		TCHAR tch = LOWORD(lKey); //文字		
		if( nRnum == HIWORD(lKey) ){ //検査領域が一致
			strText.Format(L"CharRecog.SetCharWidth('%c',%d,%.4f);",tch,nRnum,dbVal);
			pOutput->Output(strText,COLOR_WHITE,FALSE);
		}
	}    
}

//総合的な文字幅の取得
SizeDbl CharRecog::GetLineWidthAll(int nIndex,TCHAR tChar,int nRnum)
{	
	SizeDbl sizeDbl;
	//index指定が存在する
	LONG lKey = MAKELONG(nIndex,nRnum); //キーワード作成
	if( m_mapAryIndexWidth.Lookup(lKey,sizeDbl) ){//指定文字の線幅取得
		return sizeDbl;
	}
	//文字ごとの指定が存在する
	lKey = MAKELONG(tChar,nRnum); //キーワード作成
	if( m_mapAryLineWidth.Lookup(lKey,sizeDbl) ){//指定文字の線幅取得
		return sizeDbl;
	}
	//Default設定を使用
	return GetDefLineWidth(nRnum);
}

//指定文字を数値のみの設定に変更
void CharRecog::SetNumber(int nRnum,int nCnum)
{
	LONG lKey = MAKELONG(nRnum,nCnum); //キーワード作成
	m_mapIsNumber.SetAt(lKey,TRUE);
}
//指定文字は数値ですか
BOOL CharRecog::IsNumber(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //キーワード作成

	BOOL isNumber;
	if( m_mapIsNumber.Lookup(lKey,isNumber) ){
		return isNumber; //数値かどうか
	}
	return FALSE;//数値だけとは限らない
}
//指定文字をアルファベットのみの設定に変更
void CharRecog::SetAlpha(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //キーワード作成
	m_mapIsAlpha.SetAt(lKey,TRUE);
}
//指定文字はアルファベットですか
BOOL CharRecog::IsAlpha(int nRnum,int nCnum) 
{
	LONG lKey = MAKELONG(nRnum,nCnum); //キーワード作成

	BOOL isAlpha;
	if( m_mapIsAlpha.Lookup(lKey,isAlpha) ){
		return isAlpha; //アルファベットかどうか
	}
	return FALSE;//アルファベットだけとは限らない
}

//標準線幅の取得
SizeDbl CharRecog::GetDefLineWidth(int nRnum)
{
	//標準番号の取得
	SizeDbl sizeDbl;
	LONG lKey = MAKELONG(DEFAULT_CHAR_NUM,nRnum); //キーワード作成
	m_mapAryLineWidth.Lookup(lKey,sizeDbl); 
	return sizeDbl;
}

//文字描画結果表示
BOOL CharRecog::CreateStringResult(CString strName,int x,int y,CString strString,COLORREF color,LOGFONT &lf)
{	
	BOOL isOK = TRUE;
	if( strString.IsEmpty()){
		strString = _T("文字数エラー");	
		color = COLOR_RED;
		isOK = FALSE;
	}		
	GetView()->m_mgrVO.CreateStringObject(strName,x,y,strString,color,lf);
	return isOK;
}

//検査領域枠移動
void CharRecog::MoveRectObject(int nRnum,CRect rect)
{
	CKIBOView *pView = GetView();
	CString strName; //描画オブジェクト名称
	strName.Format(L"%s%d",s_tcCheckRectName,nRnum);
	RectObject *pRO = (RectObject *)pView->m_mgrVO.GetVO(strName); //検査領域表示オブジェクト取得
	if(pRO != NULL ){
		m_mapCheckRect[nRnum].m_rect = rect; //検査領域変更
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();
		rect.left   = (int)(rect.left   * dbRateCx);
		rect.top    = (int)(rect.top    * dbRateCy);
		rect.right  = (int)(rect.right  * dbRateCx);
		rect.bottom = (int)(rect.bottom * dbRateCy);
		pRO->SetRect(rect);
	}
}

//文字が存在する領域を収集する
int CharRecog::CollectCharRect(int nRnum,int nTh,CArray<CRect> &aryRect,CDIB &dibSrc) 
{
	aryRect.RemoveAll();
	
	m_dibGray.Copy(dibSrc);//Gray Image作成	
	//BYTE byCmin = GetContrastMin();//コントラスト最小値取得
	//BYTE byCmax = GetContrastMax();//コントラスト最大値取得
	//m_dibGray.MinMax(byCmin,byCmax);//コントラスト最大化

	//ラベリング
	CKIBOView *pView = GetView();
	Label &label = pView->GetLabel(); //ラベリングクラス取得
	label.Init(m_dibGray.GetWidth(),m_dibGray.GetHeight());//ラベリング初期化
	if( !label.CreateSrcBuf(m_dibGray,nTh) ){//ラベリングバッファ（2値化画像作成)
		::ErrorMsg(_T("画像の取得が出来ませんでした。カメラとの接続を確認してください。"));
		return 0;
	}
	if( !IsWhite(nRnum) ){ //白文字でない場合のみ
		label.InvertSrcBuf();//白黒変換	
	}
	
	//ラベリング要素条件
	CSize sizeChar = GetCharPixels(nRnum);
	CRect rcCheck = GetCheckRectOld(nRnum); //初期設定時の領域に戻す
	if(nRnum > 0){ //以前のRECTとの交差チェック
		CRect rcPrev = GetCheckRect(nRnum-1); //以前の領域
		CRect rectInter;
		if( rectInter.IntersectRect(rcCheck,rcPrev) ){ //交差する
			int nH = rcCheck.Height(); //高さ取得
			rcCheck.top = rcPrev.bottom + 1;
			rcCheck.bottom = rcCheck.top + nH;
		}
	}
	if( label.Exec(sizeChar.cx,sizeChar.cy,rcCheck) == 0 ){ //ラベリング実行
		return 0;
	}
	//検査領域ずれ調整	微調整 
	for(int i=0; i<ADJUST_NUM; i++){//調整回数
		Element *pElemMax = label.GetMaxElement(); //最大の要素取得
		if( pElemMax != NULL ){
			int nTopMax = pElemMax->GetMinY(); //上
			int nBtmMax = pElemMax->GetMaxY(); //下
			if( abs(nTopMax-rcCheck.top) < ADJUST_SIZE ){ //上が欠けている
				//上に移動
				int nDelta = -ADJUST_SIZE;
				rcCheck.top		+= nDelta;
				rcCheck.bottom	+= nDelta;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //ラベリング再実行
				MoveRectObject(nRnum,rcCheck); //検査領域枠移動
			}else if( abs(nBtmMax-rcCheck.bottom) < ADJUST_SIZE ){//下が欠けている
				//下に移動
				int nDelta = ADJUST_SIZE;
				rcCheck.top		+= nDelta;
				rcCheck.bottom	+= nDelta;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //ラベリング再実行
				MoveRectObject(nRnum,rcCheck); //検査領域枠移動
			}else{ //最大要素が枠の中
				//センタリング
				int nHeight = rcCheck.Height();
				int nDelta = (rcCheck.Height() - (nBtmMax-nTopMax))/2;
				rcCheck.top		= nTopMax - nDelta;
				rcCheck.bottom	= rcCheck.top + nHeight;
				label.Exec(sizeChar.cx,sizeChar.cy,rcCheck); //ラベリング再実行
				MoveRectObject(nRnum,rcCheck); //検査領域枠移動			
				break;
			}
		}
	}
	label.Sort(Label::SORT_MIN_X,true); //x座標の昇順にソート
	int nEmax = label.GetEmax();//ラベリング要素数取得

	//ラベリング要素領域描画
	for(int i=0;i<nEmax;i++){
		Element *pElem = label.GetElement(i);
		aryRect.Add(pElem->GetRect()); //文字領域を収集
	}
	//Error文字チェック
	//ラベリング要素の黒をすべて白へ
	

	return nEmax;
}

//1文字分のラベリング実行
Element* CharRecog::LabelCharElement(int nRnum,CPoint &ptTL,Label &label,int nTh,Dib256 &dibGray,ArrayPoint &aryPoint)
{
	//ダブルラベリング
	label.Init(dibGray.GetWidth(),dibGray.GetHeight());//ラベリング初期化
	if( !label.CreateSrcBuf(dibGray,nTh) ){//ラベリングバッファ（2値化画像作成)		
		return NULL;
	}
	if( !IsWhite(nRnum) ){ //白文字でない場合のみ
		label.InvertSrcBuf();//白黒変換
	}
	if( IsEraseDot(nRnum) ){ //1ドット抜けを許すかどうか
		label.EraseDot(); //1ドット抜け削除			
	}
	int nEnum = 0;
	CSize sizeChar = GetCharPixels(nRnum);
	int nEmax = label.Exec(LABEL_MIN_CHAR,sizeChar.cy);
	label.Sort(Label::SORT_PIXELS,FALSE); //0番目が最大要素になるように
	if( nEmax == 1 ){
		label.FindBorder(nEnum,aryPoint); //
		return label.GetElement(nEnum); //１文字分のラベリング要素取得
	}else if(!IsDirtyCheck(nRnum) && label.GetMaxElement(&nEnum) != NULL){ //汚れ検査なし and 最大要素が存在する
		label.FindBorder(nEnum,aryPoint); //
		return label.GetElement(nEnum); //１文字分のラベリング要素取得
	}
	//汚れアリ
	//CKIBOView *pView = GetView();
	//CString strName;
	//for(int i=0;i<nEmax;i++){
	//	Element *pElem = label.GetElement(i);
	//	strName.Format(L"ErrorElement%d",i);
	//	CRect rect = pElem->GetRect();
	//	rect += ptTL;
	//	pView->m_mgrVO.CreateRectObject(strName,rect,Color::Red,false); //矩形描画
	//}
	aryPoint.RemoveAll(); //Borderなし
	return NULL;
}

//文字特徴から文字種予想
double CharRecog::GetPredictElement(CharElement &celem,WORD wRnum,WORD wCnum)
{
	TCHAR tchar = 0; //16bit文字
	int nIndex = 0;//最短距離文字のindex
	double dbMin = DBL_MAX; //最小距離
	for(int i=0;i<m_aryCharElement.GetSize();i++){ //全サンプルデータ検索
		CharElement& elem = m_aryCharElement[i];
		if( elem.m_wRnum == wRnum || (IsCommon(elem.m_wRnum) && IsCommon(wRnum)) ){ //文字領域検査チェック 領域ごとに文字種類が異なる場合
			if( iswalpha(elem.m_tChar) ){ //アルファベットです。
				if( IsNumber(wRnum,wCnum) ){ //アルファベットなのに数値指定がある
					continue;
				}
			}else{ //数値です。
				if( IsAlpha(wRnum,wCnum) ){ //数値なのにアルファベット指定がある
					continue;
				}
			}
			double dbDelta = CalcDelta(elem,celem); //特徴量間の距離を取得
			if( dbDelta < dbMin ){
				dbMin = dbDelta; //最小距離の入れ替え
				tchar = elem.m_tChar; //予想文字
				nIndex = i;
			}
		}
	}
	celem.m_tChar = tchar;	//予想文字

/////////////////////////////FOR DEBUG//////////////////////////////
#ifdef CALC_DELTA_CHECK
	CharElement& elem = m_aryCharElement[nIndex];
	CalcDelta(elem,celem,TRUE);
#endif
/////////////////////////////FOR DEBUG//////////////////////////////

	return dbMin; //長さ
}

double AreaMax(double d1,double d2,double d3,double d4)
{
	return max(max(d1,d2), max(d3,d4));
}
double AreaMin(double d1,double d2,double d3,double d4)
{
	return min(min(d1,d2), min(d3,d4));
}

//面積特徴値を計算する
static double CalcAreaDelta(double dbA1,double dbA2)
{
	double dbDelta = dbA1 - dbA2;
	double dbVal = 4000*(dbDelta)/dbA2;	//面積All 学習データ側のサイズで正規化
	return min(dbDelta*4,dbVal); //面積値差分 or 比率
}

//特徴量間の距離を取得    学習データ,       表示文字の特徴, 
double CharRecog::CalcDelta(CharElement &elem,CharElement &celem,BOOL isDebug)
{
	if( celem.m_nAreaA <= 0 ){
		return SHRT_MAX; //最大
	}
	double d[23]; //delta
	ZeroMemory(d,sizeof(d)); //0clear
	d[ 0] = CalcAreaDelta(celem.m_nAreaA,elem.m_nAreaA);//面積All 学習データ側のサイズで正規化
	d[ 1] = CalcAreaDelta(celem.m_dbAreaT,elem.m_dbAreaT);//面積上
	d[ 2] = CalcAreaDelta(celem.m_dbAreaB,elem.m_dbAreaB);//面積下
	d[ 3] = CalcAreaDelta(celem.m_dbAreaL,elem.m_dbAreaL);//面積左
	d[ 4] = CalcAreaDelta(celem.m_dbAreaR,elem.m_dbAreaR);//面積右
	d[ 5] = 100*(celem.m_dbGx-elem.m_dbGx);	//重心x
	d[ 6] = 100*(celem.m_dbGy-elem.m_dbGy);	//重心y	
	d[ 7] = ((double)celem.m_wContourNum-elem.m_wContourNum)*200;//境界線の数
	d[ 8] = 40*100.0*(celem.m_dbContourOut - elem.m_dbContourOut)/elem.m_nAreaA;//境界線の合計長さ
	d[ 9] = 40*100.0*(celem.m_dbContourIn  - elem.m_dbContourIn )/elem.m_nAreaA;//内側の境界線
	d[10] = (celem.m_dbCircle - elem.m_dbCircle)*5000;//円形度
	d[11] = (celem.m_byEdgeLR - elem.m_byEdgeLR)*190; //エッジ右方向
	d[12] = (celem.m_byEdgeRL - elem.m_byEdgeRL)*190; //エッジ左方向
	d[13] = (celem.m_byEdgeTB - elem.m_byEdgeTB)*180; //エッジ下方向
	d[14] = (celem.m_byEdgeBT - elem.m_byEdgeBT)*180; //エッジ上方向
	CRect rcL = elem.m_rect; //学習データ領域
	CRect rcR = celem.m_rect;//表示文字領域
	d[15] = 25*( rcL.Width() -rcR.Width()  ); //サイズ横
	d[16] = 25*( rcL.Height()-rcR.Height() ); //サイズ縦
	d[17] = 5*( (rcL.CenterPoint().x-elem.m_dbGx) - (rcR.CenterPoint().x-celem.m_dbGx) );
	d[18] = 5*( (rcL.CenterPoint().y-elem.m_dbGy) - (rcR.CenterPoint().y-celem.m_dbGy) );

	d[19] = (celem.GetLengthLeft()  -elem.GetLengthLeft()  )*1000 / celem.m_nAreaA;
	d[20] = (celem.GetLengthTop()   -elem.GetLengthTop()   )*1000 / celem.m_nAreaA;
	d[21] = (celem.GetLengthRight() -elem.GetLengthRight() )*1000 / celem.m_nAreaA;
	d[22] = (celem.GetLengthBottom()-elem.GetLengthBottom())*1000 / celem.m_nAreaA;

	//距離計算
	double dbLength = 0;
	for(int j=0,jmax=sizeof(d)/sizeof(double);j<jmax;j++){
		d[j] = mid(-300,d[j],300); //上限設定
		dbLength += d[j] * d[j]; 
	}

#ifdef CALC_DELTA_CHECK
	if( isDebug ){
		TRACE("\n%c\n",elem.m_tChar);
		TRACE("%.2f\n",sqrt(dbLength));
		for(int n=0;n<sizeof(d)/sizeof(double);n++){
			TRACE("d[%d]:%.2f, ",n,d[n]);
		}		
		TRACE("Area:%d-%d\n",celem.m_nAreaA,elem.m_nAreaA	);	//面積All 
		TRACE("AreaT:%.2f-%.2f\n",celem.m_dbAreaT,elem.m_dbAreaT);//面積上			
		TRACE("AreaB:%.2f-%.2f\n",celem.m_dbAreaB,elem.m_dbAreaB);//面積下
		TRACE("AreaL:%.2f-%.2f\n",celem.m_dbAreaL,elem.m_dbAreaL);//面積左
		TRACE("AreaR:%.2f-%.2f\n",celem.m_dbAreaR,elem.m_dbAreaR);//面積右
		TRACE("Gx:%.2f-%.2f\n",celem.m_dbGx,elem.m_dbGx);	//重心x
		TRACE("Gy:%.2f-%.2f\n",celem.m_dbGy,elem.m_dbGy);	//重心y
		TRACE("ContourNum:%u-%u\n",celem.m_wContourNum,elem.m_wContourNum);//境界線の数
		TRACE("ContourOut:%.2f-%.2f\n",celem.m_dbContourOut,elem.m_dbContourOut);//境界線の合計長さ
		TRACE("ContourIn :%.2f-%.2f\n",celem.m_dbContourIn ,elem.m_dbContourIn);//境界線の差分
		TRACE("Circle:%.2f-%.2f\n",celem.m_dbCircle,elem.m_dbCircle);	//円形度		
		TRACE("EdgeLR %u - %u\n",celem.m_byEdgeLR,elem.m_byEdgeLR);
		TRACE("EdgeRL %u - %u\n",celem.m_byEdgeRL,elem.m_byEdgeRL);
		TRACE("EdgeTB %u - %u\n",celem.m_byEdgeTB,elem.m_byEdgeTB);
		TRACE("EdgeBT %u - %u\n",celem.m_byEdgeBT,elem.m_byEdgeBT);
		TRACE("Width %u - %u\n",celem.m_rect.right-celem.m_rect.left,elem.m_rect.right-elem.m_rect.left);
		TRACE("Heght %u - %u\n",celem.m_rect.bottom-celem.m_rect.top,elem.m_rect.bottom-elem.m_rect.top);
		TRACE("\n");
	}
#endif
	return sqrt(dbLength); //距離
}

//境界線の数を取得
static int GetContourNum(ArrayPoint &aryPoint)
{
	ArrayPoint aryRet; //一時的な配列
	int nStart = 0;//追加開始座標
	int nCnt = 0; //境界線の数
	int nLen = 0; //境界線の長さ
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x < 0 ){
			if( nLen >= MIN_CONTOUR_LEN){ //長さが足りている境界線だけ
				int nEnd = i;
				for(int n=nStart;n<=nEnd;n++){ //OKの時だけ追加
					aryRet.Add( aryPoint[n] );
				}
				nCnt++;	
			}					
			nLen = 0; //長さクリア
			nStart = i+1;
		}else{
			nLen++;
		}
	}
	aryPoint.RemoveAll();
	aryPoint.Copy( aryRet ); //座標ポイント変更
	return nCnt;
}

//最大境界線取得
static double GetContourMax(ArrayPoint &aryPoint,double &dbAll)
{
	dbAll = 0; //clear
	double dbMax = 0; //最大境界線
	double dbCnt = 0; //調査中の境界線
	CPoint ptPrev(-1,-1);
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( ptPrev.x >= 0 ){ //距離計算可能な時だけ
			if( pt.x < 0 ){ 
				if( dbMax < dbCnt){//より大きい境界線発見
					dbMax = dbCnt;
				}
				dbAll += dbCnt;//合計値
				dbCnt = 0;			
			}else{
				int nCheck = abs(pt.x-ptPrev.x)+abs(pt.y-ptPrev.y);
				if( nCheck == 1 ){
					dbCnt += 1;
				}else{
					dbCnt += 1.414213562; //sqrt(2)
				}
			}
		}
		ptPrev = pt; //次の点へ
	}
	return dbMax;
}
//境界線矩形領域取得
static double GetContourRect(ArrayPoint &aryPoint)
{
	CRect rcMin(INT_MAX,INT_MAX,INT_MIN,INT_MIN); //最小矩形
	CArray<double> aryArea; //面積記録
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x < 0 ){ //終端
			rcMin.NormalizeRect(); //正規化
			aryArea.Add( rcMin.Width()*rcMin.Height() ); //面積登録
			rcMin.left = INT_MAX;
			rcMin.top  = INT_MAX;
			rcMin.right= INT_MIN;
			rcMin.bottom=INT_MIN;
			continue ;
		}
		//より小さいものに差し替え
		if( rcMin.left > pt.x ){
			rcMin.left = pt.x;
		}
		if( rcMin.top > pt.y ){
			rcMin.top = pt.y;
		}
		//より大きいものに差し替え
		if( rcMin.right < pt.x ){
			rcMin.right = pt.x;
		}
		if( rcMin.bottom < pt.y ){
			rcMin.bottom = pt.y;
		}
	}
	double dbSum = 0; //全面積合計
	double dbMax = 0; //最大面積値
	double dbMin = DBL_MAX;
	for(int i=0,imax=aryArea.GetSize();i<imax;i++){
		double dbArea = aryArea[i];
		if( dbMin > dbArea){
			dbMin = dbArea;
		}
		dbSum += dbArea;
	}
	//return (dbMax - (dbSum-dbMax)); //最大面積からそれ以外の面積を引く
	return dbMin;
}

//ラベリングと境界線から文字の特徴を取得
CharElement CharRecog::GetCharElement(CRect &rect,Label &label,Element *pElem,TCHAR tchar,WORD wRnum,ArrayPoint &aryPoint)
{
	CharElement element;
	ZeroMemory(&element,sizeof(CharElement));
	if( pElem != NULL ){
		element.m_tChar   = tchar; //検査文字
		element.m_wRnum   = wRnum; //領域番号
		element.m_nAreaA  = pElem->GetPixels();	//面積All 
		element.m_dbAreaT = (REAL)pElem->GetPixelsTop();	//面積上
		element.m_dbAreaB = (REAL)pElem->GetPixelsBottom();	//面積下
		element.m_dbAreaL = (REAL)pElem->GetPixelsLeft();	//面積左
		element.m_dbAreaR = (REAL)pElem->GetPixelsRight();	//面積右
		element.m_dbGx = (REAL)pElem->GetGravityX()-pElem->GetMinX();//重心の相対位置x
		element.m_dbGy = (REAL)pElem->GetGravityY()-pElem->GetMinY();//重心の相対位置y
		int nCnt = GetContourNum(aryPoint);
		element.m_wContourNum = (WORD)nCnt;	//境界線の数
		double dbLenAll;//境界線の合計長さ
		double dbLenMax = GetContourMax(aryPoint,dbLenAll); //最大境界線の取得
		element.m_dbContourOut = (REAL)dbLenMax;//最大境界線を外部境界線とする
		element.m_dbContourIn  = (REAL)abs(dbLenAll-element.m_dbContourOut); //最大境界線とその他の境界線の差分
		element.m_dbCircle = (REAL)(4*PI*element.m_nAreaA /(dbLenMax * dbLenMax));//円形度
		AryLine aryLineV; //垂直方向線幅ライン配列
		CollectLineV(aryLineV,label);
		element.m_byEdgeTB = (BYTE)GetEdgeNumTopToBtm(wRnum,NULL,aryLineV,label,rect.TopLeft());	 //上から下方向のエッジ数
		element.m_byEdgeBT = (BYTE)GetEdgeNumBtmToTop(wRnum,NULL,aryLineV,label,rect.TopLeft());	 //下から上方向のエッジ数
		AryLine aryLineH; //水平方向線幅ライン配列
		CollectLineH(aryLineH,label);		
		element.m_byEdgeLR = (BYTE)GetEdgeNumLeftToRight(wRnum,                 0,NULL,aryLineH,label,rect.TopLeft()); //左から右方向のエッジ数
		element.m_byEdgeRL = (BYTE)GetEdgeNumRightToLeft(wRnum,label.GetWidth()-1,NULL,aryLineH,label,rect.TopLeft());  //右から左方向のエッジ数
		
		element.m_rect.left  = pElem->GetMinX();
		element.m_rect.top   = pElem->GetMinY();
		element.m_rect.right = pElem->GetMaxX();
		element.m_rect.bottom= pElem->GetMaxY();

		//矩形頂点からの距離を求める
		CRect rcElem = element.m_rect; //要素領域
		CPoint ptLT(rcElem.left ,rcElem.top);//左上
		CPoint ptRT(rcElem.right,rcElem.top);//右上
		CPoint ptLB(rcElem.left ,rcElem.bottom);//左下
		CPoint ptRB(rcElem.right,rcElem.bottom);//右下		
		element.SetLeft	 (aryPoint,ptLT,ptLB);//左特徴点からの距離
		element.SetTop	 (aryPoint,ptLT,ptRT);//上特徴点からの距離
		element.SetRight (aryPoint,ptRT,ptRB);//右特徴点からの距離
		element.SetBottom(aryPoint,ptLB,ptRB);//下特徴点からの距離
	}
	return element; //特徴量取得
}

//画像ファイルからデータの取得
//検査領域のラベリング -> 文字ごとのラベリング -> 境界線探索 -> データ作成
BOOL CharRecog::ReadSample(int nThAll,const CString &strFile,const CString &strFileBin,CString &strNumber)
{
	Dib256 dib256;
	if( dib256.LoadFile(strFile) ){ //ファイルから読み込み
		::ErrorMsg(_T("画像ファイル%sの存在を確認してください。"),strFile);
		return FALSE;
	}
	CDIB dib;
	dib256.Paste(dib);//画像読み込み


	FilePlus file;
	if( !file.Open(strFileBin,FilePlus::modeBinOut) ){ //
		::ErrorMsg(_T("データファイル%sが作成できません。"),strFileBin);
		return FALSE;
	}
	CArchive ar(&file, CArchive::store); //文字特徴データセーブ

	CKIBOView *pView = GetView();
	int nWmax = dib.GetWidth();
	int nHmax = dib.GetHeight();

	CStringArray sAry; //文字列配列
	SplitToken(' ',sAry,strNumber); //半角空白区切り
	Label &label = GetView()->GetLabel(); //ラベリングクラス取得
	for(int nRnum=0;nRnum<sAry.GetSize();nRnum++){ //検査領域の数でループ
		CRect rcCheck = GetCheckRect(nRnum); //検査領域取得
		if( rcCheck.IsRectEmpty() ){
			continue ;
		}
		CString strNum = sAry[nRnum]; //サンプル正解文字列
		CArray<CRect> aryRect; //領域
		int nEmax = CollectCharRect(nRnum,nThAll,aryRect,dib); //文字が存在する領域を収集する
		if( nEmax != strNum.GetLength()){
			::ErrorMsg(_T("%s \n サンプルデーター(%s)文字数エラー \n領域:%d %d->%d"),dib.GetFilePath(),strNum,nRnum,strNum.GetLength(),nEmax );
			//::ErrorMsg(_T("学習画像ファイル \n %s \n は使用出来ません。"),dib.GetFilePath());
#ifdef DEBUG
			//ラベリングに使用した画像を表示する
			CDIB dibErr;//エラー画像
			dibErr.Create(label.GetWidth(),label.GetHeight());
			label.FillBitmapSrc(dibErr);
			::DrawRectangle(dibErr,rcCheck,COLOR_RED);//検査領域描画
			::DrawRectangles(dibErr,aryRect,COLOR_GREEN); //指定された矩形をビットマップに描画
			dibErr.WriteBitmap(L"c:\\tmp.bmp");

			ImageDlg dlg; //イメージ表示ダイアログ
			dlg.SetBitmap(dibErr);
			dlg.DoModal();
#endif
			::DeleteFile(strFileBin); //バイナリファイルの削除
			return FALSE;
		}
		rcCheck = GetCheckRect(nRnum); //検査領域再取得
		int nTh = CalcThreshold(nThAll,dib,rcCheck);//しきい値取得
		m_aryPoint.RemoveAll();//境界線点列初期化
		for(int i=0;i<nEmax;i++){ //文字矩形要素の数だけループ
			CRect rect = aryRect[i];
			rect.left  = max(0,rect.left-CHAR_SPACE_X);
			rect.top   = max(0,rect.top -CHAR_SPACE_Y);
			rect.right = min(nWmax-1,rect.right +CHAR_SPACE_X);
			rect.bottom= min(nHmax-1,rect.bottom+CHAR_SPACE_Y);

			m_dibGray.Cut(dib,rect);//1文字分のビットマップ作成
			Element *pElem = LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint); //1文字分のラベリング実行
			if(pElem != NULL && m_aryPoint.GetSize() > 0 ){
				CharElement celem = GetCharElement(rect,label,pElem,strNum[i],nRnum,m_aryPoint); //文字要素構造体の取得
				m_aryCharElement.Add(celem); //文字特徴追加
				//要素データの保存
				Serialize(ar,celem); //文字特徴書き込み
			}
			///////////////////////FOR DEBUG/////////////////////////////////////
			else{
				m_dibGray.ChangeBinary(nTh);
				m_dibGray.SaveFile(L"c:\\cut.bmp");
				return FALSE;
			}
			///////////////////////FOR DEBUG/////////////////////////////////////
		}
	}
	return TRUE;
}

//文字認識コマンド
//戻り値　認識文字列
CString CharRecog::RecognizeChar(int nCnum,int nThAll)
{
	m_mapPrevElement.clear(); //最新データclear
	CString strRet = L""; //戻り値
	CString strRed = L""; //赤文字
	COutputView *pOutput = GetOutputView(); //結果表示
	pOutput->Clear();
	CKIBOView *pView = GetView();
	CameraEx *pCameraEx = pView->GetCameraPtr(nCnum);

	//検査画像取得
	int nAnum = GetAverageNum();
	if( nAnum ==1){ //撮像平均回数
		pCameraEx->Capture(); //0番カメラから画像取得
	}else{
		pCameraEx->CaptureAverage(nAnum,GetWaitTime());
	}

	int nWmax = pCameraEx->GetWidth();
	int nHmax = pCameraEx->GetHeight();

	CDIB &dibSrc = pCameraEx->GetDib();	
	Label &label = GetView()->GetLabel(); //ラベリングクラス
	CArray<CRect> aryRect; //収集される文字領域	
	int nRectSize = m_mapCheckRect.GetSize();//検査領域個数
	CString str1;
	for(int nRnum=0;nRnum<nRectSize;nRnum++){
		CRect rcCheck = GetCheckRect(nRnum); //検査領域取得 
		if( rcCheck.IsRectEmpty() ){
			continue ;
		}
		double dbLenTh = GetLengthTh(nRnum);//文字良否判定距離取得
		int nEmax = CollectCharRect(nRnum,nThAll,aryRect,dibSrc); //文字が存在する領域を収集する
		int nCharNum = GetCharNum(nRnum); //正解文字数取得
		CString strFix = GetCharString(nRnum); //固定文字取得
		int nFixNum = strFix.GetLength();//固定文字数
		if( nCharNum < 0 || nCharNum == nEmax || nFixNum == nEmax){ //文字数が変動する場合 || 文字数チェックOK			
			 rcCheck = GetCheckRect(nRnum); //検査領域取得 
			int nTh = CalcThreshold(nThAll,dibSrc,rcCheck); //しきい値変更
			//境界線探索
			m_aryPoint.RemoveAll(); //境界線点列初期化
			for(int i=0;i<nEmax;i++){ //要素の数だけループ
				CRect rect = aryRect[i];
				rect.left  = max(0,rect.left-CHAR_SPACE_X);
				rect.top   = max(0,rect.top -CHAR_SPACE_Y);
				rect.right = min(nWmax-1,rect.right +CHAR_SPACE_X);
				rect.bottom= min(nHmax-1,rect.bottom+CHAR_SPACE_Y);
				m_dibGray.Cut(dibSrc,rect);//1文字分のビットマップ作成

				Element *pElem = LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint);//１文字分のラベリング要素取得
				CharElement celem = GetCharElement(rect,label,pElem,0,nRnum,m_aryPoint); //文字要素の取得
#ifdef LINECHECK
				CreateBorderObject(rect.TopLeft(),m_aryPoint);//境界線表示オブジェクト作成
#endif
				//距離チェック
				double dbLen = GetPredictElement(celem,nRnum,i); //ラベリングと境界線から文字種を判断
				if( nCharNum == 0 ){ //固定文字の時
					if( i < nFixNum ){
						celem.m_tChar = strFix[i]; //固定文字取得
					}
				}
				//ここまでの認識結果を記録
				SetPrevElement(celem.m_wRnum,i,celem);
				if( dbLenTh < dbLen ){
					CString strMes;
					strMes.Format(L"?: 文字認識エラー %.1f",dbLen);
					pOutput->Output(strMes,COLOR_RED);//メッセージ表示
				}
				CheckNickData data;
				//距離によるエラーチェック
				if( dbLenTh < dbLen || //エラー文字へ変更
					!CheckElement(celem,nRnum,i) ){ //特徴値チェックが失敗した時
					celem.m_tChar = s_tcError;
				}else{
					//文字欠け検査
					data = CheckNick(i,label,celem,rect,m_aryPoint,nRnum,dbLen,nTh);
					celem.m_tChar = data.m_tChar;
					if( data.m_isOK && IsExpandCharTop(celem.m_tChar) ){//iやjのチェック
						rect.top = rcCheck.top;
						rect.bottom = rcCheck.bottom;
						m_dibGray.Cut(dibSrc,rect);//1文字分のビットマップ作成
						LabelCharElement(nRnum,rect.TopLeft(),label,nTh,m_dibGray,m_aryPoint);//１文字分のラベリング要素取得
						data.m_isOK = (label.GetEmax()==2);//複数存在する時だけOK
						if(!data.m_isOK){ //エラー
							pOutput->RemoveLastMessage();
							CString strMes;
							pOutput->Output(false,L"%c: 　異常値検出：構成要素不足",celem.m_tChar);
						}
					}
				}
				//結果の出力
				strRet += celem.m_tChar;
				if( data.m_isOK && celem.m_tChar != s_tcError){
					strRed += ' ';
				}else{
					strRed += celem.m_tChar; //赤文字で表示
					CreateErrorRect(nRnum,i,rect); //領域全体のエラー矩形描画
				}
			}
		}else if( nCharNum != nEmax ){ //文字数が異なる
			str1.Format(_T("領域内文字数エラー 個数:%d -> %d 面積 "),nCharNum,nEmax);			
			for(int i=0;i<aryRect.GetSize();i++){
				CreateErrorRect(nRnum,i,aryRect[i]); //領域全体のエラー矩形描画
			}
			CString strTmp;
			for(int i=0;i<nEmax;i++){
				Element *pElem = label.GetElement(i);
				if( pElem != NULL ){
					strTmp.Format(L"%d ",pElem->GetPixels()); //画素数表示
					str1 += strTmp;
				}
			}
			pOutput->Output(str1,COLOR_RED); //出力			
		}
		str1 = "";
		pOutput->Output(str1,COLOR_WHITE); //出力
		strRet += ' ';
		strRed += ',';
	}
	CString strConnect;
	strConnect.Format(L"%s,%s",strRet,strRed);
	return strConnect;
}

//エラーライン描画
void CharRecog::CreateErrorRect(int nRnum,int nEnum,CRect &rect) 
{
	CString strName;
	strName.Format(_T("ErrorRect%d-%d"),nRnum,nEnum);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,Color::Red,false); //矩形描画赤
	TRACE(_T("Error RECT:%d %d %d %d\n"),rect.left,rect.top,rect.right,rect.bottom);
}

//境界線表示オブジェクト作成
void CharRecog::CreateBorderObject(CPoint &ptTL,ArrayPoint &aryPoint) 
{
	int nCnt = 0;
	ArrayPoint ptAry;
	for(int i=0,imax=aryPoint.GetSize();i<imax;i++){
		CPoint &pt = aryPoint[i];
		if( pt.x >= 0 ){
			ptAry.Add(pt+ptTL);
		}else{//終端発見
			CString strContourName;
			strContourName.Format(L"Contour%d_%d%d",nCnt,ptTL.x,ptTL.y);
			GetView()->m_mgrVO.CreatePolylineObject(strContourName,ptAry,nCnt==0 ? COLOR_GREEN : nCnt==1 ? COLOR_YELLOW : COLOR_RED); //矩形描画
			ptAry.RemoveAll();
			nCnt++;
		}
	}
}


//指定番号の画像を取得
BOOL CharRecog::GetCharImage(CDIB &dibDst,int nRnum,int nCnum)
{
	CKIBOView *pView = GetView();
	CharElement& celem = GetPrevElement(nRnum,nCnum); //以前のデータ取得
	CDIB &dibSrc = pView->GetDib(); //表示ビットマップ取得
	CRect rc = celem.m_rect;//画像描画領域
	int nW = rc.Width();
	int nH = rc.Height();
	if( dibDst.Create(nW,nH) ){		
		CDC *pDC = ::AfxGetMainWnd()->GetDC();
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(pDC);
		CBitmap *pOldBmpSrc = dcSrc.SelectObject( dibSrc.GetBitmap() );

		CDC dcDst;
		dcDst.CreateCompatibleDC(pDC);
		CBitmap *pOldBmpDst = dcDst.SelectObject( dibDst.GetBitmap() );
		dcDst.BitBlt(0,0,nW,nH,&dcSrc,rc.left,rc.top,SRCCOPY);

		//後始末
		dcSrc.SelectObject( pOldBmpSrc);
		dcDst.SelectObject( pOldBmpDst);
		pDC->DeleteDC();
		return TRUE;
	}
	return FALSE;
}

//上にもう一文字ありますか？
BOOL CharRecog::IsExpandCharTop(TCHAR tc)
{
	switch(tc){
		case 'i':
		case 'j':
			return TRUE;
	}
	return FALSE;
}

//水平方向線幅ライン収集
int CharRecog::CollectLineH(AryLine &aryLineH,Label &label)
{
	aryLineH.RemoveAll(); //clear

	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得
	CLine line;
	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx; //検索開始アドレス
		for(int x=0;x<nCx;x++,pFind++){
			if( IsInPointH(pFind,x,nCx,nCy) ){ //In Pointが見つかった
				line.m_ptS.x = x; //開始点登録
				line.m_ptS.y = y;
				for(;x<nCx;x++,pFind++){
					if( IsOutPointH(pFind,x,nCx,nCy) ){ //Out Pointが見つかった
						line.m_ptE.x = x-1;
						line.m_ptE.y = y;
						aryLineH.Add(line); //線登録
						break;
					}
				}
				if( x==nCx ){ //端まで到達した場合					
					line.m_ptE.x = x-1;
					line.m_ptE.y = y;
					aryLineH.Add(line); //端までの線を登録
				}
			}
		}
	}
	return aryLineH.GetSize();
}

//横方向開始ポイント
BOOL CharRecog::IsInPointH(BYTE *pFind,int x,int nCx,int nCy)
{
	int imax = max(2,nCx / 30 );
	BOOL isOK = TRUE;
	for(int i=0;i<imax && x<nCx;i++,x++,pFind++){ //縦方向３画素検査
		isOK &= (*pFind==MAXBYTE);
	}
	return isOK;
}
//横方向終了ポイント
BOOL CharRecog::IsOutPointH(BYTE *pFind,int x,int nCx,int nCy)
{
	//int imax = max(2,nCx / 30 );
	//BOOL isOK = TRUE;
	//for(int i=0;i<imax && x<nCx;i++,x++,pFind++){ //縦方向３画素検査
	//	isOK &= !(*pFind);
	//}
	return !(*pFind);
}
//縦方向開始ポイント
BOOL CharRecog::IsInPointV(BYTE *pFind,int y,int nCx,int nCy)
{
	int imax = max(2,nCy / 30 );
	BOOL isOK = TRUE;
	for(int i=0;i<imax && y<nCy;i++,y++,pFind+=nCx){ //縦方向３画素検査
		isOK &= (*pFind==MAXBYTE);
	}
	return isOK;
}
//縦方向終了ポイント
BOOL CharRecog::IsOutPointV(BYTE *pFind,int y,int nCx,int nCy)
{
	//int imax = max(2,nCy / 30 );
	//BOOL isOK = TRUE;
	//for(int i=0;i<imax && y<nCy;i++,y++,pFind+=nCx){ //縦方向３画素検査
	//	isOK &= !(*pFind);
	//}
	return !(*pFind);
}
//垂直方向線幅ライン収集
int CharRecog::CollectLineV(AryLine &aryLineV,Label &label)
{
	aryLineV.RemoveAll(); //clear

	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //ラベリング結果取得

	CLine line;
	int nYs = 0;
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + x; //検索開始アドレス
		for(int y=nYs;y<nCy;y++,pFind+=nCx){			
			if( IsInPointV(pFind,y,nCx,nCy) ){ //In Pointが見つかった
				line.m_ptS.x = x; //開始点登録
				line.m_ptS.y = y;				
				for(;y<nCy;y++,pFind+=nCx){
					if( IsOutPointV(pFind,y,nCx,nCy) ){ //Out Pointが見つかった
						line.m_ptE.x = x;
						line.m_ptE.y = y-1;
						aryLineV.Add(line); //線登録
						break;
					}					
				}
				if( y==nCx ){ //端まで到達した場合					
					line.m_ptE.x = x;
					line.m_ptE.y = y-1;
					aryLineV.Add(line); //端までの線を登録
				}
			}
		}
	}
	return aryLineV.GetSize();
}

//要素チェック
BOOL CharRecog::CheckElement(CharElement &celem,int nRnum,int nCnum)
{
	//各要素の最小値
	double	dbAreaAMin	= INT_MAX;	//面積All
	WORD	wContourMin = MAXWORD;	//境界線数
	double	dbWidthMin	= INT_MAX;	//文字幅最小
	double	dbHeightMin	= INT_MAX;	//文字高さ最小
	double  dbLenLMin[CharElement::POINT_NUM]; //= INT_MAX;	//左端からの距離
	double  dbLenTMin[CharElement::POINT_NUM]; //= INT_MAX;	//上端からの距離
	double  dbLenRMin[CharElement::POINT_NUM]; //= INT_MAX;	//左端からの距離
	double  dbLenBMin[CharElement::POINT_NUM]; //= INT_MAX;	//下端からの距離
	double  dbLenDxMin = INT_MAX;  //左右距離の差分
	double  dbLenDyMin = INT_MAX;  //上下距離の差分

	//各要素の最大値
	double	dbAreaAMax	= 0;	//面積All
	WORD	wContourMax = 0;	//境界線数
	double	dbWidthMax	= 0;	//文字幅最小
	double	dbHeightMax	= 0;	//文字高さ最小
	double  dbLenLMax[CharElement::POINT_NUM]; //= 0;	//左端からの距離
	double  dbLenTMax[CharElement::POINT_NUM]; //= 0;	//上端からの距離
	double  dbLenRMax[CharElement::POINT_NUM]; //= 0;	//左端からの距離
	double  dbLenBMax[CharElement::POINT_NUM]; //= 0;	//下端からの距離
	double  dbLenDxMax = 0;  //左右距離の差分
	double  dbLenDyMax = 0;   //上下距離の差分

	//配列設定
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMin[i] = INT_MAX;	//左端からの距離
		dbLenTMin[i] = INT_MAX;	//上端からの距離
		dbLenRMin[i] = INT_MAX;	//左端からの距離
		dbLenBMin[i] = INT_MAX;	//下端からの距離
		dbLenLMax[i] = 0;	//左端からの距離
		dbLenTMax[i] = 0;	//上端からの距離
		dbLenRMax[i] = 0;	//左端からの距離
		dbLenBMax[i] = 0;	//下端からの距離
	}
	BOOL isSyUD = IsSymmetryUpDown(celem.m_tChar);	//上下対称文字ですか？
	//BOOL isSyLR = IsSymmetryLR(celem.m_tChar);		//左右対称文字ですか？
	BOOL isInc  = IsInclination(nRnum); //斜め文字ですか？
	for(int i=0;i<m_aryCharElement.GetSize();i++){ //全サンプルデータ検索
		CharElement& elem = m_aryCharElement[i]; //要素取得
		if( elem.m_wRnum == nRnum || (IsCommon(elem.m_wRnum) && IsCommon(nRnum)) ){ //文字領域検査チェック 領域ごとに文字種類が異なる場合
			if( elem.m_tChar == celem.m_tChar ){ //同一文字
				//文字領域
				double dbWidth  = (elem.m_rect.right -elem.m_rect.left+1);
				double dbHeight = (elem.m_rect.bottom-elem.m_rect.top +1);
				//より小さい物に置き換える
				if(elem.m_nAreaA < dbAreaAMin){ //面積All最小
					dbAreaAMin = elem.m_nAreaA; 
				}
				if(elem.m_nAreaA > dbAreaAMax){ //面積All最大
					dbAreaAMax = elem.m_nAreaA; 
				}
				if(elem.m_wContourNum < wContourMin){ //境界線の合計長さ最小
					wContourMin = elem.m_wContourNum;
				}
				if(elem.m_wContourNum > wContourMax){//境界線の合計長さ最大
					wContourMax= elem.m_wContourNum; 
				}
				if(dbWidth < dbWidthMin){//文字幅最小
					dbWidthMin = dbWidth; 
				}
				if(dbWidth > dbWidthMax){//文字幅最大
					dbWidthMax = dbWidth; 
				}
				if(dbHeight < dbHeightMin){//文字高さ最小
					dbHeightMin = dbHeight;
				}
				if(dbHeight > dbHeightMax){//文字高さ最大
					dbHeightMax = dbHeight;
				}

				for(int j=0;j<CharElement::POINT_NUM;j++){
					if(elem.m_rLeft[j] < dbLenLMin[j]){ //左端からの距離
						dbLenLMin[j] = elem.m_rLeft[j];
					}
					if(elem.m_rLeft[j] > dbLenLMax[j]){ //左端からの距離
						dbLenLMax[j] = elem.m_rLeft[j];
					}
					if(elem.m_rTop[j] < dbLenTMin[j]){ //上端からの距離
						dbLenTMin[j] = elem.m_rTop[j];
					}
					if(elem.m_rTop[j] > dbLenTMax[j]){ //上端からの距離
						dbLenTMax[j] = elem.m_rTop[j];
					}
					if(elem.m_rRight[j] < dbLenRMin[j]){ //右端からの距離
						dbLenRMin[j] = elem.m_rRight[j];
					}
					if(elem.m_rRight[j] > dbLenRMax[j]){ //右端からの距離
						dbLenRMax[j] = elem.m_rRight[j];
					}
					if(elem.m_rBottom[j] < dbLenBMin[j]){ //下端からの距離
						dbLenBMin[j] = elem.m_rBottom[j];
					}
					if(elem.m_rBottom[j] > dbLenBMax[j]){ //下端からの距離
						dbLenBMax[j] = elem.m_rBottom[j];
					}
				}
				if( !isInc ){ //斜め文字以外のときだけ
					if( isSyUD ){//上下対称文字の時
						double dbTop   = elem.GetLengthTop();	//上特徴点からの距離取得
						double dbBottom= elem.GetLengthBottom();//下特徴点からの距離取得
						double dbDy = abs(dbTop-dbBottom);
						if(dbDy < dbLenDyMin){ //上下差分最小
							dbLenDyMin = dbDy;
						}
						if(dbDy > dbLenDyMax){ //上下差分最大
							dbLenDyMax= dbDy;
						}
					}
					//if( isSyLR ){ //左右対称文字の時
					//	double dbLeft  = elem.GetLengthLeft();	//左特徴点からの距離取得
					//	double dbRight = elem.GetLengthRight();	//右特徴点からの距離取得
					//	double dbDx = abs(dbLeft-dbRight);
					//	if(dbDx < dbLenDxMin){ //上下差分最小
					//		dbLenDxMin = dbDx;
					//	}
					//	if(dbDx > dbLenDxMax){ //上下差分最大
					//		dbLenDxMax= dbDx;
					//	}
					//}					
				}
			}
		}
	}
	//各要素の最大値
	dbAreaAMax		+= DEFAULT_CHECK_SPACE;
	dbWidthMax		+= DEFAULT_CHECK_SPACE;
	dbHeightMax		+= DEFAULT_CHECK_SPACE;
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMax[i]+= DEFAULT_CHECK_SPACE; //左端からの距離
		dbLenTMax[i]+= DEFAULT_CHECK_SPACE; //上端からの距離
		dbLenRMax[i]+= DEFAULT_CHECK_SPACE; //右端からの距離
		dbLenBMax[i]+= DEFAULT_CHECK_SPACE; //下端からの距離
	}
	dbLenDyMax		+= (DEFAULT_CHECK_SPACE+1);

	//各要素の最小値
	dbAreaAMin		-= DEFAULT_CHECK_SPACE;
	dbWidthMin		-= DEFAULT_CHECK_SPACE;
	dbHeightMin		-= DEFAULT_CHECK_SPACE;
	for(int i=0;i<CharElement::POINT_NUM;i++){
		dbLenLMin[i]-= DEFAULT_CHECK_SPACE; //左端からの距離
		dbLenTMin[i]-= DEFAULT_CHECK_SPACE; //上端からの距離
		dbLenRMin[i]-= DEFAULT_CHECK_SPACE; //右端からの距離
		dbLenBMin[i]-= DEFAULT_CHECK_SPACE; //下端からの距離
	}
	dbLenDyMin -= (DEFAULT_CHECK_SPACE+1);

	CString strErrType;
	BOOL isOK = TRUE;
	COutputView *pOutput = GetOutputView(); //結果表示
	double dbWidth  = (celem.m_rect.right -celem.m_rect.left+1);
	double dbHeight = (celem.m_rect.bottom-celem.m_rect.top +1);
	//▼範囲内にあるかどうか
	if(celem.m_wContourNum < wContourMin){
		strErrType = L"境界線数不足";
		isOK = FALSE;
	}else if(celem.m_wContourNum > wContourMax){
		strErrType = L"境界線数超";
		isOK = FALSE;
	}else if( dbWidth < dbWidthMin ){
		strErrType = L"文字幅不足";
		isOK = FALSE;
	}else if(dbWidth > dbWidthMax){
		strErrType = L"文字幅超";
		isOK = FALSE;
	}else if( dbHeight < dbHeightMin ){
		strErrType = L"文字高さ不足";
		isOK = FALSE;
	}else if(dbHeight > dbHeightMax){
		strErrType = L"文字高さ超";
		isOK = FALSE;
	}else if(celem.m_nAreaA < dbAreaAMin){ //面積最小値チェック
		strErrType = L"面積不足";
		isOK = FALSE;
	}else if( !celem.IsLeftOK(dbLenLMin,dbLenLMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsTopOK(dbLenTMin,dbLenTMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsRightOK(dbLenRMin,dbLenRMax,strErrType) ){
		isOK = FALSE;
	}else if( !celem.IsBottomOK(dbLenBMin,dbLenBMax,strErrType) ){
		isOK = FALSE;
	}else if( !isInc ){ //斜め文字以外の時だけ
		if( isSyUD ){//上下対称文字の時
			double dbTAll = celem.GetLengthTop();
			double dbBAll = celem.GetLengthBottom();
			double dbDy = abs(dbTAll-dbBAll);			
			if( dbDy < dbLenDyMin || dbLenDyMax < dbDy ){ //上下の距離の差異が学習データの範囲内かどうか
				strErrType = L"上下対称エラー";
				isOK = FALSE;
			}
		}
		//if( isSyLR ){//左右対称文字の時だけ
		//	double dbLAll = celem.GetLengthLeft();
		//	double dbRAll = celem.GetLengthRight();
		//	double dbDx = abs(dbRAll-dbLAll);
		//	if( dbDx < dbLenDxMin || dbLenDxMax < dbDx ){ //上下の距離の差異が学習データの範囲内かどうか
		//		strErrType = L"左右対称エラー";
		//		isOK = FALSE;
		//	}
		//}
	}
	if(!isOK){
		pOutput->Output(false,L"%c: 異常値検出：%s",celem.m_tChar,strErrType);
		TRACE(L"\n特徴値によるエラー検出\n");
		TRACE(L"%c %d\n",celem.m_tChar,nRnum);
		TRACE(L"面積：%d %.1f-%.1f\n",celem.m_nAreaA,dbAreaAMin,dbAreaAMax);
		TRACE(L"境界線数：%u %u-%u\n",celem.m_wContourNum,wContourMin,wContourMax); //境界線の合計長さ
		TRACE(L"文字領域：%.1f %.1f-%.1f\n",dbWidth	,dbWidthMin	,dbWidthMax	 ); //文字領域
		TRACE(L"文字領域：%.1f %.1f-%.1f\n",dbHeight,dbHeightMin,dbHeightMax ); //文字領域
		TRACE(L"左:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rLeft[i]);
		}
		TRACE(L"\n上:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rTop[i]);
		}
		TRACE(L"\n右:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rRight[i]);
		}
		TRACE(L"\n下:");
		for(int i=0;i<CharElement::POINT_NUM;i++){
			TRACE(L"%.3f,",celem.m_rBottom[i]);
		}
	}
	return isOK;
}

//
//CharRecog.ini
//[400001105.bmp]
//TIME=YYYYMMDDhhmmss
//領域番号-文字番号=1,1,1000,100,100.0,198...
//ELEMENT2=1,1,1000,100,100.0,198...
//ELEMENT3=1,1,1000,100,100.0,198...

//セクションから要素データの書き込み
BOOL CharRecog::ReadProfileElement(const CString &strFile,int nRnum,int nCnum,CharElement &elem)
{
	CString strKey;
	strKey.Format(L"%02d-%02d",nRnum,nCnum);//領域番号-文字番号		
	CString strElement = ReadIniString(strFile,strKey);
	CStringArray sAry; //文字列配列
	SplitToken(',',sAry,strElement); //カンマ区切り	
	if(sAry.GetSize() >= (21+CharElement::POINT_NUM*4) ){
		elem.m_tChar   = sAry[0].GetAt(0);	//検査文字
		elem.m_wRnum   = _wtoi(sAry[1]);	//領域番号
		elem.m_nAreaA  = _wtoi(sAry[2]);	//面積All		
		elem.m_dbAreaT = (REAL)_wtof(sAry[3]);	//面積上
		elem.m_dbAreaB = (REAL)_wtof(sAry[4]);	//面積下
		elem.m_dbAreaL = (REAL)_wtof(sAry[5]);	//面積左
		elem.m_dbAreaR = (REAL)_wtof(sAry[6]);	//面積右
		elem.m_dbGx	   = (REAL)_wtof(sAry[7]);	//重心x
		elem.m_dbGy	   = (REAL)_wtof(sAry[8]);	//重心y
		elem.m_wContourNum  = _wtoi(sAry[9] );//境界線の数
		elem.m_dbContourOut = (REAL)_wtof(sAry[10]);//境界線の合計長さ
		elem.m_dbContourIn  = (REAL)_wtof(sAry[11]);//外側境界線-内側境界線
		elem.m_dbCircle		= (REAL)_wtof(sAry[12]);//円形度
		elem.m_byEdgeLR		= _wtoi(sAry[13]);//左から右方向のエッジ数
		elem.m_byEdgeRL		= _wtoi(sAry[14]);	//右から左方向のエッジ数
		elem.m_byEdgeTB		= _wtoi(sAry[15]);	//上から下方向のエッジ数
		elem.m_byEdgeBT		= _wtoi(sAry[16]);	//下から上方向のエッジ数
		elem.m_rect.left	= _wtoi(sAry[17]);	//文字領域左
		elem.m_rect.top		= _wtoi(sAry[18]);	//文字領域上
		elem.m_rect.right	= _wtoi(sAry[19]);	//文字領域右
		elem.m_rect.bottom	= _wtoi(sAry[20]);	//文字領域下
		int nIndex = 21;
		for(int i=0;i<CharElement::POINT_NUM;i++){
			elem.m_rLeft[i]	 = (REAL)_wtof(sAry[nIndex+i]);//左特徴点からの距離
			elem.m_rTop[i]   = (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM]);//上特徴点からの距離
			elem.m_rRight[i] = (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM*2]);//右特徴点からの距離
			elem.m_rBottom[i]= (REAL)_wtof(sAry[nIndex+i+CharElement::POINT_NUM*3]);//下特徴点からの距離
		}
		return TRUE;
	}
	return FALSE;
}
//セクションに要素データの書き込み
void CharRecog::WriteProfileElement(const CString &strFile,CharElement &elem,int nCnum)
{
	CString strKey;
	strKey.Format(L"%02d-%02d",elem.m_wRnum,nCnum);//領域番号-文字番号

	CString strVal;
	strVal.Format(L"%c,%u,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%u,%.2f,%.2f,%.2f,%u,%u,%u,%u,%d,%d,%d,%d,"
					,elem.m_tChar		//検査文字
					,elem.m_wRnum		//領域番号
					,elem.m_nAreaA		//面積All		
					,elem.m_dbAreaT		//面積上
					,elem.m_dbAreaB		//面積下
					,elem.m_dbAreaL		//面積左
					,elem.m_dbAreaR		//面積右
					,elem.m_dbGx		//重心x
					,elem.m_dbGy		//重心y
					,elem.m_wContourNum	//境界線の数
					,elem.m_dbContourOut//外側境界線の長さ
					,elem.m_dbContourIn //内側境界線の長さ
					,elem.m_dbCircle	//円形度
					,elem.m_byEdgeLR	//左から右方向のエッジ数
					,elem.m_byEdgeRL	//右から左方向のエッジ数
					,elem.m_byEdgeTB	//上から下方向のエッジ数
					,elem.m_byEdgeBT	//下から上方向のエッジ数
					,elem.m_rect.left	//文字領域左
					,elem.m_rect.top	//文字領域上		
					,elem.m_rect.right	//文字領域右	
					,elem.m_rect.bottom	//文字領域下	
					);
	strVal += elem.GetLeftCsv();
	strVal += ',';
	strVal += elem.GetTopCsv();
	strVal += ',';
	strVal += elem.GetRightCsv();
	strVal += ',';
	strVal += elem.GetBottomCsv();
	WriteIniString(strFile,strKey,strVal);
}

//.iniファイルにに文字列の書き込み
void CharRecog::WriteIniString(const CString &strSection,const CString &strKey,LPCWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);

	CString strPath;
	strPath.Format(L"%s\\CharRecog.ini",GetApp()->GetAppPath());	
	::WritePrivateProfileString(strSection,strKey,strFormat,strPath);
}

//.iniファイルから文字列の読み込み
CString CharRecog::ReadIniString(const CString &strSection,const CString &strKey)
{
	CString strPath;
	strPath.Format(L"%s\\CharRecog.ini",GetApp()->GetAppPath());	

	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));
	::GetPrivateProfileString(strSection,strKey,g_strEmpty,szBuf,sizeof(szBuf),strPath);
	CString strRet = szBuf; //戻り値作成
	return strRet;
}
//.binファイルから文字列データの読み込み
//ファイル名 -> 学習データ\SN123456789_134826.bin
//領域番号,文字順番号,文字

//上下対称文字の時
BOOL CharRecog::IsSymmetryUpDown(TCHAR tc)
{
	switch(tc){
		case '0':
		case '3':
		case '8':
		case 'N':
		case 'C':
		case 'D':
		case 'E':
		case 'H':
		case 'O':
		case 'S':
		case 'X':
		case 'Z':
		case 'l':
		case 'o':
		case 'x':
		case 'z':
			return TRUE;
			break;
	}
	return FALSE;
}

//左右対称文字の時
BOOL CharRecog::IsSymmetryLR(TCHAR tc)
{
	switch(tc){
		case '0':
		case '8':
		case 'H':
		case 'O':
		case 'X':
		case 'l':
		case 'o':
		case 'x':
			return TRUE;
			break;
	}
	return FALSE;
}


//文字特徴読み込み　& m_aryCharElementへ追加
BOOL CharRecog::ReadElement(CString &strFileBin,CStringArray &sAry)
{
	FilePlus file;
	if( file.Open(strFileBin,FilePlus::modeBinIn) ){
		CharElement elem; //文字特徴
		CArchive ar(&file, CArchive::load);
		int nRmax = sAry.GetSize();
		for(int i=0;i<nRmax;i++){//検査領域の数だけループ
			if( !GetCheckRect(i).IsRectEmpty() ){
				CString strText = sAry[i]; //印字文字列取得
				int nLen = strText.GetLength();
				for(int n=0;n<nLen;n++){ //文字列の数だけループ
					if( Serialize(ar,elem) ){
						m_aryCharElement.Add(elem);
					}
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}
//シリアライズ
BOOL CharRecog::Serialize(CArchive& ar,CharElement &elem)
{
	if (ar.IsStoring()){ //Save
		ar << elem.m_tChar		;	//検査文字
		ar << elem.m_wRnum		;	//領域番号
		ar << elem.m_nAreaA		;	//面積All
		ar << elem.m_dbAreaT	;	//面積上
		ar << elem.m_dbAreaB	;	//面積下
		ar << elem.m_dbAreaL	;	//面積左
		ar << elem.m_dbAreaR	;	//面積右
		ar << elem.m_dbGx		;	//重心x
		ar << elem.m_dbGy		;	//重心y
		ar << elem.m_wContourNum;	//境界線の数
		ar << elem.m_dbContourOut;	//外部境界線の長さ
		ar << elem.m_dbContourIn;	//内部境界線の長さ
		ar << elem.m_dbCircle	;	//円形度
		ar << elem.m_byEdgeLR	;	//左から右方向のエッジ数
		ar << elem.m_byEdgeRL	;	//右から左方向のエッジ数
		ar << elem.m_byEdgeTB	;	//上から下方向のエッジ数
		ar << elem.m_byEdgeBT	;	//下から上方向のエッジ数
		ar << elem.m_rect.left	;	//文字領域
		ar << elem.m_rect.top	;	//文字領域
		ar << elem.m_rect.right	;	//文字領域
		ar << elem.m_rect.bottom;	//文字領域
		for(int i=0;i<CharElement::POINT_NUM;i++){
			ar << elem.m_rLeft[i];	//左特徴点からの距離
			ar << elem.m_rTop[i];	//上特徴点からの距離
			ar << elem.m_rRight [i];//右特徴点からの距離
			ar << elem.m_rBottom[i];//下特徴点からの距離
		}
		return TRUE;
	}else{ //Load
		ar >> elem.m_tChar		;	//検査文字
		ar >> elem.m_wRnum		;	//領域番号
		ar >> elem.m_nAreaA		;	//面積All
		ar >> elem.m_dbAreaT	;	//面積上
		ar >> elem.m_dbAreaB	;	//面積下
		ar >> elem.m_dbAreaL	;	//面積左
		ar >> elem.m_dbAreaR	;	//面積右
		ar >> elem.m_dbGx		;	//重心x
		ar >> elem.m_dbGy		;	//重心y
		ar >> elem.m_wContourNum;	//境界線の数
		ar >> elem.m_dbContourOut;	//外部境界線の長さ
		ar >> elem.m_dbContourIn;	//内部境界線の長さ
		ar >> elem.m_dbCircle	;	//円形度
		ar >> elem.m_byEdgeLR	;	//左から右方向のエッジ数
		ar >> elem.m_byEdgeRL	;	//右から左方向のエッジ数
		ar >> elem.m_byEdgeTB	;	//上から下方向のエッジ数
		ar >> elem.m_byEdgeBT	;	//下から上方向のエッジ数
		ar >> elem.m_rect.left	;	//文字領域
		ar >> elem.m_rect.top	;	//文字領域
		ar >> elem.m_rect.right	;	//文字領域
		ar >> elem.m_rect.bottom;	//文字領域
		for(int i=0;i<CharElement::POINT_NUM;i++){
			ar >> elem.m_rLeft[i];	//左特徴点からの距離
			ar >> elem.m_rTop[i];	//上特徴点からの距離
			ar >> elem.m_rRight [i];//右特徴点からの距離
			ar >> elem.m_rBottom[i];//下特徴点からの距離
		}
		return TRUE;
	}	
	return FALSE;
}

//領域ごとの2値化
void CharRecog::BinaryCheckRect(int nCnum,int nRnum,int nTh)
{
	COutputView *pOutput = GetOutputView(); //結果表示

	CameraEx *pCameraEx = GetView()->GetCameraPtr(nCnum);
	CDIB &dibSrc = pCameraEx->GetDib();	
	if( GetCheckRect(nRnum).IsRectEmpty() ){//指定検査領域設定チェック
		return ;
	}
	CArray<CRect> aryRect; //収集される文字領域	
	int nEmax = CollectCharRect(nRnum,nTh,aryRect,dibSrc); //文字が存在する領域を収集する
	CRect rect = GetCheckRect(nRnum); //検査領域取得
	nTh = CalcThreshold(nTh,dibSrc,rect);
	CDIB dibDst;
	m_dibGray.Paste(dibDst);
	dibDst.ChangeBinary(nTh); //2値化画像作成
	dibSrc.SetDib(rect.left,rect.top,dibDst); //2値化画像貼り付け
}

//領域内のしきい値計算
int CharRecog::CalcThreshold(int nTh,CDIB &dibSrc,CRect &rcCheck)
{
	//領域ごとに2値化
	if( m_dibGray.Cut(dibSrc,rcCheck) ){//指定領域ビットマップ作成					
		int nBlack = 0; //黒の画素数
		int nHist[256];
		m_dibGray.CollectHistgram(nHist);//ヒストグラム収集
		//nTh以上の最大要素取得		
		int nMax = 0;
		int nImax = 0;
		for(int i=nTh;i<=MAXBYTE;i++){
			if( nMax < nHist[i] ){//より大きいほうへ
				nMax = nHist[i];
				nImax = i;
			}
		}
		//nThより下の最大要素
		nMax = 0;
		int nImin = 0;
		for(int i=nTh-1;i>=0;i--){
			if( nMax < nHist[i] ){ //より大きい方へ
				nMax = nHist[i];
				nImin = i;
			}
		}
		////nIminからnImax間の最小値を求める
		//int nMin = INT_MAX;
		//int nTh1 = nTh;
		//for(int i=nImin;i<=nImax;i++){
		//	if( nMin > nHist[i] ){ //より小さい方へ
		//		nMin = nHist[i];
		//		nTh1 = i;
		//	}	
		//}
		////nImaxからnImin間の最小値を求める
		//nMin = INT_MAX;
		//int nTh2 = nTh;
		//for(int i=nImax;i>=nImin;i--){
		//	if( nMin > nHist[i] ){ //より小さい方へ
		//		nMin = nHist[i];
		//		nTh2 = i;
		//	}	
		//}
		nTh = int( (nImin + nImax) / 2.0 + 0.5 ); //あたらしいしきい値
	}
	//////FOR DEBUG///////////////////
	TRACE("TH:%d,left:%d,top:%d,right:%d,bottom:%d\n",nTh,rcCheck.left,rcCheck.top,rcCheck.right,rcCheck.bottom);
	//////FOR DEBUG///////////////////
	return nTh;
}

#endif


