#pragma once

#include <QWidget>

/// Returns true when the system/app is running in dark mode.
inline bool isDark(const QWidget *w)
{
    return w->palette().window().color().lightness() < 128;
}
