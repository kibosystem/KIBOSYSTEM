#include "stdafx.h"
#include "LabelingCV.h"



LabelingCV::LabelingCV()
{
}


LabelingCV::~LabelingCV()
{
}

//������
BOOL LabelingCV::Init(CDIB &dib, int nTh, int nType)
{
	dib.ToMat(m_matSrc);
	// ��l��
	cv::threshold(m_matSrc, m_matBin, nTh, 255, nType);
	return TRUE;
}

//���x�����O���s �摜�S��
int LabelingCV::Exec(int nConnect /*= 4*/)
{
	cv::Mat matImg;
	matImg.create(m_matSrc.rows, m_matSrc.cols, CV_16UC1);

	//���׃����O����
	int nLabNum = cv::connectedComponentsWithStats(m_matBin, matImg, m_stats, m_centroids, nConnect);
	return nLabNum;
}



//���x�����O���s �摜�ꕔ
int LabelingCV::Exec(CRect &rect,int nConnect /*= 4*/)
{
	cv::Mat cut_img(m_matBin, cv::Rect(rect.left ,rect.top , rect.Width(), rect.Height())); //�؂蔲���摜

	cv::Mat matImg;
	matImg.create(rect.Width(), rect.Height(), CV_16UC1);

	//���׃����O����
	int nLabNum = cv::connectedComponentsWithStats(cut_img, matImg, m_stats, m_centroids, nConnect);

	return nLabNum;
}

//�ʐώ擾
int LabelingCV::GetArea(int nIndex)
{
	int *param = m_stats.ptr<int>(nIndex);
	if (param != NULL) {
		return param[cv::ConnectedComponentsTypes::CC_STAT_AREA]; //�ʐ�
	}
	return 0;
}

//�����擾
int LabelingCV::GetHeight(int nIndex)
{
	int *param = m_stats.ptr<int>(nIndex);
	if (param != NULL) {
		return param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
	}
	return 0;
}

//���擾
int LabelingCV::GetWidth(int nIndex)
{
	int *param = m_stats.ptr<int>(nIndex);
	if (param != NULL) {
		return param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
	}
	return 0;
}

//�d�S�擾
cv::Point2d LabelingCV::GetCentroids(int nIndex)
{	
	cv::Point2d ptRet;

	double *pCentroids = m_centroids.ptr<double>(nIndex);
	if (pCentroids != NULL) {
		ptRet.x = pCentroids[0];
		ptRet.y = pCentroids[1];
	}
	return ptRet;
}