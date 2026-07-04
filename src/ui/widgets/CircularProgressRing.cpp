#include "CircularProgressRing.h"
#include <QPainter>
#include <QPainterPath>
#include <QVector>

CircularProgressRing::CircularProgressRing(QWidget *parent)
    : QWidget(parent), m_progress(0.0), m_color(QColor("#006c49")), m_iconColor(QColor("#8e9994")), m_dashed(false), m_lineWidth(8), m_showText(false) {
}

void CircularProgressRing::setProgress(double progress) {
    if (m_progress != progress) {
        m_progress = qBound(0.0, progress, 1.0);
        update();
    }
}

void CircularProgressRing::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int size = qMin(width(), height());
    int pad = m_lineWidth / 2 + 2;
    QRectF rect(pad, pad, size - 2 * pad, size - 2 * pad);

    // 1. Draw background track (light grey)
    QPen trackPen(QColor("#eef0f1"), m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(trackPen);
    painter.drawEllipse(rect);

    // 2. Draw active progress ring
    if (m_dashed) {
        // Dashed ring for Open state
        QPen activePen(m_color, m_lineWidth, Qt::CustomDashLine, Qt::RoundCap, Qt::RoundJoin);
        QVector<qreal> dashes;
        dashes << 2.0 << 6.0; // small dots/dashes
        activePen.setDashPattern(dashes);
        painter.setPen(activePen);
        painter.drawEllipse(rect);
    } else if (m_progress > 0.0) {
        // Solid arc for progress
        QPen activePen(m_color, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(activePen);
        int startAngle = 90 * 16; // 12 o'clock
        int spanAngle = -static_cast<int>(m_progress * 360 * 16);
        painter.drawArc(rect, startAngle, spanAngle);
    }

    // 3. Center Area: Draw either text or washing machine icon
    if (m_showText) {
        // Draw percentage and remaining time text
        painter.setPen(QColor("#191c1d"));
        
        // Large top text (percentage, e.g. "40%")
        QFont topFont("Manrope", size * 0.16, QFont::Bold);
        painter.setFont(topFont);
        QRectF topRect(rect.left(), rect.top() + rect.height() * 0.28, rect.width(), rect.height() * 0.25);
        painter.drawText(topRect, Qt::AlignCenter, m_centerTopText);

        // Small bottom text (remaining time, e.g. "còn 6 phút")
        painter.setPen(QColor("#8e9994"));
        QFont bottomFont("Inter", size * 0.065, QFont::Bold);
        painter.setFont(bottomFont);
        QRectF bottomRect(rect.left(), rect.top() + rect.height() * 0.54, rect.width(), rect.height() * 0.15);
        painter.drawText(bottomRect, Qt::AlignCenter, m_centerBottomText);
    } else {
        // Draw Washing Machine Icon in the center
        int iconSize = size * 0.32;
        int cx = width() / 2;
        int cy = height() / 2;
        QRectF iconRect(cx - iconSize / 2, cy - iconSize / 2, iconSize, iconSize);

        QPen iconPen(m_iconColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(iconPen);
        painter.setBrush(Qt::NoBrush);

        // Outer frame of washing machine
        painter.drawRoundedRect(iconRect, iconSize * 0.12, iconSize * 0.12);

        // Top control panel separator line (at 24% of height)
        double lineY = iconRect.top() + iconRect.height() * 0.24;
        painter.drawLine(QPointF(iconRect.left(), lineY), QPointF(iconRect.right(), lineY));

        // Two small circles representing knobs/buttons
        double knobR = iconSize * 0.04;
        double knobY = iconRect.top() + iconRect.height() * 0.12;
        painter.setBrush(m_iconColor);
        painter.drawEllipse(QPointF(iconRect.left() + iconRect.width() * 0.25, knobY), knobR, knobR);
        painter.drawEllipse(QPointF(iconRect.left() + iconRect.width() * 0.45, knobY), knobR, knobR);

        // Large drum door in the center of lower part
        painter.setBrush(Qt::NoBrush);
        double drumY = iconRect.top() + iconRect.height() * 0.62;
        double drumR = iconSize * 0.25;
        painter.drawEllipse(QPointF(cx, drumY), drumR, drumR);

        // Wave inside the drum (representing water)
        if (!m_dashed) {
            QPainterPath wavePath;
            double waveLeft = cx - drumR * 0.8;
            double waveRight = cx + drumR * 0.8;
            double waveCenterY = drumY + drumR * 0.15;
            wavePath.moveTo(waveLeft, waveCenterY);
            // Draw double cubic bezier wave
            wavePath.cubicTo(
                cx - drumR * 0.4, waveCenterY - drumR * 0.2,
                cx + drumR * 0.4, waveCenterY + drumR * 0.2,
                waveRight, waveCenterY
            );
            painter.drawPath(wavePath);
        }
    }
}
