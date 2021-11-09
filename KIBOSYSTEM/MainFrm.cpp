
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "KIBOSYSTEM.h"
#include "MainFrm.h"
#include "BinaryDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
inline CMainFrame* GetMainFrame()
{
	return (CMainFrame*)AfxGetMainWnd();
}

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(IDM_USER, IDM_USER_MAX, OnCommandFile)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(ID_BINARY_DLG, OnBinaryDialog)  //�\��-2�l��
	ON_MESSAGE(ID_INIT, OnInit)  //����������
	ON_MESSAGE(ID_CALIBRATION, OnCalibration) //�L�����u���[�V����
	ON_COMMAND(IDM_CAMERAFILE_SAVE,OnCameraFile) //�J�����摜�ۑ�
	ON_UPDATE_COMMAND_UI(IDM_CAMERAFILE_SAVE,OnUpdateEnable)

	ON_MESSAGE(ID_USER1, OnUser1)	//���[�U�[�R�}���h1 //
	ON_MESSAGE(ID_USER2, OnUser2)	//���[�U�[�R�}���h2 //
	ON_MESSAGE(ID_USER3, OnUser3)	//���[�U�[�R�}���h3	//
	ON_MESSAGE(ID_USER4, OnUser4)	//���[�U�[�R�}���h4 //
	ON_MESSAGE(ID_USER5, OnUser5)	//���[�U�[�R�}���h5 //
	ON_MESSAGE(ID_USER6, OnUser6)	//���[�U�[�R�}���h6 //
	ON_MESSAGE(ID_USER7, OnUser7)	//���[�U�[�R�}���h7 //
	ON_MESSAGE(ID_USER8, OnUser8)	//���[�U�[�R�}���h8 //
	ON_MESSAGE(ID_USER9, OnUser9)	//���[�U�[�R�}���h9 //
	ON_MESSAGE(ID_USERA, OnUserA)	//���[�U�[�R�}���hA //
	ON_MESSAGE(ID_USERB, OnUserB)	//���[�U�[�R�}���hB //
	ON_MESSAGE(ID_USERC, OnUserC)	//���[�U�[�R�}���hC //
	ON_MESSAGE(ID_USERD, OnUserD)	//���[�U�[�R�}���hD 

	ON_COMMAND(ID_HOME, OnHome)
	ON_COMMAND(ID_A, OnA)
	ON_COMMAND(ID_B, OnB)
	ON_COMMAND(ID_C, OnC)
	ON_COMMAND(ID_D, OnD)
	ON_COMMAND(ID_E, OnE)
	ON_COMMAND(ID_F, OnF)
	ON_COMMAND(ID_G, OnG)
	ON_COMMAND(ID_H, OnH)
	ON_COMMAND(ID_I, OnI)
	ON_COMMAND(ID_J, OnJ)
	ON_COMMAND(ID_K, OnK)
	ON_COMMAND(ID_L, OnL)
	ON_COMMAND(ID_M, OnM)
	ON_COMMAND(ID_N, OnN)
	ON_COMMAND(ID_O, OnO)
	ON_COMMAND(ID_P, OnP)
	ON_COMMAND(ID_Q, OnQ)
	ON_COMMAND(ID_R, OnR)
	ON_COMMAND(ID_S, OnS)
	ON_COMMAND(ID_T, OnT)
	ON_COMMAND(ID_U, OnU)
	ON_COMMAND(ID_V, OnV)
	ON_COMMAND(ID_W, OnW)
	ON_COMMAND(ID_X, OnX)
	ON_COMMAND(ID_Y, OnY)
	ON_COMMAND(ID_Z, OnZ)
	ON_COMMAND(ID_F1, OnF1)
	ON_COMMAND(ID_F2, OnF2)
	ON_COMMAND(ID_F3, OnF3)
	ON_COMMAND(ID_F4, OnF4)
	ON_COMMAND(ID_F5, OnF5)
	ON_COMMAND(ID_F6, OnF6)
	ON_COMMAND(ID_F7, OnF7)
	ON_COMMAND(ID_F8, OnF8)
	ON_COMMAND(ID_F9, OnF9)
	ON_COMMAND(ID_F10, OnF10)
	ON_COMMAND(ID_F11, OnF11)
	ON_COMMAND(ID_F12, OnF12)

	ON_COMMAND(ID_RIGHT, OnRight)
	ON_COMMAND(ID_LEFT, OnLeft)
	ON_COMMAND(ID_UP, OnUp)
	ON_COMMAND(ID_DOWN, OnDown)

	ON_COMMAND(IDM_800, On800)
	ON_COMMAND(IDM_1024, On1024)
	ON_COMMAND(IDM_2048, On2048)
	ON_COMMAND(IDM_2592, On2592)
	ON_COMMAND(IDM_STREATCH, OnStreatch)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^�[
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

void CMainFrame::OnUpdateEnable(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

static ArrayPointDbl s_ptAryPoint1;
static ArrayPointDbl s_ptAryPoint2;
static LabelingCV s_labelCV;

//�T���v���P
LRESULT CMainFrame::OnUser1(WPARAM wParam, LPARAM lParam)
{
	s_ptAryPoint1.RemoveAll();
	s_ptAryPoint2.RemoveAll();

	App *pApp = App::GetInstance();
	ImageDevice  *pImageDevice = ImageDevice::GetInstance();
	CDIB &dib = pImageDevice->GetDib();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();

	dib.MaskRect(CRect(0, 0, nW, 400), COLOR_WHITE); //��400�����
	dib.MaskRect(CRect(500, 400, nW, nH), COLOR_WHITE); //500�ȍ~�̉E�����
	dib.MaskRect(CRect(0, 400, 268, nH), COLOR_WHITE); //��268�����

	CRect rect;
	rect.left = 268;
	rect.top = 400;
	rect.right = 500;
	rect.bottom = nH;


	//�\�[�X�摜�擾
	int nThMax = pApp->GetNum(L"�ő債�����l");
	int nThMin = pApp->GetNum(L"�ŏ��������l");
	int nThDec = pApp->GetNum(L"�f�N�������g");
	int nPixMin = pApp->GetNum(L"�ʐύŏ��l");
	int nPixMax = pApp->GetNum(L"�ʐύő�l");
	int nWidthMax = pApp->GetNum(L"width");
	int nDeltaY = pApp->GetNum(L"DeltaY");
	CString strText;

	//�򌟏o
	for (int nTh = nThMax; nTh > nThMin; nTh -= nThDec) { //�������l�ϓ�
		s_labelCV.Init(dib, nTh, THRESH_BINARY_INV); //�����o
		int nEnum = s_labelCV.Exec(rect);
		for (int i = 0; i < nEnum; ++i) {
			int nPixels = s_labelCV.GetArea(i); //�ʐώ擾

			//�ʐϒl�̏o��
			CString strText;
			if (nPixMin < nPixels && nPixels < nPixMax) {
				Point2d pt = s_labelCV.GetCentroids(i);
				double x = pt.x + rect.left;
				double y = pt.y + rect.top;

				int height = s_labelCV.GetHeight(i); //�����擾
				int width = s_labelCV.GetWidth(i); //���擾
				if (width < nWidthMax) {
					//if ((nLeft <= x && x <= nRight) || (nLeft <= x + width && x + width <= nRight) && nTop < y) {
					if (TRUE) {
						//�d�S�̏o��
						s_ptAryPoint1.AddPoint(x, y); //�d�S�o�^

						strText.Format(L"BLUE Th %d = %d w = %d x,y=%.2f,%.2f", nTh, nPixels, width, x, y);
						m_wndOutput.OutputText(strText);
					}
				}
			}
		}
		//break;
	}

	//�]���o�^
	for (int nTh = nThMax; nTh > nThMin; nTh -= nThDec) {
		s_labelCV.Init(dib, nTh, THRESH_BINARY); //�����o
		int nEnum = s_labelCV.Exec(rect);
		CString strText;
		for (int i = 0; i < nEnum; ++i) {
			int nPixels = s_labelCV.GetArea(i); //�ʐώ擾

			if (nPixMin < nPixels && nPixels < nPixMax) {
				Point2d pt = s_labelCV.GetCentroids(i);
				double x = pt.x + rect.left;
				double y = pt.y + rect.top;
				int height = s_labelCV.GetHeight(i); //�����擾
				int width = s_labelCV.GetWidth(i); //���擾

				if (width < nWidthMax) {
					//if ((nLeft <= x && x <= nRight) || (nLeft <= x + width && x + width <= nRight) && nTop < y) {
					if (TRUE) {

						//�d�S�̏o��
						s_ptAryPoint2.AddPoint(x, y);

						strText.Format(L"RED Th %d = %d w = %d x,y=%.2f,%.2f", nTh, nPixels, width, x, y);
						m_wndOutput.OutputText(strText);
					}
				}
			}
		}
		//break;
	}
	//

	//���x�����O���ʂ���d�����폜
	for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) { //����Y���W���폜
		PointDbl &pt_i = s_ptAryPoint1[i];
		for (int j = (int)s_ptAryPoint1.GetSize() - 1; j >= 0; j--) {
			if (i == j) {
				continue;
			}
			PointDbl &pt_j = s_ptAryPoint1[j];
			double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
			if (dbDelta < nDeltaY) {
				s_ptAryPoint1.RemoveAt(j);
			}
		}
	}
	for (int i = 0; i < s_ptAryPoint2.GetSize(); i++) { //����Y���W���폜
		PointDbl &pt_i = s_ptAryPoint2[i];
		for (int j = (int)s_ptAryPoint2.GetSize() - 1; j >= 0; j--) {
			if (i == j) {
				continue;
			}
			PointDbl &pt_j = s_ptAryPoint2[j];
			double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
			if (dbDelta < nDeltaY) {
				s_ptAryPoint2.RemoveAt(j);
			}
		}
	}
	for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) { //����Y���W���폜
		PointDbl &pt_i = s_ptAryPoint1[i];
		TRACE("pt_i = %.2f\n", pt_i.y);
		for (int j = (int)s_ptAryPoint2.GetSize() - 1; j >= 0; j--) {
			PointDbl &pt_j = s_ptAryPoint2[j];
			TRACE("j(%d) = %.2f\n", j, pt_j.y);
			double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
			if (dbDelta < nDeltaY) {
				s_ptAryPoint2.RemoveAt(j);
			}
		}
	}

	LOGBRUSH lbB;
	lbB.lbColor = COLOR_BLUE; //�F�w��
	lbB.lbHatch = 0;
	lbB.lbStyle = BS_SOLID; //NULL�u���V
	LOGPEN lpB;
	lpB.lopnStyle = PS_SOLID;
	lpB.lopnWidth.x = 1;
	lpB.lopnColor = COLOR_BLUE; //�F�w��
	for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) {
		PointDbl &pt = s_ptAryPoint1[i];
		dib.DrawRectangle(CRect((int)pt.x - 5, (int)pt.y - 5, (int)pt.x + 5, (int)pt.y + 5), lbB, lpB);
		strText.Format(L"BLUE pt.x,pt.y = %.2f , %.2f", pt.x, pt.y);
		m_wndOutput.OutputText(strText);

	}
	LOGBRUSH lbR;
	lbR.lbColor = COLOR_RED;
	lbR.lbHatch = 0;
	lbR.lbStyle = BS_SOLID; //NULL�u���V
	LOGPEN lpR;
	lpR.lopnStyle = PS_SOLID;
	lpR.lopnWidth.x = 1;
	lpR.lopnColor = COLOR_RED; //�F�w��
	for (int i = 0; i < s_ptAryPoint2.GetSize(); i++) {
		PointDbl &pt = s_ptAryPoint2[i];
		dib.DrawRectangle(CRect((int)pt.x - 5, (int)pt.y - 5, (int)pt.x + 5, (int)pt.y + 5), lbR, lpR);
		strText.Format(L"RED pt.x,pt.y = %.2f , %.2f", pt.x, pt.y);
		m_wndOutput.OutputText(strText);
	}

	int nNum = (int)s_ptAryPoint1.GetSize() + (int)s_ptAryPoint2.GetSize();
	strText.Format(L"�� %d ", nNum);
	m_wndOutput.OutputText(strText);

	return nNum;
}



