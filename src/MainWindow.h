#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "widgets/TransitionStackedWidget.h"
#include "pages/HomePage.h"
#include "pages/MachineDetailPage.h"
#include "pages/PaymentPage.h"
#include "pages/RunningStatusPage.h"
#include "pages/AdminLoginPage.h"
#include "pages/AdminDashboardPage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    TransitionStackedWidget *stackedWidget;
    
    HomePage *homePage;
    MachineDetailPage *machineDetailPage;
    PaymentPage *paymentPage;
    RunningStatusPage *runningStatusPage;
    AdminLoginPage *adminLoginPage;
    AdminDashboardPage *adminDashboardPage;

    QList<MachineData> m_machines;

    void initializeMachines();
    void setupConnections();
    void refreshAllPages();
};

#endif // MAINWINDOW_H
