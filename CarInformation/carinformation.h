#pragma once
#include <QString>

#include "carinformation_global.h"
#include "muserinfo.h"
#include "mcarinfo.h"

class CARINFORMATION_EXPORT CarInformation
{
public:
	CarInformation();
	~CarInformation();
public:
	QString LicensePlateNumber; //车牌号
	MUserInfo* UserInfo; //车主信息
	MCarInfo* CarInfo; //车辆信息
public:
	void Release();
};
