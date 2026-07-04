#ifndef ADMINAUTHPAGE_H
#define ADMINAUTHPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "widgets/HeaderWidget.h"

class AdminAuthPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminAuthPage(QWidget *parent = nullptr);
    void reset();

signals:
    void backRequested();
    void authSucceeded();
    void authFailed();

private:
    HeaderWidget *m_header = nullptr;
    QLineEdit *m_passwordInput = nullptr;
    QLabel *m_errorLabel = nullptr;
    QPushButton *m_confirmButton = nullptr;
    QPushButton *m_cancelButton = nullptr;

    const QString m_adminPassword = "1234";

    void updateConfirmState();
    void verifyPassword();
};

#endif // ADMINAUTHPAGE_H
