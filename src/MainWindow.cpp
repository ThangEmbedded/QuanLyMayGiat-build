#include "MainWindow.h"
#include <QFile>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowFlags(Qt::FramelessWindowHint);
    
    // Load QSS stylesheet
    QFile file(":/resources/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
    }

    stackedWidget = new TransitionStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Initialize Pages
    homePage = new HomePage(this);
    machineDetailPage = new MachineDetailPage(this);
    paymentPage = new PaymentPage(this);
    runningStatusPage = new RunningStatusPage(this);
    adminLoginPage = new AdminLoginPage(this);
    adminDashboardPage = new AdminDashboardPage(this);

    // Add to Stack
    stackedWidget->addWidget(homePage);            // Index 0
    stackedWidget->addWidget(machineDetailPage);    // Index 1
    stackedWidget->addWidget(paymentPage);          // Index 2
    stackedWidget->addWidget(runningStatusPage);    // Index 3
    stackedWidget->addWidget(adminLoginPage);       // Index 4
    stackedWidget->addWidget(adminDashboardPage);   // Index 5

    // Initialize mock data
    initializeMachines();
    
    // Setup connections
    setupConnections();

    // Refresh pages
    refreshAllPages();

    // Set initial page
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
}

void MainWindow::initializeMachines() {
    m_machines.clear();
    m_machines.append({1, "Máy 1", "running", "Phòng 302", 18, 45, "Giặt thường", "14:05", "14:50"});
    m_machines.append({2, "Máy 2", "running", "Phòng 108", 6, 40, "Giặt nhanh", "14:18", "14:58"});
    m_machines.append({3, "Máy 3", "open", "", 0, 45, "", "", ""});
    m_machines.append({4, "Máy 4", "offline", "", 0, 45, "", "", ""});
}

void MainWindow::setupConnections() {
    // 1. Home Page selection
    connect(homePage, &HomePage::machineSelected, this, [this](int id) {
        for (const MachineData &m : m_machines) {
            if (m.id == id) {
                if (m.state == "open") {
                    machineDetailPage->setMachine(m);
                    stackedWidget->setCurrentIndexWithFade(1);
                } else if (m.state == "running") {
                    runningStatusPage->setMachine(m);
                    stackedWidget->setCurrentIndexWithFade(3);
                }
                break;
            }
        }
    });

    connect(homePage, &HomePage::adminRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(4);
    });

    // 2. Machine Detail Page
    connect(machineDetailPage, &MachineDetailPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });
    connect(machineDetailPage, &MachineDetailPage::paymentRequested, this, [this](int id) {
        paymentPage->setMachine(id, machineDetailPage->getSelectedCycle(), machineDetailPage->getSelectedPrice());
        stackedWidget->setCurrentIndexWithFade(2);
    });

    // 3. Payment Page
    connect(paymentPage, &PaymentPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(1);
    });
    connect(paymentPage, &PaymentPage::paymentConfirmed, this, [this](int machineId, const QString &room) {
        for (MachineData &m : m_machines) {
            if (m.id == machineId) {
                m.state = "running";
                m.claimer = QString("Phòng %1").arg(room);
                m.remaining = machineDetailPage->getSelectedDuration();
                m.total = machineDetailPage->getSelectedDuration();
                m.cycle = machineDetailPage->getSelectedCycle();
                
                QTime start = QTime::currentTime();
                QTime end = start.addSecs(m.total * 60);
                m.start = start.toString("HH:mm");
                m.end = end.toString("HH:mm");

                runningStatusPage->setMachine(m);
                adminDashboardPage->addLog(QString("%1 - %2 nhận %3 (%4)").arg(start.toString("HH:mm")).arg(m.claimer).arg(m.name).arg(m.cycle));
                break;
            }
        }
        refreshAllPages();
        stackedWidget->setCurrentIndexWithFade(3);
    });

    // 4. Running Status Page
    connect(runningStatusPage, &RunningStatusPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });

    // 5. Admin Login Page
    connect(adminLoginPage, &AdminLoginPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });
    connect(adminLoginPage, &AdminLoginPage::loginSuccess, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(5);
    });

    // 6. Admin Dashboard Page
    connect(adminDashboardPage, &AdminDashboardPage::backRequested, this, [this]() {
        stackedWidget->setCurrentIndexWithFade(0);
    });
    connect(adminDashboardPage, &AdminDashboardPage::toggleMachineState, this, [this](int id) {
        for (MachineData &m : m_machines) {
            if (m.id == id) {
                if (m.state == "offline") {
                    m.state = "open";
                    adminDashboardPage->addLog(QString("%1 - Bật %2").arg(QTime::currentTime().toString("HH:mm")).arg(m.name));
                } else {
                    m.state = "offline";
                    adminDashboardPage->addLog(QString("%1 - Tắt %2 (Ngoại tuyến)").arg(QTime::currentTime().toString("HH:mm")).arg(m.name));
                }
                break;
            }
        }
        refreshAllPages();
    });
    connect(adminDashboardPage, &AdminDashboardPage::resetMachineState, this, [this](int id) {
        for (MachineData &m : m_machines) {
            if (m.id == id) {
                m.state = "open";
                m.claimer = "";
                m.remaining = 0;
                m.cycle = "";
                m.start = "";
                m.end = "";
                adminDashboardPage->addLog(QString("%1 - Reset %2 về trạng thái Trống").arg(QTime::currentTime().toString("HH:mm")).arg(m.name));
                break;
            }
        }
        refreshAllPages();
    });
}

void MainWindow::refreshAllPages() {
    homePage->updateMachines(m_machines);
    adminDashboardPage->updateMachines(m_machines);
}
