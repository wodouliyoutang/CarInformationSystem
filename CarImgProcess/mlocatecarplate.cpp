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
*�������ܣ���ͼ���з��ֲ���λ����
*���룺
*	cv::Mat
*�����
*	cv::Mat
*���ߣ�ZYX
*ʱ��汾��2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::DetectCarPlate(cv::Mat img)
{
	//�ҶȻ�����
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	//��̬ѧ����ʹ�ø�˹ƽ������ֵ�˲��ȷ�������ͼ�񣬶�Ӧ�����е�7ҳ��ʼ����Ե����õ�Sobel����
	//ʹ���˸�˹ƽ������ֵ�˲���Sobel���ӱ�Ե��⡢��ֵ������ʹ�ú˺����������͡���ʴ����ʹ��ͼƬ�����ױ�ʶ��
	cv::Mat prc = PhotoPreProcess(gray);
	QList<cv::Mat> regions;
	regions = GetRegion(prc,img);
	//ʹ����ɫƥ�䣬������ӽ����Ƶľ���
	cv::Mat BestRegion = ColorMatch(regions);
	cv::imwrite(path + "/BestRegion.jpg", BestRegion);
	////������б����
	////
	//std::vector<std::vector<cv::Point>> contours; //������������
	////��������
	////�ҶȻ�����
	//cv::Mat gray1;
	//cv::cvtColor(BestRegion, gray1, cv::COLOR_BGR2GRAY);
	////��̬ѧ����ʹ�ø�˹ƽ������ֵ�˲��ȷ�������ͼ�񣬶�Ӧ�����е�7ҳ��ʼ����Ե����õ�Sobel����
	////ʹ���˸�˹ƽ������ֵ�˲���Sobel���ӱ�Ե��⡢��ֵ������ʹ�ú˺����������͡���ʴ����ʹ��ͼƬ�����ױ�ʶ��
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
*�������ܣ�ʹ����ɫƥ�䣬������ӽ����Ƶ�ͼ��
*���룺
*	cv::Mat
*�����
*	cv::Mat
*���ߣ�ZYX
*ʱ��汾��2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::ColorMatch(QList<cv::Mat> regions)
{
	QList<float> blueColorRate;
	int cPointB, cPointG, cPointR;
	for (int k = 0; k < regions.size(); k++)
	{
		cv::Mat src = regions.at(k);
		//��ɫ����ֵ����
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

				int dif_B_R = cPointB - cPointR; //������ɫ����
				int dif_B_G = cPointB - cPointG;
				if (dif_B_G > 0 && dif_B_R > 0)
				{
					ColorFeture[i][j] = weightBlueRed * dif_B_R + weightBlueGreen * dif_B_G;
					if (ColorFeture[i][j] > 0)
					{
						m_featureNum++;   //ͳ�Ʒ�0������������
						m_featureSum += ColorFeture[i][j]; //������������ֵ�ܺ�
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
*�������ܣ���̬ѧ����ʹ�ø�˹ƽ������ֵ�˲��ȷ�������ͼ�񣬶�Ӧ�����е�7ҳ��ʼ����Ե����õ�Sobel����
*          ʹ���˸�˹ƽ������ֵ�˲���Sobel���ӱ�Ե��⡢��ֵ������ʹ�ú˺����������͡���ʴ����ʹ��ͼƬ�����ױ�ʶ��
*���룺
*	cv::Mat
*�����
*	cv::Mat
*���ߣ�ZYX
*ʱ��汾��2019-09-25-V1.0
*************************************************/
cv::Mat MLocateCarPlate::PhotoPreProcess(cv::Mat img)
{
	//��˹ƽ��
	cv::Mat gaussian;
	cv::GaussianBlur(img, gaussian, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
#ifdef _DEBUG
	cv::imwrite(path+"/gaussian.jpg", gaussian);
#endif
	//��ֵ�˲�
	cv::Mat median;
	cv::medianBlur(gaussian, median,5);
#ifdef _DEBUG
	cv::imwrite(path + "/median.jpg", median);
#endif
	//sobel����
	//�ݶȷ���x
	cv::Mat sobel;
	cv::Sobel(median, sobel,CV_8U,1,0,3);
#ifdef _DEBUG
	cv::imwrite(path + "/sobel.jpg", sobel);
#endif
	//��ֵ����Ŀ���ǳ��������е�������������������һ����
	cv::Mat binary;
	cv::threshold(sobel, binary, 170, 255, cv::THRESH_BINARY);
#ifdef _DEBUG
	cv::imwrite(path + "/binary.jpg", binary);
#endif
	//�˺���
	cv::Mat element1;
	element1=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1));
	cv::Mat element2;
	element2=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 7));
	//����
	cv::Mat dilation;
	cv::dilate(binary, dilation, element2);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation.jpg", dilation);
#endif
	//��ʴ
	cv::Mat erosion;
	cv::dilate(dilation, erosion, element1);
#ifdef _DEBUG
	cv::imwrite(path + "/erosion.jpg", erosion);
#endif
	//����
	cv::Mat dilation2;
	cv::dilate(erosion, dilation2, element2,cv::Point(-1,-1),3);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation2.jpg", dilation2);
#endif
	return dilation2;
}


cv::Mat MLocateCarPlate::PhotoPreProcess1(cv::Mat img)
{
	//��˹ƽ��
	cv::Mat gaussian;
	cv::GaussianBlur(img, gaussian, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
#ifdef _DEBUG
	cv::imwrite(path + "/gaussian.jpg", gaussian);
#endif
	//��ֵ�˲�
	cv::Mat median;
	cv::medianBlur(gaussian, median, 5);
#ifdef _DEBUG
	cv::imwrite(path + "/median.jpg", median);
#endif
	//sobel����
	//�ݶȷ���x
	cv::Mat sobel;
	cv::Sobel(median, sobel, CV_8U, 1, 0, 3);
#ifdef _DEBUG
	cv::imwrite(path + "/sobel.jpg", sobel);
#endif
	//��ֵ����Ŀ���ǳ��������е�������������������һ����
	cv::Mat binary;
	cv::threshold(sobel, binary, 170, 255, cv::THRESH_BINARY);
#ifdef _DEBUG
	cv::imwrite(path + "/binary.jpg", binary);
#endif
	//�˺���
	cv::Mat element1;
	element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1));
	cv::Mat element2;
	element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 7));
	//����
	cv::Mat dilation;
	cv::dilate(binary, dilation, element2);
