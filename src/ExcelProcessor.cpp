#include "ExcelProcessor.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStringList>
#include <QTemporaryDir>

namespace {

constexpr int kProcessTimeoutMs = 60'000;

// Removes sheetProtection / workbookProtection tags from one XML file.
// Returns true if the file was modified.
bool cleanXmlFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString content = QString::fromUtf8(file.readAll());
    file.close();

    static const QRegularExpression reSheet(
        R"(<sheetProtection\b[^>]*/?>)",
        QRegularExpression::DotMatchesEverythingOption
    );
    static const QRegularExpression reWorkbook(
        R"(<workbookProtection\b[^>]*/?>)",
        QRegularExpression::DotMatchesEverythingOption
    );

    const int sizeBefore = content.size();
    content.remove(reSheet);
    content.remove(reWorkbook);

    if (content.size() == sizeBefore)
        return false; // nothing changed

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(content.toUtf8());
    return true;
}

bool runProcess(const QString &program, const QStringList &args,
                const QString &workingDir = {})
{
    QProcess proc;
    if (!workingDir.isEmpty())
        proc.setWorkingDirectory(workingDir);
    proc.start(program, args);
    return proc.waitForFinished(kProcessTimeoutMs) && proc.exitCode() == 0;
}

#ifdef Q_OS_WIN
// PowerShell single-quoted string literal escape.
QString psQuote(const QString &s)
{
    QString r = s;
    r.replace('\'', QStringLiteral("''"));
    return QStringLiteral("'%1'").arg(r);
}
#endif

} // anonymous namespace

namespace ExcelProcessor {

Result process(const QString &inputPath)
{
    Result result;

    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        result.errorMessage = "No se pudo crear el directorio temporal.";
        return result;
    }

    // ── 1. Extraer .xlsx (es un ZIP) ──────────────────────────────────────
#ifdef Q_OS_WIN
    // Usamos .NET ZipFile directamente (no Expand-Archive) para evitar
    // problemas con corchetes en nombres (p.ej. [Content_Types].xml).
    const QString extractScript = QStringLiteral(
        "Add-Type -AssemblyName System.IO.Compression.FileSystem; "
        "[System.IO.Compression.ZipFile]::ExtractToDirectory(%1,%2)"
    ).arg(psQuote(inputPath), psQuote(tempDir.path()));
    const bool extracted = runProcess("powershell", {
        "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", extractScript
    });
#else
    const bool extracted = runProcess("unzip", {"-o", inputPath, "-d", tempDir.path()});
#endif

    if (!extracted) {
        result.errorMessage =
            "No se pudo extraer el archivo.\n"
            "¿Está cifrado con contraseña para abrir?";
        return result;
    }

    // ── 2. Limpiar protecciones ───────────────────────────────────────────
    const QString wbPath = tempDir.filePath("xl/workbook.xml");
    if (QFile::exists(wbPath) && cleanXmlFile(wbPath))
        ++result.protectionsRemoved;

    const QDir sheetsDir(tempDir.filePath("xl/worksheets"));
    if (sheetsDir.exists()) {
        for (const QString &name : sheetsDir.entryList({"*.xml"}, QDir::Files)) {
            if (cleanXmlFile(sheetsDir.filePath(name)))
                ++result.protectionsRemoved;
        }
    }

    // ── 3. Reempaquetar ───────────────────────────────────────────────────
    const QFileInfo fi(inputPath);
    QString baseName = fi.baseName();
    if (baseName.endsWith("_sin_proteccion"))
        baseName.chop(QStringLiteral("_sin_proteccion").size());

    const QString outPath = QDir::tempPath() + '/' + baseName + "_sin_proteccion.xlsx";
    QFile::remove(outPath);

#ifdef Q_OS_WIN
    // Compress-Archive de PowerShell 5.1 usa '\' como separador en el ZIP,
    // violando la spec de ZIP → Excel rechaza el archivo y lo marca como
    // dañado/protegido. Usamos .NET ZipFile::CreateFromDirectory que
    // produce separadores '/' correctos.
    const QString packScript = QStringLiteral(
        "Add-Type -AssemblyName System.IO.Compression.FileSystem; "
        "[System.IO.Compression.ZipFile]::CreateFromDirectory(%1,%2,"
        "[System.IO.Compression.CompressionLevel]::Optimal,$false)"
    ).arg(psQuote(tempDir.path()), psQuote(outPath));
    const bool repacked = runProcess("powershell", {
        "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", packScript
    });
#else
    const bool repacked = runProcess("ditto", {"-c", "-k", "--norsrc", tempDir.path(), outPath});
#endif

    if (!repacked) {
        result.errorMessage = "Error al reempaquetar el archivo.";
        return result;
    }

    result.success    = true;
    result.outputPath = outPath;
    return result;
}

} // namespace ExcelProcessor
