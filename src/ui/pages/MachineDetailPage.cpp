#include "MachineDetailPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

MachineDetailPage::MachineDetailPage(QWidget *parent)
    : QWidget(parent), m_machineId(1), m_selectedCycle("Giặt thường"), m_selectedPrice(45000), m_selectedDuration(45) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Điều khiển máy giặt");
    mainLayout->addWidget(m_header);

    // 2. Content
    QWidget *content = new QWidget(this);
    content->setObjectName("DetailContent");
    content->setStyleSheet("QWidget#DetailContent { background-color: #f8f9fa; }");
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->setSpacing(20);

    // Left Panel: Machine Summary Card
    QWidget *leftPanel = new QWidget(content);
    leftPanel->setObjectName("SummaryCard");
    leftPanel->setStyleSheet("QWidget#SummaryCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    leftPanel->setFixedWidth(280);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(20, 20, 20, 20);
    leftLayout->setSpacing(12);

    m_machineTitleLabel = new QLabel("MÁY 3", leftPanel);
    m_machineTitleLabel->setStyleSheet("font-family: 'Manrope'; font-size: 24px; font-weight: 800; color: #191c1d;");
    leftLayout->addWidget(m_machineTitleLabel);

    QFrame *line = new QFrame(leftPanel);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: rgba(25, 28, 29, 0.08);");
    leftLayout->addWidget(line);

    auto addSummaryRow = [leftLayout, leftPanel](const QString &labelName, QLabel* &valueLabel) {
        QVBoxLayout *row = new QVBoxLayout();
        row->setSpacing(2);
        QLabel *lbl = new QLabel(labelName, leftPanel);
        lbl->setObjectName("DetailLabel");
        valueLabel = new QLabel(leftPanel);
        valueLabel->setObjectName("DetailValue");
        row->addWidget(lbl);
        row->addWidget(valueLabel);
        leftLayout->addLayout(row);
    };

    addSummaryRow("Chương trình đã chọn:", m_cycleLabel);
    addSummaryRow("Thời gian giặt:", m_durationLabel);
    addSummaryRow("Chi phí demo:", m_priceLabel);

    leftLayout->addStretch();
    contentLayout->addWidget(leftPanel);

    // Right Panel: Cycle Selection List
    QWidget *rightPanel = new QWidget(content);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    QLabel *selectionTitle = new QLabel("CHỌN CHU KỲ GIẶT", rightPanel);
    selectionTitle->setObjectName("SectionTitle");
    rightLayout->addWidget(selectionTitle);

    QPushButton *btnStd = new QPushButton(rightPanel);
    QPushButton *btnQuick = new QPushButton(rightPanel);
    QPushButton *btnEco = new QPushButton(rightPanel);

    setupCycleButton(btnStd, "Giặt thường", 45000, 45);
    setupCycleButton(btnQuick, "Giặt nhanh", 35000, 30);
    setupCycleButton(btnEco, "Giặt kỹ", 55000, 60);

    rightLayout->addWidget(btnStd);
    rightLayout->addWidget(btnQuick);
    rightLayout->addWidget(btnEco);
    rightLayout->addStretch();

    // Bottom Action Row
    QHBoxLayout *actionLayout = new QHBoxLayout();
    m_payButton = new QPushButton("Tiếp tục xác nhận bật máy", rightPanel);
    m_payButton->setObjectName("PrimaryButton");
    m_payButton->setFixedHeight(48);
    m_payButton->setFixedWidth(200);
    actionLayout->addStretch();
    actionLayout->addWidget(m_payButton);
    rightLayout->addLayout(actionLayout);

    contentLayout->addWidget(rightPanel);
    mainLayout->addWidget(content);

    // Connects
    connect(m_header, &HeaderWidget::backClicked, this, &MachineDetailPage::backRequested);
    connect(m_payButton, &QPushButton::clicked, this, [this]() {
        emit paymentRequested(m_machineId);
    });

    connect(btnStd, &QPushButton::clicked, this, [this]() {
        m_selectedCycle = "Giặt thường";
        m_selectedPrice = 45000;
        m_selectedDuration = 45;
        setMachine({m_machineId, QString("Máy %1").arg(m_machineId), MachineState::Open, "", 0, 0, "", "", ""});
    });
    connect(btnQuick, &QPushButton::clicked, this, [this]() {
        m_selectedCycle = "Giặt nhanh";
        m_selectedPrice = 35000;
        m_selectedDuration = 30;
        setMachine({m_machineId, QString("Máy %1").arg(m_machineId), MachineState::Open, "", 0, 0, "", "", ""});
    });
    connect(btnEco, &QPushButton::clicked, this, [this]() {
        m_selectedCycle = "Giặt kỹ";
        m_selectedPrice = 55000;
        m_selectedDuration = 60;
        setMachine({m_machineId, QString("Máy %1").arg(m_machineId), MachineState::Open, "", 0, 0, "", "", ""});
    });

    // Default select standard
    setMachine({1, "Máy 1", MachineState::Open, "", 0, 0, "", "", ""});
}

void MachineDetailPage::setupCycleButton(QPushButton *btn, const QString &title, int price, int duration) {
    btn->setFixedHeight(54);
    btn->setCursor(Qt::PointingHandCursor);
    
    // Create custom layout inside button to format neatly
    QHBoxLayout *lay = new QHBoxLayout(btn);
    lay->setContentsMargins(16, 0, 16, 0);
    
    QLabel *lblTitle = new QLabel(title, btn);
    lblTitle->setStyleSheet("font-weight: bold; font-size: 14px; color: #191c1d; background: transparent;");
    
    QLabel *lblDuration = new QLabel(QString("%1 phút").arg(duration), btn);
    lblDuration->setStyleSheet("font-size: 13px; color: #8e9994; background: transparent;");
    
    QLabel *lblPrice = new QLabel(QString("%L1đ").arg(price), btn);
    lblPrice->setStyleSheet("font-family: 'Manrope'; font-weight: 800; font-size: 14px; color: #006c49; background: transparent;");

    lay->addWidget(lblTitle);
    lay->addStretch();
    lay->addWidget(lblDuration);
    lay->addSpacing(16);
    lay->addWidget(lblPrice);
}

void MachineDetailPage::setMachine(const MachineData &machine) {
    m_machineId = machine.id;
    m_machineTitleLabel->setText(QString("MÁY %1").arg(machine.id));
    m_header->setTitle(QString("Điều khiển Máy %1").arg(machine.id));
    
    m_cycleLabel->setText(m_selectedCycle);
    m_durationLabel->setText(QString("%1 phút").arg(m_selectedDuration));
    m_priceLabel->setText(QString("%L1 VND").arg(m_selectedPrice));
}
