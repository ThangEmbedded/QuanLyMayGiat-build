#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.setFixedSize(800, 480);
    w.showFullScreen();

    return app.exec();
}
