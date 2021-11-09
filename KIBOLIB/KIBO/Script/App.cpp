#include "../stdafx.h"
#include "Windows.h"
#include "App.h"
#include "../enum.h"
#include "../KIBOView.h"
#include "../KIBOFrame.h"
#include "../OutputWnd.h"
#include "../LIB/functions.h"
#include "../KIBODoc.h"
#include "../LIB/FolderDialog.h"
#include "../LIB/Object.h"
#include "../LIB/FilePlus.h"
#include <mmsystem.h>	// timeGetTime()
#pragma comment(lib, "winmm.lib")

extern CStringW ConvertUTF8ToUnicode(const CStringA& utf8);

static App *s_pInstance = NULL; //唯一のクラス実体
App* App::GetInstance()//実体取得
{
	return s_pInstance;
}
App::App(void)
{
	s_pInstance = this;
	m_hWnd = ::AfxGetMainWnd()->GetSafeHwnd();
	m_crText = COLOR_BLACK; //文字色
	ZeroMemory(&m_lf,sizeof(LOGFONT)); //フォントデータ
	wcscpy_s(m_lf.lfFaceName,LF_FACESIZE,_T("ＭＳ 明朝"));		//Defalutフォント
	m_lf.lfHeight = 15; //Default Font Size
	m_rect.SetRectEmpty();		//汎用領域リセット
	m_strSectionInt = L"INT";		//レジストリ値セクション
	m_strSectionString = L"STRING";	//レジストリ文字列セクション

	m_hAccel = ::LoadAccelerators(::AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));//←追加
	m_ulCount = ::GetTickCount64();//時間計測カウンター
}

App::~App(void)
{
}

static CommandTable s_table[] =
{
	 {App::FUNC_WAIT		,1, L"Wait"}		//指定時間停止
	,{App::FUNC_CLICK_WAIT	,0, L"ClickWait"}	//マウスクリックされるまで停止
	,{App::FUNC_KEY_WAIT	,1, L"KeyboardWait"}//キーボードメッセージ待ち
	,{App::FUNC_COUNTSTART	,0, L"CountStart"} //時間計測開始		
	,{App::FUNC_COUNTEND	,0, L"CountEnd"} //時間計測終了
	,{App::FUNC_STARTSCRIPTTIMER,1,L"StartScriptTimer"}	//スクリプトタイマー停止
	,{App::FUNC_STOPSCRIPTTIMER,0,L"StopScriptTimer"}	//スクリプトタイマー停止
	,{App::FUNC_SET_REPEAT_SCRIPT,2,L"SetRepeatScript"}	//繰り返しスクリプトファイル設定
	,{App::FUNC_SETWINDOWTEXT,1,L"SetWindowText"}  //メインWindow Caption変更
	,{App::FUNC_DRAWRECT	, 9,L"DrawRect"}	//矩形描画
	,{App::FUNC_DRAWLINE	, 8,L"DrawLine"}	//指定ライン描画
	,{App::FUNC_PAINTMASK	, 4,L"PaintMask"}	//マスク領域描画
	,{App::FUNC_RENDER		, 0,L"Render"}		//レンダリング
	,{App::FUNC_REMOVEALL	, 0,L"RemoveAll"}	//全削除
	,{App::FUNC_REMOVE		, 1,L"Remove"}		//指定オブジェクト削除
	,{App::FUNC_REMOVEEXCEPT, 1,L"RemoveExcept"}//指定オブジェクト以外削除
	,{App::FUNC_SETTEXTCOLOR, 3,L"SetTextColor"}//文字色の設定
	,{App::FUNC_SETFONTNAME	, 1,L"SetFontName"}	//Font名称設定
	,{App::FUNC_SETFONTSIZE	, 1,L"SetFontSize"}	//FontSize設定
	,{App::FUNC_DRAWTEXT	, 4,L"DrawText"}	//文字列描画
	,{App::FUNC_DRAWCHAR	, 4,L"DrawChar"}	//文字描画
	,{App::FUNC_SETVISIBLE  , 2,L"SetVisible"}	//表示ON・OFF切り替え
	,{App::FUNC_SETTEXTRECT , 7,L"SetTextRect"}	//文字列表示領域指定
	,{App::FUNC_BINARYIMAGE	, 1,L"BinaryImage"}	//表示画像2値化
	,{App::FUNC_SETKEYFILE	, 2,L"SetKeyFile"}	//コマンドファイルのキー割り当て
	,{App::FUNC_GETBITMAP	,3,L"GetBitmap"}	//指定フォルダ内ビットマップの取得
	,{App::FUNC_GETBITMAPNUM,2,L"GetBitmapNum"}//指定フォルダ内ビットマップ数の取得
	,{App::FUNC_TEXTOUTPUT	,-1,L"TextOutput"}	//文字列出力
	,{App::FUNC_MESSAGE		,-1,L"Message"}		//メッセージ表示
	,{App::FUNC_SETSTATUSBARTEXT,1,L"SetStatusBarText"}	//ステータスバー文字セット
	,{App::FUNC_OUTPUTCLEAR	, 0,L"OutputClear"}	//文字列出力クリア
	,{App::FUNC_SETVIEWRATE , 2,L"SetViewRate"}	//表示倍率設定
	,{App::FUNC_GETVIEWRATECX,0,L"GetViewRateCx"}//表示倍率取得 x方向 
	,{App::FUNC_GETVIEWRATECY,0,L"GetViewRateCy"}//表示倍率取得 y方向
	,{App::FUNC_SHOWWINDOW	, 1,L"ShowWindow"}  //メインWindow表示状態変更
	,{App::FUNC_MOVEWINDOW	, 4,L"MoveWindow"}  //メインWindow表示サイズ変更
	,{App::FUNC_VIEWHEIGHT  , 1,L"ViewHeight"}	//結果出力高さ変更
	,{App::FUNC_VIEWWIDTH   , 1,L"ViewWidth"}	//結果出力幅変更
	,{App::FUNC_SETTEXT	, 2,L"SetText"}	//汎用文字列設定
	,{App::FUNC_GETTEXT	, 1,L"GetText"}	//汎用文字列取得
	,{App::FUNC_SETNUM,2,L"SetNum"}	//汎用数値設定
	,{App::FUNC_GETNUM,1,L"GetNum"}	//汎用数値取得
	,{App::FUNC_SETDBL,2,L"SetDbl"}	//汎用実数設定
	,{App::FUNC_GETDBL,1,L"GetDbl"}	//汎用実数取得
	,{App::FUNC_CREATE_FOLDER,1,L"CreateFolder"}//指定フォルダ作成	
	,{App::FUNC_SENDMESSAGE	 ,-1,L"SendMessage"}//コマンド呼び出し
	,{App::FUNC_POSTMESSAGE	 ,-1,L"PostMessage"}//コマンド呼び出し
	,{App::FUNC_SETRECT		 , 4,L"SetRect"}	//領域設定
	,{App::FUNC_GETTIME		 , 0,L"GetTime"}	//マルティメデアタイマー取得
	,{App::FUNC_GETVALUE	 , 1,L"GetValue"}	//指定値取得
	,{App::FUNC_SETLINECOLOR , 3,L"SetLineColor"}//線色設定
	,{App::FUNC_GETPRIVATEPROFILEINT,4,L"GetPrivateProfileInt"}//.iniファイル値取得
	,{App::FUNC_WRITEPRIVATEPROFILEINT,4,L"WritePrivateProfileInt"}//.iniファイル値書き込み
	,{App::FUNC_GETPRIVATEPROFILEDBL,4,L"GetPrivateProfileDbl"}//.iniファイル値取得
	,{App::FUNC_WRITEPRIVATEPROFILEDBL,4,L"WritePrivateProfileDbl"}//.iniファイル値書き込み
	,{App::FUNC_GETPRIVATEPROFILESTRING,4,L"GetPrivateProfileString"}//.iniファイル文字列取得
	,{App::FUNC_WRITEPRIVATEPROFILESTRING,4,L"WritePrivateProfileString"}//.iniファイル文字列書き込み
	,{App::FUNC_GETPROFILEINT, 1,L"GetProfileInt"}//レジストリ値取得
	,{App::FUNC_WRITEPROFILEINT, 2,L"WriteProfileInt"}//レジストリ値設定
	,{App::FUNC_GETPROFILEDBL, 1,L"GetProfileDbl"}//レジストリ値実数取得
	,{App::FUNC_WRITEPROFILEDBL, 2,L"WriteProfileDbl"}//レジストリ値実数設定
	,{App::FUNC_GETPROFILESTRING,1,L"GetProfileString"}//レジストリ値取得
	,{App::FUNC_WRITEPROFILESTRING,2,L"WriteProfileString"}//レジストリ値設定
	,{App::FUNC_SETINPUTNAME,2,L"SetInuptName"}//GUI入力値変更
	,{App::FUNC_CENTERPOINT,1,L"CenterPoint"} //中心点表示・非表示
	,{App::FUNC_ENABLEWINDOW,1,L"EnableWindow"} //Menu＆ボタン Enable/DisEnable
	,{App::FUNC_GETPIXELSIZE,1,L"GetPixelSize"} //1画素あたりの大きさを取得
	,{App::FUNC_SELECTFOLDER ,0,L"SelectFolder"}		//フォルダ選択
	,{App::FUNC_SELECTBITMAP ,0,L"SelectBitmap"}		//ビットマップファイル選択
	,{App::FUNC_EXEC,1,L"Exec" }		//EXE実行
	,{App::FUNC_READTEXT,1,L"ReadText" }	//文字列全行読み込み
	,{App::FUNC_READSTRING,2,L"ReadString"}	//文字列１行読み込み
	,{App::FUNC_MAX			 ,0,NULL} //コマンドテーブル終端
};

