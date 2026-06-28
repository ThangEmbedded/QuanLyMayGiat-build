#include "HomePage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::OverviewStyle);
    m_header->setTitle("Chọn máy còn trống");
    m_header->setSubtitle("Trạm giặt · Tầng 3");
    m_header->setBadgeText("0 máy trống");
    mainLayout->addWidget(m_header);

    connect(m_header, &HeaderWidget::backClicked, this, &HomePage::adminRequested);

    // 2. Content Area (Grid of machine cards)
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setObjectName("HomeContent");
    contentWidget->setStyleSheet("QWidget#HomeContent { background-color: #f8f9fa; }");

    QHBoxLayout *gridLayout = new QHBoxLayout(contentWidget);
    gridLayout->setContentsMargins(24, 20, 24, 20);
    gridLayout->setSpacing(16);

    for (int id = 1; id <= 4; id++) {
        MachineCardWidget *card = new MachineCardWidget(id, this);
        gridLayout->addWidget(card);
        m_cards.append(card);

        connect(card, &MachineCardWidget::clicked, this, &HomePage::machineSelected);
    }

    mainLayout->addWidget(contentWidget);
}

void HomePage::updateMachines(const QList<MachineData> &machines) {
    int openCount = 0;
    for (const MachineData &m : machines) {
        if (m.id > 0 && m.id <= m_cards.size()) {
            m_cards[m.id - 1]->setMachineData(m.name, m.state, m.claimer, m.remaining, m.total);
            if (m.state == "open") {
                openCount++;
            }
        }
    }
    m_header->setBadgeText(QString("%1 máy trống").arg(openCount));
}
