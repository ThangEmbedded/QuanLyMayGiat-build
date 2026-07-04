#include "MainWindow.h"
#include "services/MockHardwareService.h"
#include <QFile>
#include <QMessageBox>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setWindowFlags(Qt::FramelessWindowHint);

    QFile file(":/resources/style.qss");
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
    }

    machineController = new MachineController(std::make_unique<MockHardwareService>(), this);

    setupUi();
    setupConnections();
    machineController->initialize();
    refreshAllPages();
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    stackedWidget = new TransitionStackedWidget(this);
    setCentralWidget(stackedWidget);

    homePage = new HomePage(this);
    machineDetailPage = new MachineDetailPage(this);
    paymentPage = new PaymentPage(this);
    runningStatusPage = new RunningStatusPage(this);
    adminLoginPage = new AdminLoginPage(this);
    adminDashboardPage = new AdminDashboardPage(this);

    stackedWidget->addWidget(homePage);            // Index 0
    stackedWidget->addWidget(machineDetailPage);   // Index 1
    stackedWidget->addWidget(paymentPage);         // Index 2
    stackedWidget->addWidget(runningStatusPage);   // Index 3
    stackedWidget->addWidget(adminLoginPage);      // Index 4
    stackedWidget->addWidget(adminDashboardPage);  // Index 5
}

void MainWindow::setupConnections() {
    connect(machineController, &MachineController::machinesChanged,
            this, [this](const QList<MachineData>&) {
        refreshAllPages();
    });

    connect(machineController, &MachineController::machineUpdated,
            this, [this](const MachineData &machine) {
        if (stackedWidget->currentWidget() == runningStatusPage && machine.id == m_selectedMachineId) {
            runningStatusPage->setMachine(machine);
        }
    });

    connect(machineController, &MachineController::logCreated,
            adminDashboardPage, &AdminDashboardPage::addLog);

    connect(machineController, &MachineController::operationFailed,
            this, [this](const QString &message) {
        QMessageBox::warning(this, "Thao tác không thành công", message);
    });

    connect(homePage, &HomePage::machineSelected, this, [this](int id) {
        const MachineData machine = machineController->machineById(id);
        if (machine.id == 0) {
            return;
        }

        m_selectedMachineId = id;
        if (machine.state == MachineState::Open) {
            machineDetailPage->setMachine(machine);
            stackedWidget->setCurrentIndexWithFade(1);
        } else if (machine.state == MachineState::Running) {
            runningStatusPage->setMachine(machine);
            stackedWidget->setCurrentIndexWithFade(3);
        }
    });

    connect(homePage, &HomePage::adminRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(4);
    });

    connect(machineDetailPage, &MachineDetailPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });

    connect(machineDetailPage, &MachineDetailPage::paymentRequested, this, [this](int id) {
        paymentPage->setMachine(id,
                                machineDetailPage->getSelectedCycle(),
                                machineDetailPage->getSelectedPrice());
        stackedWidget->setCurrentIndexWithFade(2);
    });

    connect(paymentPage, &PaymentPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(1);
    });

    connect(paymentPage, &PaymentPage::paymentConfirmed, this,
            [this](int machineId, const QString &room) {
        WashCycle cycle;
        cycle.name = machineDetailPage->getSelectedCycle();
        cycle.priceVnd = machineDetailPage->getSelectedPrice();
        cycle.durationMinutes = machineDetailPage->getSelectedDuration();

        if (machineController->startMachine(machineId, cycle, room)) {
            const MachineData machine = machineController->machineById(machineId);
            runningStatusPage->setMachine(machine);
            m_selectedMachineId = machineId;
            stackedWidget->setCurrentIndexWithFade(3);
        }
    });

    connect(runningStatusPage, &RunningStatusPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });

    connect(adminLoginPage, &AdminLoginPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });

    connect(adminLoginPage, &AdminLoginPage::loginSuccess, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(5);
    });

    connect(adminDashboardPage, &AdminDashboardPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });

    connect(adminDashboardPage, &AdminDashboardPage::toggleMachineState,
            machineController, &MachineController::toggleMachineOnlineState);

    connect(adminDashboardPage, &AdminDashboardPage::resetMachineState,
            machineController, &MachineController::resetMachine);
}

void MainWindow::refreshAllPages() {
    homePage->updateMachines(machineController->machines());
    adminDashboardPage->updateMachines(machineController->machines());
}
