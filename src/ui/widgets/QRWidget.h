#ifndef QRWIDGET_H
#define QRWIDGET_H

#include <QWidget>

class QRWidget : public QWidget {
    Q_OBJECT
public:
    explicit QRWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // QRWIDGET_H
