#include "SpinnerWidget.h"
#include "Utils.h"

#include <QPainter>
#include <QPen>

namespace {
    constexpr int kFrameIntervalMs  = 28;
    constexpr int kRotationStepDeg  = 8;
    constexpr int kArcSpanDeg       = 270;
}

SpinnerWidget::SpinnerWidget(int size, QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(size, size);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_timer = new QTimer(this);
    m_timer->setInterval(kFrameIntervalMs);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        m_angle = (m_angle + kRotationStepDeg) % 360;
        update();
    });
    hide();
}

void SpinnerWidget::start()
{
    m_angle = 0;
    show();
    m_timer->start();
}

void SpinnerWidget::stop()
{
    m_timer->stop();
    hide();
}

void SpinnerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QColor track = isDark(this) ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 18);
    const QColor arc   = isDark(this) ? QColor("#4CAF50")          : QColor("#1D7044");
    const QRect  r     = rect().adjusted(4, 4, -4, -4);

    p.setPen(QPen(track, 3.5, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(r);

    p.setPen(QPen(arc, 3.5, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(r, m_angle * 16, kArcSpanDeg * 16);
}