//関数名チェック
STDMETHODIMP App::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
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
STDMETHODIMP App::Invoke(DISPID dispIdMember	//Function ID 番号
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
		case FUNC_WAIT: //Wait
			{
				BOOL isOK = Wait( (DWORD)(pDispParams->rgvarg[0].ulVal) ); //Sleep時間
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_CLICK_WAIT: //マウスクリック wait
			{
				BOOL isEnd = ClickWait();
				ReturnBool(wFlags,pVarResult,isEnd);
			}
			break;
		case FUNC_KEY_WAIT:		//キーボードメッセージ待ち
			{
				int nID = GetValientInt(&pDispParams->rgvarg[0]); //キーボードID
				BOOL isEnd = KeyWait(nID);
				ReturnBool(wFlags,pVarResult,isEnd);
			}
			break;
		case FUNC_COUNTSTART:	//時間計測開始
			{
				m_ulCount = ::GetTickCount64();//時間計測カウンター
			}
			break;
		case FUNC_COUNTEND: //時間計測終了
			{
				ULONGLONG ulCount = ::GetTickCount64();//時間計測カウンター
				UINT ulVal = (UINT)(ulCount - m_ulCount );
				ReturnUint(wFlags,pVarResult,ulVal);
			}
			break;
		case FUNC_STARTSCRIPTTIMER: //スクリプトタイマー開始
			{
				CString strFilePath = pDispParams->rgvarg[0].bstrVal; 
				GetFrame()->ScriptStart(strFilePath);
			}
			break;
		case FUNC_STOPSCRIPTTIMER: //スクリプトタイマー停止
			GetFrame()->KillTimer(TIMER_SCRIPT);
			break;
		case FUNC_SET_REPEAT_SCRIPT://繰り返しスクリプトファイル設定
			{
				CString strFilePath = pDispParams->rgvarg[1].bstrVal; 
				int nElapse = GetValientInt(&pDispParams->rgvarg[0]);
				GetFrame()->ScriptRepeat(strFilePath,nElapse);
			}
			break;
		case FUNC_SETWINDOWTEXT:  //メインWindow Caption変更
			{
				CString strCaption = pDispParams->rgvarg[0].bstrVal; 
				GetFrame()->SetWindowText(strCaption);
			}
			break;
		case FUNC_DRAWRECT: //矩形描画
			{
				CString strName = pDispParams->rgvarg[8].bstrVal; //描画オブジェクト名称
				int nLeft = GetValientInt(&pDispParams->rgvarg[7]); //左
				int nTop  = GetValientInt(&pDispParams->rgvarg[6]); //上
				int nRight = GetValientInt(&pDispParams->rgvarg[5]); //右
				int nBottom = GetValientInt(&pDispParams->rgvarg[4]); //下
				int nA = 255;//A
				int nR = GetValientInt(&pDispParams->rgvarg[3]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[2]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[1]); //B
				BOOL isPaint = (BOOL)GetValientInt(&pDispParams->rgvarg[0]); //塗りつぶしますか？				
				DrawRect(strName,nLeft,nTop,nRight,nBottom,nA,nR,nG,nB,isPaint);
			}
			break;
		case FUNC_DRAWLINE://指定ライン描画
			{
				CString strName = pDispParams->rgvarg[7].bstrVal; //描画オブジェクト名称
				int nXS = GetValientInt(&pDispParams->rgvarg[6]); //始点x
				int nYS  = GetValientInt(&pDispParams->rgvarg[5]); //始点y
				int nXE = GetValientInt(&pDispParams->rgvarg[4]); //終点x
				int nYE = GetValientInt(&pDispParams->rgvarg[3]); //終点y
				int nA = 255;//A
				int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
				int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
				int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
				DrawLine(strName,nXS,nYS,nXE,nYE,nA,nR,nG,nB);
			}
			break;
		//case FUNC_PAINTMASK:	//マスク領域描画
		//	{
		//		int nCnum = GetValientInt(&pDispParams->rgvarg[3]); //カメラ番号
		//		int nR    = GetValientInt(&pDispParams->rgvarg[2]); //R
		//		int nG    = GetValientInt(&pDispParams->rgvarg[1]); //G
		//		int nB    = GetValientInt(&pDispParams->rgvarg[0]); //B
		//		CameraEx *pCamera = GetView()->GetCameraPtr( nCnum );
		//		if(pCamera != NULL){
		//			CString strName;
		//			for(int i=0;i<pCamera->m_aryMask.GetSize();i++){
		//				strName.Format(L"Mask%d",i);
		//				CRect rect = pCamera->m_aryMask[i].m_rect;
		//				DrawRect(strName,rect.left,rect.top,rect.right,rect.bottom,255,nR,nG,nB,TRUE);
		//			}
		//		}				
		//	}
			break;
		case FUNC_RENDER:		//レンダリング
			Render();
			break;
		case FUNC_REMOVEALL: //全描画オブジェクト削除
			RemoveAll();			
			break;
		case FUNC_REMOVE://指定オブジェクトを削除
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //削除名称
				GetView()->m_mgrVO.RemoveByName(strName.GetBuffer()); //指定オブジェクトを削除
			}
			break;
		case FUNC_REMOVEEXCEPT: //指定オブジェクト以外を削除
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //削除名称
				RemoveExcept(strName);				
			}
			break;
		case FUNC_SETTEXTCOLOR: //文字色の設定
			{
				int r = pDispParams->rgvarg[2].ulVal; //R
				int g = pDispParams->rgvarg[1].ulVal; //G
				int b = pDispParams->rgvarg[0].ulVal; //B
				SetTextColor(RGB(r,g,b));
			}
			break;
		case FUNC_SETFONTNAME: //Font名称設定
			{
				CString strName = pDispParams->rgvarg[0].bstrVal; //Font 名称
				wcscpy_s(m_lf.lfFaceName,LF_FACESIZE,strName);		//Defalutフォント
			}
			break;
		case FUNC_SETFONTSIZE: //FontSize設定
			{
				m_lf.lfHeight = pDispParams->rgvarg[0].ulVal; //Font Size
			}
			break;
		case FUNC_DRAWTEXT://文字列設定
			{
				CString strID   = pDispParams->rgvarg[3].bstrVal; //ID文字列
				CString strText = pDispParams->rgvarg[2].bstrVal; //表示文字列
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //x座標
				int nY = GetValientInt(&pDispParams->rgvarg[0]); //y座標
				DrawText(strID,strText,nX,nY);
			}
			break;
		case FUNC_DRAWCHAR:		//文字描画
			{	
				CString strID = pDispParams->rgvarg[3].bstrVal; //ID文字列
				int nChar = pDispParams->rgvarg[2].intVal; //表示文字
				int nX = GetValientInt(&pDispParams->rgvarg[1]); //x座標
				int nY = GetValientInt(&pDispParams->rgvarg[0]); //y座標
				CString strText;
				strText.Format(L"%c",nChar);
				GetView()->m_mgrVO.CreateStringObject(strID,nX,nY,strText,GetTextColor(),GetLogFont()); //矩形描画				
			}
			break;
		case FUNC_SETVISIBLE:	//表示ON・OFF切り替え
			{
				CString strID = pDispParams->rgvarg[1].bstrVal; //ID文字列
				BOOL isVisible = (BOOL)GetValientInt(&pDispParams->rgvarg[0]); //ON/OFF
				SetVisible(strID,(bool)isVisible);
			}
			break;
		case FUNC_SETTEXTRECT: //文字列表示領域指定
			{
				CString strID = pDispParams->rgvarg[6].bstrVal; //ID文字列
				CString strH = pDispParams->rgvarg[5].bstrVal; //FormatH
				CString strV = pDispParams->rgvarg[4].bstrVal; //FormatV
				int nLeft   = GetValientInt(&pDispParams->rgvarg[3]); //左
				int nTop    = GetValientInt(&pDispParams->rgvarg[2]); //上
				int nRight  = GetValientInt(&pDispParams->rgvarg[1]); //右
				int nBottom = GetValientInt(&pDispParams->rgvarg[0]); //下
				StringObject *pSO = (StringObject *)GetView()->m_mgrVO.GetVO(strID); 
				if( pSO != NULL ){
					pSO->SetRect(strH,strV,nLeft,nTop,nRight,nBottom); //表示領域設定
				}
			}
			break;
		case FUNC_BINARYIMAGE: //2値化画像表示
			{
				int nTh = pDispParams->rgvarg[0].ulVal; //しきい値
				GetView()->DoBinary(nTh);
			}
			break;
		case FUNC_SETKEYFILE: //コマンドファイルのキー割り当て
			{
				int nVK = GetValientInt(&pDispParams->rgvarg[1]); //VirtualKey番号
				CString strFilePath = pDispParams->rgvarg[0].bstrVal; //ファイル文字列				
				GetFrame()->m_mapKeyFilePath.SetAt(nVK,strFilePath); //キーマッピング登録
			}
			break;
		case FUNC_GETBITMAP://指定フォルダ内ビットマップの取得
			{
				CString strFolder = pDispParams->rgvarg[2].bstrVal; //フォルダパス
				int nNo = GetValientInt(&pDispParams->rgvarg[1]); //ファイル番号				
				CString strExt = L"*.";
				strExt += pDispParams->rgvarg[0].bstrVal; //拡張子

				CString strRet;
				int nCnt = 0; //ビットマップファイル数カウンター
				CFileFind find;
				BOOL isWorking = find.FindFile(strExt);
				while(isWorking){
				   isWorking = find.FindNextFile();
				   if(nCnt==nNo){//指定番号が見つかった
						strRet=find.GetFilePath(); //ファイルパス取得
						break;
				   }
				   nCnt++;				   
				} 
				ReturnString(wFlags,pVarResult,strRet); //戻り値セット							
			}
			break;
		case FUNC_GETBITMAPNUM://指定フォルダ内ビットマップ数の取得
			{
				CString strFolder = pDispParams->rgvarg[1].bstrVal; //フォルダパス
				CString strExt = L"*.";
				strExt += pDispParams->rgvarg[0].bstrVal; //拡張子
				int nCnt = 0; //ビットマップファイル数カウンター
				CFileFind find;
				BOOL isWorking = find.FindFile(strExt);
				while(isWorking){
				   isWorking = find.FindNextFile();
				   nCnt++;
				} 
				ReturnInt(wFlags,pVarResult,nCnt); //スクリプトに渡す
			}
			break;
		case FUNC_TEXTOUTPUT: //検査結果出力
			{
				CString strText; //表示文字列
				COLORREF color = COLOR_WHITE; //標準文字色
				if(pDispParams->cArgs == 2 ){ //引数が2つ
					int nNo = GetValientInt(&pDispParams->rgvarg[1]); //表示行数
					CString strText = pDispParams->rgvarg[0].bstrVal; //表示文字列
					GetOutputWnd()->OutputText(nNo,strText);
				}else if(pDispParams->cArgs == 1) {//引数が１つ
					CString strText = pDispParams->rgvarg[0].bstrVal; //表示文字列
					GetOutputWnd()->OutputText(strText);
				}								
				
			}
			break;
		case FUNC_MESSAGE: //メッセージ表示
			{
				if( pDispParams->cArgs == 5 ){ //引数が4つ				
					int nNo = GetValientInt(&pDispParams->rgvarg[4]); //表示行数
					CString strText = pDispParams->rgvarg[3].bstrVal; //表示文字列
					int nR = GetValientInt(&pDispParams->rgvarg[2]); //R
					int nG = GetValientInt(&pDispParams->rgvarg[1]); //G
					int nB = GetValientInt(&pDispParams->rgvarg[0]); //B
					Message(nNo,strText,RGB(nR,nG,nB));
				}else if(pDispParams->cArgs == 2 ){//引数が1つ
					int nNo = GetValientInt(&pDispParams->rgvarg[1]); //表示行数
					CString strText = pDispParams->rgvarg[0].bstrVal; //表示文字列
					Message(nNo,strText,COLOR_WHITE);
				}
			}
			break;

		case FUNC_SETSTATUSBARTEXT: //ステータスバー文字セット
			{
				CString str = pDispParams->rgvarg[0].bstrVal; //表示文字列
				//GetFrame()->GetStatusBarPtr()->SetPaneText(0,str);
			}
			break;
		case FUNC_OUTPUTCLEAR: //文字列出力クリア
			OutputClear();
			break;

		case FUNC_SETVIEWRATE:	//表示座標倍率設定
			{
				double dbRateX = GetValientDbl(&pDispParams->rgvarg[1]); //x方向倍率
				double dbRateY = GetValientDbl(&pDispParams->rgvarg[0]); //y方向倍率
				SetViewRate(dbRateX,dbRateY);
			}
			break;
		case FUNC_GETVIEWRATECX:	//表示倍率取得 x方向 
			{
				double dbRateCx = GetView()->GetRateCx();	
				ReturnDbl(wFlags,pVarResult,dbRateCx ); //戻り値セット
			}
			break;
		case FUNC_GETVIEWRATECY:	//表示倍率取得 y方向
			{
				double dbRateCy = GetView()->GetRateCy();	
				ReturnDbl(wFlags,pVarResult,dbRateCy); //戻り値セット
			}
			break;
		case FUNC_SHOWWINDOW: //メインWindow表示状態変更
			{
				int nShow = GetValientInt(&pDispParams->rgvarg[0]); //表示状態
				GetFrame()->ShowWindow(nShow); //表示状態変更
			}
			break;
		case FUNC_MOVEWINDOW: //メインWindow表示サイズ変更
			{
				int nX = GetValientInt(&pDispParams->rgvarg[3]); //x座標
				int nY = GetValientInt(&pDispParams->rgvarg[2]); //y座標
				int nW = GetValientInt(&pDispParams->rgvarg[1]); //幅
				int nH = GetValientInt(&pDispParams->rgvarg[0]); //高さ
				GetFrame()->MoveWindow(nX,nY,nW,nH); //表示状態変更
			}
			break;
		case FUNC_VIEWHEIGHT:	//結果出力高さ変更	
			{
				int nH = GetValientInt(&pDispParams->rgvarg[0]); //高さ
//				GetFrame()->SetViewHeight(nH);
			}
			break;
		case FUNC_VIEWWIDTH:		//結果出力幅変更
			{
				int nW = GetValientInt(&pDispParams->rgvarg[0]); //幅
//				GetFrame()->SetViewWidth(nW);
			}
			break;
		case FUNC_SETTEXT:	//汎用文字列記憶
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				CString strVal = pDispParams->rgvarg[0].bstrVal; //記憶文字列取得
				SetText(strKey,strVal);//文字列を記憶させる
			}
			break;
		case FUNC_GETTEXT:	//汎用文字列取得
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				CString strVal = GetText(strKey);	//文字列取得;
				ReturnString(wFlags,pVarResult,strVal); //戻り値セット
			}
			break;
		case FUNC_SETNUM:	//汎用数値設定
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				int nVal = GetValientInt(&pDispParams->rgvarg[0]); //設定値
				SetNum(strKey,nVal);
			}
			break;
		case FUNC_GETNUM:	//汎用数値取得
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				int nVal = GetNum(strKey);//数値取得
				ReturnInt(wFlags,pVarResult,nVal); //戻り値セット
			}
			break;
		case FUNC_SETDBL:	//汎用実数設定
			{
				CString strKey = pDispParams->rgvarg[1].bstrVal; //Key
				double dbVal = GetValientDbl(&pDispParams->rgvarg[0]); //設定値
				SetDbl(strKey,dbVal);
			}
			break;
		case FUNC_GETDBL:	//汎用実数取得
			{
				CString strKey = pDispParams->rgvarg[0].bstrVal; //Key
				double dbVal = GetDbl(strKey);//数値取得
				ReturnDbl(wFlags,pVarResult,dbVal); //戻り値セット
			}
			break;
		case FUNC_CREATE_FOLDER:	//指定フォルダ作成
			{
				::SetCurrentDirectory( GetApp()->GetAppPath() ); //カレントディレクトリ設定
				CString strDirectory = pDispParams->rgvarg[0].bstrVal; //フォルダ名称
				if( strDirectory.Find(_T("TODAY")) >= 0 ){//本日の日付でフォルダ作成
					CTime time = CTime::GetCurrentTime(); //現在時刻取得
					CString strDate = time.Format(_T("%Y%m%d"));
					strDirectory.Replace(_T("TODAY"),strDate);
				}
				::CreateDirectory(strDirectory,NULL);
			}
			break;
		case FUNC_SENDMESSAGE:	//コマンド呼び出し
			{
				UINT unID = 0;
				WPARAM wParam = 0;
				LPARAM lParam = 0;
				int nIndex = pDispParams->cArgs;
				if( nIndex >= 1 ){ //引数が1つ以上
					unID = pDispParams->rgvarg[nIndex-1].ulVal; //メッセージID
				}
				if(nIndex >= 2){//引数が2つ以上
					wParam = pDispParams->rgvarg[nIndex-2].ulVal; //wParam
				}
				if(nIndex >= 3){//引数が3つ以上
					lParam = pDispParams->rgvarg[nIndex-3].ulVal; //lParam
				}
				int nRet = SendMessage(unID,wParam,lParam);
				ReturnInt(wFlags,pVarResult,nRet); //戻り値セット
			}
			break;
		case FUNC_POSTMESSAGE:
			{
				UINT unID = 0;
				WPARAM wParam = 0;
				LPARAM lParam = 0;
				int nIndex = pDispParams->cArgs;
				if( nIndex >= 1 ){ //引数が1つ以上
					unID = pDispParams->rgvarg[nIndex-1].ulVal; //メッセージID
				}
				if(nIndex >= 2){//引数が2つ以上
					wParam = GetValientInt(&pDispParams->rgvarg[nIndex-2]); //wParam
				}
				if(nIndex >= 3){//引数が3つ以上
					lParam = GetValientInt(&pDispParams->rgvarg[nIndex-3]); //lParam
				}
				GetFrame()->PostMessage(unID,wParam,lParam); //Userメッセージ
			}
			break;
		case FUNC_SETRECT: //領域設定
			{
				int nLeft   = pDispParams->rgvarg[3].ulVal; //左
				int nTop    = pDispParams->rgvarg[2].ulVal; //上
				int nRight  = pDispParams->rgvarg[1].ulVal; //右
				int nBottom = pDispParams->rgvarg[0].ulVal; //下
				this->SetRect(nLeft,nTop,nRight,nBottom); //領域設定
			}
			break;
		case FUNC_GETTIME:	//マルティメデアタイマー取得
			{
			
				DWORD dwTime = timeGetTime();
				ReturnUint(wFlags,pVarResult,dwTime); //Uint
			}
			break;
		case FUNC_GETVALUE:	//指定値取得
			{
				double dbVal = 0;
				CString strType = pDispParams->rgvarg[0].bstrVal; //値指定
				if( strType == "CLICKLINE" ){
					dbVal = GetView()->GetClickLineLength();//クリックライン取得
				}
				ReturnDbl(wFlags,pVarResult,dbVal);
			}
			break;
		case FUNC_SETLINECOLOR://線色設定
			{
				int r = pDispParams->rgvarg[2].ulVal; //R
				int g = pDispParams->rgvarg[1].ulVal; //G
				int b = pDispParams->rgvarg[0].ulVal; //B
				Color color(r,g,b);
				GetView()->SetClickClineColor(color);
			}
			break;
		case FUNC_GETPRIVATEPROFILEINT://.iniファイル値取得
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				int nDefault   = pDispParams->rgvarg[1].intVal; //Default値
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);	
				int nRet = ::GetPrivateProfileInt(strSec,strKey,nDefault,strPath);
				ReturnInt(wFlags,pVarResult,nRet);				
			}
			break;
		case FUNC_WRITEPRIVATEPROFILEINT: //.iniファイル値書き込み
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				int nVal       = pDispParams->rgvarg[1].intVal; //設定値
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);	
				CString strVal;
				strVal.Format(L"%d",nVal);
				::WritePrivateProfileString(strSec,strKey,strVal,strPath);
			}
			break;
		case FUNC_GETPRIVATEPROFILEDBL:		//.iniファイルDbl値取得
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				double dbVal = pDispParams->rgvarg[1].dblVal; //Double値
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strDef;
				strDef.Format(L"%.f",dbVal);
				CString strRet = GetIniString(strSec,strKey,strDef,strFileName);
				double dbRet = _wtof(strRet);
				ReturnDbl(wFlags,pVarResult,dbRet);
			}
			break;
		case FUNC_WRITEPRIVATEPROFILEDBL:	//.iniファイルDbl値書き込み
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				double dbVal = pDispParams->rgvarg[1].dblVal; //Double値
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);
				::WritePrivateProfileDbl(strSec,strKey,L"%.10f",dbVal,strPath);
			}
			break;
		case FUNC_GETPRIVATEPROFILESTRING://.iniファイル文字列取得
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				CString strDef = pDispParams->rgvarg[1].bstrVal; //文字列
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strRet = GetIniString(strSec,strKey,strDef,strFileName);
				ReturnString(wFlags,pVarResult,strRet);
			}
			break;
		case FUNC_WRITEPRIVATEPROFILESTRING://.iniファイル文字列書き込み
			{
				CString strSec = pDispParams->rgvarg[3].bstrVal; //セクション
				CString strKey = pDispParams->rgvarg[2].bstrVal; //キー
				CString strString = pDispParams->rgvarg[1].bstrVal; //文字列
				CString strFileName = pDispParams->rgvarg[0].bstrVal; //ファイル名称
				CString strPath;
				strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),strFileName);					
				::WritePrivateProfileString(strSec,strKey,strString,strPath);
			}
			break;
		case FUNC_GETPROFILEINT: //レジストリ値取得
			{
				CString strEntry = pDispParams->rgvarg[0].bstrVal; //エントリ
				int nRet = GetProfileInt(strEntry);
				ReturnInt(wFlags,pVarResult,nRet);
			}
			break;
		case FUNC_WRITEPROFILEINT: //レジストリ値設定
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //エントリ
				int nValue = pDispParams->rgvarg[0].intVal; //書き込む値
				SetProfileInt(strEntry,nValue);
			}
			break;
		case FUNC_GETPROFILEDBL:	//レジストリ値実数取得
			{
				CString strEntry   = pDispParams->rgvarg[0].bstrVal; //エントリ
				double dbRet = GetProfileDbl(strEntry);
				ReturnDbl(wFlags,pVarResult,dbRet);
			}
			break;
		case FUNC_WRITEPROFILEDBL:	//レジストリ値実数設定
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //エントリ
				double dbValue = GetValientDbl(&pDispParams->rgvarg[0]); //書き込む値
				SetProfileDbl(strEntry,dbValue);
			}
			break;
		case FUNC_GETPROFILESTRING:	//レジストリ値取得
			{
				CString strEntry   = pDispParams->rgvarg[0].bstrVal; //エントリ
				CString strRet = this->GetProfileString(strEntry);
				ReturnString(wFlags,pVarResult,strRet);
			}
			break;
		case FUNC_WRITEPROFILESTRING:	//レジストリ値設定
			{
				CString strEntry   = pDispParams->rgvarg[1].bstrVal; //エントリ
				CString strValue   = pDispParams->rgvarg[0].bstrVal; //書き込む値
				this->SetProfileString(strEntry,strValue);
			}
			break;
		case FUNC_SETINPUTNAME:		//GUI入力値変更
			{
				CString strName = pDispParams->rgvarg[1].bstrVal; //Key
				int nValue = pDispParams->rgvarg[0].intVal; //Value
				//CKIBOFrame *pFrame = GetFrame();
				BOOL isOK = FALSE;// pFrame->SetInputName(strName, nValue);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_CENTERPOINT:	//中心点表示・非表示
			{
				BOOL isVisible = pDispParams->rgvarg[0].boolVal; //Value
				CenterPoint(isVisible);
			}
			break;
		case FUNC_ENABLEWINDOW:	//Menu＆ボタン Enable/DisEnable
			{
				BOOL isEnable = pDispParams->rgvarg[0].boolVal; //Value
				EnableWindow(isEnable);
			}
			break;
		case FUNC_GETPIXELSIZE: //1画素あたりの大きさを取得
		{
			double dbPixel = GetValientDbl(&pDispParams->rgvarg[0]); //
			double dbRet = GetPixelSize(dbPixel);
			ReturnDbl(wFlags, pVarResult, dbRet);
		}
			break;
		case FUNC_SELECTFOLDER:	//フォルダ選択
			{
				CFolderDialog dlg;
				dlg.m_ofn.lpstrInitialDir = GetApp()->GetAppPath();
				dlg.DoModal();
				ReturnString(wFlags,pVarResult,dlg.m_strFolder); //戻り値セット
			}
			break;
		case FUNC_SELECTBITMAP:	//ビットマップファイル選択
			{
				CString strFileName = ::SelectBitmapFile(TRUE,GetApp()->GetAppPath());
				ReturnString(wFlags,pVarResult,strFileName); //戻り値セット
			}
			break;
		case FUNC_EXEC: //exeファイル実行
			if (pDispParams->cArgs == 1) {//引数が1つ
				CString strPath = pDispParams->rgvarg[0].bstrVal; //Exe File Path 
				::Exec(SW_HIDE,strPath.GetBuffer(),L"");
				ReturnBool(wFlags, pVarResult, TRUE);
			}
			break;
		case FUNC_READTEXT:	//文字列全行読み込み
			{
				m_aryText.RemoveAll();
				//引数が1つ
				CString strPath = pDispParams->rgvarg[0].bstrVal; //File Path

				CString line;
				FILE *pFile = fopen("result.txt", "r, ccs=UTF-8");
				if (pFile != NULL) {
					CStdioFile file(pFile);
					CString strLine;
					while (file.ReadString(strLine)) {
						m_aryText.Add(strLine);//１行づつ登録
					}
				}
				if (pFile != NULL) {
					fclose(pFile);
				}
			}
			break;
		case FUNC_READSTRING: //文字列１行読み込み
			{
				CString strPath = pDispParams->rgvarg[1].bstrVal; //File Path
				int nLine = pDispParams->rgvarg[0].iVal;
				CString strLine;
				if (nLine < m_aryText.GetCount()) {
					strLine = m_aryText.GetAt(nLine); //1行だけ文字列取り出し
				}
				ReturnString(wFlags, pVarResult, strLine); //戻り値セット
			}
			break;
	}
	return S_OK;
}

