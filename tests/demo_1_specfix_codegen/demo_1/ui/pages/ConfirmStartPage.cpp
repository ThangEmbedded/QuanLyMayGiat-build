#include "ConfirmStartPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLocale>

ConfirmStartPage::ConfirmStartPage(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Xác nhận bật máy giặt");
    mainLayout->addWidget(m_header);

    QWidget *content = new QWidget(this);
    content->setObjectName("ConfirmContent");
    content->setStyleSheet("QWidget#ConfirmContent { background-color: #f8f9fa; }");

    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->setSpacing(20);

    QWidget *summaryCard = new QWidget(content);
    summaryCard->setObjectName("SummaryCard");
    summaryCard->setStyleSheet("QWidget#SummaryCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryCard);
    summaryLayout->setContentsMargins(24, 24, 24, 24);
    summaryLayout->setSpacing(14);

    QLabel *title = new QLabel("THÔNG TIN KHỞI ĐỘNG", summaryCard);
    title->setObjectName("SectionTitle");
    summaryLayout->addWidget(title);

    m_machineLabel = new QLabel(summaryCard);
    m_machineLabel->setStyleSheet("font-size: 28px; font-weight: 800; color: #191c1d;");
    summaryLayout->addWidget(m_machineLabel);

    auto addRow = [summaryLayout, summaryCard](const QString &labelText, QLabel *&valueLabel) {
        QGridLayout *row = new QGridLayout();
        row->setContentsMargins(0, 0, 0, 0);
        QLabel *label = new QLabel(labelText, summaryCard);
        label->setObjectName("DetailLabel");
        valueLabel = new QLabel(summaryCard);
        valueLabel->setObjectName("DetailValue");
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        row->addWidget(label, 0, 0);
        row->addWidget(valueLabel, 0, 1);
        summaryLayout->addLayout(row);
    };

    addRow("Chương trình", m_cycleLabel);
    addRow("Thời gian", m_durationLabel);
    addRow("Chi phí demo", m_priceLabel);

    m_statusLabel = new QLabel("Demo hiện chỉ đổi trạng thái mock, chưa bật relay thật.", summaryCard);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("padding: 12px; border-radius: 10px; background-color: #eaf7ef; color: #006c49; font-weight: 600;");
    summaryLayout->addWidget(m_statusLabel);
    summaryLayout->addStretch();

    contentLayout->addWidget(summaryCard, 3);

    QWidget *actionCard = new QWidget(content);
    actionCard->setObjectName("ActionCard");
    actionCard->setStyleSheet("QWidget#ActionCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *actionLayout = new QVBoxLayout(actionCard);
    actionLayout->setContentsMargins(24, 24, 24, 24);
    actionLayout->setSpacing(14);

    QLabel *roomTitle = new QLabel("XÁC THỰC PHÒNG", actionCard);
    roomTitle->setObjectName("SectionTitle");
    actionLayout->addWidget(roomTitle);

    m_roomInput = new QLineEdit(actionCard);
    m_roomInput->setObjectName("RoomInputDisplay");
    m_roomInput->setPlaceholderText("VD: 302");
    m_roomInput->setMaxLength(8);
    m_roomInput->setFixedHeight(52);
    actionLayout->addWidget(m_roomInput);

    QLabel *hint = new QLabel("Nhấn xác nhận để chuyển máy sang trạng thái đang chạy trong mock controller.", actionCard);
    hint->setWordWrap(true);
    hint->setStyleSheet("font-size: 13px; color: #5f6f66;");
    actionLayout->addWidget(hint);

    actionLayout->addStretch();

    m_confirmButton = new QPushButton("Xác nhận bật máy", actionCard);
    m_confirmButton->setObjectName("PrimaryButton");
    m_confirmButton->setFixedHeight(48);
    m_confirmButton->setEnabled(false);
    actionLayout->addWidget(m_confirmButton);

    m_cancelButton = new QPushButton("Quay lại Home", actionCard);
    m_cancelButton->setFixedHeight(42);
    actionLayout->addWidget(m_cancelButton);

    contentLayout->addWidget(actionCard, 2);
    mainLayout->addWidget(content);

    connect(m_header, &HeaderWidget::backClicked, this, &ConfirmStartPage::backRequested);
    connect(m_cancelButton, &QPushButton::clicked, this, &ConfirmStartPage::backRequested);
    connect(m_roomInput, &QLineEdit::textChanged, this, &ConfirmStartPage::updateConfirmState);
    connect(m_confirmButton, &QPushButton::clicked, this, [this]() {
        emit startConfirmed(m_machineId, m_roomInput->text().trimmed());
    });
}

void ConfirmStartPage::setMachineAndCycle(const MachineData &machine,
                                          const QString &cycleName,
                                          int durationMinutes,
                                          int priceVnd) {
    m_machineId = machine.id;
    m_machineLabel->setText(QString("MÁY %1").arg(machine.id));
    m_cycleLabel->setText(cycleName);
    m_durationLabel->setText(QString("%1 phút").arg(durationMinutes));
    m_priceLabel->setText(priceVnd > 0 ? QString("%1 VND").arg(QLocale(QLocale::Vietnamese, QLocale::Vietnam).toString(priceVnd)) : "Miễn phí demo");
    m_header->setTitle(QString("Xác nhận bật Máy %1").arg(machine.id));
    m_roomInput->clear();
    updateConfirmState();
}

void ConfirmStartPage::updateConfirmState() {
    m_confirmButton->setEnabled(m_machineId > 0 && !m_roomInput->text().trimmed().isEmpty());
}
