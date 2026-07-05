#include "NumericInputDialog.h"

#include <QEvent>
#include <QGridLayout>
#include <QKeyEvent>
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
    installEventFilter(this);

    // Compact kiosk keypad.  It intentionally does NOT cover the whole screen:
    // the user still sees the current page context, and the touch target area
    // stays predictable on linuxfb/eglfs.
    setFixedSize(360, 420);
    if (parent && parent->window()) {
        const QRect parentRect = parent->window()->geometry();
        move(parentRect.center() - rect().center());
    }

    setStyleSheet(
        "QDialog {"
        "  background: #ffffff;"
        "  border: 2px solid #b8c7d9;"
        "  border-radius: 18px;"
        "}"
        "QLabel#KeypadTitle {"
        "  color: #172033;"
        "  font-size: 22px;"
        "  font-weight: 900;"
        "}"
        "QLabel#KeypadDisplay {"
        "  color: #0f172a;"
        "  background: #f4f8ff;"
        "  border: 2px solid #c9d8ea;"
        "  border-radius: 12px;"
        "  font-size: 30px;"
        "  font-weight: 900;"
        "  min-height: 54px;"
        "  padding: 2px 12px;"
        "}"
        "QPushButton {"
        "  border-radius: 12px;"
        "  font-weight: 900;"
        "}"
        "QPushButton#DigitButton {"
        "  background: #f8fafc;"
        "  border: 2px solid #d4deea;"
        "  color: #111827;"
        "  font-size: 26px;"
        "  min-height: 52px;"
        "}"
        "QPushButton#DigitButton:pressed {"
        "  background: #dbeafe;"
        "  border-color: #2563eb;"
        "}"
        "QPushButton#CancelButton {"
        "  background: #fff5f5;"
        "  border: 2px solid #f0b7b7;"
        "  color: #b42318;"
        "  font-size: 18px;"
        "  min-height: 52px;"
        "}"
        "QPushButton#CancelButton:pressed { background: #fee2e2; }"
        "QPushButton#BackspaceButton {"
        "  background: #eef2f7;"
        "  border: 2px solid #c7d0dd;"
        "  color: #263142;"
        "  font-size: 24px;"
        "  min-height: 52px;"
        "}"
        "QPushButton#BackspaceButton:pressed { background: #dce6f2; }"
        "QPushButton#OkButton {"
        "  background: #2563eb;"
        "  border: 2px solid #1d4ed8;"
        "  color: #ffffff;"
        "  font-size: 21px;"
        "  min-height: 54px;"
        "}"
        "QPushButton#OkButton:pressed { background: #1d4ed8; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 16, 18, 18);
    mainLayout->setSpacing(10);

    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("KeypadTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    m_display = new QLabel(this);
    m_display->setObjectName("KeypadDisplay");
    m_display->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_display);

    QGridLayout *grid = new QGridLayout();
    grid->setContentsMargins(0, 4, 0, 0);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);

    auto addButton = [this, grid](const QString &text,
                                  int row,
                                  int col,
                                  const QString &objectName) {
        QPushButton *button = new QPushButton(text, this);
        button->setObjectName(objectName);
        button->setFocusPolicy(Qt::NoFocus);
        grid->addWidget(button, row, col);
        return button;
    };

    for (int i = 1; i <= 9; ++i) {
        QPushButton *digit = addButton(QString::number(i), (i - 1) / 3, (i - 1) % 3, "DigitButton");
        connect(digit, &QPushButton::clicked, this, [this, i]() { appendDigit(QString::number(i)); });
    }

    QPushButton *cancel = addButton("HỦY", 3, 0, "CancelButton");
    QPushButton *zero = addButton("0", 3, 1, "DigitButton");
    QPushButton *back = addButton("⌫", 3, 2, "BackspaceButton");

    connect(cancel, &QPushButton::clicked, this, &NumericInputDialog::reject);
    connect(zero, &QPushButton::clicked, this, [this]() { appendDigit("0"); });
    connect(back, &QPushButton::clicked, this, &NumericInputDialog::backspace);

    mainLayout->addLayout(grid);

    QPushButton *ok = new QPushButton("XÁC NHẬN", this);
    ok->setObjectName("OkButton");
    ok->setFocusPolicy(Qt::NoFocus);
    connect(ok, &QPushButton::clicked, this, &NumericInputDialog::confirm);
    mainLayout->addWidget(ok);

    refreshDisplay();
}

QString NumericInputDialog::value() const {
    return m_value;
}

bool NumericInputDialog::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        const int key = keyEvent->key();
        if (key >= Qt::Key_0 && key <= Qt::Key_9) {
            appendDigit(QString::number(key - Qt::Key_0));
            return true;
        }
        if (key == Qt::Key_Backspace) {
            backspace();
            return true;
        }
        if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            confirm();
            return true;
        }
        if (key == Qt::Key_Escape) {
            reject();
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
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

void NumericInputDialog::confirm() {
    accept();
}

void NumericInputDialog::refreshDisplay() {
    if (!m_display) {
        return;
    }

    if (m_value.isEmpty()) {
        m_display->setText(m_passwordMode ? "••••" : "Nhập số");
        return;
    }

    m_display->setText(m_passwordMode ? QString(m_value.length(), QChar(0x2022)) : m_value);
}