LRESULT CMainFrame::OnUser2(WPARAM wParam, LPARAM lParam)
{
	s_ptAryPoint1.RemoveAll();
	s_ptAryPoint2.RemoveAll();

	App *pApp = App::GetInstance();
	ImageDevice  *pImageDevice = ImageDevice::GetInstance();
	CDIB &dib = pImageDevice->GetDib();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();

	dib.MaskRect(CRect(0, 0, nW, 400), COLOR_WHITE);
	dib.MaskRect(CRect(500, 400, nW, nH), COLOR_WHITE);
	dib.MaskRect(CRect(0, 400, 268, nH), COLOR_WHITE);


	//�\�[�X�摜�擾
	int nThMax = pApp->GetNum(L"�ő債�����l");
	int nThMin = pApp->GetNum(L"�ŏ��������l");
	int nThDec = pApp->GetNum(L"�f�N�������g");
	int nPixMin = pApp->GetNum(L"�ʐύŏ��l");
	int nPixMax = pApp->GetNum(L"�ʐύő�l");
	int nWidthMax = pApp->GetNum(L"width");
	int nDeltaY = pApp->GetNum(L"DeltaY");
	CString strText;

	CRect rect;
	rect .left = 268;
	rect.top  = 400;
	rect.right  = 500;
	rect.bottom  = nH;

	//�򌟏o
	for (int nTh = nThMax; nTh > nThMin; nTh -= nThDec) { //�������l�ϓ�
		s_labelCV.Init(dib, nTh, THRESH_BINARY_INV);
		int nLab =	s_labelCV.Exec(rect, 4); //�̈�w�胉�x�����O

		//// ���x�����O���ʂ̕`��
		//cv::Mat dst(matSrc.size(), CV_8UC3);
		//dib.ToMat(dst);
		for (int i = 1; i < nLab; ++i) {
			//�ʐϒl�̏o��
			CString strText;

			cv::Mat &stats = s_labelCV.GetStats();
			int *param = stats.ptr<int>(i);
			int nPixels = param[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�

			if (nPixMin < nPixels && nPixels < nPixMax) {
				cv::Mat &centroids = s_labelCV.GetCentroids();

				int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT]+ rect.left ;
				int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP]+ rect.top ;
				int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
				int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
				if (width < nWidthMax) {
					if ((rect.left  <= x && x <= rect.right ) || (rect.left <= x + width && x + width <= rect.right ) && rect.top  < y) {
						//if (TRUE) {
						//nLeft = x;
						//nRight = x + width;
						//nTop = y;

						//�d�S�̏o��
						double *pCentroids = centroids.ptr<double>(i);
						x = static_cast<int>(pCentroids[0]) + rect.left ;
						y = static_cast<int>(pCentroids[1]) + rect.top;
						s_ptAryPoint1.AddPoint(x, y); //�d�S�o�^

						strText.Format(L"BLUE Th = %d Pixels = %d w = %d x,y=%d,%d",nTh, nPixels, width, x, y);
						m_wndOutput.OutputText(strText);
						//}
					}
				}
			}
		}
	}




	//	for (int i = 0; i < nEnum; ++i) {
	//		//�ʐϒl�̏o��
	//		CString strText;
	//		Element *pE = label.GetElement(i);
	//		int nPixels = pE->GetPixels();

	//		if (nPixMin < nPixels && nPixels < nPixMax) {
	//			double x = pE->GetGx();
	//			double y = pE->GetGy();
	//			int height = pE->GetHeight();
	//			int width = pE->GetWidth();
	//			if (width < nWidthMax) {
	//				//if ((nLeft <= x && x <= nRight) || (nLeft <= x + width && x + width <= nRight) && nTop < y) {
	//				if (TRUE) {
	//					nLeft = (int)x;
	//					nRight = (int)x + width;
	//					nTop = (int)y;

	//					//�d�S�̏o��
	//					s_ptAryPoint1.AddPoint(x, y); //�d�S�o�^

	//					strText.Format(L"BLUE Th %d = %d w = %d x,y=%.2f,%.2f", nTh, nPixels, width, x, y);
	//					m_wndOutput.OutputText(strText);
	//				}
	//			}
	//		}
	//	}
	//	//break;
	//}










	//s_ptAryPoint1.RemoveAll();

	//App *pApp = App::GetInstance();
	//ImageDevice  *pImageDevice = ImageDevice::GetInstance();
	//CDIB &dib = pImageDevice->GetDib();
	//int nW = dib.GetWidth();
	//int nH = dib.GetHeight();
	//Mat matSrc;
	//matSrc.create(nH, nW, CV_8UC1); //GRAYSCALE


	//dib.MaskRect(CRect(0, 0, nW, 400), COLOR_WHITE);
	//dib.MaskRect(CRect(600, 0, nW, nH), COLOR_WHITE);


	////�\�[�X�摜�擾
	//int nThMax = pApp->GetNum(L"�ő債�����l");
	//int nThMin = pApp->GetNum(L"�ŏ��������l");
	//int nThDec = pApp->GetNum(L"�f�N�������g");
	//int nPixMin = pApp->GetNum(L"�ʐύŏ��l");
	//int nPixMax = pApp->GetNum(L"�ʐύő�l");
	//int nWidthMax = pApp->GetNum(L"width");
	//int nDeltaY = pApp->GetNum(L"DeltaY");
	//CString strText;

	//int nLeft = 0;
	//int nRight = nW;
	//int nTop = 0;
	////�򌟏o
	//for (int nTh = nThMax; nTh > nThMin; nTh -= nThDec) { //�������l�ϓ�
	//	dib.ToMat(matSrc);
	//	// ��l��
	//	Mat matBin;
	//	cv::threshold(matSrc, matBin, nTh, 255, cv::THRESH_BINARY_INV /*| cv::THRESH_OTSU*/);

	//	cv::Mat matImg(matSrc.size(), CV_16U);
	//	//���׃����O����
	//	cv::Mat stats;
	//	cv::Mat centroids;
	//	int nLab = cv::connectedComponentsWithStats(matBin, matImg, stats, centroids);

	//	//// ���x�����O���ʂ̕`��
	//	//cv::Mat dst(matSrc.size(), CV_8UC3);
	//	//dib.ToMat(dst);
	//	for (int i = 1; i < nLab; ++i) {
	//		//�ʐϒl�̏o��
	//		CString strText;
	//		int *param = stats.ptr<int>(i);
	//		int nPixels = param[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�

	//		if (nPixMin < nPixels && nPixels < nPixMax) {
	//			int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
	//			int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
	//			int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
	//			int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
	//			if (width < nWidthMax) {
	//				if ((nLeft <= x && x <= nRight) || (nLeft <= x + width && x + width <= nRight) && nTop < y) {
	//					//if (TRUE) {
	//					nLeft = x;
	//					nRight = x + width;
	//					nTop = y;

	//					//�d�S�̏o��
	//					double *pCentroids = centroids.ptr<double>(i);
	//					x = static_cast<int>(pCentroids[0]);
	//					y = static_cast<int>(pCentroids[1]);
	//					s_ptAryPoint1.AddPoint(x, y); //�d�S�o�^

	//					strText.Format(L"BLUE area %d = %d w = %d x,y=%d,%d", i, nPixels, width, x, y);
	//					m_wndOutput.OutputText(strText);
	//				}
	//			}
	//		}
	//	}
	//}

	////�]���o�^
	//for (int nTh = nThMax; nTh > nThMin; nTh -= nThDec) {

	//	dib.ToMat(matSrc);
	//	// ��l��
	//	Mat matBin;
	//	cv::threshold(matSrc, matBin, nTh, 255, cv::THRESH_BINARY /*| cv::THRESH_OTSU*/);

	//	cv::Mat matImg(matSrc.size(), CV_16U);
	//	//���׃����O����
	//	cv::Mat stats;
	//	cv::Mat centroids;
	//	int nLab = cv::connectedComponentsWithStats(matBin, matImg, stats, centroids);

	//	CString strText;
	//	for (int i = 1; i < nLab; ++i) {
	//		int *param = stats.ptr<int>(i);
	//		int nPixels = param[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�

	//		if (nPixMin < nPixels && nPixels < nPixMax) {
	//			int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
	//			int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
	//			int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
	//			int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
	//			if (width < nWidthMax) {
	//				if ((nLeft <= x && x <= nRight) || (nLeft <= x + width && x + width <= nRight) && nTop < y) {
	//					//if (TRUE) {
	//					//strText.Format(L"RED area %d = %d w = %d", i, nPixels, width);
	//					//m_wndOutput.OutputText(strText);

	//					nLeft = x;
	//					nRight = x + width;
	//					nTop = y;

	//					//�d�S�̏o��
	//					double *pCentroids = centroids.ptr<double>(i);
	//					x = static_cast<int>(pCentroids[0]);
	//					y = static_cast<int>(pCentroids[1]);

	//					s_ptAryPoint2.AddPoint(x, y);
	//				}
	//			}
	//		}
	//	}
	//}
	////

	////���x�����O���ʂ���d�����폜
	//for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) { //����Y���W���폜
	//	PointDbl &pt_i = s_ptAryPoint1[i];
	//	for (int j = (int)s_ptAryPoint1.GetSize() - 1; j >= 0; j--) {
	//		if (i == j) {
	//			continue;
	//		}
	//		PointDbl &pt_j = s_ptAryPoint1[j];
	//		double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
	//		if (dbDelta < nDeltaY) {
	//			s_ptAryPoint1.RemoveAt(j);
	//		}
	//	}
	//}
	//for (int i = 0; i < s_ptAryPoint2.GetSize(); i++) { //����Y���W���폜
	//	PointDbl &pt_i = s_ptAryPoint2[i];
	//	for (int j = (int)s_ptAryPoint2.GetSize() - 1; j >= 0; j--) {
	//		if (i == j) {
	//			continue;
	//		}
	//		PointDbl &pt_j = s_ptAryPoint2[j];
	//		double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
	//		if (dbDelta < nDeltaY) {
	//			s_ptAryPoint2.RemoveAt(j);
	//		}
	//	}
	//}
	//for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) { //����Y���W���폜
	//	PointDbl &pt_i = s_ptAryPoint1[i];
	//	TRACE("pt_i = %.2f\n", pt_i.y);
	//	for (int j = (int)s_ptAryPoint2.GetSize() - 1; j >= 0; j--) {
	//		PointDbl &pt_j = s_ptAryPoint2[j];
	//		TRACE("j(%d) = %.2f\n", j, pt_j.y);
	//		double dbDelta = abs(pt_i.y - pt_j.y); //y���W����
	//		if (dbDelta < nDeltaY) {
	//			s_ptAryPoint2.RemoveAt(j);
	//		}
	//	}
	//}
	//// ���x�����O���ʂ̕`��
	//cv::Mat dst(matSrc.size(), CV_8UC3);
	//dib.ToMat(dst);

	//for (int i = 0; i < s_ptAryPoint1.GetSize(); i++) {
	//	PointDbl &pt = s_ptAryPoint1[i];
	//	cv::circle(dst, cv::Point((int)pt.x, (int)pt.y), 3, cv::Scalar(255, 0, 0), CV_AA);

	//	strText.Format(L"BLUE pt.x,pt.y = %.2f , %.2f", pt.x, pt.y);
	//	m_wndOutput.OutputText(strText);

	//}

	//for (int i = 0; i < s_ptAryPoint2.GetSize(); i++) {
	//	PointDbl &pt = s_ptAryPoint2[i];
	//	cv::circle(dst, cv::Point((int)pt.x, (int)pt.y), 3, cv::Scalar(0, 0, 255), CV_AA);
	//	strText.Format(L"RED pt.x,pt.y = %.2f , %.2f", pt.x, pt.y);
	//	m_wndOutput.OutputText(strText);
	//}

	//dib.FromMat(dst); //�`��

	//int nNum = (int)s_ptAryPoint1.GetSize() + (int)s_ptAryPoint2.GetSize();
	//strText.Format(L"�� %d ", nNum);
	//m_wndOutput.OutputText(strText);

	return 0;
}

