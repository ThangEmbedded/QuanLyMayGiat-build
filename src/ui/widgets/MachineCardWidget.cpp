#include "MachineCardWidget.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStyle>

MachineCardWidget::MachineCardWidget(int id, QWidget *parent)
    : QWidget(parent), m_id(id), m_state("open") {
    
    setObjectName("MachineCard");
    setCursor(Qt::PointingHandCursor);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 16, 12, 16);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignCenter);

    // Machine Name (e.g. MÁY 1)
    m_nameLabel = new QLabel(QString("MÁY %1").arg(m_id), this);
    m_nameLabel->setObjectName("CardTitle");
    m_nameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_nameLabel);

    // Progress Ring (Diameter: ~120px)
    m_progressRing = new CircularProgressRing(this);
    m_progressRing->setFixedSize(120, 120);
    m_progressRing->setLineWidth(8);
    layout->addWidget(m_progressRing);

    // Status Label (e.g. Còn trống, Đang chạy, Tạm ngưng)
    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName("CardState");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_statusLabel);

    // Claimer / Remaining label (e.g. Phòng 302 / Còn 18 phút)
    m_claimerLabel = new QLabel(this);
    m_claimerLabel->setObjectName("CardDetail");
    m_claimerLabel->setAlignment(Qt::AlignCenter);
    m_claimerLabel->setWordWrap(true);
    layout->addWidget(m_claimerLabel);

    // Add small stretch
    layout->addStretch();

    // Claim button
    m_claimButton = new QPushButton("Nhận máy", this);
    m_claimButton->setObjectName("PrimaryButton");
    m_claimButton->setFixedHeight(36);
    // Remove pointer click blocking on QPushButton so the whole card is clickable easily
    m_claimButton->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    layout->addWidget(m_claimButton);

    // Initial data setup
    setMachineData(QString("MÁY %1").arg(m_id), "open");
}

void MachineCardWidget::setMachineData(const QString &name, const QString &state, const QString &claimer, int remaining, int total) {
    m_state = state;
    m_nameLabel->setText(name.toUpper());

    // Update QSS properties
    setProperty("mState", state);
    m_statusLabel->setProperty("mState", state);
    
    // Polish style to apply stylesheet changes
    style()->unpolish(this);
    style()->polish(this);
    style()->unpolish(m_statusLabel);
    style()->polish(m_statusLabel);

    if (state == "open") {
        setCursor(Qt::PointingHandCursor);
        m_statusLabel->setText("Còn trống");
        m_claimerLabel->setText("Sẵn sàng sử dụng");
        m_claimerLabel->setStyleSheet("color: #8e9994;");
        m_claimButton->show();

        m_progressRing->setColor(QColor("#006c49"));
        m_progressRing->setIconColor(QColor("#006c49"));
        m_progressRing->setDashed(true);
        m_progressRing->setProgress(0.0);
    } 
    else if (state == "running") {
        setCursor(Qt::PointingHandCursor);
        m_statusLabel->setText("Đang giặt");
        m_claimerLabel->setText(QString("%1\nCòn %2 phút").arg(claimer).arg(remaining));
        m_claimerLabel->setStyleSheet("color: #191c1d; font-weight: bold;");
        m_claimButton->hide();

        m_progressRing->setColor(QColor("#0051ca"));
        m_progressRing->setIconColor(QColor("#0051ca"));
        m_progressRing->setDashed(false);
        double pct = (total - remaining) / static_cast<double>(total);
        m_progressRing->setProgress(pct);
    } 
    else { // offline
        setCursor(Qt::ArrowCursor);
        m_statusLabel->setText("Tạm ngưng");
        m_claimerLabel->setText("Ngoại tuyến");
        m_claimerLabel->setStyleSheet("color: #8e9994;");
        m_claimButton->hide();

        m_progressRing->setColor(QColor("#8e9994"));
        m_progressRing->setIconColor(QColor("#8e9994"));
        m_progressRing->setDashed(false);
        m_progressRing->setProgress(0.0);
    }
}

void MachineCardWidget::mousePressEvent(QMouseEvent *event) {
    if (m_state != "offline" && event->button() == Qt::LeftButton) {
        emit clicked(m_id);
    }
    QWidget::mousePressEvent(event);
}
