#include "DropZone.h"
#include "Utils.h"

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMimeData>
#include <QPainter>
#include <QPen>
#include <QUrl>
#include <QVBoxLayout>

DropZone::DropZone(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);
    setMinimumHeight(160);
    setCursor(Qt::PointingHandCursor);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(8);
    layout->setContentsMargins(20, 20, 20, 20);

    m_hintLabel   = new QLabel("Arrastra tu Excel aquí\no haz clic para seleccionar", this);
    m_hintLabel->setAlignment(Qt::AlignCenter);

    m_iconLabel   = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->hide();

    m_nameLabel   = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setWordWrap(true);
    m_nameLabel->hide();

    m_changeLabel = new QLabel("Haz clic o arrastra para cambiar", this);
    m_changeLabel->setAlignment(Qt::AlignCenter);
    m_changeLabel->hide();

    layout->addWidget(m_hintLabel);
    layout->addWidget(m_iconLabel);
    layout->addWidget(m_nameLabel);
    layout->addWidget(m_changeLabel);

    applyTheme();
}

void DropZone::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange ||
        e->type() == QEvent::ApplicationPaletteChange)
        applyTheme();
}

void DropZone::applyTheme()
{
    const bool dark = isDark(this);
    m_hintLabel->setStyleSheet(
        QString("color: %1; font-size: 14px;").arg(dark ? "#777" : "#888"));
    m_nameLabel->setStyleSheet(
        QString("color: %1; font-size: 13px; font-weight: 600;").arg(dark ? "#DDD" : "#222"));
    m_changeLabel->setStyleSheet(
        QString("color: %1; font-size: 11px;").arg(dark ? "#555" : "#AAA"));
    update();
}

void DropZone::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls()) { event->ignore(); return; }
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty() || !isExcelFile(urls.first().toLocalFile())) { event->ignore(); return; }

    m_dragOver = true;
    update();
    event->acceptProposedAction();
}

void DropZone::dragLeaveEvent(QDragLeaveEvent *event)
{
    m_dragOver = false;
    update();
    QWidget::dragLeaveEvent(event);
}

void DropZone::dropEvent(QDropEvent *event)
{
    m_dragOver = false;
    update();
    const QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        loadFile(urls.first().toLocalFile());
        event->acceptProposedAction();
    }
}

void DropZone::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    const QString path = QFileDialog::getOpenFileName(
        this, "Seleccionar archivo Excel",
        {}, "Archivos Excel (*.xlsx *.xls *.csv)"
    );
    if (!path.isEmpty())
        loadFile(path);
}

void DropZone::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    const bool dark = isDark(this);
    QColor bg, border;

    if (m_dragOver) {
        bg     = dark ? QColor("#1A3328") : QColor("#F0FFF4");
        border = dark ? QColor("#4CAF50") : QColor("#1D7044");
    } else if (m_hasFile) {
        bg     = dark ? QColor("#1C2A22") : QColor("#F5FDF7");
        border = dark ? QColor("#3A7A50") : QColor("#88C8A0");
    } else {
        bg     = dark ? QColor("#1E1E1E") : QColor("#FAFAFA");
        border = dark ? QColor("#444")    : QColor("#CCC");
    }

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), bg);
    p.setPen(QPen(border, 1.5, m_hasFile ? Qt::SolidLine : Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rect().adjusted(3, 3, -3, -3), 10, 10);
}

void DropZone::loadFile(const QString &filePath)
{
    QFileIconProvider provider;
    m_iconLabel->setPixmap(provider.icon(QFileInfo(filePath)).pixmap(72, 72));
    m_nameLabel->setText(QFileInfo(filePath).fileName());

    m_hintLabel->hide();
    m_iconLabel->show();
    m_nameLabel->show();
    m_changeLabel->show();

    m_hasFile = true;
    update();
    emit fileSelected(filePath);
}

bool DropZone::isExcelFile(const QString &filePath) const
{
    const QString ext = QFileInfo(filePath).suffix().toLower();
    return ext == "xlsx" || ext == "xls" || ext == "csv";
}
