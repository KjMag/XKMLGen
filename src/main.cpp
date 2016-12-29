#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xkmlgen);

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