//コマンド

//全削除
void App::RemoveAll()
{
	GetView()->m_mgrVO.RemoveAll(); //描画要素Clear
}

//矩形描画
void App::DrawRect(CString strName,int nLeft,int nTop,int nRight,int nBottom,int nA,int nR,int nG,int nB,BOOL isPaint)
{
	CRect rect(nLeft,nTop,nRight,nBottom);
	Color color(nR,nG,nB);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,color,isPaint); //矩形描画
}
//楕円描画
void App::DrawCircle(CString strName, int nLeft, int nTop, int nRight, int nBottom, int nA, int nR, int nG, int nB, BOOL isPaint)
{
	CRect rect(nLeft, nTop, nRight, nBottom);
	Color color(nR, nG, nB);
	GetView()->m_mgrVO.CreateCircleObject(strName, rect, color, isPaint);
}
//ライン描画
void App::DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nA,int nR,int nG,int nB)
{
	DrawLine(strName,nXS,nYS,nXE,nYE,nR,nG,nB);
}
void App::DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nR,int nG,int nB)
{
	CLine line(nXS,nYS,nXE,nYE);
	Color color(nR,nG,nB);	
	GetView()->m_mgrVO.CreateLineObject(strName,line,color);//ライン描画
}
void App::DrawLine(CString strName, int nXS, int nYS, int nXE, int nYE, COLORREF cr)
{
	CLine line(nXS, nYS, nXE, nYE);
	Color color;
	color.SetFromCOLORREF(cr);
	GetView()->m_mgrVO.CreateLineObject(strName, line, color);//ライン描画
}

