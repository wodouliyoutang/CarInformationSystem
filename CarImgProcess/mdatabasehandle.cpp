#include"mdatabasehandle.h"
#include<QMessageBox>

MDatabaseHandle::MDatabaseHandle()
{
}

/************************************************
*�������ܣ�����car_information���ݿ�
*���룺
*	void
*�����
*	bool   �������ݿ��Ƿ�ɹ�
*���ߣ�ZYX
*ʱ��汾��2019-09-25-V1.0
*************************************************/
bool MDatabaseHandle::DatabaseConnector()
{
	//���ݿ�����
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("car_information");
	db.setUserName("root");
	db.setPassword("zyx1994319");   //���������������
	//�������ݿ�
	if (!db.open())
	{
		QMessageBox::critical(0, QStringLiteral("�޷������ݿ�"),QStringLiteral("�޷��������ݿ����ӣ�"),QMessageBox::Cancel);
		return false;
	}
	return true;
}

/************************************************
*�������ܣ���car_information���ݿ�д������
*���룺
*	CarInformation   ������Ϣ
*�����
*	bool			 ���������Ƿ�ɹ�
*���ߣ�ZYX
*ʱ��汾��2019-09-25-V1.0
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