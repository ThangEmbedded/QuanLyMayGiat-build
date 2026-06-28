#include "HeaderWidget.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

HeaderWidget::HeaderWidget(QWidget *parent)
    : QWidget(parent), m_style(OverviewStyle) {
    
    setFixedHeight(68);

    m_leftContainer = new QWidget(this);
    m_leftLayout = new QHBoxLayout(m_leftContainer);
    m_leftLayout->setContentsMargins(0, 0, 0, 0);
    m_leftLayout->setSpacing(8);

    m_backButton = new QPushButton(m_leftContainer);
    m_backButton->setObjectName("BackButton");
    m_backButton->setText("← Quay lại");
    m_backButton->setCursor(Qt::PointingHandCursor);
    m_leftLayout->addWidget(m_backButton);

    QWidget *titleContainer = new QWidget(m_leftContainer);
    QVBoxLayout *titleLayout = new QVBoxLayout(titleContainer);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(2);

    m_subtitleLabel = new QLabel("Trạm giặt · Tầng 3", titleContainer);
    m_subtitleLabel->setObjectName("HeaderSub");
    m_titleLabel = new QLabel("Chọn máy còn trống", titleContainer);
    m_titleLabel->setObjectName("HeaderTitle");

    titleLayout->addWidget(m_subtitleLabel);
    titleLayout->addWidget(m_titleLabel);
    m_leftLayout->addWidget(titleContainer);

    m_badgeLabel = new QLabel(this);
    m_badgeLabel->setObjectName("HeaderBadge");
    m_badgeLabel->setAlignment(Qt::AlignCenter);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setObjectName("HeaderTime");
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->addWidget(m_leftContainer);
    mainLayout->addStretch();
    mainLayout->addWidget(m_badgeLabel);
    mainLayout->addWidget(m_timeLabel);

    connect(m_backButton, &QPushButton::clicked, this, &HeaderWidget::backClicked);

    setHeaderStyle(OverviewStyle);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        setTimeText(QTime::currentTime().toString("HH:mm"));
    });
    timer->start(1000);
    setTimeText(QTime::currentTime().toString("HH:mm"));
}

void HeaderWidget::setHeaderStyle(HeaderStyle style) {
    m_style = style;
    if (m_style == OverviewStyle) {
        m_backButton->hide();
        m_subtitleLabel->show();
        m_badgeLabel->show();
        m_titleLabel->setStyleSheet("color: #ffffff; font-size: 20px; font-weight: 800;");
        m_timeLabel->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: 800;");
    } else {
        m_backButton->show();
        m_subtitleLabel->hide();
        m_badgeLabel->hide();
        m_titleLabel->setStyleSheet("color: #191c1d; font-size: 18px; font-weight: 800;");
        m_timeLabel->setStyleSheet("color: #8e9994; font-size: 18px; font-weight: 800;");
    }
    update();
}

void HeaderWidget::setTitle(const QString &title) {
    m_titleLabel->setText(title);
}

void HeaderWidget::setSubtitle(const QString &subtitle) {
    m_subtitleLabel->setText(subtitle);
}

void HeaderWidget::setBadgeText(const QString &text) {
    m_badgeLabel->setText(text);
}

void HeaderWidget::setTimeText(const QString &time) {
    m_timeLabel->setText(time);
}

void HeaderWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_style == OverviewStyle) {
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0.0, QColor("#10b981"));
        gradient.setColorAt(1.0, QColor("#006c49"));
        painter.fillRect(rect(), gradient);
    } else {
        painter.fillRect(rect(), QColor("#ffffff"));
        painter.setPen(QColor("#e1e3e4"));
        painter.drawLine(0, height() - 1, width(), height() - 1);
    }
}

void HeaderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (m_style == OverviewStyle) {
        emit backClicked();
    }
    QWidget::mouseDoubleClickEvent(event);
}
