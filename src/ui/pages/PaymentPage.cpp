#include "PaymentPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

PaymentPage::PaymentPage(QWidget *parent)
    : QWidget(parent), m_machineId(1), m_price(45000) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Nhận máy");
    mainLayout->addWidget(m_header);

    // 2. Content
    QWidget *content = new QWidget(this);
    content->setObjectName("PaymentContent");
    content->setStyleSheet("QWidget#PaymentContent { background-color: #f8f9fa; }");
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->setSpacing(20);

    // Left Panel: QR Code Box
    QWidget *qrBox = new QWidget(content);
    qrBox->setObjectName("QRBox");
    qrBox->setStyleSheet("QWidget#QRBox { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *qrLayout = new QVBoxLayout(qrBox);
    qrLayout->setContentsMargins(20, 20, 20, 20);
    qrLayout->setSpacing(8);
    qrLayout->setAlignment(Qt::AlignCenter);

    QLabel *qrMeta = new QLabel("QUÉT MÃ QR", qrBox);
    qrMeta->setObjectName("SectionTitle");
    qrMeta->setAlignment(Qt::AlignCenter);
    
    QLabel *qrTitle = new QLabel("Mở khoá bằng điện thoại", qrBox);
    qrTitle->setStyleSheet("font-family: 'Manrope'; font-size: 16px; font-weight: 800; color: #191c1d;");
    qrTitle->setAlignment(Qt::AlignCenter);

    m_qrWidget = new QRWidget(qrBox);
    m_qrWidget->setFixedSize(160, 160);
    
    QLabel *qrDesc = new QLabel("Quét để nhận máy và nhận thông báo khi giặt xong.", qrBox);
    qrDesc->setStyleSheet("font-size: 12px; color: #8e9994;");
    qrDesc->setAlignment(Qt::AlignCenter);
    qrDesc->setWordWrap(true);

    qrLayout->addWidget(qrMeta);
    qrLayout->addWidget(qrTitle);
    qrLayout->addWidget(m_qrWidget);
    qrLayout->addWidget(qrDesc);

    contentLayout->addWidget(qrBox, 1);

    // Center Divider (OR / HOẶC)
    QWidget *divider = new QWidget(content);
    QVBoxLayout *divLayout = new QVBoxLayout(divider);
    divLayout->setContentsMargins(0, 0, 0, 0);
    divLayout->setSpacing(8);
    divLayout->setAlignment(Qt::AlignCenter);

    QFrame *lineTop = new QFrame(divider);
    lineTop->setFrameShape(QFrame::VLine);
    lineTop->setStyleSheet("background-color: rgba(25, 28, 29, 0.08);");
    lineTop->setFixedHeight(120);

    QLabel *orLabel = new QLabel("HOẶC", divider);
    orLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #8e9994;");

    QFrame *lineBottom = new QFrame(divider);
    lineBottom->setFrameShape(QFrame::VLine);
    lineBottom->setStyleSheet("background-color: rgba(25, 28, 29, 0.08);");
    lineBottom->setFixedHeight(120);

    divLayout->addWidget(lineTop);
    divLayout->addWidget(orLabel);
    divLayout->addWidget(lineBottom);

    contentLayout->addWidget(divider);

    // Right Panel: Numpad Room Input Box
    QWidget *numpadBox = new QWidget(content);
    numpadBox->setObjectName("NumpadBox");
    numpadBox->setStyleSheet("QWidget#NumpadBox { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    QVBoxLayout *numLayout = new QVBoxLayout(numpadBox);
    numLayout->setContentsMargins(20, 20, 20, 20);
    numLayout->setSpacing(10);

    QLabel *numMeta = new QLabel("NHẬP SỐ PHÒNG", numpadBox);
    numMeta->setObjectName("SectionTitle");
    numLayout->addWidget(numMeta);

    // Room Input display field
    m_roomInput = new QLineEdit(numpadBox);
    m_roomInput->setObjectName("RoomInputDisplay");
    m_roomInput->setReadOnly(true);
    m_roomInput->setPlaceholderText("Nhập số phòng của bạn");
    m_roomInput->setFixedHeight(50);
    numLayout->addWidget(m_roomInput);

    // Keypad Grid
    QWidget *keypadGrid = new QWidget(numpadBox);
    QGridLayout *gridLayout = new QGridLayout(keypadGrid);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(8);

    QStringList keys = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "C", "0", "DEL"};
    for (int i = 0; i < keys.size(); i++) {
        QString key = keys[i];
        QPushButton *btn = new QPushButton(keypadGrid);
        btn->setText(key == "DEL" ? "⌫" : key);
        btn->setFixedHeight(44);
        btn->setCursor(Qt::PointingHandCursor);

        if (key == "C" || key == "DEL") {
            btn->setObjectName("KeyButtonSpecial");
        } else {
            btn->setObjectName("KeyButton");
        }

        connect(btn, &QPushButton::clicked, this, [this, key]() {
            handleKeyPress(key);
        });

        int r = i / 3;
        int c = i % 3;
        gridLayout->addWidget(btn, r, c);
    }
    numLayout->addWidget(keypadGrid);

    // Confirm button
    m_confirmButton = new QPushButton("Xác nhận nhận máy", numpadBox);
    m_confirmButton->setObjectName("PrimaryButton");
    m_confirmButton->setFixedHeight(46);
    m_confirmButton->setEnabled(false); // starts disabled
    numLayout->addWidget(m_confirmButton);

    contentLayout->addWidget(numpadBox, 1);
    mainLayout->addWidget(content);

    // Connects
    connect(m_header, &HeaderWidget::backClicked, this, &PaymentPage::backRequested);
    connect(m_confirmButton, &QPushButton::clicked, this, [this]() {
        emit paymentConfirmed(m_machineId, m_roomInput->text());
    });
}

void PaymentPage::setMachine(int machineId, const QString &cycle, int price) {
    m_machineId = machineId;
    m_cycle = cycle;
    m_price = price;
    m_roomInput->clear();
    m_confirmButton->setEnabled(false);
    m_header->setTitle(QString("Nhận Máy %1").arg(machineId));
}

void PaymentPage::handleKeyPress(const QString &key) {
    QString text = m_roomInput->text();
    if (key == "C") {
        m_roomInput->clear();
    } 
    else if (key == "DEL") {
        if (!text.isEmpty()) {
            text.chop(1);
            m_roomInput->setText(text);
        }
    } 
    else { // Number keys
        if (text.length() < 4) { // Limit to 4 digits
            text.append(key);
            m_roomInput->setText(text);
        }
    }

    // Enable/Disable confirm button based on input
    bool hasInput = !m_roomInput->text().isEmpty();
    m_confirmButton->setEnabled(hasInput);
}
