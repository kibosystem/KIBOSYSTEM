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
	int GetArea(int nIndex);	//面積取得
	int GetHeight(int nIndex);	//高さ取得
	int GetWidth(int nIndex);	//幅取得
	cv::Point2d GetCentroids(int nIndex); //重心取得

public:
	LabelingCV();
	~LabelingCV();

	BOOL Init(CDIB &dib, int nTh, int nType= THRESH_BINARY); //THRESH_BINARY: 白検出 or THRESH_BINARY_INV:黒検出
	int Exec(int nConnect = 4);//ラベリング実行 画像全体							   
	int Exec(CRect &rect, int nConnect = 4);//ラベリング実行 画像一部



};

