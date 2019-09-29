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
	//��ֱͶӰ��
	QList<cv::Mat> VerticalProjectionMethod(cv::Mat img);
	void ClearTempFiles(const QString& temp_path); //����ļ����д洢���ļ�

public:
	double ** ColorFeture;
	double weightBlueRed;
	double weightBlueGreen;
private:
	cv::String path; //��������·��
	QString *appExeFolder = nullptr; //��������·��
};