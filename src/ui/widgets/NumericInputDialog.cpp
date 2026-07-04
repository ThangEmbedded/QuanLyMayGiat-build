#include "NumericInputDialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

NumericInputDialog::NumericInputDialog(const QString &title,
                                       const QString &initialValue,
                                       int maxLength,
                                       bool passwordMode,
                                       QWidget *parent)
    : QDialog(parent),
      m_value(initialValue),
      m_maxLength(maxLength),
      m_passwordMode(passwordMode) {
    setWindowTitle(title);
    setModal(true);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setFixedSize(420, 440);
    setStyleSheet(
        "QDialog { background: #ffffff; border: 2px solid #7f9186; border-radius: 18px; }"
        "QLabel#Title { font-size: 22px; font-weight: 900; color: #191c1d; }"
        "QLabel#Display { font-size: 32px; font-weight: 900; padding: 12px; border: 1px solid #bbcabf; border-radius: 12px; background: #f8f9fa; }"
        "QPushButton { font-size: 24px; font-weight: 800; border-radius: 12px; background: #eef3ef; min-height: 54px; }"
        "QPushButton:pressed { background: #d7e4da; }"
        "QPushButton#OkButton { background: #006dff; color: white; }"
        "QPushButton#CancelButton { background: #f2b8b5; color: #410002; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 18, 18, 18);
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
    grid->setSpacing(10);

    auto addButton = [this, grid](const QString &text, int row, int col, int rowSpan = 1, int colSpan = 1) {
        QPushButton *button = new QPushButton(text, this);
        grid->addWidget(button, row, col, rowSpan, colSpan);
        if (text >= "0" && text <= "9") {
            connect(button, &QPushButton::clicked, this, [this, text]() { appendDigit(text); });
        } else if (text == "Xóa") {
            connect(button, &QPushButton::clicked, this, &NumericInputDialog::backspace);
        } else if (text == "C") {
            connect(button, &QPushButton::clicked, this, &NumericInputDialog::clearValue);
        }
        return button;
    };

    addButton("1", 0, 0); addButton("2", 0, 1); addButton("3", 0, 2);
    addButton("4", 1, 0); addButton("5", 1, 1); addButton("6", 1, 2);
    addButton("7", 2, 0); addButton("8", 2, 1); addButton("9", 2, 2);
    addButton("C", 3, 0); addButton("0", 3, 1); addButton("Xóa", 3, 2);

    QPushButton *cancel = new QPushButton("Hủy", this);
    cancel->setObjectName("CancelButton");
    grid->addWidget(cancel, 4, 0);
    connect(cancel, &QPushButton::clicked, this, &NumericInputDialog::reject);

    QPushButton *ok = new QPushButton("OK", this);
    ok->setObjectName("OkButton");
    grid->addWidget(ok, 4, 1, 1, 2);
    connect(ok, &QPushButton::clicked, this, &NumericInputDialog::accept);

    mainLayout->addLayout(grid);
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
        m_display->setText("—");
    } else if (m_passwordMode) {
        m_display->setText(QString(m_value.length(), QChar(0x2022)));
    } else {
        m_display->setText(m_value);
    }
}
