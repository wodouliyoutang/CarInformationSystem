#include "mlocatecarplate.h"
#include <QtCore/QCoreApplication>
#include <QDebug>

MLocateCarPlate::MLocateCarPlate()
{
	appExeFolder = new QString(QCoreApplication::applicationDirPath());
	path = QCoreApplication::applicationDirPath().toStdString().data();
	path += +"/imgLocateCarPlate";
	weightBlueRed = 0.33748;
	weightBlueGreen = 0.66252;
	ClearTempFiles((*appExeFolder)+"/imgLocateCarPlate");
}

/************************************************
*函数功能：从图像中发现并定位车牌
*输入：
*	cv::Mat
*输出：
*	cv::Mat
*作者：ZYX
*时间版本：2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::DetectCarPlate(cv::Mat img)
{
	//灰度化处理
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	//形态学处理，使用高斯平滑、中值滤波等方法处理图像，对应论文中第7页开始，边缘检测用的Sobel算子
	//使用了高斯平滑、中值滤波、Sobel算子边缘检测、二值化处理、使用核函数进行膨胀、腐蚀处理使得图片更容易被识别
	cv::Mat prc = PhotoPreProcess(gray);
	QList<cv::Mat> regions;
	regions = GetRegion(prc,img);
	//使用颜色匹配，返回最接近车牌的矩阵
	cv::Mat BestRegion = ColorMatch(regions);
	cv::imwrite(path + "/BestRegion.jpg", BestRegion);
	////尝试倾斜矫正
	////
	//std::vector<std::vector<cv::Point>> contours; //定义轮廓矩阵
	////查找轮廓
	////灰度化处理
	//cv::Mat gray1;
	//cv::cvtColor(BestRegion, gray1, cv::COLOR_BGR2GRAY);
	////形态学处理，使用高斯平滑、中值滤波等方法处理图像，对应论文中第7页开始，边缘检测用的Sobel算子
	////使用了高斯平滑、中值滤波、Sobel算子边缘检测、二值化处理、使用核函数进行膨胀、腐蚀处理使得图片更容易被识别
	//cv::Mat prc1 = PhotoPreProcess(gray1);
	//cv::findContours(prc1, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	//cv::RotatedRect minRect = cv::minAreaRect(contours.at(0));
	//float angle = minRect.angle;
	////Create and rotate image
	//cv::Mat rotmat = cv::getRotationMatrix2D(minRect.center, angle, 1);
	//cv::Mat rotatedRegion;
	//cv::warpAffine(BestRegion, rotatedRegion, rotmat, BestRegion.size(), cv::INTER_CUBIC);
	//cv::imwrite(path + "/img_rotated.jpg", rotatedRegion);
	return BestRegion;
}

/************************************************
*函数功能：使用颜色匹配，返回最接近车牌的图像
*输入：
*	cv::Mat
*输出：
*	cv::Mat
*作者：ZYX
*时间版本：2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::ColorMatch(QList<cv::Mat> regions)
{
	QList<float> blueColorRate;
	int cPointB, cPointG, cPointR;
	for (int k = 0; k < regions.size(); k++)
	{
		cv::Mat src = regions.at(k);
		//颜色特征值数组
		ColorFeture = new double *[src.rows];
		for (int i = 0; i < src.rows; i++)
			ColorFeture[i] = new double[src.cols]();
		int m_featureNum = 0;
		float m_featureSum = 0;
		float m_featureMean = 0;
		for (int i = 1; i < src.rows; i++)
		{
			for (int j = 1; j < src.cols; j++)
			{
				cPointB = src.at<cv::Vec3b>(i, j)[0];
				cPointG = src.at<cv::Vec3b>(i, j)[1];
				cPointR = src.at<cv::Vec3b>(i, j)[2];

				int dif_B_R = cPointB - cPointR; //计算蓝色特征
				int dif_B_G = cPointB - cPointG;
				if (dif_B_G > 0 && dif_B_R > 0)
				{
					ColorFeture[i][j] = weightBlueRed * dif_B_R + weightBlueGreen * dif_B_G;
					if (ColorFeture[i][j] > 0)
					{
						m_featureNum++;   //统计非0的特征点数量
						m_featureSum += ColorFeture[i][j]; //计算所有特征值总和
					}
				}
				else
					ColorFeture[i][j] = 0;
			}
		}
		if (m_featureNum > 0)
		{
			m_featureMean = m_featureSum / m_featureNum;
		}
		blueColorRate.append(m_featureMean);
	}
	float maxNum = 0.0;
	int maxIndex = 0;
	for (int i = 0; i < blueColorRate.size(); i++)
	{
		if (blueColorRate.at(i) > maxNum)
		{
			maxNum = blueColorRate.at(i);
			maxIndex = i;
		}
	}
	return regions.at(maxIndex);
}



/************************************************
*函数功能：形态学处理，使用高斯平滑、中值滤波等方法处理图像，对应论文中第7页开始，边缘检测用的Sobel算子
*          使用了高斯平滑、中值滤波、Sobel算子边缘检测、二值化处理、使用核函数进行膨胀、腐蚀处理使得图片更容易被识别
*输入：
*	cv::Mat
*输出：
*	cv::Mat
*作者：ZYX
*时间版本：2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::PhotoPreProcess(cv::Mat img)
{
	//高斯平滑
	cv::Mat gaussian;
	cv::GaussianBlur(img, gaussian, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
#ifdef _DEBUG
	cv::imwrite(path+"/gaussian.jpg", gaussian);
#endif
	//中值滤波
	cv::Mat median;
	cv::medianBlur(gaussian, median,5);
#ifdef _DEBUG
	cv::imwrite(path + "/median.jpg", median);
#endif
	//sobel算子
	//梯度方向：x
	cv::Mat sobel;
	cv::Sobel(median, sobel,CV_8U,1,0,3);
#ifdef _DEBUG
	cv::imwrite(path + "/sobel.jpg", sobel);
#endif
	//二值化，目的是除掉背景中的噪声，单独处理车牌这一对象
	cv::Mat binary;
	cv::threshold(sobel, binary, 170, 255, cv::THRESH_BINARY);
#ifdef _DEBUG
	cv::imwrite(path + "/binary.jpg", binary);
#endif
	//核函数
	cv::Mat element1;
	element1=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1));
	cv::Mat element2;
	element2=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 7));
	//膨胀
	cv::Mat dilation;
	cv::dilate(binary, dilation, element2);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation.jpg", dilation);
#endif
	//腐蚀
	cv::Mat erosion;
	cv::dilate(dilation, erosion, element1);
#ifdef _DEBUG
	cv::imwrite(path + "/erosion.jpg", erosion);
#endif
	//膨胀
	cv::Mat dilation2;
	cv::dilate(erosion, dilation2, element2,cv::Point(-1,-1),3);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation2.jpg", dilation2);
#endif
	return dilation2;
}


cv::Mat MLocateCarPlate::PhotoPreProcess1(cv::Mat img)
{
	//高斯平滑
	cv::Mat gaussian;
	cv::GaussianBlur(img, gaussian, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
#ifdef _DEBUG
	cv::imwrite(path + "/gaussian.jpg", gaussian);
#endif
	//中值滤波
	cv::Mat median;
	cv::medianBlur(gaussian, median, 5);
#ifdef _DEBUG
	cv::imwrite(path + "/median.jpg", median);
#endif
	//sobel算子
	//梯度方向：x
	cv::Mat sobel;
	cv::Sobel(median, sobel, CV_8U, 1, 0, 3);
#ifdef _DEBUG
	cv::imwrite(path + "/sobel.jpg", sobel);
#endif
	//二值化，目的是除掉背景中的噪声，单独处理车牌这一对象
	cv::Mat binary;
	cv::threshold(sobel, binary, 170, 255, cv::THRESH_BINARY);
#ifdef _DEBUG
	cv::imwrite(path + "/binary.jpg", binary);
#endif
	//核函数
	cv::Mat element1;
	element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1));
	cv::Mat element2;
	element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 7));
	//膨胀
	cv::Mat dilation;
	cv::dilate(binary, dilation, element2);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation.jpg", dilation);
#endif
	//腐蚀
	cv::Mat erosion;
	cv::dilate(dilation, erosion, element1);
#ifdef _DEBUG
	cv::imwrite(path + "/erosion.jpg", erosion);
#endif
	return erosion;
}



/************************************************
*函数功能：获得可能为车牌的区域，对应论文中的点对搜索
*输入：
*	cv::Mat
*输出：
*	cv::Mat
*作者：ZYX
*时间版本：2019-09-26-V1.0
*************************************************/
QList<cv::Mat> MLocateCarPlate::GetRegion(cv::Mat img, cv::Mat OringinImg)
{
	QList<cv::Mat> regions;
	std::vector<std::vector<cv::Point>> contours; //定义轮廓矩阵
	//查找轮廓
	cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	/*
		参数说明：
		RETR_EXTERNAL:表示只检测最外层轮廓，对所有轮廓设置hierarchy[i][2] = hierarchy[i][3] = -1
		RETR_LIST : 提取所有轮廓，并放置在list中，检测的轮廓不建立等级关系
		RETR_CCOMP : 提取所有轮廓，并将轮廓组织成双层结构(two - level hierarchy), 顶层为连通域的外围边界，次层位内层边界
		RETR_TREE : 提取所有轮廓并重新建立网状轮廓结构
		RETR_FLOODFILL：官网没有介绍，应该是洪水填充法

		CHAIN_APPROX_NONE：获取每个轮廓的每个像素，相邻的两个点的像素位置差不超过1
		CHAIN_APPROX_SIMPLE：压缩水平方向，垂直方向，对角线方向的元素，值保留该方向的重点坐标，如果一个矩形轮廓只需4个点来保存轮廓信息
		CHAIN_APPROX_TC89_L1和CHAIN_APPROX_TC89_KCOS使用Teh-Chinl链逼近算法中的一种
	*/
	for (int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours.at(i));
		if (area < 2000)  //todo:实际环境中，可能根据摄像头的采集图像，选取合适的矩形区域大小2000
		{
			//如果搜寻区域面积太小，则跳过此区域
			qDebug() << "area is " << area;
//#ifdef _DEBUG
//			cv::Mat tempImg = cv::imread("D:/CasitPlusWorkSpace/src/CarInformationSystem/src/2.jpg");
//			//cv::Mat tempImg = img;
//			std::vector<std::vector<cv::Point>> tempcontours;
//			tempcontours.push_back(contours.at(i));
//			cv::drawContours(tempImg, tempcontours, 0, cv::Scalar(0,0,255,0), 2);
//			cv::imwrite(path + "/minArea"+QString::number(i).toStdString()+".jpg", tempImg);
//#endif
			continue;
		}
		double eps = cv::arcLength(contours.at(i), true) / 1000;
		std::vector<cv::Point> approx;
		cv::approxPolyDP(contours.at(i), approx, eps,true); //true是代表是否封闭：是
		cv::RotatedRect rect;
		rect = cv::minAreaRect(contours.at(i));
		cv::Mat box;
		cv::boxPoints(rect, box);
		std::cout<< box;
		int height=abs(box.at<float>(0, 1)- box.at<float>(2, 1));
		int width=abs(box.at<float>(0, 0) - box.at<float>(2, 0));
		float ratio = float(width) / float(height);
		if (ratio <5 && ratio >1.8)
		{
#ifdef _DEBUG
			cv::Mat tempImg1 = OringinImg.clone();
			std::vector<std::vector<cv::Point>> tempcontours1;
			tempcontours1.push_back(contours.at(i));
			std::vector<cv::Mat> tempcontours;
			cv::drawContours(tempImg1, tempcontours1, 0, cv::Scalar(0, 0, 255, 0), 2);
			cv::imwrite(path + "/minArea" + QString::number(i).toStdString() + ".jpg", tempImg1);
#endif
			//将要分析的车牌矩阵提取出来
			cv::Mat tempImg = OringinImg;
			float xp1 = MIN(box.at<float>(1, 0), box.at<float>(2, 0));
			float xp2 = MAX(box.at<float>(0, 0), box.at<float>(3, 0));
			float yp1 = MIN(box.at<float>(2, 1), box.at<float>(3, 1));
			float yp2 = MAX(box.at<float>(0, 1), box.at<float>(1, 1));
			cv::Mat img1 = tempImg(cv::Range(yp1, yp2), cv::Range(xp1, xp2));
			cv::imwrite(path + "/locateResult.jpg", img1);
			regions.append(img1);
		}
		//
		//
	}
	return regions;
}

/***************************************
*函数功能：清空临时文件夹内的东西
*输入：
*	void
*输出：
*	void
*作者：ZYX
*时间版本：2019-07-09-V1.0
***************************************/
void MLocateCarPlate::ClearTempFiles(const QString& temp_path)
{
	QDir dir(temp_path);
	if (!dir.exists())
	{
		dir.mkpath(temp_path);
		return;
	}
	if (dir.isEmpty())
	{
		return;
	}
	QStringList filter; //过滤器
	filter.append("*");
	QDirIterator it(temp_path, filter, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())//若容器中还有成员，继续执行删除操作
		dir.remove(it.next());
}