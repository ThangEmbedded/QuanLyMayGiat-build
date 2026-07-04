#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/TransitionStackedWidget.h"
#include "pages/HomePage.h"
#include "pages/ConfirmStartPage.h"
#include "pages/AdminAuthPage.h"
#include "pages/AdminDashboardPage.h"
#include "controllers/MachineController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum PageIndex {
        HomeIndex = 0,
        ConfirmStartIndex = 1,
        AdminAuthIndex = 2,
        AdminSetupIndex = 3
    };

    TransitionStackedWidget *stackedWidget = nullptr;

    HomePage *homePage = nullptr;
    ConfirmStartPage *confirmStartPage = nullptr;
    AdminAuthPage *adminAuthPage = nullptr;
    AdminDashboardPage *adminSetupPage = nullptr;

    MachineController *machineController = nullptr;
    int m_selectedMachineId = 0;

    void setupUi();
    void setupConnections();
    void refreshAllPages();
    void goHome();
};

#endif // MAINWINDOW_H
