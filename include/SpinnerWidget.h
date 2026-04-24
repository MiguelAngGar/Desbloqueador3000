#pragma once

#include <QWidget>
#include <QTimer>

class SpinnerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpinnerWidget(int size = 44, QWidget *parent = nullptr);
    void start();
    void stop();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QTimer *m_timer;
    int     m_angle = 0;
};
