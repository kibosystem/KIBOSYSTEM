// MainFrmEx.cpp : CKIBOFrame �N���X���̔�W���I�ȏ���
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
///�s�[�j���O�p����////////////////

////�摜�l�\���_�C�A���O
//void CKIBOFrame::OnImageValueDlg()
//{
//	CKIBOView *pView = GetView();
//	CameraEx *pCamera = pView->GetCameraPtr(0); //0�Ԗڂ̃J�����擾
//	if( pCamera != NULL ){
//		m_wndStatusBar.SetPaneText(0,_T("Esc �L�[�ŏI��"));
//		pCamera->KillTimer(); //�^�C�}�[���~
//		ImageRectDlg dlg(this);
//		dlg.DoModal();
//		pCamera->ReplayTimer(); //�^�C�}�[�ĊJ
//		m_wndStatusBar.SetPaneText(0,_T(""));
//	}
//}
//
////�摜�f�[�^�̕ۑ�
//void CKIBOFrame::OnSaveData()
//{
//	CKIBOView *pView = GetView();
//	CString strImagePath = pView->SaveCameraImage( GetApp()->GetAppPath() );//�J�����摜�ۑ�
//	if( !strImagePath.IsEmpty() ){		
//		CameraEx *pCamera = pView->GetCameraPtr(0);
//		if(pCamera != NULL){
//
//			strImagePath.Replace(_T(".bmp"),_T(".txt")); //�o�̓t�@�C�����쐬
//			FilePlus file;
//			file.Open(strImagePath.GetBuffer(),FilePlus::modeTextOut);
//
//			CDIB &dib = pCamera->GetDib(); //�J�����摜�擾
//			int nW = dib.GetWidth();
//			int nH = dib.GetHeight();
//			//�f�[�^�o��
//			for(int y=0;y<nH;y++){
//				BYTE *pBits = dib.GetPBitsB(0,nH-y-1); //�r�b�g�f�[�^�㉺���]
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

///�s�[�j���O�p����////////////////
////////////////////////////////////////////////
//
//////////////////////////////////////////////////
/////�L�Y�����p����////////////////
//
////�q�X�g�O����
//void CKIBOFrame::OnHistgram()
//{
//	Bitmap24 *pBmp = Bitmap24::GetInstance();
//	pBmp->FromCamera(0);
//	pBmp->ShowHistgram();
//}
//
////StatusBar�ɕ�����Z�b�g
//void CKIBOFrame::SetStatusBarText(int nIndex,LPCWSTR lpsz,...)
//{
//	//��������N���X�쐬
//	CString strFormat;
//	va_list args;
//	va_start(args, lpsz);
//	strFormat.FormatV(lpsz, args);
//	va_end(args);
//	//���s�t�������񏑂�����
//	GetStatusBarPtr()->SetPaneText(nIndex,strFormat);
//}
