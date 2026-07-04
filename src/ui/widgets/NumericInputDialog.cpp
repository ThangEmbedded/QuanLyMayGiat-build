#include "NumericInputDialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
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
    setFixedSize(430, 430);

    setStyleSheet(
        "QDialog {"
        "  background: #ffffff;"
        "  border: 3px solid #1266d6;"
        "  border-radius: 22px;"
        "}"
        "QLabel#Title {"
        "  font-size: 24px;"
        "  font-weight: 900;"
        "  color: #111827;"
        "}"
        "QLabel#Hint {"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "  color: #5b677a;"
        "}"
        "QLabel#Display {"
        "  font-size: 34px;"
        "  font-weight: 900;"
        "  letter-spacing: 2px;"
        "  color: #0f172a;"
        "  background: #f8fafc;"
        "  border: 2px solid #d6e2f2;"
        "  border-radius: 14px;"
        "  padding: 6px 12px;"
        "  min-height: 48px;"
        "}"
        "QPushButton {"
        "  font-size: 25px;"
        "  font-weight: 900;"
        "  color: #111827;"
        "  background: #ffffff;"
        "  border: 2px solid #d7e0ec;"
        "  border-radius: 13px;"
        "  min-height: 50px;"
        "}"
        "QPushButton:pressed {"
        "  background: #dbeafe;"
        "  border-color: #1266d6;"
        "}"
        "QPushButton#OkButton {"
        "  font-size: 22px;"
        "  background: #005de8;"
        "  color: white;"
        "  border-color: #004fc4;"
        "  min-height: 54px;"
        "}"
        "QPushButton#OkButton:pressed {"
        "  background: #004bb8;"
        "}"
        "QPushButton#OkButton:disabled {"
        "  background: #a9c8f7;"
        "  border-color: #a9c8f7;"
        "  color: #eff6ff;"
        "}"
        "QPushButton#CancelButton {"
        "  font-size: 20px;"
        "  background: #ef4444;"
        "  color: white;"
        "  border-color: #dc2626;"
        "}"
        "QPushButton#BackspaceButton {"
        "  font-size: 22px;"
        "  background: #334155;"
        "  color: white;"
        "  border-color: #1e293b;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(22, 18, 22, 18);
    mainLayout->setSpacing(9);

    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("Title");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel *hintLabel = new QLabel(passwordMode ? "Nhập mật khẩu bằng phím số" : "Nhập số phòng bằng phím số", this);
    hintLabel->setObjectName("Hint");
    hintLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(hintLabel);

    m_display = new QLabel(this);
    m_display->setObjectName("Display");
    m_display->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_display);

    QGridLayout *grid = new QGridLayout();
    grid->setContentsMargins(0, 4, 0, 0);
    grid->setHorizontalSpacing(9);
    grid->setVerticalSpacing(9);

    auto addDigit = [this, grid](const QString &text, int row, int col) {
        QPushButton *button = createButton(text);
        grid->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, this, [this, text]() { appendDigit(text); });
    };

    addDigit("1", 0, 0); addDigit("2", 0, 1); addDigit("3", 0, 2);
    addDigit("4", 1, 0); addDigit("5", 1, 1); addDigit("6", 1, 2);
    addDigit("7", 2, 0); addDigit("8", 2, 1); addDigit("9", 2, 2);

    QPushButton *cancel = createButton("HỦY", "CancelButton");
    connect(cancel, &QPushButton::clicked, this, &NumericInputDialog::reject);
    grid->addWidget(cancel, 3, 0);

    addDigit("0", 3, 1);

    QPushButton *backspaceButton = createButton(QString::fromUtf8("⌫"), "BackspaceButton");
    connect(backspaceButton, &QPushButton::clicked, this, &NumericInputDialog::backspace);
    grid->addWidget(backspaceButton, 3, 2);

    mainLayout->addLayout(grid);

    m_okButton = createButton(QString::fromUtf8("✓ XÁC NHẬN"), "OkButton");
    connect(m_okButton, &QPushButton::clicked, this, &NumericInputDialog::accept);
    mainLayout->addWidget(m_okButton);

    refreshDisplay();
}

QString NumericInputDialog::value() const {
    return m_value;
}

void NumericInputDialog::keyPressEvent(QKeyEvent *event) {
    if (!event) {
        return;
    }

    const int key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        appendDigit(QString::number(key - Qt::Key_0));
        return;
    }

    if (key == Qt::Key_Backspace) {
        backspace();
        return;
    }

    if (key == Qt::Key_Delete) {
        clearValue();
        return;
    }

    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        if (!m_value.isEmpty()) {
            accept();
        }
        return;
    }

    if (key == Qt::Key_Escape) {
        reject();
        return;
    }

    QDialog::keyPressEvent(event);
}

QPushButton *NumericInputDialog::createButton(const QString &text, const QString &objectName) {
    QPushButton *button = new QPushButton(text, this);
    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);
    if (!objectName.isEmpty()) {
        button->setObjectName(objectName);
    }
    return button;
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
        m_display->setText("—");
    } else if (m_passwordMode) {
        m_display->setText(QString(m_value.length(), QChar(0x2022)));
    } else {
        m_display->setText(m_value);
    }

    if (m_okButton) {
        m_okButton->setEnabled(!m_value.isEmpty());
    }
}
