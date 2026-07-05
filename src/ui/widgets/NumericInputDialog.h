#ifndef NUMERICINPUTDIALOG_H
#define NUMERICINPUTDIALOG_H

#include <QDialog>
#include <QLabel>
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
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *m_display = nullptr;
    QString m_value;
    int m_maxLength = 8;
    bool m_passwordMode = false;

    void appendDigit(const QString &digit);
    void backspace();
    void clearValue();
    void confirm();
    void refreshDisplay();
};

#endif // NUMERICINPUTDIALOG_H
