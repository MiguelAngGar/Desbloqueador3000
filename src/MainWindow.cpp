#include "MainWindow.h"
#include "ExcelProcessor.h"

#include <QEvent>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

namespace {
    constexpr int kProcessDelayMs = 50; // one paint cycle before spawning the thread
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Desbloqueador3000");
    setMinimumSize(500, 540);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(6);

    m_labelTop = new QLabel("Archivo original", central);
    m_labelTop->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_labelTop);

    m_dropZone = new DropZone(central);
    layout->addWidget(m_dropZone, 1);

    layout->addSpacing(6);

    m_labelBottom = new QLabel("Resultado", central);
    m_labelBottom->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_labelBottom);

    m_resultPanel = new ResultPanel(central);
    layout->addWidget(m_resultPanel, 1);

    updateSectionColors();

    connect(m_dropZone, &DropZone::fileSelected,
            this,       &MainWindow::onFileSelected);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::PaletteChange ||
        e->type() == QEvent::ApplicationPaletteChange)
        updateSectionColors();
}

void MainWindow::updateSectionColors()
{
    const QColor c = palette().color(QPalette::PlaceholderText);
    const QString style = QString(
        "font-size: 11px; font-weight: 600; color: %1; letter-spacing: 0.6px;"
    ).arg(c.name(QColor::HexRgb));

    m_labelTop->setStyleSheet(style);
    m_labelBottom->setStyleSheet(style);
}

void MainWindow::onFileSelected(const QString &filePath)
{
    m_dropZone->setEnabled(false);
    m_resultPanel->setProcessing(true);

    QTimer::singleShot(kProcessDelayMs, this, [this, filePath]() {
        auto *watcher = new QFutureWatcher<ExcelProcessor::Result>(this);

        connect(watcher, &QFutureWatcher<ExcelProcessor::Result>::finished,
                this, [this, watcher]() {
            const ExcelProcessor::Result r = watcher->result();
            watcher->deleteLater();
            m_dropZone->setEnabled(true);

            if (r.success)
                m_resultPanel->setFile(r.outputPath);
            else {
                m_resultPanel->clear();
                QMessageBox::warning(this, "Error al procesar", r.errorMessage);
            }
        });

        watcher->setFuture(
            QtConcurrent::run([filePath]() {
                return ExcelProcessor::process(filePath);
            })
        );
    });
}
