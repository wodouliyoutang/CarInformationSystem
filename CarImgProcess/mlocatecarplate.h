#pragma once

#include "carimgprocess_global.h"
#include "carinformation.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MatSource.h"
#include <QDir>
#include <QDirIterator>
#define CV_8U   0

class CARIMGPROCESS_EXPORT MLocateCarPlate
{
public:
	MLocateCarPlate();
public:
	//从图像中发现并定位车牌
	cv::Mat DetectCarPlate(cv::Mat img); 
	//形态学处理，使用高斯平滑、中值滤波等方法处理图像，对应论文中第7页开始，边缘检测用的Sobel算子
	//使用了高斯平滑、中值滤波、Sobel算子边缘检测、二值化处理、使用核函数进行膨胀、腐蚀处理使得图片更容易被识别
	cv::Mat PhotoPreProcess(cv::Mat img);
	cv::Mat PhotoPreProcess1(cv::Mat img);
	//获得可能为车牌的区域，对应论文中的点对搜索
	QList<cv::Mat> GetRegion(cv::Mat img, cv::Mat oringinImg);
	//使用颜色匹配，返回最接近车牌的矩阵
	cv::Mat ColorMatch(QList<cv::Mat> regions);
	void ClearTempFiles(const QString& temp_path); //清空文件夹中存储的文件

public:
	double ** ColorFeture;
	double weightBlueRed;
	double weightBlueGreen;
private:
	cv::String path; //程序运行路径
	QString *appExeFolder = nullptr; //程序运行路径
};