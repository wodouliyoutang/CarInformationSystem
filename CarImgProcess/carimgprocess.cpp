#include "carimgprocess.h"

CarImgProcess::CarImgProcess()
{
	DatabaseHandler = new MDatabaseHandle();
	CarTemplateLocator = new MLocateCarPlate();  //定位识别车牌
	CarPlateSegmentor = new MSegmentCarPlate();  //分割车牌字符
}
CarImgProcess::~CarImgProcess()
{
	if (DatabaseHandler != nullptr)
	{
		delete DatabaseHandler;
		DatabaseHandler = nullptr;
	}
	if (CarTemplateLocator != nullptr)
	{
		delete CarTemplateLocator;
		CarTemplateLocator = nullptr;
	}
	if (CarPlateSegmentor != nullptr)
	{
		delete CarPlateSegmentor;
		CarPlateSegmentor = nullptr;
	}
}


