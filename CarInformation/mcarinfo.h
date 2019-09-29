#pragma once
#include <QString>

#include "carinformation_global.h"

class CARINFORMATION_EXPORT MCarInfo
{
public:
	MCarInfo();
	~MCarInfo();
public:
	QString CarType; //车辆种类
	float SweptVolumne; //车排量

};
