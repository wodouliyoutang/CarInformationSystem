#include "msegmentcarplate.h"
#include <qlist.h>

MSegmentCarPlate::MSegmentCarPlate()
{
	appExeFolder = new QString(QCoreApplication::applicationDirPath());
	path = QCoreApplication::applicationDirPath().toStdString().data();
	path += +"/imgSegmentCarPlate";
	weightBlueRed = 0.33748;
	weightBlueGreen = 0.66252;
	ClearTempFiles((*appExeFolder) + "/imgSegmentCarPlate");
}

MSegmentCarPlate::~MSegmentCarPlate()
{
}

QList<cv::Mat>  MSegmentCarPlate::VerticalProjectionMethod(cv::Mat img)
{
	QList<cv::Mat> words;
	//�ҶȻ�����
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
#ifdef _DEBUG
	cv::imwrite(path + "/gray.jpg", gray);
#endif
	//��ֵ��
	cv::Mat thresh;
	cv::threshold(gray, thresh, 150, 255, cv::THRESH_BINARY);
#ifdef _DEBUG
	cv::imwrite(path + "/thresh.jpg", thresh);
#endif
	//�����ɫ�����ʹЧ��������
	cv::Mat kernel;
	kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // ��̬ѧ����������νṹ
	cv::Mat closed;
	cv::dilate(thresh, closed, kernel);
#ifdef _DEBUG
	cv::imwrite(path + "/kernel.jpg", closed);
#endif
	int height=closed.rows;
	int width = closed.cols;
	QList<int> countVerticals;
	QList<int> countHorizontals;
	//ˮƽͶӰ
	//ͳ�Ʋ��洢ÿһ�еİ׵���
	for (int i = 0; i < height; i++)
	{
		int countHorizontal = 0;
		for (int j = 0; j < width; j++)
		{
			if (closed.at<uchar>(i, j) == 255)
			{
				countHorizontal += 1;
			}
		}
		countHorizontals.append(countHorizontal);
	}
	//ͳ�ƴ�ֱͶӰ�İ׵�ƽ����
	int horizontalCountSum = 0;
	int height1 = 0;
	for (int i = 0; i < height; i++)
	{
		if (countHorizontals.at(i) > 0 && i > height / 4 && i < height * 3 / 4)
		{
			height1 += 1;
			horizontalCountSum += countHorizontals.at(i);
		}
	}
	float horizontalCountMean = horizontalCountSum / height1 * 2 / 3;
	//�����հ�ͼƬ�����ƴ�ֱͶӰͼ
	cv::Mat emptyImage1(height, width, CV_8UC1); //����һ���Ҷ�ͼ
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (countHorizontals.at(i) > horizontalCountMean)
			{
				emptyImage1.at<uchar>(i, j) = 255;
			}
			else
			{
				emptyImage1.at<uchar>(i, j) = 0;
			}
		}
	}
#ifdef _DEBUG
	cv::imwrite(path + "/horizontal.jpg", emptyImage1);
#endif
	//����ˮƽͶӰ���ȷ��ˮƽ����λ�ã�Ȼ���ȡͼƬ���д�ֱͶӰ
	int flag = 0;
	int iStart = 0;
	int iEnd = 0;
	int maxH = 0;
	QList<int> segmentsH;
	segmentsH.append(0);
	segmentsH.append(0);
	for (int i = 0; i < height;i++)
	{
		if (emptyImage1.at<uchar>(i, 0) == 255  && flag == 0)
		{
			flag = 1;
			iStart = i;
		}
		if (emptyImage1.at<uchar>(i, 0) == 0 && flag == 1)
		{
			flag = 2;
			iEnd = i;
		}
		if (flag == 2)
		{
			flag = 0;
			if (iEnd - iStart > maxH)
			{
				segmentsH[0] = iStart;
				segmentsH[1] = iEnd-1;
				maxH = iEnd - iStart;
			}
		}
	}
	//����ˮƽͶӰ��ȡ���ĳ�������ͼ�������segmentH��ԭͼ���н�ȡ
	cv::Rect rect;
	rect.x = 0;
	rect.width = width;
	rect.y = segmentsH[0];
	rect.height = segmentsH[1]- segmentsH[0];
	cv::Mat imgAfterHorizontalProjection;
	imgAfterHorizontalProjection=thresh(rect);
#ifdef _DEBUG
	cv::imwrite(path + "/imgAfterHorizontalProjection.jpg", imgAfterHorizontalProjection);
#endif
	//�����ɫ�����ʹЧ��������
	//cv::dilate(imgAfterHorizontalProjection, closed, kernel);
	closed= imgAfterHorizontalProjection;
#ifdef _DEBUG
	cv::imwrite(path + "/kernel2.jpg", closed);
#endif
	//������б����
	//std::vector<std::vector<cv::Point>> contours; //������������
	//cv::findContours(closed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	//cv::RotatedRect minRect = cv::minAreaRect(contours.at(0));
	//float angle = minRect.angle;
	////Create and rotate image
	//cv::Mat rotmat = cv::getRotationMatrix2D(minRect.center, angle, 1);
	//cv::Mat rotatedRegion;
	//cv::warpAffine(closed, rotatedRegion, rotmat, closed.size(), cv::INTER_CUBIC);
	//cv::imwrite(path + "/img_rotated.jpg", rotatedRegion);

	//��ֱͶӰ
//ͳ�Ʋ��洢ÿһ�еİ׵���
	countVerticals.clear();
	height = closed.rows;
	width = closed.cols;
	for (int i = 0; i < width; i++)
	{
		int countVertical = 0;
		for (int j = 0; j < height; j++)
		{
			if (closed.at<uchar>(j, i) == 255)
			{
				countVertical += 1;
			}
		}
		countVerticals.append(countVertical);
	}
	//ͳ�ƴ�ֱͶӰ�İ׵�ƽ����
	int verticalCountSum = 0;
	int width1 = 0;
	for (int i = 0; i < width; i++)
	{
		if (countVerticals.at(i)>0 && i > width / 4 && i < width * 3 / 4)
		{
			width1 += 1;
			verticalCountSum += countVerticals.at(i);
		}
	}
	float verticalCountMean = verticalCountSum / width * 2 / 3;
	//�����հ�ͼƬ�����ƴ�ֱͶӰͼ
	cv::Mat emptyImage(height, width, CV_8UC1); //����һ���Ҷ�ͼ
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (countVerticals.at(j) > verticalCountMean)
			{
				emptyImage.at<uchar>(i, j) = 255;
			}
			else
			{
				emptyImage.at<uchar>(i, j) = 0;
			}
		}
	}
#ifdef _DEBUG
	cv::imwrite(path + "/vertical.jpg", emptyImage);
#endif
	//��ʼ�ָ��ַ�
	QList<int> vi;
	QList<int> hi;
	for (int i = 0; i < width;i++)
	{
		if (emptyImage.at<uchar>(0, i) == 255)
		{
			vi.append(1);
		}
		else
		{
			vi.append(0);
		}
	}
	//float vAverage=
	return words;
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
void MSegmentCarPlate::ClearTempFiles(const QString& temp_path)
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