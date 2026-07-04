#include "MachineCardWidget.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStyle>
#include <algorithm>

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

    // Status Label (e.g. Đang trống, Đang giặt, Tạm ngưng)
    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName("CardState");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_statusLabel);

    // Claimer / elapsed label (e.g. Phòng 302 / Đã giặt 12 phút)
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

    m_runningPulseTimer.setInterval(2 * 60 * 1000);
    m_runningPulseTimer.setSingleShot(false);
    connect(&m_runningPulseTimer, &QTimer::timeout, this, &MachineCardWidget::runningPulseOff);

    m_runningRestoreTimer.setInterval(1000);
    m_runningRestoreTimer.setSingleShot(true);
    connect(&m_runningRestoreTimer, &QTimer::timeout, this, &MachineCardWidget::runningPulseOn);

    m_finishedBlinkTimer.setInterval(500);
    connect(&m_finishedBlinkTimer, &QTimer::timeout, this, &MachineCardWidget::finishedBlinkStep);

    m_finishedHoldTimer.setInterval(10 * 1000);
    m_finishedHoldTimer.setSingleShot(true);
    connect(&m_finishedHoldTimer, &QTimer::timeout, this, &MachineCardWidget::finishedHoldDone);

    // Initial data setup
    setMachineData(QString("MÁY %1").arg(m_id), "open");
}

void MachineCardWidget::setMachineData(const QString &name, const QString &state, const QString &claimer, int remaining, int total) {
    m_state = state;
    m_nameLabel->setText(name.toUpper());

    if (m_visualState != state) {
        m_visualState = state;
        applyStatusAnimation(state);
    }

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
        m_statusLabel->setText("Đang trống");
        m_claimerLabel->setText("Sẵn sàng khởi động");
        m_claimerLabel->setStyleSheet("color: #8e9994;");
        m_claimButton->show();

        m_progressRing->setStatusMode(true);
        m_progressRing->setActiveVisible(true);
        m_progressRing->setColor(QColor("#8e9994"));
        m_progressRing->setIconColor(QColor("#8e9994"));
        m_progressRing->setDashed(false);
        m_progressRing->setProgress(1.0);
    } 
    else if (state == "running") {
        setCursor(Qt::ArrowCursor);
        m_statusLabel->setText("Đang giặt");
        const int elapsed = total > 0 ? std::max(0, total - remaining) : 0;
        const QString roomText = claimer.isEmpty() ? "Chưa rõ phòng" : claimer;
        m_claimerLabel->setText(QString("%1\nĐã giặt %2 phút").arg(roomText).arg(elapsed));
        m_claimerLabel->setStyleSheet("color: #191c1d; font-weight: bold;");
        m_claimButton->hide();

        m_progressRing->setStatusMode(true);
        m_progressRing->setColor(QColor("#0051ca"));
        m_progressRing->setIconColor(QColor("#0051ca"));
        m_progressRing->setDashed(false);
        m_progressRing->setProgress(1.0);
    } 
    else if (state == "finished") {
        setCursor(Qt::ArrowCursor);
        m_statusLabel->setText("Đã giặt xong");
        const QString roomText = claimer.isEmpty() ? "Chưa rõ phòng" : claimer;
        m_claimerLabel->setText(QString("%1\nVui lòng lấy đồ").arg(roomText));
        m_claimerLabel->setStyleSheet("color: #a06400; font-weight: bold;");
        m_claimButton->hide();

        m_progressRing->setStatusMode(true);
        m_progressRing->setColor(QColor("#f4b400"));
        m_progressRing->setIconColor(QColor("#f4b400"));
        m_progressRing->setDashed(false);
        m_progressRing->setProgress(1.0);
    }
    else { // offline
        setCursor(Qt::ArrowCursor);
        m_statusLabel->setText("Tạm ngưng");
        m_claimerLabel->setText("Ngoại tuyến");
        m_claimerLabel->setStyleSheet("color: #8e9994;");
        m_claimButton->hide();

        m_progressRing->setStatusMode(true);
        m_progressRing->setActiveVisible(true);
        m_progressRing->setColor(QColor("#8e9994"));
        m_progressRing->setIconColor(QColor("#8e9994"));
        m_progressRing->setDashed(false);
        m_progressRing->setProgress(1.0);
    }
}

void MachineCardWidget::mousePressEvent(QMouseEvent *event) {
    if (m_state == "open" && event->button() == Qt::LeftButton) {
        emit clicked(m_id);
    }
    QWidget::mousePressEvent(event);
}


void MachineCardWidget::applyStatusAnimation(const QString &state) {
    stopStatusAnimation();
    m_progressRing->setActiveVisible(true);

    if (state == "running") {
        startRunningAnimation();
    } else if (state == "finished") {
        startFinishedBlinkCycle();
    }
}

void MachineCardWidget::stopStatusAnimation() {
    m_runningPulseTimer.stop();
    m_runningRestoreTimer.stop();
    m_finishedBlinkTimer.stop();
    m_finishedHoldTimer.stop();
    m_finishedBlinkStep = 0;
}

void MachineCardWidget::startRunningAnimation() {
    m_runningPulseTimer.start();
}

void MachineCardWidget::runningPulseOff() {
    if (m_state != "running") {
        return;
    }
    m_progressRing->setActiveVisible(false);
    m_runningRestoreTimer.start();
}

void MachineCardWidget::runningPulseOn() {
    m_progressRing->setActiveVisible(true);
}

void MachineCardWidget::startFinishedBlinkCycle() {
    m_finishedBlinkStep = 0;
    m_finishedBlinkTimer.start();
}

void MachineCardWidget::finishedBlinkStep() {
    if (m_state != "finished") {
        return;
    }

    // 4 half-second steps = 2 blinks in a 1-second cycle each.
    const bool visible = (m_finishedBlinkStep % 2) == 1;
    m_progressRing->setActiveVisible(visible);
    m_finishedBlinkStep++;

    if (m_finishedBlinkStep >= 4) {
        m_finishedBlinkTimer.stop();
        m_progressRing->setActiveVisible(true);
        m_finishedHoldTimer.start();
    }
}

void MachineCardWidget::finishedHoldDone() {
    if (m_state == "finished") {
        startFinishedBlinkCycle();
    }
}
