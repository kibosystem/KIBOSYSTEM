#pragma once

#include "./LIB/DIB.h"


class LabelingCV
{
private:
	cv::Mat m_matSrc;
	cv::Mat m_matBin;
	cv::Mat m_stats;
	cv::Mat m_centroids;
public:
	cv::Mat& GetStats() { return m_stats;}
	cv::Mat& GetCentroids() { return m_centroids; }
	int GetArea(int nIndex);	//�ʐώ擾
	int GetHeight(int nIndex);	//�����擾
	int GetWidth(int nIndex);	//���擾
	cv::Point2d GetCentroids(int nIndex); //�d�S�擾

public:
	LabelingCV();
	~LabelingCV();

	BOOL Init(CDIB &dib, int nTh, int nType= THRESH_BINARY); //THRESH_BINARY: �����o or THRESH_BINARY_INV:�����o
	int Exec(int nConnect = 4);//���x�����O���s �摜�S��							   
	int Exec(CRect &rect, int nConnect = 4);//���x�����O���s �摜�ꕔ



};

