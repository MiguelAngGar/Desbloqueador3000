#pragma once

#include <QString>

namespace ExcelProcessor {

struct Result {
    bool    success            = false;
    QString outputPath;
    QString errorMessage;
    int     protectionsRemoved = 0;
};

Result process(const QString &inputPath);

} // namespace ExcelProcessor
