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
	QString LicensePlateNumber; //���ƺ�
	MUserInfo* UserInfo; //������Ϣ
	MCarInfo* CarInfo; //������Ϣ
public:
	void Release();
};
