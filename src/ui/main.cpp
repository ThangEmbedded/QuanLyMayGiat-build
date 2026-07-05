#include <QApplication>
#include "MainWindow.h"

#ifdef LAUNDRY_USE_REAL_RELAY
#include "services/RelayHardwareService.h"
#include <memory>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef LAUNDRY_USE_REAL_RELAY
    MainWindow w(std::make_unique<RelayHardwareService>());
#else
    MainWindow w;
#endif
    w.setFixedSize(800, 480);
    w.showFullScreen();

    return app.exec();
}
