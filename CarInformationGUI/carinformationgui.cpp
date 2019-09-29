#include "carinformationgui.h"
#include "muserinfo.h"
#include "mcarinfo.h"
#include "mlocatecarplate.h"
#include "carimgprocess.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QString>
#include <string.h>

CarInformationGUI::CarInformationGUI(QWidget *parent)
	: QMainWindow(parent)
{
	CarImgProcess *picHandle = new CarImgProcess();
	ui.setupUi(this);
	std::string ss = "D:/CasitPlusWorkSpace/src/CarInformationSystem/src/4.jpg";
	cv::Mat aa = cv::imread(ss);
	cv::Mat bb;
	QList<cv::Mat> cc;
	bb = picHandle->CarTemplateLocator->DetectCarPlate(aa);
	cc = picHandle->CarPlateSegmentor->VerticalProjectionMethod(bb);
	mdataBaseHandeler = new MDatabaseHandle();
	bool a = false;
	a=mdataBaseHandeler->DatabaseConnector();
	if (a)
	{
		MLocateCarPlate* carLocator=nullptr;
		carLocator = new MLocateCarPlate();

		MUserInfo* userInfo = nullptr;
		userInfo = new MUserInfo();
		MCarInfo* carDetailInfo = nullptr;
		carDetailInfo = new MCarInfo();
		CarInformation* carInfo=nullptr;
		carInfo = new CarInformation();

		userInfo->UserName = QStringLiteral("张小花");
		userInfo->UserGender = 1;
		carDetailInfo->CarType = "miniCooper";
		carDetailInfo->SweptVolumne = 1.6;
		
		carInfo->CarInfo=carDetailInfo;
		carInfo->UserInfo = userInfo;
		carInfo->LicensePlateNumber = QStringLiteral("渝CRQ818");

		mdataBaseHandeler->WriteToDatabase(carInfo);

		//将数据存到数据库之后，释放指针
		carInfo->Release();
		delete carInfo;
		carInfo = nullptr;
	}
}

CarInformationGUI::~CarInformationGUI()
{
	if (mdataBaseHandeler != nullptr)
	{
		delete mdataBaseHandeler;
		mdataBaseHandeler = nullptr;
	}
}