#ifdef _DEBUG
	cv::imwrite(path + "/dilation.jpg", dilation);
#endif
	//��ʴ
	cv::Mat erosion;
	cv::dilate(dilation, erosion, element1);
#ifdef _DEBUG
	cv::imwrite(path + "/erosion.jpg", erosion);
#endif
	return erosion;
}



/************************************************
*�������ܣ���ÿ���Ϊ���Ƶ����򣬶�Ӧ�����еĵ������
*���룺
*	cv::Mat
*�����
*	cv::Mat
*���ߣ�ZYX
*ʱ��汾��2019-09-26-V1.0
*************************************************/
QList<cv::Mat> MLocateCarPlate::GetRegion(cv::Mat img, cv::Mat OringinImg)
{
	QList<cv::Mat> regions;
	std::vector<std::vector<cv::Point>> contours; //������������
	//��������
	cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	/*
		����˵����
		RETR_EXTERNAL:��ʾֻ����������������������������hierarchy[i][2] = hierarchy[i][3] = -1
		RETR_LIST : ��ȡ������������������list�У����������������ȼ���ϵ
		RETR_CCOMP : ��ȡ��������������������֯��˫��ṹ(two - level hierarchy), ����Ϊ��ͨ�����Χ�߽磬�β�λ�ڲ�߽�
		RETR_TREE : ��ȡ�������������½�����״�����ṹ
		RETR_FLOODFILL������û�н��ܣ�Ӧ���Ǻ�ˮ��䷨

		CHAIN_APPROX_NONE����ȡÿ��������ÿ�����أ����ڵ������������λ�ò����1
		CHAIN_APPROX_SIMPLE��ѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֵ�����÷�����ص����꣬���һ����������ֻ��4����������������Ϣ
		CHAIN_APPROX_TC89_L1��CHAIN_APPROX_TC89_KCOSʹ��Teh-Chinl���ƽ��㷨�е�һ��
	*/
	for (int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours.at(i));
		if (area < 2000)  //todo:ʵ�ʻ����У����ܸ�������ͷ�Ĳɼ�ͼ��ѡȡ���ʵľ��������С2000
		{
			//�����Ѱ�������̫С��������������
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
		cv::approxPolyDP(contours.at(i), approx, eps,true); //true�Ǵ����Ƿ��գ���
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
			//��Ҫ�����ĳ��ƾ�����ȡ����
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
*�������ܣ������ʱ�ļ����ڵĶ���
*���룺
*	void
*�����
*	void
*���ߣ�ZYX
*ʱ��汾��2019-07-09-V1.0
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
	QStringList filter; //������
	filter.append("*");
	QDirIterator it(temp_path, filter, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())//�������л��г�Ա������ִ��ɾ������
		dir.remove(it.next());
}