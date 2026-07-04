#ifndef CIRCULARPROGRESSRING_H
#define CIRCULARPROGRESSRING_H

#include <QWidget>
#include <QColor>

class CircularProgressRing : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double progress READ getProgress WRITE setProgress)

public:
    explicit CircularProgressRing(QWidget *parent = nullptr);

    double getProgress() const { return m_progress; }
    void setProgress(double progress);

    void setColor(const QColor &color) { m_color = color; update(); }
    void setDashed(bool dashed) { m_dashed = dashed; update(); }
    void setLineWidth(int width) { m_lineWidth = width; update(); }
    void setIconColor(const QColor &color) { m_iconColor = color; update(); }
    void setCenterText(bool show, const QString &top = "", const QString &bottom = "") {
        m_showText = show;
        m_centerTopText = top;
        m_centerBottomText = bottom;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_progress;
    QColor m_color;
    QColor m_iconColor;
    bool m_dashed;
    int m_lineWidth;
    bool m_showText;
    QString m_centerTopText;
    QString m_centerBottomText;
};

#endif // CIRCULARPROGRESSRING_H
