#pragma once

#include "carimgprocess_global.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include "carinformation.h"

class CARIMGPROCESS_EXPORT MDatabaseHandle
{
public:
	MDatabaseHandle();
public:
	bool DatabaseConnector();
	bool WriteToDatabase(CarInformation* carInformation);
public:
	QSqlDatabase db; //操作的数据库对象
};