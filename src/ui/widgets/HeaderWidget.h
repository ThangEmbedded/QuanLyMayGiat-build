#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class HeaderWidget : public QWidget {
    Q_OBJECT
public:
    enum HeaderStyle {
        OverviewStyle,
        SubPageStyle
    };

    explicit HeaderWidget(QWidget *parent = nullptr);

    void setHeaderStyle(HeaderStyle style);
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void setBadgeText(const QString &text);
    void setTimeText(const QString &time);

signals:
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    HeaderStyle m_style;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_badgeLabel;
    QLabel *m_timeLabel;
    QPushButton *m_backButton;
    QWidget *m_leftContainer;
    QHBoxLayout *m_leftLayout;
};

#endif // HEADERWIDGET_H
