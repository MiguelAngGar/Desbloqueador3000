#include "ResultPanel.h"
#include "Utils.h"

#include <QApplication>
#include <QDrag>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QUrl>
#include <QVBoxLayout>

ResultPanel::ResultPanel(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(160);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(8);
    layout->setContentsMargins(20, 20, 20, 20);

    m_spinner = new SpinnerWidget(44, this);

    m_statusLabel = new QLabel("El archivo sin protección aparecerá aquí", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->hide();

    m_nameLabel = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setWordWrap(true);
    m_nameLabel->hide();

    layout->addWidget(m_spinner,      0, Qt::AlignCenter);
    layout->addWidget(m_statusLabel,  0, Qt::AlignCenter);
    layout->addWidget(m_iconLabel,    0, Qt::AlignCenter);
    layout->addWidget(m_nameLabel,    0, Qt::AlignCenter);

    applyTheme();
}

void ResultPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange ||
        e->type() == QEvent::ApplicationPaletteChange)
        applyTheme();
}

void ResultPanel::applyTheme()
{
    const bool dark        = isDark(this);
    const QString nameCol  = dark ? "#DDD"     : "#222";
    const QString hintCol  = dark ? "#666"     : "#AAA";
    const QString accentCol= dark ? "#4CAF50"  : "#1D7044";

    m_statusLabel->setStyleSheet(
        QString("color: %1; font-size: 13px;").arg(hintCol));

    if (m_hasFile) {
        const QString fileName = QFileInfo(m_filePath).fileName();
        m_nameLabel->setText(
            QString("<span style='font-size:13px;font-weight:600;color:%1;'>%2</span>"
                    "<br><span style='font-size:11px;color:%3;'>"
                    "↕ Arrastra para guardar donde quieras</span>")
                .arg(nameCol, fileName, accentCol)
        );
    }
    update();
}

void ResultPanel::setFile(const QString &filePath)
{
    m_filePath   = filePath;
    m_hasFile    = true;

    m_spinner->stop();
    m_statusLabel->hide();

    QFileIconProvider provider;
    m_iconLabel->setPixmap(provider.icon(QFileInfo(filePath)).pixmap(72, 72));
    m_iconLabel->show();

    applyTheme(); // sets m_nameLabel text with correct colors
    m_nameLabel->show();
    update();
}

void ResultPanel::setProcessing(bool on)
{
    m_hasFile = false;
    m_filePath.clear();
    m_iconLabel->hide();
    m_nameLabel->hide();

    if (on) {
        m_statusLabel->setText("Procesando…");
        m_statusLabel->show();
        m_spinner->start();
    } else {
        m_spinner->stop();
        m_statusLabel->setText("El archivo sin protección aparecerá aquí");
        m_statusLabel->show();
    }
    update();
}

void ResultPanel::clear()
{
    setProcessing(false);
}

void ResultPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_hasFile)
        m_dragStartPos = event->pos();
}

void ResultPanel::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_hasFile) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) return;

    auto *drag = new QDrag(this);
    auto *mime = new QMimeData;
    mime->setUrls({QUrl::fromLocalFile(m_filePath)});
    drag->setMimeData(mime);

    QFileIconProvider provider;
    const QPixmap pix = provider.icon(QFileInfo(m_filePath)).pixmap(56, 56);
    drag->setPixmap(pix);
    drag->setHotSpot({pix.width() / 2, pix.height() / 2});

    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void ResultPanel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    const bool dark = isDark(this);
    QColor bg, border;

    if (m_hasFile) {
        bg     = dark ? QColor("#1C2A22") : QColor("#F4FDF7");
        border = dark ? QColor("#3A7A50") : QColor("#88C8A0");
    } else if (m_processing) {
        bg     = dark ? QColor("#1E1E1E") : QColor("#FAFAFA");
        border = dark ? QColor("#4CAF50") : QColor("#1D7044");
    } else {
        bg     = dark ? QColor("#1A1A1A") : QColor("#F5F5F5");
        border = dark ? QColor("#333")    : QColor("#E0E0E0");
    }

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), bg);
    p.setPen(QPen(border, 1.5, Qt::SolidLine));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rect().adjusted(3, 3, -3, -3), 10, 10);
}
