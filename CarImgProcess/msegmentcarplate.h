#pragma once

#include "carimgprocess_global.h"
#include "carinformation.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MatSource.h"

#include <QDir>
#include <QDirIterator>
#include <QtCore/QCoreApplication>

#define CV_8U   0

class CARIMGPROCESS_EXPORT MSegmentCarPlate
{
public:
	MSegmentCarPlate();
	~MSegmentCarPlate();
public:
	//垂直投影法
	QList<cv::Mat> VerticalProjectionMethod(cv::Mat img);
	void ClearTempFiles(const QString& temp_path); //清空文件夹中存储的文件

public:
	double ** ColorFeture;
	double weightBlueRed;
	double weightBlueGreen;
private:
	cv::String path; //程序运行路径
	QString *appExeFolder = nullptr; //程序运行路径
};