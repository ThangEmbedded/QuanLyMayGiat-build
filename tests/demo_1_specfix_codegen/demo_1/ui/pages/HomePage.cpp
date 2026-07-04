#include "HomePage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::OverviewStyle);
    m_header->setTitle("Home control máy giặt");
    m_header->setSubtitle("Demo 1 · Mock UI");
    m_header->setBadgeText("0 máy trống");
    mainLayout->addWidget(m_header);

    connect(m_header, &HeaderWidget::backClicked, this, &HomePage::adminRequested);

    QWidget *contentWidget = new QWidget(this);
    contentWidget->setObjectName("HomeContent");
    contentWidget->setStyleSheet("QWidget#HomeContent { background-color: #f8f9fa; }");

    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->setSpacing(14);

    QHBoxLayout *topRow = new QHBoxLayout();
    QLabel *hint = new QLabel("Chọn máy đang trống để nhập phòng và xác nhận khởi động.", contentWidget);
    hint->setStyleSheet("font-size: 14px; color: #5f6f66; font-weight: 600;");
    m_adminButton = new QPushButton("Admin setup", contentWidget);
    m_adminButton->setFixedHeight(36);
    m_adminButton->setCursor(Qt::PointingHandCursor);
    topRow->addWidget(hint);
    topRow->addStretch();
    topRow->addWidget(m_adminButton);
    contentLayout->addLayout(topRow);

    connect(m_adminButton, &QPushButton::clicked, this, &HomePage::adminRequested);

    QHBoxLayout *gridLayout = new QHBoxLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(16);

    for (int id = 1; id <= 4; id++) {
        MachineCardWidget *card = new MachineCardWidget(id, contentWidget);
        gridLayout->addWidget(card);
        m_cards.append(card);

        connect(card, &MachineCardWidget::clicked, this, &HomePage::machineSelected);
    }

    contentLayout->addLayout(gridLayout, 1);
    mainLayout->addWidget(contentWidget);
}

void HomePage::updateMachines(const QList<MachineData> &machines) {
    int openCount = 0;
    for (const MachineData &m : machines) {
        if (m.id > 0 && m.id <= m_cards.size()) {
            m_cards[m.id - 1]->setMachineData(m.name, machineStateToString(m.state), m.claimer, m.remaining, m.total);
            if (m.state == MachineState::Open) {
                openCount++;
            }
        }
    }
    m_header->setBadgeText(QString("%1 máy trống").arg(openCount));
}