//raw data TO Tiff�t�@�C��
LRESULT CMainFrame::OnUser3(WPARAM wParam, LPARAM lParam)
{
	//// �摜�ǂݍ���
	//auto image = cv::imread("moji1.png");
	//// �O���[�X�P�[����
	//cv::Mat gray;
	//cv::cvtColor(image, gray, COLOR_RGB2GRAY);
	//// �����F���N���X�̃C���X�^���X����
	//auto ocr = cv::text::OCRTesseract::create("(����f�[�^�̃p�X)/tessdata", "eng");

	//std::string text;
	//std::vector<cv::Rect> boxes;
	//std::vector<std::string> words;
	//vector<float> confidences;
	//// �����F���̎��s
	//ocr->run(gray, text, &boxes, &words, &confidences);

	//// ���ʏo��
	//printf("%s\n", text.c_str());
	//// �����̂����܂育�Ƃɏo��
	//printf(" ����      | �ʒu       | �傫��     | �M���x\n");
	//printf("-----------+------------+------------+----------\n");
	//for (int i = 0; i < boxes.size(); i++)
	//{
	//	printf("%-10s | (%3d, %3d) | (%3d, %3d) | %f\n",
	//		words[i].c_str(),
	//		boxes[i].x, boxes[i].y,
	//		boxes[i].width, boxes[i].height,
	//		confidences[i]);
	//}
	return 0;
}

