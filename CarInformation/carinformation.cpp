#include "carinformation.h"

CarInformation::CarInformation()
{
}
CarInformation::~CarInformation()
{
}

void CarInformation::Release()
{
	if (UserInfo != nullptr)
	{
		delete UserInfo;
		UserInfo = nullptr;
	}
	if (CarInfo != nullptr)
	{
		delete CarInfo;
		CarInfo = nullptr;
	}
}
