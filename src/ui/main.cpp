#include <QApplication>

#include "MainWindow.h"
#include "services/RelayHardwareService.h"

#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window(std::make_unique<RelayHardwareService>());
    window.setFixedSize(800, 480);
    window.showFullScreen();

    return app.exec();
}
