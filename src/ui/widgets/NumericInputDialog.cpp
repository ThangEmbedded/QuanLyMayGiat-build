#include "NumericInputDialog.h"

#include <QApplication>
#include <QEvent>
#include <QGridLayout>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>

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

    QSize overlaySize(800, 480);
    if (parent && parent->window()) {
        overlaySize = parent->window()->size();
    } else if (QGuiApplication::primaryScreen()) {
        overlaySize = QGuiApplication::primaryScreen()->availableGeometry().size();
    }
    setFixedSize(overlaySize);

    setStyleSheet(
        "QDialog {"
        "  background: #f4f8fc;"
        "}"
        "QWidget#KeypadPanel {"
        "  background: #ffffff;"
        "  border: 2px solid #d6e2f0;"
        "  border-radius: 22px;"
        "}"
        "QLabel#KeypadTitle {"
        "  color: #102033;"
        "  font-size: 24px;"
        "  font-weight: 900;"
        "}"
        "QLabel#KeypadHint {"
        "  color: #66758a;"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "}"
        "QLabel#KeypadDisplay {"
        "  color: #102033;"
        "  background: #f7fbff;"
        "  border: 2px solid #c7d9ee;"
        "  border-radius: 14px;"
        "  font-size: 30px;"
        "  font-weight: 900;"
        "  min-height: 50px;"
        "  padding: 4px 14px;"
        "}"
        "QPushButton#DigitButton {"
        "  background: #ffffff;"
        "  border: 2px solid #d9e3ef;"
        "  border-radius: 14px;"
        "  color: #111827;"
        "  font-size: 27px;"
        "  font-weight: 900;"
        "  min-height: 54px;"
        "}"
        "QPushButton#DigitButton:pressed {"
        "  background: #e5f1ff;"
        "  border-color: #006dff;"
        "}"
        "QPushButton#CancelButton {"
        "  background: #fff1f1;"
        "  border: 2px solid #efb4b4;"
        "  border-radius: 14px;"
        "  color: #b3261e;"
        "  font-size: 20px;"
        "  font-weight: 900;"
        "  min-height: 54px;"
        "}"
        "QPushButton#CancelButton:pressed { background: #ffdede; }"
        "QPushButton#BackspaceButton {"
        "  background: #eef3f8;"
        "  border: 2px solid #bdc9d8;"
        "  border-radius: 14px;"
        "  color: #253140;"
        "  font-size: 24px;"
        "  font-weight: 900;"
        "  min-height: 54px;"
        "}"
        "QPushButton#BackspaceButton:pressed { background: #dce7f2; }"
        "QPushButton#OkButton {"
        "  background: #006dff;"
        "  border: 2px solid #0057cc;"
        "  border-radius: 14px;"
        "  color: #ffffff;"
        "  font-size: 22px;"
        "  font-weight: 900;"
        "  min-height: 56px;"
        "}"
        "QPushButton#OkButton:pressed { background: #0057cc; }"
    );

    QVBoxLayout *overlayLayout = new QVBoxLayout(this);
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->setSpacing(0);
    overlayLayout->addStretch();

    QWidget *panel = new QWidget(this);
    panel->setObjectName("KeypadPanel");
    panel->setFixedSize(430, 430);

    QVBoxLayout *mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(22, 18, 22, 20);
    mainLayout->setSpacing(8);

    QLabel *titleLabel = new QLabel(title, panel);
    titleLabel->setObjectName("KeypadTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel *hint = new QLabel(passwordMode ? "Nhập mật khẩu bằng số" : "Nhập số phòng cần sử dụng", panel);
    hint->setObjectName("KeypadHint");
    hint->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(hint);

    m_display = new QLabel(panel);
    m_display->setObjectName("KeypadDisplay");
    m_display->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_display);

    QGridLayout *grid = new QGridLayout();
    grid->setContentsMargins(0, 8, 0, 0);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);

    auto addButton = [this, grid, panel](const QString &text,
                                         int row,
                                         int col,
                                         const QString &objectName) {
        QPushButton *button = new QPushButton(text, panel);
        button->setObjectName(objectName);
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
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

    QPushButton *ok = new QPushButton("✓ XÁC NHẬN", panel);
    ok->setObjectName("OkButton");
    ok->setFocusPolicy(Qt::NoFocus);
    ok->setCursor(Qt::PointingHandCursor);
    connect(ok, &QPushButton::clicked, this, &NumericInputDialog::confirm);
    mainLayout->addWidget(ok);

    overlayLayout->addWidget(panel, 0, Qt::AlignCenter);
    overlayLayout->addStretch();

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
        m_display->setText(m_passwordMode ? "••••" : "VD: 302");
        return;
    }

    m_display->setText(m_passwordMode ? QString(m_value.length(), QChar(0x2022)) : m_value);
}
