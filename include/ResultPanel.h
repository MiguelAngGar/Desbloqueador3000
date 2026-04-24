#pragma once

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPoint>
#include <QEvent>
#include "SpinnerWidget.h"

class ResultPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ResultPanel(QWidget *parent = nullptr);

    void setFile(const QString &filePath);
    void setProcessing(bool on);
    void clear();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)  override;
    void paintEvent(QPaintEvent *event)      override;
    void changeEvent(QEvent *e)              override;

private:
    void applyTheme();

    QLabel        *m_iconLabel;
    QLabel        *m_nameLabel;
    QLabel        *m_statusLabel;
    SpinnerWidget *m_spinner;

    QString  m_filePath;
    QPoint   m_dragStartPos;
    bool     m_hasFile    = false;
    bool     m_processing = false;
};
