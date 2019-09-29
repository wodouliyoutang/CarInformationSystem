/********************************************************************************
** Form generated from reading UI file 'carinformationgui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARINFORMATIONGUI_H
#define UI_CARINFORMATIONGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CarInformationGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CarInformationGUIClass)
    {
        if (CarInformationGUIClass->objectName().isEmpty())
            CarInformationGUIClass->setObjectName(QString::fromUtf8("CarInformationGUIClass"));
        CarInformationGUIClass->resize(600, 400);
        menuBar = new QMenuBar(CarInformationGUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        CarInformationGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CarInformationGUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CarInformationGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CarInformationGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        CarInformationGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CarInformationGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CarInformationGUIClass->setStatusBar(statusBar);

        retranslateUi(CarInformationGUIClass);

        QMetaObject::connectSlotsByName(CarInformationGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *CarInformationGUIClass)
    {
        CarInformationGUIClass->setWindowTitle(QApplication::translate("CarInformationGUIClass", "CarInformationGUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CarInformationGUIClass: public Ui_CarInformationGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARINFORMATIONGUI_H
