#include "MainWindow.h"
#include "services/MockHardwareService.h"
#include "services/MockRelayService.h"
#include <QFile>
#include <QTimer>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : MainWindow(std::make_unique<MockRelayService>(), parent)
{
}

MainWindow::MainWindow(std::unique_ptr<IRelayService> relayService, QWidget *parent)
    : QMainWindow(parent) {

    setWindowFlags(Qt::FramelessWindowHint);

    QFile file(":/resources/style.qss");
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
    }

    machineController = new MachineController(std::make_unique<MockHardwareService>(),
                                              std::move(relayService),
                                              this);

    setupUi();
    setupConnections();
    machineController->initialize();
    refreshAllPages();
    stackedWidget->setCurrentIndex(HomeIndex);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    stackedWidget = new TransitionStackedWidget(this);
    setCentralWidget(stackedWidget);

    homePage = new HomePage(this);
    confirmStartPage = new ConfirmStartPage(this);
    adminAuthPage = new AdminAuthPage(this);
    adminSetupPage = new AdminDashboardPage(this);

    stackedWidget->addWidget(homePage);          // 0: Home control máy giặt
    stackedWidget->addWidget(confirmStartPage);  // 1: Nhập phòng + xác nhận bật máy
    stackedWidget->addWidget(adminAuthPage);     // 2: Xác thực mật khẩu admin
    stackedWidget->addWidget(adminSetupPage);    // 3: Admin setup
}

void MainWindow::setupConnections() {
    connect(machineController, &MachineController::machinesChanged,
            this, [this](const QList<MachineData>&) {
        refreshAllPages();
    });

    connect(machineController, &MachineController::logCreated,
            adminSetupPage, &AdminDashboardPage::addLog);

    connect(machineController, &MachineController::operationFailed,
            this, [this](const QString &message) {
        if (adminSetupPage) {
            adminSetupPage->addLog(QString("Cảnh báo: %1").arg(message));
        }
    });

    connect(homePage, &HomePage::machineSelected, this, [this](int id) {
        const MachineData machine = machineController->machineById(id);
        if (machine.id == 0) {
            return;
        }
        if (machine.state != MachineState::Open) {
            if (adminSetupPage) {
                adminSetupPage->addLog(QString("Máy %1 chưa sẵn sàng.").arg(id));
            }
            return;
        }

        m_selectedMachineId = id;
        confirmStartPage->setMachineAndCycle(machine, "Giặt thường", 45, 0);
        stackedWidget->setCurrentIndexWithFade(ConfirmStartIndex);
    });

    connect(homePage, &HomePage::adminRequested, this, [this]() {
        adminAuthPage->reset();
        stackedWidget->setCurrentIndexWithFade(AdminAuthIndex);
    });

    connect(adminAuthPage, &AdminAuthPage::backRequested,
            this, &MainWindow::goHome);

    connect(adminAuthPage, &AdminAuthPage::authSucceeded, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(AdminSetupIndex);
    });

    connect(adminAuthPage, &AdminAuthPage::authFailed, this, [this]() {
        if (adminSetupPage) {
            adminSetupPage->addLog("Sai mật khẩu admin.");
        }
        QTimer::singleShot(5000, this, &MainWindow::goHome);
    });

    connect(confirmStartPage, &ConfirmStartPage::backRequested,
            this, &MainWindow::goHome);

    connect(confirmStartPage, &ConfirmStartPage::startConfirmed,
            this, [this](int machineId, const QString &room) {
        WashCycle cycle;
        cycle.name = "Giặt thường";
        cycle.priceVnd = 0;
        cycle.durationMinutes = 45;

        if (machineController->startMachine(machineId, cycle, room)) {
            goHome();
        }
    });

    connect(adminSetupPage, &AdminDashboardPage::backRequested,
            this, &MainWindow::goHome);

    connect(adminSetupPage, &AdminDashboardPage::toggleMachineState,
            machineController, &MachineController::toggleMachineOnlineState);

    connect(adminSetupPage, &AdminDashboardPage::resetMachineState,
            machineController, &MachineController::resetMachine);
}

void MainWindow::refreshAllPages() {
    homePage->updateMachines(machineController->machines());
    adminSetupPage->updateMachines(machineController->machines());
}

void MainWindow::goHome() {
    stackedWidget->setCurrentIndexWithFade(HomeIndex);
}
