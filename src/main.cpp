#include <QApplication>

#include "mainwindow.h"

using namespace tln::xkmlgen;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xkmlgen);

	QString appname = "xkmlgen";
    QApplication app(argc, argv);
	QCoreApplication::setApplicationName(appname);
    MainWindow window;
	window.setWindowTitle(appname);
    window.show();
    return app.exec();
}
