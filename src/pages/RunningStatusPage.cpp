#include "RunningStatusPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

RunningStatusPage::RunningStatusPage(QWidget *parent)
    : QWidget(parent), m_machineId(1) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Theo dõi trạng thái");
    mainLayout->addWidget(m_header);

    // 2. Content
    QWidget *content = new QWidget(this);
    content->setObjectName("StatusContent");
    content->setStyleSheet("QWidget#StatusContent { background-color: #f8f9fa; }");
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(40, 24, 40, 24);
    contentLayout->setSpacing(40);

    // Left Column: Progress Ring (diameter: ~200px)
    m_progressRing = new CircularProgressRing(content);
    m_progressRing->setFixedSize(200, 200);
    m_progressRing->setLineWidth(14);
    contentLayout->addWidget(m_progressRing, 0, Qt::AlignCenter);

    // Right Column: Details Sheet
    QWidget *detailsPanel = new QWidget(content);
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsPanel);
    detailsLayout->setContentsMargins(0, 0, 0, 0);
    detailsLayout->setSpacing(20);
    detailsLayout->setAlignment(Qt::AlignVCenter);

    // Title Row (Name + State Badge)
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(12);
    m_machineNameLabel = new QLabel("Máy 1", detailsPanel);
    m_machineNameLabel->setStyleSheet("font-family: 'Manrope'; font-size: 28px; font-weight: 800; color: #191c1d;");
    
    m_stateBadgeLabel = new QLabel("ĐANG GIẶT", detailsPanel);
    m_stateBadgeLabel->setStyleSheet("background-color: #eef4ff; color: #0051ca; font-weight: 800; font-size: 11px; border-radius: 12px; padding: 4px 10px;");
    
    titleRow->addWidget(m_machineNameLabel);
    titleRow->addWidget(m_stateBadgeLabel);
    titleRow->addStretch();
    detailsLayout->addLayout(titleRow);

    // Grid details 2x2
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);

    auto addDetailItem = [gridLayout, detailsPanel](const QString &labelName, QLabel* &valueLabel, int row, int col) {
        QVBoxLayout *itemLayout = new QVBoxLayout();
        itemLayout->setSpacing(2);
        
        QLabel *lbl = new QLabel(labelName, detailsPanel);
        lbl->setObjectName("DetailLabel");
        
        valueLabel = new QLabel(detailsPanel);
        valueLabel->setObjectName("DetailValue");
        
        itemLayout->addWidget(lbl);
        itemLayout->addWidget(valueLabel);
        gridLayout->addLayout(itemLayout, row, col);
    };

    addDetailItem("NGƯỜI DÙNG", m_userVal, 0, 0);
    addDetailItem("CHU KỲ", m_cycleVal, 0, 1);
    addDetailItem("BẮT ĐẦU", m_startVal, 1, 0);
    addDetailItem("DỰ KIẾN XONG", m_endVal, 1, 1);

    detailsLayout->addLayout(gridLayout);

    // Action Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    m_notifyBtn = new QPushButton("🔔 Nhắc khi xong", detailsPanel);
    m_notifyBtn->setObjectName("PillSoftButton");
    m_notifyBtn->setFixedHeight(44);
    m_notifyBtn->setCursor(Qt::PointingHandCursor);

    m_homeBtn = new QPushButton("Về danh sách", detailsPanel);
    m_homeBtn->setObjectName("PillSecondaryButton");
    m_homeBtn->setFixedHeight(44);
    m_homeBtn->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(m_notifyBtn);
    btnLayout->addWidget(m_homeBtn);
    btnLayout->addStretch();

    detailsLayout->addLayout(btnLayout);
    contentLayout->addWidget(detailsPanel, 1);

    mainLayout->addWidget(content);

    // Connections
    connect(m_header, &HeaderWidget::backClicked, this, &RunningStatusPage::backRequested);
    connect(m_homeBtn, &QPushButton::clicked, this, &RunningStatusPage::backRequested);
    connect(m_notifyBtn, &QPushButton::clicked, this, [this]() {
        m_notifyBtn->setText("✓ Đã đăng ký nhắc");
        m_notifyBtn->setEnabled(false);
    });
}

void RunningStatusPage::setMachine(const MachineData &machine) {
    m_machineId = machine.id;
    m_machineNameLabel->setText(QString("Máy %1").arg(machine.id));
    m_header->setTitle(QString("Máy %1").arg(machine.id));

    // Reset notification button status
    m_notifyBtn->setText("🔔 Nhắc khi xong");
    m_notifyBtn->setEnabled(true);

    m_userVal->setText(machine.claimer.isEmpty() ? "—" : machine.claimer);
    m_cycleVal->setText(machine.cycle.isEmpty() ? "—" : machine.cycle);
    m_startVal->setText(machine.start.isEmpty() ? "—" : machine.start);
    m_endVal->setText(machine.end.isEmpty() ? "—" : machine.end);

    if (machine.state == "running") {
        m_stateBadgeLabel->setText("ĐANG GIẶT");
        m_stateBadgeLabel->setStyleSheet("background-color: #eef4ff; color: #0051ca; font-weight: 800; font-size: 11px; border-radius: 12px; padding: 4px 10px;");
        m_progressRing->setColor(QColor("#0051ca"));
        m_progressRing->setDashed(false);
        double pct = (machine.total - machine.remaining) / static_cast<double>(machine.total);
        m_progressRing->setProgress(pct);
        m_progressRing->setCenterText(true, QString("%1%").arg(qRound(pct * 100)), QString("còn %1 phút").arg(machine.remaining));
    } 
    else if (machine.state == "open") {
        m_stateBadgeLabel->setText("CÒN TRỐNG");
        m_stateBadgeLabel->setStyleSheet("background-color: #e8faed; color: #006c49; font-weight: 800; font-size: 11px; border-radius: 12px; padding: 4px 10px;");
        m_progressRing->setColor(QColor("#006c49"));
        m_progressRing->setDashed(true);
        m_progressRing->setProgress(0.0);
        m_progressRing->setCenterText(true, "0%", "Sẵn sàng");
    }
}
