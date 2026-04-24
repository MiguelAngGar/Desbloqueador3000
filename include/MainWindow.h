#pragma once

#include <QMainWindow>
#include <QLabel>
#include "DropZone.h"
#include "ResultPanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *e) override;

private slots:
    void onFileSelected(const QString &filePath);

private:
    void updateSectionColors();

    DropZone    *m_dropZone;
    ResultPanel *m_resultPanel;
    QLabel      *m_labelTop;
    QLabel      *m_labelBottom;
};
