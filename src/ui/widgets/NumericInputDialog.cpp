#include "NumericInputDialog.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

NumericInputDialog::NumericInputDialog(const QString &title,
                                       const QString &initialValue,
                                       int maxLength,
                                       bool passwordMode,
                                       QWidget *parent)
    : QDialog(parent),
      m_value(initialValue),
      m_maxLength(maxLength),
      m_passwordMode(passwordMode) {
    setModal(true);
    setWindowTitle(title);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedSize(560, 390);

    setStyleSheet(
        "QDialog {"
        "  background: #f7fbff;"
        "  border: 3px solid #1d6fd8;"
        "  border-radius: 24px;"
        "}"
        "QLabel#Title {"
        "  font-size: 26px;"
        "  font-weight: 900;"
        "  color: #102033;"
        "  padding: 2px 4px;"
        "}"
        "QLabel#Display {"
        "  font-size: 34px;"
        "  font-weight: 900;"
        "  color: #0b1f35;"
        "  background: white;"
        "  border: 2px solid #b9d6f7;"
        "  border-radius: 16px;"
        "  padding: 10px 14px;"
        "  min-height: 48px;"
        "}"
        "QPushButton {"
        "  font-size: 26px;"
        "  font-weight: 900;"
        "  color: #102033;"
        "  background: #ffffff;"
        "  border: 2px solid #c7d9ee;"
        "  border-radius: 16px;"
        "  min-height: 58px;"
        "}"
        "QPushButton:pressed {"
        "  background: #d8ebff;"
        "  border-color: #1d6fd8;"
        "}"
        "QPushButton#OkButton {"
        "  background: #006dff;"
        "  color: white;"
        "  border-color: #0057cc;"
        "}"
        "QPushButton#OkButton:pressed {"
        "  background: #0057cc;"
        "}"
        "QPushButton#CancelButton {"
        "  background: #fff0f0;"
        "  color: #b3261e;"
        "  border-color: #f0b8b8;"
        "}"
        "QPushButton#ClearButton {"
        "  background: #fff8e5;"
        "  color: #7a4d00;"
        "  border-color: #f2cf77;"
        "}"
        "QPushButton#BackspaceButton {"
        "  background: #eef6ff;"
        "  color: #0b57a3;"
        "  border-color: #95c4f4;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(22, 18, 22, 20);
    mainLayout->setSpacing(12);

    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("Title");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    m_display = new QLabel(this);
    m_display->setObjectName("Display");
    m_display->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_display);

    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    auto addButton = [this, grid](const QString &text,
                                  int row,
                                  int col,
                                  int rowSpan = 1,
                                  int colSpan = 1,
                                  const char *objectName = nullptr) {
        QPushButton *button = new QPushButton(text, this);
        button->setCursor(Qt::PointingHandCursor);
        if (objectName) {
            button->setObjectName(objectName);
        }
        grid->addWidget(button, row, col, rowSpan, colSpan);

        if (text.size() == 1 && text[0].isDigit()) {
            connect(button, &QPushButton::clicked, this, [this, text]() { appendDigit(text); });
        } else if (text == "⌫") {
            connect(button, &QPushButton::clicked, this, &NumericInputDialog::backspace);
        } else if (text == "C") {
            connect(button, &QPushButton::clicked, this, &NumericInputDialog::clearValue);
        }
        return button;
    };

    addButton("1", 0, 0); addButton("2", 0, 1); addButton("3", 0, 2);
    addButton("4", 1, 0); addButton("5", 1, 1); addButton("6", 1, 2);
    addButton("7", 2, 0); addButton("8", 2, 1); addButton("9", 2, 2);
    addButton("C", 3, 0, 1, 1, "ClearButton");
    addButton("0", 3, 1);
    addButton("⌫", 3, 2, 1, 1, "BackspaceButton");

    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(12);

    QPushButton *cancel = new QPushButton("Hủy", this);
    cancel->setObjectName("CancelButton");
    cancel->setCursor(Qt::PointingHandCursor);
    cancel->setMinimumHeight(62);
    connect(cancel, &QPushButton::clicked, this, &NumericInputDialog::reject);

    QPushButton *ok = new QPushButton("OK", this);
    ok->setObjectName("OkButton");
    ok->setCursor(Qt::PointingHandCursor);
    ok->setMinimumHeight(62);
    connect(ok, &QPushButton::clicked, this, &NumericInputDialog::accept);

    actionLayout->addWidget(cancel, 1);
    actionLayout->addWidget(ok, 2);

    mainLayout->addLayout(grid);
    mainLayout->addLayout(actionLayout);
    refreshDisplay();
}

QString NumericInputDialog::value() const {
    return m_value;
}

void NumericInputDialog::appendDigit(const QString &digit) {
    if (m_value.length() >= m_maxLength) {
        return;
    }
    m_value.append(digit);
    refreshDisplay();
}

void NumericInputDialog::backspace() {
    if (!m_value.isEmpty()) {
        m_value.chop(1);
    }
    refreshDisplay();
}

void NumericInputDialog::clearValue() {
    m_value.clear();
    refreshDisplay();
}

void NumericInputDialog::refreshDisplay() {
    if (!m_display) {
        return;
    }

    if (m_value.isEmpty()) {
        m_display->setText("Nhập số");
    } else {
        if (m_passwordMode) {
            m_display->setText(QString(m_value.length(), QChar(0x2022)));
        } else {
            m_display->setText(m_value);
        }
    }
}
