#pragma once
#include <QString>

#include "carinformation_global.h"

class CARINFORMATION_EXPORT MUserInfo
{
public:
	MUserInfo();
	~MUserInfo();
public:
	QString UserName; //车主姓名
	int UserGender; //车主性别，0男，1女

};
