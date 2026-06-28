#ifndef ADMINLOGINPAGE_H
#define ADMINLOGINPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "widgets/HeaderWidget.h"

class AdminLoginPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminLoginPage(QWidget *parent = nullptr);

signals:
    void backRequested();
    void loginSuccess();

private:
    HeaderWidget *m_header;
    QLineEdit *m_usernameInput;
    QLineEdit *m_passwordInput;
    QPushButton *m_loginButton;
    QLabel *m_errorLabel;
};

#endif // ADMINLOGINPAGE_H
