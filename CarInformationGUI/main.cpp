#include "carinformationgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CarInformationGUI w;
	w.show();
	return a.exec();
}
