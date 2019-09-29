#include"mdatabasehandle.h"
#include<QMessageBox>

MDatabaseHandle::MDatabaseHandle()
{
}

/************************************************
*函数功能：连接car_information数据库
*输入：
*	void
*输出：
*	bool   连接数据库是否成功
*作者：ZYX
*时间版本：2019-09-25-V1.0
*************************************************/
bool MDatabaseHandle::DatabaseConnector()
{
	//数据库配置
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("car_information");
	db.setUserName("root");
	db.setPassword("zyx1994319");   //这里输入你的密码
	//连接数据库
	if (!db.open())
	{
		QMessageBox::critical(0, QStringLiteral("无法打开数据库"),QStringLiteral("无法创建数据库连接！"),QMessageBox::Cancel);
		return false;
	}
	return true;
}

/************************************************
*函数功能：往car_information数据库写入数据
*输入：
*	CarInformation   车辆信息
*输出：
*	bool			 输入数据是否成功
*作者：ZYX
*时间版本：2019-09-25-V1.0
*************************************************/
bool MDatabaseHandle::WriteToDatabase(CarInformation* carInformation)
{
	if (carInformation == nullptr)
	{
		return false;
	}
	QSqlQuery query(db);
	QString sql;
	sql = "";
	sql = QString("insert into car_information(license_plate_number,user_name,user_gender,car_type,swept_volumn) values('%1','%2',%3,'%4',%5)").arg(carInformation->LicensePlateNumber).arg(carInformation->UserInfo->UserName).arg(carInformation->UserInfo->UserGender).arg(carInformation->CarInfo->CarType).arg(carInformation->CarInfo->SweptVolumne);
	//sql = QString("insert into car_information values(%1,%2,%3,%4,%5,%6)").arg(0).arg(carInformation->LicensePlateNumber).arg(carInformation->UserInfo->UserName).arg(carInformation->UserInfo->UserGender).arg(carInformation->CarInfo->CarType).arg(carInformation->CarInfo->SweptVolumne);
	bool isSuccess=query.exec(sql);
	return true;
}