#pragma once

#include <QWidget>
#include <QLabel>
#include <QEvent>

class DropZone : public QWidget
{
    Q_OBJECT

public:
    explicit DropZone(QWidget *parent = nullptr);

signals:
    void fileSelected(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event)            override;
    void mousePressEvent(QMouseEvent *event)     override;
    void paintEvent(QPaintEvent *event)          override;
    void changeEvent(QEvent *e)                  override;

private:
    void loadFile(const QString &filePath);
    bool isExcelFile(const QString &filePath) const;
    void applyTheme();

    QLabel *m_iconLabel;
    QLabel *m_nameLabel;
    QLabel *m_hintLabel;
    QLabel *m_changeLabel;
    bool    m_hasFile  = false;
    bool    m_dragOver = false;
};
