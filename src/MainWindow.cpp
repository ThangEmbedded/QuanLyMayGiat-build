#include "MainWindow.h"
#include "pages/HomePage.h"
#include "pages/MachineDetailPage.h"
#include "pages/PaymentPage.h"
#include "pages/RunningStatusPage.h"
#include "pages/AdminLoginPage.h"
#include "pages/AdminDashboardPage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);

    // Add pages to the stack
    HomePage *homePage = new HomePage(this);
    MachineDetailPage *machineDetailPage = new MachineDetailPage(this);
    PaymentPage *paymentPage = new PaymentPage(this);
    RunningStatusPage *runningStatusPage = new RunningStatusPage(this);
    AdminLoginPage *adminLoginPage = new AdminLoginPage(this);
    AdminDashboardPage *adminDashboardPage = new AdminDashboardPage(this);

    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(machineDetailPage);
    stackedWidget->addWidget(paymentPage);
    stackedWidget->addWidget(runningStatusPage);
    stackedWidget->addWidget(adminLoginPage);
    stackedWidget->addWidget(adminDashboardPage);

    // Set the initial page
    stackedWidget->setCurrentIndex(0); // Home Dashboard
}

MainWindow::~MainWindow()
{
}
