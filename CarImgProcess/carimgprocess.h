#pragma once

#include "carimgprocess_global.h"
#include "mdatabasehandle.h"
#include "mlocatecarplate.h"
#include "msegmentcarplate.h"
#include<QSqlDatabase>
#include<QSqlQuery>

class CARIMGPROCESS_EXPORT CarImgProcess
{
public:
	CarImgProcess();
	~CarImgProcess();
public:
	MDatabaseHandle* DatabaseHandler=nullptr;   //���ݿ�������
	MLocateCarPlate* CarTemplateLocator=nullptr;  //��λʶ����
	MSegmentCarPlate* CarPlateSegmentor=nullptr;  //�ָ���ַ�
};