//マウスクリック待ち
BOOL App::Wait(DWORD dwTime)
{
	DWORD dwStart = GetTickCount();
	MSG msg;
	while(IsWindowOK() && GetTickCount() >= dwStart && GetTickCount() - dwStart < dwTime ){
        if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
			if( msg.message == WM_CLOSE || GetApp()->IsEnd() ){
				return FALSE; //異常終了
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
	}
	return TRUE;//正常終了
}

//マウスクリック待ち
BOOL App::ClickWait()
{
	MSG msg;
	while( IsWindowOK() ){
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CLOSE){
				return FALSE; //異常終了
			}
			if(WM_LBUTTONDOWN == msg.message || msg.message == WM_RBUTTONDOWN){
				return TRUE; //正常終了
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		Sleep(0);
	}
	return FALSE;//異常終了
}
//キーボードメッセージ待ち
BOOL App::KeyWait(int nKeyID)
{
	KIBOFrame *pFrame = ::GetFrame();
	MSG msg;
	while( IsWindowOK() ){
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_CLOSE){
				return FALSE; //異常終了
			}
			if(WM_KEYUP == msg.message){
				if(msg.wParam == nKeyID){
					return TRUE; //正常終了
				}else if(msg.wParam == VK_ESCAPE){ //ESC Key
					return FALSE; //キャンセル
				}
				
			}
			if(!::TranslateAccelerator(pFrame->GetSafeHwnd(), m_hAccel, &msg) ){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		Sleep(0);
		if( nKeyID == 0 ){ //ループ無し
			if( GetAsyncKeyState(VK_ESCAPE) < 0 ){
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;//異常終了
}

//全画面書き換え
void App::Render()
{
	GetView()->Render(TRUE);
}

//文字列取得
CString App::GetText(const CString &strKey)
{
	CString strValue = L"";
	m_mapString.Lookup(strKey,strValue);
	return strValue;
}
//文字列を記憶させる
void App::SetText(CString &strKey,CString &strVal)
{
	m_mapString.SetAt(strKey,strVal);
}
//値取得
int App::GetNum(const CString &strKey)
{
	CString strValue;
	if( m_mapString.Lookup(strKey,strValue) ){
		return _wtoi(strValue);
	}
	return 0;
}
//値設定
void App::SetNum(CString strKey,int nVal)
{
	CString strVal; //文字列変換
	strVal.Format(L"%d",nVal);
	m_mapString.SetAt(strKey,strVal);//文字列として記憶させる
}
//整数設定
void App::SetNum(int nVal,LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//改行付き文字列書き込み
	SetNum(strFormat,nVal);
}
//実数取得 
double App::GetDbl(const CString &strKey)
{
	CString strValue;
	if( m_mapString.Lookup(strKey,strValue) ){
		return _wtof(strValue);
	}
	return 0;
}
double App::GetDbl(LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//改行付き文字列書き込み
	return GetDbl(strFormat);
}
//実数設定
void App::SetDbl(CString strKey,double dbVal)
{
	CString strVal; //文字列変換
	strVal.Format(L"%f",dbVal);
	m_mapString.SetAt(strKey,strVal);//文字列として記憶させる
}
 //実数設定
void App::SetDbl(double dbVal,LPWSTR lpsz,...)
{
	//▼文字列クラス作成
	CString strFormat;
	va_list args;
	va_start(args, lpsz);
	strFormat.FormatV(lpsz, args);
	va_end(args);
	//改行付き文字列書き込み
	SetDbl(strFormat,dbVal);
}
//レジストリ値取得
int App::GetProfileInt(LPCWSTR strEntry) 
{
	return GetApp()->GetProfileInt(m_strSectionInt,strEntry,-1);
}
//レジストリ値設定
void App::SetProfileInt(LPCWSTR strEntry,int nValue) 
{
	GetApp()->WriteProfileInt(m_strSectionInt,strEntry,nValue);
}

//レジストリ値取得
double App::GetProfileDbl(LPCWSTR strEntry) 
{
	CString strVal = GetProfileString(strEntry);
	return _wtof(strVal);
}
//レジストリ値設定
void App::SetProfileDbl(LPCWSTR strEntry,double dbValue) 
{
	CString strValue;
	strValue.Format(L"%f",dbValue);
	SetProfileString(strEntry,strValue);
}

//レジストリ文字列取得
CString App::GetProfileString(LPCWSTR strEntry) 
{
	return GetApp()->GetProfileString(m_strSectionString,strEntry,L"");
}

//レジストリ文字列設定
void App::SetProfileString(LPCWSTR strEntry,CString& strValue) 
{
	GetApp()->WriteProfileString(m_strSectionString,strEntry,strValue);
}

//.exeファイル中の文字列取得
CString App::GetIniString(LPCTSTR lpszSec,LPCTSTR lpszKey,LPCTSTR lpszDef,LPCTSTR lpszFileName)
{
	CString strPath;
	strPath.Format(L"%s\\%s",GetApp()->GetAppPath(),lpszFileName);
	TCHAR szBuf[1024]; //1K
	ZeroMemory(szBuf,sizeof(szBuf));
	::GetPrivateProfileString(lpszSec,lpszKey,lpszDef,szBuf,sizeof(szBuf),strPath);
	return CString(szBuf);	
}
//座標表示倍率設定
void App::SetViewRate(double dbRateX,double dbRateY) 
{
	GetView()->SetViewRate(dbRateX,dbRateY);
}

//指定オブジェクト以外を削除
void App::RemoveExcept(LPCTSTR lpszName) 
{
	GetView()->m_mgrVO.RemoveExcept( lpszName ); 
}
void App::DrawText(LPCTSTR lpszID,LPCTSTR lpszText,int nX,int nY) 
{
	GetView()->m_mgrVO.CreateStringObject(lpszID,nX,nY,lpszText,GetTextColor(),GetLogFont()); //矩形描画
}
//メッセージ表示
void App::Message(int nNo,LPCTSTR lpszText,COLORREF color)
{
//	GetFrame()->DlgBarMsg(nNo,lpszText,color);
}
//メッセージ処理
int App::SendMessage(UINT unID,WPARAM wParam/*=0*/,LPARAM lParam/*=0*/)
{
	return (int)GetFrame()->SendMessage(unID,wParam,lParam); //Userメッセージ
}

//表示切り替え
void App::SetVisible(CString &strID,bool isVisible)
{
	VisualObject* pVO = GetView()->m_mgrVO.GetVO(strID);
	if( pVO != NULL ){
		pVO->SetVisible(isVisible);
	}
}

//文字列出力クリア
void App::OutputClear()
{
	GetOutputWnd()->Clear();
}
//文字列出力
void App::TextOutput(CString &str)
{
	GetOutputWnd()->OutputText(str);
}
void App::TextOutput(LPCTSTR lpszText)
{
	CString str(lpszText);
	GetOutputWnd()->OutputText(str);
}
//１画素あたりの大きさをmm単位で取得
double App::GetPixelSize(double dbPixel)
{
	CString strPath;
	strPath.Format(L"%s\\kibosystem.ini", GetApp()->GetAppPath());

	double dbDmin = DBL_MAX; //最小値
	double dbPerSelect = 0;//mm / pixel
	CString strSection = L"CALIBRATION"; //キャリブレーションセクション
	double dbPerPixel = ::GetPrivateProfileDbl(strSection, L"PERPIXEL", 0, strPath);
	return dbPerPixel;
}





CStringW ConvertUTF8ToUnicode(const CStringA& utf8)
{
	if (utf8.IsEmpty()) return L""; // nothing to do
	CStringW uni;
	int cc = 0;
	// get length (cc) of the new widechar excluding the \0 terminator first
	if ((cc = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0) - 1) > 0)
	{
		// convert
		wchar_t *buf = uni.GetBuffer(cc);
		if (buf) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, buf, cc);
		uni.ReleaseBuffer();
	}
	return uni;
}