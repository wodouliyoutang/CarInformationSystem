#pragma once

#include <QtWidgets/QMainWindow>
#include "mdatabasehandle.h"
#include "carinformation.h"
#include "ui_carinformationgui.h"

class CarInformationGUI : public QMainWindow
{
	Q_OBJECT

public:
	CarInformationGUI(QWidget *parent = Q_NULLPTR);
	~CarInformationGUI();
public:
	MDatabaseHandle* mdataBaseHandeler=nullptr;

private:
	Ui::CarInformationGUIClass ui;
};
