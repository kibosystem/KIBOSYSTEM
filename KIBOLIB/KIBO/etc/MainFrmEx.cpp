// MainFrmEx.cpp : CKIBOFrame クラス中の非標準的な処理
//

#include "../stdafx.h"
#include "../KIBO.h"
#include "../enum.h"
#include "../KIBODoc.h"
#include "../KIBOView.h"
#include "../etc/DibEx.h"
#include "../LIB/FilePlus.h"
#include "../SCript/Bitmap.h"
#include "../SCript/SerialPort.h"


////////////////////////////////////////////////
///ピーニング用処理////////////////

////画像値表示ダイアログ
//void CKIBOFrame::OnImageValueDlg()
//{
//	CKIBOView *pView = GetView();
//	CameraEx *pCamera = pView->GetCameraPtr(0); //0番目のカメラ取得
//	if( pCamera != NULL ){
//		m_wndStatusBar.SetPaneText(0,_T("Esc キーで終了"));
//		pCamera->KillTimer(); //タイマー中止
//		ImageRectDlg dlg(this);
//		dlg.DoModal();
//		pCamera->ReplayTimer(); //タイマー再開
//		m_wndStatusBar.SetPaneText(0,_T(""));
//	}
//}
//
////画像データの保存
//void CKIBOFrame::OnSaveData()
//{
//	CKIBOView *pView = GetView();
//	CString strImagePath = pView->SaveCameraImage( GetApp()->GetAppPath() );//カメラ画像保存
//	if( !strImagePath.IsEmpty() ){		
//		CameraEx *pCamera = pView->GetCameraPtr(0);
//		if(pCamera != NULL){
//
//			strImagePath.Replace(_T(".bmp"),_T(".txt")); //出力ファイル名作成
//			FilePlus file;
//			file.Open(strImagePath.GetBuffer(),FilePlus::modeTextOut);
//
//			CDIB &dib = pCamera->GetDib(); //カメラ画像取得
//			int nW = dib.GetWidth();
//			int nH = dib.GetHeight();
//			//データ出力
//			for(int y=0;y<nH;y++){
//				BYTE *pBits = dib.GetPBitsB(0,nH-y-1); //ビットデータ上下反転
//				for(int x=0;x<nW;x++){
//					BYTE byB = *pBits++;//B
//					BYTE byG = *pBits++;//G
//					BYTE byR = *pBits++;//R
//					BYTE byGray = (BYTE)(0.299*byR + 0.587*byG + 0.114*byB + 0.5);
//					file.WriteFormat(_T("%u,"),byGray);
//				}
//				file.WriteString(_T("\n"));
//			}
//		}
//	}
//}

///ピーニング用処理////////////////
////////////////////////////////////////////////
//
//////////////////////////////////////////////////
/////キズ検査用処理////////////////
//
////ヒストグラム
//void CKIBOFrame::OnHistgram()
//{
//	Bitmap24 *pBmp = Bitmap24::GetInstance();
//	pBmp->FromCamera(0);
//	pBmp->ShowHistgram();
//}
//
////StatusBarに文字列セット
//void CKIBOFrame::SetStatusBarText(int nIndex,LPCWSTR lpsz,...)
//{
//	//▼文字列クラス作成
//	CString strFormat;
//	va_list args;
//	va_start(args, lpsz);
//	strFormat.FormatV(lpsz, args);
//	va_end(args);
//	//改行付き文字列書き込み
//	GetStatusBarPtr()->SetPaneText(nIndex,strFormat);
//}
