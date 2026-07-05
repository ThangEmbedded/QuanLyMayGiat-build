#ifndef ADMINDASHBOARDPAGE_H
#define ADMINDASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include "widgets/HeaderWidget.h"
#include "models/MachineData.h"

class AdminDashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminDashboardPage(QWidget *parent = nullptr);

    void updateMachines(const QList<MachineData> &machines);
    void addLog(const QString &logText);

signals:
    void backRequested();
    void toggleMachineState(int id);
    void resetMachineState(int id);

private:
    HeaderWidget *m_header;
    QListWidget *m_logWidget;
    
    struct MachineControl {
        int id;
        QLabel *nameLabel;
        QLabel *statusLabel;
        QPushButton *toggleBtn;
        QPushButton *resetBtn;
    };
    QList<MachineControl> m_controls;
};

#endif // ADMINDASHBOARDPAGE_H
