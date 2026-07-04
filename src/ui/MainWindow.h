#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/TransitionStackedWidget.h"
#include "pages/HomePage.h"
#include "pages/MachineDetailPage.h"
#include "pages/PaymentPage.h"
#include "pages/RunningStatusPage.h"
#include "pages/AdminLoginPage.h"
#include "pages/AdminDashboardPage.h"
#include "controllers/MachineController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    TransitionStackedWidget *stackedWidget = nullptr;

    HomePage *homePage = nullptr;
    MachineDetailPage *machineDetailPage = nullptr;
    PaymentPage *paymentPage = nullptr;
    RunningStatusPage *runningStatusPage = nullptr;
    AdminLoginPage *adminLoginPage = nullptr;
    AdminDashboardPage *adminDashboardPage = nullptr;

    MachineController *machineController = nullptr;
    int m_selectedMachineId = 0;

    void setupUi();
    void setupConnections();
    void refreshAllPages();
};

#endif // MAINWINDOW_H
