#include "AdminDashboardPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

AdminDashboardPage::AdminDashboardPage(QWidget *parent)
    : QWidget(parent) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Bảng điều khiển Admin");
    mainLayout->addWidget(m_header);

    // 2. Content
    QWidget *content = new QWidget(this);
    content->setObjectName("AdminContent");
    content->setStyleSheet("QWidget#AdminContent { background-color: #f8f9fa; }");
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->setSpacing(20);

    // Left Panel: Machine Control List
    QWidget *controlPanel = new QWidget(content);
    controlPanel->setObjectName("ControlCard");
    controlPanel->setStyleSheet("QWidget#ControlCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);
    controlLayout->setContentsMargins(20, 20, 20, 20);
    controlLayout->setSpacing(12);

    QLabel *controlTitle = new QLabel("QUẢN LÝ MÁY GIẶT", controlPanel);
    controlTitle->setObjectName("SectionTitle");
    controlLayout->addWidget(controlTitle);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(10);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);

    for (int id = 1; id <= 4; id++) {
        QLabel *nameLbl = new QLabel(QString("Máy %1").arg(id), controlPanel);
        nameLbl->setStyleSheet("font-weight: bold; font-size: 14px;");
        
        QLabel *statusLbl = new QLabel("Ngoại tuyến", controlPanel);
        statusLbl->setStyleSheet("font-size: 13px; color: #8e9994;");

        QPushButton *toggleBtn = new QPushButton("Tắt máy", controlPanel);
        toggleBtn->setFixedHeight(32);
        toggleBtn->setCursor(Qt::PointingHandCursor);
        toggleBtn->setStyleSheet("font-size: 12px; padding: 4px 10px;");

        QPushButton *resetBtn = new QPushButton("Reset", controlPanel);
        resetBtn->setFixedHeight(32);
        resetBtn->setCursor(Qt::PointingHandCursor);
        resetBtn->setStyleSheet("font-size: 12px; padding: 4px 10px;");

        int r = id - 1;
        grid->addWidget(nameLbl, r, 0);
        grid->addWidget(statusLbl, r, 1);
        grid->addWidget(toggleBtn, r, 2);
        grid->addWidget(resetBtn, r, 3);

        m_controls.append({id, nameLbl, statusLbl, toggleBtn, resetBtn});

        connect(toggleBtn, &QPushButton::clicked, this, [this, id]() {
            emit toggleMachineState(id);
        });
        connect(resetBtn, &QPushButton::clicked, this, [this, id]() {
            emit resetMachineState(id);
        });
    }
    controlLayout->addLayout(grid);
    controlLayout->addStretch();
    contentLayout->addWidget(controlPanel, 3);

    // Right Panel: Log Viewer
    QWidget *logPanel = new QWidget(content);
    logPanel->setObjectName("LogCard");
    logPanel->setStyleSheet("QWidget#LogCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *logLayout = new QVBoxLayout(logPanel);
    logLayout->setContentsMargins(20, 20, 20, 20);
    logLayout->setSpacing(10);

    QLabel *logTitle = new QLabel("NHẬT KÝ HỆ THỐNG", logPanel);
    logTitle->setObjectName("SectionTitle");
    logLayout->addWidget(logTitle);

    m_logWidget = new QListWidget(logPanel);
    m_logWidget->setStyleSheet("border: 1px solid rgba(25,28,29,0.08); border-radius: 8px; font-size: 11px; font-family: monospace; color: #3c4a42;");
    logLayout->addWidget(m_logWidget);

    contentLayout->addWidget(logPanel, 2);
    mainLayout->addWidget(content);

    // Seed initial logs
    addLog("Hệ thống khởi động thành công.");
    addLog("Tải tệp giao diện QSS stylesheet.");

    // Connects
    connect(m_header, &HeaderWidget::backClicked, this, &AdminDashboardPage::backRequested);
}

void AdminDashboardPage::addLog(const QString &logText) {
    m_logWidget->addItem(logText);
    m_logWidget->scrollToBottom();
}

void AdminDashboardPage::updateMachines(const QList<MachineData> &machines) {
    for (const MachineData &m : machines) {
        if (m.id > 0 && m.id <= m_controls.size()) {
            MachineControl &ctrl = m_controls[m.id - 1];
            if (m.state == "open") {
                ctrl.statusLabel->setText("Còn trống");
                ctrl.statusLabel->setStyleSheet("color: #006c49; font-weight: bold;");
                ctrl.toggleBtn->setText("Tắt máy");
                ctrl.toggleBtn->setEnabled(true);
                ctrl.resetBtn->setEnabled(false);
            } 
            else if (m.state == "running") {
                ctrl.statusLabel->setText(QString("Đang chạy (%1)").arg(m.claimer));
                ctrl.statusLabel->setStyleSheet("color: #0051ca; font-weight: bold;");
                ctrl.toggleBtn->setText("Tắt máy");
                ctrl.toggleBtn->setEnabled(true);
                ctrl.resetBtn->setEnabled(true);
            } 
            else { // offline
                ctrl.statusLabel->setText("Ngoại tuyến");
                ctrl.statusLabel->setStyleSheet("color: #8e9994;");
                ctrl.toggleBtn->setText("Bật máy");
                ctrl.toggleBtn->setEnabled(true);
                ctrl.resetBtn->setEnabled(false);
            }
        }
    }
}
