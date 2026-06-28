#include "QRWidget.h"
#include <QPainter>
#include <QtMath>

QRWidget::QRWidget(QWidget *parent)
    : QWidget(parent) {
}

void QRWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    int size = qMin(width(), height());
    double offset_x = (width() - size) / 2.0;
    double offset_y = (height() - size) / 2.0;

    int N = 25; // QR Grid size
    double px = static_cast<double>(size) / N;

    auto isSeededOn = [](int i) -> bool {
        double x = qSin(i * 99.73) * 10000.0;
        return (x - qFloor(x)) > 0.52;
    };

    auto finderPattern = [N](int r, int c) -> int {
        auto getFinderColor = [](int R, int C, int r, int c) -> int {
            int dist = qMax(qAbs(r - R - 3), qAbs(c - C - 3));
            if (dist == 0 || dist == 2 || dist == 3) return 1; // black
            return 0; // white
        };

        if (r < 7 && c < 7) return getFinderColor(0, 0, r, c);
        if (r < 7 && c >= N - 7) return getFinderColor(0, N - 7, r, c);
        if (r >= N - 7 && c < 7) return getFinderColor(N - 7, 0, r, c);
        return -1;
    };

    auto inFinderSeparator = [N](int r, int c) -> bool {
        return (r < 8 && c < 8) || (r < 8 && c >= N - 8) || (r >= N - 8 && c < 8);
    };

    painter.fillRect(rect(), Qt::white);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#191c1d"));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int f = finderPattern(r, c);
            bool active = false;
            if (f != -1) {
                active = (f == 1);
            } else if (inFinderSeparator(r, c)) {
                active = false;
            } else {
                active = isSeededOn(r * N + c);
            }

            if (active) {
                QRectF pixelRect(offset_x + c * px, offset_y + r * px, px + 0.1, px + 0.1);
                painter.drawRect(pixelRect);
            }
        }
    }
}
