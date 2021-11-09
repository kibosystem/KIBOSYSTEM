#pragma once
#include "ScriptBase.h"
#include "../LIB/color.h"
#include "../LIB/typedef.h"

//アプリケーションアクセスクラス
class App : public ScriptBase 
{
private:
	HWND				m_hWnd;	  //メインウィンドウハンドル
	COLORREF			m_crText; //文字色の色
	LOGFONT				m_lf;	  //フォントデータ
	CMapStringToString	m_mapString; //汎用文字列配列	
	CRect				m_rect;		 //汎用領域
	ULONGLONG			m_ulCount; //時間計測カウンター
public:
	HACCEL				m_hAccel; //アクセラレータキー
	CString m_strSectionInt;//レジストリ値セクション
	CString m_strSectionString;//レジストリ文字列セクション

	StringArrayPlus m_aryText;
public:
	enum{
		 FUNC_WAIT = 1		//指定時間待ち
		,FUNC_CLICK_WAIT	//マウスクリック待ち
		,FUNC_KEY_WAIT		//キーボードメッセージ待ち
		,FUNC_COUNTSTART	//時間計測開始
		,FUNC_COUNTEND		//時間計測終了
		,FUNC_STARTSCRIPTTIMER  //スクリプトタイマー開始
		,FUNC_STOPSCRIPTTIMER   //スクリプトタイマー停止
		,FUNC_SET_REPEAT_SCRIPT //繰り返しスクリプトファイル設定
		,FUNC_SETWINDOWTEXT		//メインWindow Caption変更
		,FUNC_DRAWRECT		//矩形描画 10
		,FUNC_DRAWMARK		//指定マーク描画
		,FUNC_DRAWLINE		//指定ライン描画
		,FUNC_PAINTMASK		//マスク領域描画
		,FUNC_RENDER		//レンダリング
		,FUNC_REMOVEALL		//全削除
		,FUNC_REMOVE		//指定オブジェクト削除
		,FUNC_REMOVEEXCEPT  //指定オブジェクト以外を削除
		,FUNC_SETTEXTCOLOR	//文字色の設定
		,FUNC_SETFONTNAME	//Font名称設定
		,FUNC_SETFONTSIZE	//FontSize設定 20
		,FUNC_DRAWTEXT		//文字列設定
		,FUNC_DRAWCHAR		//文字描画
		,FUNC_SETVISIBLE	//表示ON・OFF切り替え
		,FUNC_SETTEXTRECT	//文字列表示領域指定
		,FUNC_BINARYIMAGE	//2値化画像
		,FUNC_SETKEYFILE	//コマンドファイルのキー割り当て
		,FUNC_GETBITMAP		//指定フォルダ内ビットマップの取得
		,FUNC_GETBITMAPNUM	//指定フォルダ内ビットマップ数の取得
		,FUNC_TEXTOUTPUT	//文字列出力
		,FUNC_MESSAGE		//メッセージ表示 30
		,FUNC_SETSTATUSBARTEXT//ステータスバー文字セット
		,FUNC_OUTPUTCLEAR	//文字列出力クリア
		,FUNC_SETVIEWRATE	//表示倍率設定
		,FUNC_GETVIEWRATECX	//表示倍率取得 x方向 
		,FUNC_GETVIEWRATECY	//表示倍率取得 y方向
		,FUNC_SHOWWINDOW	//メインウィンドウ表示状態変更
		,FUNC_MOVEWINDOW	//メインウィンドウ表示サイズ変更
		,FUNC_VIEWHEIGHT	//結果出力高さ変更	
		,FUNC_VIEWWIDTH		//結果出力幅変更
		,FUNC_SETTEXT	//汎用文字列記憶
		,FUNC_GETTEXT	//汎用文字列取得
		,FUNC_SETNUM	//汎用数値設定
		,FUNC_GETNUM	//汎用数値取得
		,FUNC_SETDBL	//汎用実数設定
		,FUNC_GETDBL	//汎用実数取得
		,FUNC_CREATE_FOLDER	//フォルダ作成
		,FUNC_SENDMESSAGE	//コマンド呼び出し
		,FUNC_POSTMESSAGE	//コマンド呼び出し
		,FUNC_SETRECT		//領域設定
		,FUNC_GETTIME		//マルティメデアタイマー取得
		,FUNC_GETVALUE		//指定値取得
		,FUNC_SETLINECOLOR	//線色設定
		,FUNC_GETPRIVATEPROFILEINT   //.iniファイル値取得
		,FUNC_WRITEPRIVATEPROFILEINT //.iniファイル値書き込み
		,FUNC_GETPRIVATEPROFILEDBL		//.iniファイルDbl値取得
		,FUNC_WRITEPRIVATEPROFILEDBL	//.iniファイルDbl値書き込み
		,FUNC_GETPRIVATEPROFILESTRING	//.iniファイル文字列取得
		,FUNC_WRITEPRIVATEPROFILESTRING	//.iniファイル文字列書き込み
		,FUNC_GETPROFILEINT	//レジストリ値取得
		,FUNC_WRITEPROFILEINT	//レジストリ値設定
		,FUNC_GETPROFILEDBL	//レジストリ値実数取得
		,FUNC_WRITEPROFILEDBL	//レジストリ値実数設定
		,FUNC_GETPROFILESTRING	//レジストリ値取得
		,FUNC_WRITEPROFILESTRING	//レジストリ値設定
		,FUNC_SETINPUTNAME		//GUI入力値変更
		,FUNC_CENTERPOINT	//中心点表示・非表示
		,FUNC_ENABLEWINDOW	//Menu＆ボタン Enable/DisEnable
		,FUNC_GETPIXELSIZE	//1画素あたりの大きさを取得
		,FUNC_SELECTFOLDER	//フォルダ選択
		,FUNC_SELECTBITMAP	//ビットマップファイル選択
		,FUNC_EXEC			//EXE実行
		,FUNC_READTEXT		//文字列全行読み込み
		,FUNC_READSTRING	//文字列１行読み込み
		,FUNC_MAX			//最大数
	};
	HWND GetWindow(){return m_hWnd;}
	void SetWindow(HWND h){m_hWnd = h;}
	void SetTextColor(COLORREF cr){m_crText=cr;}//文字色設定
	COLORREF GetTextColor(){return m_crText;}	//文字色取得
	LOGFONT& GetLogFont(){return m_lf;} //フォントデータ取得
	void SetRect(int nLeft,int nTop,int nRight,int nBottom){ //領域設定
		m_rect.SetRect(nLeft,nTop,nRight,nBottom);
	}
	CRect GetRect(){return m_rect;} //領域取得
	double GetPixelSize(double dbPixel);//１画素あたりの大きさをmm単位で取得
public:
	App(void);
	virtual ~App(void);

