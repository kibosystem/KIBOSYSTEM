#pragma once

#include "PointDbl.h"

#define PAI			(3.1415926535897932384626433832795)

//メッセージボックス表示　Type指定
extern int UIMessageBox(UINT nType,LPWSTR lpszText,...);

//メッセージボックス表示 IDOK専用
extern int UIMessageBox(LPWSTR lpszText,...);

//エラーメッセージ専用メッセージボックス
extern void ErrorMsg(LPWSTR lpszText, ...);


//////// 文字列 関連 ///////

extern int SplitToken(TCHAR cSep,CStringArray &sAry,CString& sText);//指定区切り文字で文字列配列作成
extern int SplitToken(CString strSep,CStringArray &sAry,CString sText);//指定区切り文字で文字列配列作成
extern CString GetStringInt(int n); //指定数値を文字列に変換します
extern CString Name(LPWSTR lpsz,int n);//連番名称取得

//////// File 関連 ///////

extern CString SelectBitmapFile(BOOL isOpen,LPCWSTR lpstrInitialDir);//ビットマップファイル選択


//////// Window 関連 ///////

//指定値より小さい値のランダムな数値を返す
extern int GetRandom(int nMax);

//シャットダウン権限取得
extern void ShutDownNameSet();

extern CString GetFileName(CString &strPath); //ファイル名称取得
extern CString GetFileTitle(CString &strPath); //ファイル名称取得
extern int GetFileSize(CString &strPath); //ファイルサイズ取得
extern CString GetLeft(CString& str,LPWSTR lpszSplit);//区切り文字以前の文字列を取得します。
extern CString GetRight(CString& str,LPWSTR lpszSplit);//区切り文字以降の文字列を取得します。

//二点間の距離を計算
extern double GetLength(CPoint &pt1,CPoint &pt2);
extern double GetLength(POINT pt1,POINT pt2);
extern double GetLength(double dbX1,double dbY1,double dbX2,double dbY2);

extern double GetAngle(CSize &vec1,CSize &vec2);//ベクトル角度取得
extern double GetAngle(PointDbl &vec1,PointDbl &vec2);
extern double GetCos(CSize &vec1,CSize &vec2);//コサイン取得
extern double GetSin(CSize &vec1,CSize &vec2);//サイン取得
extern CTime GetLastWriteTime(const CString &strPath);

//指定ファイル実行
extern int Exec(int nShow,LPWSTR lpszExec,LPWSTR lpszFile);

//.iniファイルから文字列取得
extern CString GetPrivateProfileText(LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszPath);
//.iniファイルから少数値取得
extern double GetPrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,double dbDefault,LPCWSTR lpszPath);
//.iniファイルに数値書き込み
extern void WritePrivateProfileInt(LPCWSTR lpszSection,LPCWSTR lpszKey,int nVal,LPCWSTR lpszPath);
//.iniファイルに実数書き込み
extern void WritePrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,LPCTSTR lpszFormat,double dbVal,LPCWSTR lpszPath);

//色計算
struct YSH
{
	double m_dbY;
	double m_dbS;
	double m_dbH;
};
extern YSH GetYSH(BYTE byR, BYTE byG, BYTE byB); //RGB -> YSH

extern void KeyAction(WORD VirtualKey, BOOL bKeepPressing);//キー入力