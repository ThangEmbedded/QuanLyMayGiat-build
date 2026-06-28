#include "AdminLoginPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AdminLoginPage::AdminLoginPage(QWidget *parent)
    : QWidget(parent) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header
    m_header = new HeaderWidget(this);
    m_header->setHeaderStyle(HeaderWidget::SubPageStyle);
    m_header->setTitle("Quản trị hệ thống");
    mainLayout->addWidget(m_header);

    // 2. Center Content
    QWidget *content = new QWidget(this);
    content->setStyleSheet("background-color: #f4f7f6;"); // Soft green/grey auth background
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setAlignment(Qt::AlignCenter);

    // Login Card
    QWidget *loginCard = new QWidget(content);
    loginCard->setObjectName("LoginCard");
    loginCard->setStyleSheet("QWidget#LoginCard { background-color: #ffffff; border: 1px solid #bbcabf; border-radius: 18px; }");
    loginCard->setFixedWidth(340);
    QVBoxLayout *cardLayout = new QVBoxLayout(loginCard);
    cardLayout->setContentsMargins(28, 28, 28, 28);
    cardLayout->setSpacing(14);

    QLabel *titleLabel = new QLabel("ĐĂNG NHẬP ADMIN", loginCard);
    titleLabel->setObjectName("SectionTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(titleLabel);

    m_usernameInput = new QLineEdit(loginCard);
    m_usernameInput->setPlaceholderText("Tên đăng nhập");
    m_usernameInput->setFixedHeight(40);
    m_usernameInput->setStyleSheet("padding-left: 8px; border: 1px solid #bbcabf; border-radius: 8px;");
    cardLayout->addWidget(m_usernameInput);

    m_passwordInput = new QLineEdit(loginCard);
    m_passwordInput->setPlaceholderText("Mật khẩu");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setFixedHeight(40);
    m_passwordInput->setStyleSheet("padding-left: 8px; border: 1px solid #bbcabf; border-radius: 8px;");
    cardLayout->addWidget(m_passwordInput);

    m_errorLabel = new QLabel(loginCard);
    m_errorLabel->setStyleSheet("color: #ba1a1a; font-size: 11px; font-weight: bold;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->hide();
    cardLayout->addWidget(m_errorLabel);

    m_loginButton = new QPushButton("Đăng nhập", loginCard);
    m_loginButton->setObjectName("PrimaryButton");
    m_loginButton->setFixedHeight(44);
    m_loginButton->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(m_loginButton);

    contentLayout->addWidget(loginCard);
    mainLayout->addWidget(content);

    // Connections
    connect(m_header, &HeaderWidget::backClicked, this, &AdminLoginPage::backRequested);
    connect(m_loginButton, &QPushButton::clicked, this, [this]() {
        QString user = m_usernameInput->text();
        QString pass = m_passwordInput->text();

        // Check credentials (simple admin/admin check, or allow empty username/pass for easy kiosk dev testing)
        if ((user == "admin" && pass == "admin") || (user.isEmpty() && pass.isEmpty())) {
            m_errorLabel->hide();
            m_usernameInput->clear();
            m_passwordInput->clear();
            emit loginSuccess();
        } else {
            m_errorLabel->setText("Tài khoản hoặc mật khẩu không đúng!");
            m_errorLabel->show();
        }
    });
}