	static App* GetInstance();
	//関数名チェック
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
							,UINT cNames ,LCID lcid
							,DISPID __RPC_FAR *rgDispId);

	//実行
	virtual STDMETHODIMP Invoke(DISPID dispIdMember	//Function ID 番号
						,REFIID riid
						,LCID lcid
						,WORD wFlags
						,DISPPARAMS FAR *pDispParams //Script文字列
						,VARIANT FAR *pVarResult
						,EXCEPINFO FAR *pExcepInfo
						,unsigned int FAR *puArgErr);

	CString GetText(const CString &strKey);//文字列取得
	void SetText(CString &strKey,CString &strVal); //文字列設定
	int  GetNum(const CString &strKey); //値取得	
	void SetNum(CString strKey,int nVal); //整数設定
	void SetNum(int nVal,LPWSTR lpsz,...);//整数設定
	double GetDbl(const CString &strKey);//実数取得 
	double GetDbl(LPWSTR lpsz,...);//実数取得 
	void   SetDbl(CString strKey,double dbVal); //実数設定
	void   SetDbl(double dbVal,LPWSTR lpsz,...); //実数設定
	int GetProfileInt(LPCWSTR strEntry); //レジストリ値取得
	void SetProfileInt(LPCWSTR strEntry,int nVal); //レジストリ値設定
	double GetProfileDbl(LPCWSTR strEntry); //レジストリ値実数取得
	void SetProfileDbl(LPCWSTR strEntry,double dbVal); //レジストリ値実数設定
	CString GetProfileString(LPCWSTR strEntry);//レジストリ文字列取得
	void SetProfileString(LPCWSTR strEntry,CString& strVal);//レジストリ文字列設定	
	CString GetIniString(LPCTSTR lpszSec,LPCTSTR lpszKey,LPCTSTR lpszDef,LPCTSTR lpszFileName);//.exeファイル中の文字列取得
	BOOL Wait(DWORD dwTime);//指定時間待ち
	BOOL ClickWait();		//マウスクリック待ち
	BOOL KeyWait(int nKeyID);//キーボードメッセージ待ち	
	void DrawCircle(CString strName, int nLeft, int nTop, int nRight, int nBottom, int nA, int nR, int nG, int nB, BOOL isPaint);//楕円描画
	void DrawRect(CString strName,int nLeft,int nTop,int nRight,int nBottom,int nA,int nR,int nG,int nB,BOOL isPaint);	//矩形描画	
	void DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nA,int nR,int nG,int nB);//ライン描画
	void DrawLine(CString strName,int nXS,int nYS,int nXE,int nYE,int nR,int nG,int nB);//ライン描画
	void DrawLine(CString strName, int nXS, int nYS, int nXE, int nYE, COLORREF cr);//ライン描画
	void Render();		//レンダリング
	void RemoveAll();	//全削除		
	void SetViewRate(double dbRateX,double dbRateY);	
	void RemoveExcept(LPCTSTR lpszName); //指定オブジェクト以外を削除
	void DrawText(LPCTSTR lpszID,LPCTSTR lpszText,int nX,int nY);
	void Message(int nNo,LPCTSTR lpszText,COLORREF color);	
	int SendMessage(UINT unID,WPARAM wParam=0,LPARAM lParam=0);//メッセージ処理
	void SetVisible(CString &strID,bool isVisible);//表示切り替え	
	void OutputClear();//文字列出力クリア
	void TextOutput(CString &str);//文字列出力
	void TextOutput(LPCTSTR lpszText);
protected: //virtual
	virtual BOOL CenterPoint(BOOL isVisible){return FALSE;}
	virtual BOOL EnableWindow(BOOL isEnable){return FALSE;}
};