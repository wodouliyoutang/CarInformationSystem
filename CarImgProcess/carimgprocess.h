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
	MDatabaseHandle* DatabaseHandler=nullptr;   //数据库操作相关
	MLocateCarPlate* CarTemplateLocator=nullptr;  //定位识别车牌
	MSegmentCarPlate* CarPlateSegmentor=nullptr;  //分割车牌字符
};
