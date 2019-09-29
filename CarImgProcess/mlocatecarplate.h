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
	//��ͼ���з��ֲ���λ����
	cv::Mat DetectCarPlate(cv::Mat img); 
	//��̬ѧ����ʹ�ø�˹ƽ������ֵ�˲��ȷ�������ͼ�񣬶�Ӧ�����е�7ҳ��ʼ����Ե����õ�Sobel����
	//ʹ���˸�˹ƽ������ֵ�˲���Sobel���ӱ�Ե��⡢��ֵ������ʹ�ú˺����������͡���ʴ����ʹ��ͼƬ�����ױ�ʶ��
	cv::Mat PhotoPreProcess(cv::Mat img);
	cv::Mat PhotoPreProcess1(cv::Mat img);
	//��ÿ���Ϊ���Ƶ����򣬶�Ӧ�����еĵ������
	QList<cv::Mat> GetRegion(cv::Mat img, cv::Mat oringinImg);
	//ʹ����ɫƥ�䣬������ӽ����Ƶľ���
	cv::Mat ColorMatch(QList<cv::Mat> regions);
	void ClearTempFiles(const QString& temp_path); //����ļ����д洢���ļ�

public:
	double ** ColorFeture;
	double weightBlueRed;
	double weightBlueGreen;
private:
	cv::String path; //��������·��
	QString *appExeFolder = nullptr; //��������·��
};