#include "AdminAuthPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include "widgets/NumericInputDialog.h"

AdminAuthPage::AdminAuthPage(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("");
    mainLayout->addWidget(m_header);

    QWidget *content = new QWidget(this);
    content->setObjectName("AdminAuthContent");
    content->setStyleSheet("QWidget#AdminAuthContent { background-color: #f8f9fa; }");

    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(24, 20, 24, 20);
    contentLayout->addStretch();

    QWidget *card = new QWidget(content);
    card->setObjectName("AuthCard");
    card->setFixedWidth(420);
    card->setStyleSheet("QWidget#AuthCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(28, 28, 28, 28);
    cardLayout->setSpacing(14);

    QLabel *title = new QLabel("NHẬP MẬT KHẨU ADMIN", card);
    title->setObjectName("SectionTitle");
    cardLayout->addWidget(title);


    m_passwordInput = new QLineEdit(card);
    m_passwordInput->setObjectName("RoomInputDisplay");
    m_passwordInput->setPlaceholderText("Mật khẩu admin");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setMaxLength(16);
    m_passwordInput->setReadOnly(true);
    m_passwordInput->setInputMethodHints(Qt::ImhDigitsOnly | Qt::ImhNoPredictiveText);
    m_passwordInput->setValidator(new QIntValidator(0, 999999999, m_passwordInput));
    m_passwordInput->setFixedHeight(52);
    m_passwordInput->installEventFilter(this);
    cardLayout->addWidget(m_passwordInput);

    m_errorLabel = new QLabel(" ", card);
    m_errorLabel->setStyleSheet("font-size: 13px; color: #ba1a1a; font-weight: 700;");
    cardLayout->addWidget(m_errorLabel);

    cardLayout->addStretch();

    m_confirmButton = new QPushButton("Xác nhận", card);
    m_confirmButton->setObjectName("PrimaryButton");
    m_confirmButton->setFixedHeight(48);
    m_confirmButton->setEnabled(false);
    cardLayout->addWidget(m_confirmButton);

    m_cancelButton = new QPushButton("Quay lại Home", card);
    m_cancelButton->setFixedHeight(42);
    cardLayout->addWidget(m_cancelButton);

    contentLayout->addWidget(card);
    contentLayout->addStretch();
    mainLayout->addWidget(content);

    connect(m_header, &HeaderWidget::backClicked, this, &AdminAuthPage::backRequested);
    connect(m_cancelButton, &QPushButton::clicked, this, &AdminAuthPage::backRequested);
    connect(m_passwordInput, &QLineEdit::textChanged, this, &AdminAuthPage::updateConfirmState);
    connect(m_passwordInput, &QLineEdit::returnPressed, this, &AdminAuthPage::verifyPassword);
    connect(m_confirmButton, &QPushButton::clicked, this, &AdminAuthPage::verifyPassword);
}

void AdminAuthPage::reset() {
    m_passwordInput->clear();
    m_errorLabel->setText(" ");
    updateConfirmState();
    m_passwordInput->setFocus();
}

void AdminAuthPage::updateConfirmState() {
    m_errorLabel->setText(" ");
    m_confirmButton->setEnabled(!m_passwordInput->text().trimmed().isEmpty());
}

void AdminAuthPage::verifyPassword() {
    if (m_passwordInput->text() == m_adminPassword) {
        emit authSucceeded();
        return;
    }

    m_errorLabel->setText("Sai mật khẩu.");
    emit authFailed();
}


bool AdminAuthPage::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_passwordInput && event->type() == QEvent::MouseButtonPress) {
        NumericInputDialog dialog("Nhập mật khẩu", m_passwordInput->text(), 16, true, this);
        if (dialog.exec() == QDialog::Accepted) {
            m_passwordInput->setText(dialog.value());
        }
        return true;
    }
    return QWidget::eventFilter(watched, event);
}
