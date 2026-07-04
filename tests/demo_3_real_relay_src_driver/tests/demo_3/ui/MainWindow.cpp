#include "MainWindow.h"
#include "services/MockHardwareService.h"
#ifdef USE_REAL_RELAY
#include "services/RealRelayService.h"
#else
#include "services/MockRelayService.h"
#endif
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setWindowFlags(Qt::FramelessWindowHint);

    QFile file(":/resources/style.qss");
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
    }

    #ifdef USE_REAL_RELAY
    machineController = new MachineController(std::make_unique<MockHardwareService>(),
                                              std::make_unique<RealRelayService>(),
                                              this);
#else
    machineController = new MachineController(std::make_unique<MockHardwareService>(),
                                              std::make_unique<MockRelayService>(),
                                              this);
#endif

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
        QMessageBox::warning(this, "Thao tác không thành công", message);
    });

    connect(homePage, &HomePage::machineSelected, this, [this](int id) {
        const MachineData machine = machineController->machineById(id);
        if (machine.id == 0) {
            return;
        }
        if (machine.state != MachineState::Open) {
            QMessageBox::information(this, "Máy chưa sẵn sàng", "Chỉ có thể chọn máy đang trống.");
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
        QMessageBox *box = new QMessageBox(this);
        box->setIcon(QMessageBox::Warning);
        box->setWindowTitle("Sai mật khẩu");
        box->setText("Sai mật khẩu. Tự động quay về Home sau 5 giây.");
        box->setStandardButtons(QMessageBox::NoButton);

        QTimer::singleShot(5000, box, [this, box]() {
            box->close();
            box->deleteLater();
            goHome();
        });

        box->show();
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
