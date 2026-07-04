#ifndef NUMERICINPUTDIALOG_H
#define NUMERICINPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QString>

class NumericInputDialog : public QDialog {
    Q_OBJECT
public:
    explicit NumericInputDialog(const QString &title,
                                const QString &initialValue = QString(),
                                int maxLength = 8,
                                bool passwordMode = false,
                                QWidget *parent = nullptr);

    QString value() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *m_display = nullptr;
    QPushButton *m_okButton = nullptr;
    QString m_value;
    int m_maxLength = 8;
    bool m_passwordMode = false;

    QPushButton *createButton(const QString &text, const QString &objectName = QString());
    void appendDigit(const QString &digit);
    void backspace();
    void clearValue();
    void refreshDisplay();
};

#endif // NUMERICINPUTDIALOG_H