LRESULT CMainFrame::OnUser4(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUser5(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUser6(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUser7(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUser8(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUser9(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUserA(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUserB(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUserC(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CMainFrame::OnUserD(WPARAM wParam, LPARAM lParam)
{
	return 0;
}



// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

//Menu������̕\���ʒu���擾���܂�
static int GetMenuPosition(CMenu *pMenu, LPWSTR lpszCaption)
{
	if (pMenu != NULL) {
		for (int i = 0, imax = (int)pMenu->GetMenuItemCount(); i<imax; i++) {
			CString strItem;
			pMenu->GetMenuString(i, strItem, MF_BYPOSITION);
			if (strItem == lpszCaption) { //��������
				return i;
			}
		}
	}
	//������Ȃ�����
	return -1;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("���j���[ �o�[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �A�N�e�B�u�ɂȂ����Ƃ����j���[ �o�[�Ƀt�H�[�J�X���ړ����Ȃ�
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �c�[�� �o�[����у��j���[ �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A���� 5 �̍s���폜���܂�
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�����L���ɂ��܂�
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�̎�����\�������L���ɂ��܂�
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���j���[���ڃC���[�W (�ǂ̕W���c�[�� �o�[�ɂ��Ȃ��C���[�W) ��ǂݍ��݂܂�:
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// �h�b�L���O �E�B���h�E���쐬���܂�
	if (!CreateDockingWindows())
	{
		TRACE0("�h�b�L���O �E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// �Œ�l�Ɋ�Â��ăr�W���A�� �}�l�[�W���[�� visual �X�^�C����ݒ肵�܂�
	OnApplicationLook(theApp.m_nAppLook);

	// �c�[�� �o�[�ƃh�b�L���O �E�B���h�E ���j���[�̔z�u�ύX��L���ɂ��܂�
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// �c�[�� �o�[�̃N�C�b�N (Alt �L�[�������Ȃ���h���b�O) �J�X�^�}�C�Y��L���ɂ��܂�
	CMFCToolBar::EnableQuickCustomization();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// �t�@�C�� �r���[���쐬���܂�
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("�t�@�C�� �r���[ �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// �o�̓E�B���h�E���쐬���܂�
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// �v���p�e�B �E�B���h�E���쐬���܂�
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�v���p�e�B �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h���[

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ���j���[���X�L�������܂�*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

//���j���[�ύX
BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu* pMenuPopup)
{
	if (!pMenuPopup) {
		return FALSE;
	}
	//���j���[�p�R�}���h�t�@�C���ꗗ�擾

	CSortStringArray aryCommandPath;
	ReadCommandMenu(aryCommandPath); //�R�}���h�ꗗ�ǂݍ���
	aryCommandPath.Sort();

	//���j���[��T���܂��B
	int index = pMenuPopup->GetMenuBar()->CommandToIndex(IDM_DUMY1);
	if (index >= 0) {
		//���j���[���폜���܂��B
		pMenuPopup->RemoveItem(index);

		//���j���[��ǉ����܂��B
		pMenuPopup->InsertSeparator(index);
		for (int i = 0; i < aryCommandPath.GetSize(); i++) {
			CString strTitle = GetFileTitle(aryCommandPath[i]); //�R�}���h�t�@�C���^�C�g��
			pMenuPopup->InsertItem(CMFCToolBarMenuButton(IDM_USER + i, NULL, -1, strTitle),index);
		}
		pMenuPopup->InsertSeparator(index);

		//MESSAGE_MAP�Ƀ��b�Z�[�W�n���h��(ON_COMMAND)��ǉ�����̂�Y��Ȃ��ŁI
	}

	return KIBOFrame::OnShowPopupMenu(pMenuPopup);
}


void CMainFrame::OnHome()
{
}
void CMainFrame::OnRight()
{
}
void CMainFrame::OnLeft()
{
}
void CMainFrame::OnUp()
{
}
void CMainFrame::OnDown()
{
}
void CMainFrame::OnPageUp() {}
void CMainFrame::OnPageDown() {}
void CMainFrame::OnPlus() {}
void CMainFrame::OnMinus() {}
void CMainFrame::OnFunctionKey(int nVK)
{
	CString strFilePath;
	if (m_mapKeyFilePath.Lookup(nVK, strFilePath)) { //File���w�肳��Ă���ꍇ��
		ScriptExec(strFilePath); //Script File ���s
	}
}
void CMainFrame::OnF1() {OnFunctionKey(VK_F1);}
void CMainFrame::OnF2() {OnFunctionKey(VK_F2);}
void CMainFrame::OnF3() { OnFunctionKey(VK_F3); }
void CMainFrame::OnF4() { OnFunctionKey(VK_F4); }
void CMainFrame::OnF5() { OnFunctionKey(VK_F5); }
void CMainFrame::OnF6() { OnFunctionKey(VK_F6); }
void CMainFrame::OnF7() { OnFunctionKey(VK_F7); }
void CMainFrame::OnF8() { OnFunctionKey(VK_F8); }
void CMainFrame::OnF9() { OnFunctionKey(VK_F9); }
void CMainFrame::OnF10() { OnFunctionKey(VK_F10); }
void CMainFrame::OnF11() { OnFunctionKey(VK_F11); }
void CMainFrame::OnF12() { OnFunctionKey(VK_F12); }


//.ini�t�@�C���ҏW
void CMainFrame::OnFileOpen()
{
	ShellExecute(NULL, TEXT("edit"), g_lpszIniFileName, NULL, NULL, SW_SHOW);
}

void CMainFrame::OnA() {}
void CMainFrame::OnB() {}
void CMainFrame::OnC(){}
void CMainFrame::OnD() {}
void CMainFrame::OnE() {}
void CMainFrame::OnF() {}
void CMainFrame::OnG() {}
void CMainFrame::OnH() {}
void CMainFrame::OnI(){}
void CMainFrame::OnJ() {}
void CMainFrame::OnK() {}
void CMainFrame::OnL() {}
void CMainFrame::OnM() {}
void CMainFrame::OnN() {}
void CMainFrame::OnO() {}
void CMainFrame::OnP() {}
void CMainFrame::OnQ() {}
void CMainFrame::OnR() {}
void CMainFrame::OnS() {}
void CMainFrame::OnT() {}
void CMainFrame::OnU() {}
void CMainFrame::OnV(){}
void CMainFrame::OnW() {}
void CMainFrame::OnX() {}
void CMainFrame::OnY() {}
void CMainFrame::OnZ() {}

//�`��T�C�Y�ύX
static void ChangeDrawSize(int nCx, int nCy)
{
	ImageDevice  *pImageDevice = ImageDevice::GetInstance();
	CDIB &dib = pImageDevice->GetDib();
	int nW = dib.GetWidth(); //�L���v�`���T�C�Y
	int nH = dib.GetHeight();
	int nCnum = 0; //�J�����ԍ�0
	pImageDevice->SetDrawSize(nCx, nCy); //�\���T�C�Y��nCx,nCy�ɕύX
	App *pApp = App::GetInstance();
	bool isStreatch = pImageDevice->IsStreatchDraw();
	if (isStreatch) {
		pApp->SetViewRate((double)nCx / nW, (double)nCy / nH); //�\�����W�̔{���ݒ�			
	}
	else {
		pApp->SetViewRate(1.0, 1.0); //�\�����W�̔{���ݒ�
	}
	pApp->Render();
}

void CMainFrame::On800()
{
	ChangeDrawSize(800, 600);
}

void CMainFrame::On1024()
{
	ChangeDrawSize(1024, 768);
}

void CMainFrame::On2048()
{
	ChangeDrawSize(2048, 1536);
}

void CMainFrame::On2592()
{
	ChangeDrawSize(2592, 1944);
}

//�g��k��
void CMainFrame::OnStreatch()
{
	App *pApp = App::GetInstance();
	ImageDevice  *pImageDevice = ImageDevice::GetInstance();

	CDIB &dib = GetView()->GetDib();
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	int nCx = nW;
	int nCy = min(nH, (int)(nW * 600.0 / 800.0 + 0.5));
	ChangeDrawSize(nCx, nCy);
}

//�R�}���h�t�@�C�����s
void CMainFrame::OnCommandFile(UINT unID)
{
	CSortStringArray aryCommandPath;
	ReadCommandMenu(aryCommandPath); //�R�}���h�ꗗ�ǂݍ���
	aryCommandPath.Sort();

	int nIndex = (int)unID - IDM_USER;
	if (0 <= nIndex && nIndex < aryCommandPath.GetSize()) {
		CString strPath = aryCommandPath[nIndex]; //Script File Path�擾		
		ScriptExec(strPath);
	}
}
//2�l���_�C�A���O�\��
LRESULT CMainFrame::OnBinaryDialog(WPARAM wParam, LPARAM lParam)
{
	CKIBOView *pView = GetView();
	pView->m_mgrVO.SetVisible(false); //��\��
	int nCnum = 0;

	BinaryDlg dlg(this);
	dlg.SetInput(128); //�����l
	dlg.SetRange(0, 255); //�����W�ݒ�
	if (dlg.DoModal() != IDOK) {
		pView->GetDib().SetDib(0, 0, dlg.m_dibSrc); //���̉摜�ɖ߂�
		pView->Invalidate();
	}
	pView->m_mgrVO.SetVisible(true); //�\��
	return 0;
}

//����������
LRESULT CMainFrame::OnInit(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//�J�����摜�ۑ�
void CMainFrame::OnCameraFile()
{
	ImageDevice  *pImageDevice = ImageDevice::GetInstance();
	CDIB &dib = pImageDevice->GetDib();

	CString strFilePath = SelectBitmapFile(FALSE, theApp.GetAppPath());

	dib.WriteBitmap(strFilePath);
}
//�L�����u���[�V����
LRESULT CMainFrame::OnCalibration(WPARAM wParam, LPARAM lParam)
{
	CString strPath;
	strPath.Format(L"%s\\%s", theApp.GetAppPath(), g_lpszIniFileName);//KIBOSYSTEM.ini �t�@�C���p�X�擾

	//�ŏ��ʐςƍő�ʐς̒��Ԃ��L�����u���[�V�����p��`�̖ʐςƂ��܂��B
	int nMinPix = ::GetPrivateProfileInt(g_lpszCalibration, L"MINPIXELS ", 100, strPath);	//�ŏ��ʐ�
	int nMaxPix = ::GetPrivateProfileInt(g_lpszCalibration, L"MAXPIXELS", 1000000, strPath);//�ő�ʐ�

	double dbViewWidth = ::GetPrivateProfileInt(g_lpszCamera, L"VIEWWIDTH", 1920, strPath);//�\����
	double dbViewHeight= ::GetPrivateProfileInt(g_lpszCamera, L"VIEWHEIGHT",1080, strPath);//�\������

	App *pApp = App::GetInstance();
	ImageDevice  *pImageDevice = ImageDevice::GetInstance(); //�J����
	CDIB &dib = pImageDevice->GetDib();//�J��������DIB���擾
	int nW = dib.GetWidth();  //��
	int nH = dib.GetHeight(); //����

	Mat matSrc;//���x�����O�\�[�X�摜
	matSrc.create(nH, nW, CV_8UC1); //GRAYSCALE
	if (dib.ToMat(matSrc)) { //�\�[�X�摜�擾
		int nTh = pApp->GetNum(L"�������l");
		// ��l��
		Mat matBin;//�Q�l���摜
		cv::threshold(matSrc, matBin, nTh, 255, cv::THRESH_BINARY /*| cv::THRESH_OTSU*/);

		cv::Mat matImg(matSrc.size(), CV_8U); //���x�����O�摜
		//���׃����O����
		cv::Mat stats;
		cv::Mat centroids;//�d�S
		int nLab = cv::connectedComponentsWithStats(matBin, matImg, stats, centroids);

		// ���x�����O���ʂ̕`��F������
		cv::Vec3b color = cv::Vec3b(255, 0, 0); //BGR

		// ���x�����O���ʂ̕`��
		cv::Mat dst(matSrc.size(), CV_8UC3);
		dib.ToMat(dst);

		//�h��Ԃ�
		for (int y = 0; y < dst.rows; ++y)
		{
			for (int x = 0; x < dst.cols; ++x)
			{
				int nLnum = matImg.at<int>(y, x);//���x�����O�ԍ��擾
				if (nLnum > 0) {//���x�����O�v�f�m�F
					int *pStats = stats.ptr<int>(nLnum);
					int height = pStats[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
					int width = pStats[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
					int xMin = pStats[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
					int yMin = pStats[cv::ConnectedComponentsTypes::CC_STAT_TOP];
					int xMax = xMin + width;
					int yMax = yMin + height;
					if (xMin > 0 && yMin > 0 && nW > xMax && nH > yMax) {
						dst.at<cv::Vec3b>(y, x) = color; //���x�����O���ꂽ�Ƃ����h��Ԃ�
					}
				}
			}
		}

		CString strID;
		CString strText;
		//ROI�̐ݒ�
		int nNo = 1;
		int nCol = 0; //Column�����@��
		int nColMax = 0;
		int nRow = 0; //Row�����@��
		int nRowMax = 0;
		PointDbl ptPre(0,0); //�ȑO�̏d�S
		double dbLenMax = 0;
		int indexI = 0;
		int indexJ = 0;
		for (int i = 1; i < nLab; ++i) {//�v�f�����̃��[�v
			//�ʐϒl�̏o��
			CString strText;
			int *pStats = stats.ptr<int>(i);
			int nPixels = pStats[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�
			if (nMinPix <= nPixels && nPixels <= nMaxPix) {//�ʐσ`�F�b�N
				strText.Format(L"area %d = %d", i, nPixels);
				m_wndOutput.OutputText(strText);
				int height = pStats[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
				int width = pStats[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
				int xMin = pStats[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
				int yMin = pStats[cv::ConnectedComponentsTypes::CC_STAT_TOP];
				int xMax = xMin + width;
				int yMax = yMin + height;

				if (xMin <= 0 || yMin <= 0 || nW <= xMax || nH <= yMax) {
					continue;
				}
				//�d�S�̏o��
				double *pCentroids = centroids.ptr<double>(i);
				double xCen = pCentroids[0];//�d�Sx
				double yCen = pCentroids[1];//�d�Sy
				cv::circle(dst, cv::Point((int)xCen, (int)yCen), 3, cv::Scalar(0, 0, 255), -1);

				//ROI�̍���ɔԍ�����������
				strID.Format(L"%d", nNo);
				strText.Format(L"%d", nNo);
				cv::String sText = CStringA(strText).GetBuffer();

				cv::Point ptText(xCen + 5, yCen + 20);
				cv::putText(dst, sText, ptText, cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(128, 255, 0), 2, cv::LINE_AA);
				nNo++;

				if (ptPre.x > 0 ) {
					nCol++;
				}
				if (ptPre.y > 0) {
					if (abs(yCen - ptPre.y) > height/3 ) { //�d�S�ԋ���x�̕ω�
						nCol = 0;
						nRow++;
					}
				}

				ptPre.x = xCen; //�ȑO�̏d�S
				ptPre.y = yCen;
				if (nColMax < nCol) { //�ő�l�̍����ւ�
					nColMax = nCol;
				}
				if (nRowMax < nRow) { //�ő�l�̍����ւ�
					nRowMax = nRow;
				}
				//�ő�d�S�ԋ��������߂�
				for (int j = 1; j < nLab; j++) {
					if (i != j) {
						int *paramJ = stats.ptr<int>(j);
						int nPixels = paramJ[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�
						if (nMinPix <= nPixels && nPixels <= nMaxPix) {//�ʐσ`�F�b�N
							double *pCentroidsJ = centroids.ptr<double>(j);
							double xCenJ = pCentroidsJ[0];//�d�Sx
							double yCenJ = pCentroidsJ[1];//�d�Sy
							int *pStatsJ = stats.ptr<int>(j);
							int height = pStatsJ[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
							int width = pStatsJ[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
							int xMin = pStatsJ[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
							int yMin = pStatsJ[cv::ConnectedComponentsTypes::CC_STAT_TOP];
							int xMax = xMin + width;
							int yMax = yMin + height;
							if (xMin <= 0 || yMin <= 0 || nW <= xMax || nH <= yMax) {
								continue;
							}

							double dbLen = ::GetLength((int)xCen, (int)yCen, (int)xCenJ, (int)yCenJ);
							if (dbLenMax < dbLen) {
								indexI = i;
								indexJ = j;
								dbLenMax = dbLen;
							}
						}
					}
				}

			}
		}

		//�ő勗��
		double *pCentroidsI = centroids.ptr<double>(indexI);
		double xCenI = pCentroidsI[0];//�d�Sx
		double yCenI = pCentroidsI[1];//�d�Sy
		double *pCentroidsJ = centroids.ptr<double>(indexJ);
		double xCenJ = pCentroidsJ[0];//�d�Sx
		double yCenJ = pCentroidsJ[1];//�d�Sy

		cv::line(dst, cv::Point((int)xCenI, (int)yCenI), cv::Point((int)xCenJ, (int)yCenJ), cv::Scalar(0, 0, 200), 3, 4);//�d�S�ԋ����`��
		dib.FromMat(dst);
		strText.Format(L"�L�����u���[�V�������� %d�� %d�s", nColMax + 1, nRowMax + 1);
		m_wndOutput.OutputText(strText);

		//dbLenMax Pixels ����1��f������̋��������߂�
		double dbSpace = ::GetPrivateProfileDbl(L"CALIBRATION", L"SPACE", 0, strPath);//�L�����u���[�V�����p�����1���T�C�Y�擾
		double dbCx = dbSpace * nColMax;
		double dbCy = dbSpace * nRowMax;
		double dbMM = sqrt( (dbCx * dbCx) + (dbCy * dbCy) ); //mm 
		double dbLengthPerPix = dbMM / dbLenMax; //1��f������̋���mm�����߂�
		strText.Format(L"1��f������̋��� %.5f [mm]", dbLengthPerPix);
		m_wndOutput.OutputText(strText);

		::WritePrivateProfileDbl(L"CALIBRATION", L"PERPIXEL", L"%.10f", dbLengthPerPix, strPath);//1��f������̃T�C�Y���L�^
		
	}
	return 0;
}

//StatusBar�ɕ�������Z�b�g���܂�
void CMainFrame::SetStatusBarText(LPCTSTR lpszText)
{
	m_wndStatusBar.SetPaneText(0, lpszText);
}