#include "carimgprocess.h"

CarImgProcess::CarImgProcess()
{
	DatabaseHandler = new MDatabaseHandle();
	CarTemplateLocator = new MLocateCarPlate();  //��λʶ����
	CarPlateSegmentor = new MSegmentCarPlate();  //�ָ���ַ�
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


