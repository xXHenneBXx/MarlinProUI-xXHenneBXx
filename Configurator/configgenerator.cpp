#include "configgenerator.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegularExpression>
#include <QTextStream>
#include <QDebug>

namespace {
QString g_logLines;
bool g_error = false;
QString g_currentLines;

QString findProjectRoot()
{
    const QStringList candidates = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(".."),
        QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../.."),
        QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../../.."),
        QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("src")
    };

    for (const QString &candidate : candidates) {
        const QFileInfo printers(candidate + "/_printers");
        const QFileInfo originals(candidate + "/Original Configs");
        if (printers.exists() && originals.exists()) {
            return QDir(candidate).absolutePath();
        }
    }
    return QDir::currentPath();
}

QString resolveFolderPath(const QString &folderPath)
{
    QString resolved = folderPath;
    if (QDir(resolved).isAbsolute()) {
        return resolved;
    }

    const QString root = findProjectRoot();
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList checks = {
        root + "/" + resolved,
        appDir + "/" + resolved,
        QDir(appDir).absoluteFilePath("../" + resolved),
        QDir(appDir).absoluteFilePath("src/" + resolved),
        QDir(root).absoluteFilePath("src/" + resolved)
    };

    for (const QString &candidate : checks) {
        if (QFileInfo::exists(candidate) && QDir(candidate).exists()) {
            return QDir(candidate).absolutePath();
        }
    }
    return QDir(root).absoluteFilePath(resolved);
}

void resetState()
{
    g_logLines.clear();
    g_error = false;
    g_currentLines.clear();
}

void appendLog(const QString &text)
{
    g_logLines += text + "\n";
    qDebug().noquote() << text;
}

QString normalizeNewlines(QString text)
{
    text.replace("\r\n", "\n");
    text.replace("\r", "\n");
    return text;
}
}

QStringList ConfigGenerator::availableValues(const QString &folderPath)
{
    const QString resolvedFolder = resolveFolderPath(folderPath);
    QDir directory(resolvedFolder);
    QStringList items;
    if (!directory.exists()) {
        return items;
    }

    const QFileInfoList files = directory.entryInfoList(QDir::Files, QDir::Name);
    for (const QFileInfo &info : files) {
        if (info.suffix().compare("json", Qt::CaseInsensitive) == 0) {
            items << info.baseName();
        }
    }
    return items;
}

QString ConfigGenerator::buildCommandString(const QString &machineName, const QStringList &mode)
{
    QStringList quotedMode;
    for (const QString &item : mode) {
        QString sanitized = item;
        sanitized.replace('"', "\\\"");
        quotedMode << QString("\"%1\"").arg(sanitized);
    }

    QString nameText;
    if (!machineName.isEmpty()) {
        nameText = machineName;
        nameText.replace('"', "\\\"");
    }

    const QString items = quotedMode.join(", ");
    return QString("CreateConfigs.Generate(\"%1\", [%2])")
        .arg(nameText)
        .arg(items);
}

bool ConfigGenerator::Generate(const QString &machineName, const QStringList &mode)
{
    resetState();
    const QString root = findProjectRoot();

    appendLog("Pro FW Configurations Generator");
    appendLog("Created by xXHenneBXx");
    appendLog("");

    const QString targetDir = machineName.isEmpty() ? (mode.join("-") + "/") : (machineName + "/");
    const QString outputRoot = root + "/";
    QDir outputDir(outputRoot);
    if (!outputDir.mkpath(targetDir)) {
        appendLog("Failed to create output directory: " + outputRoot + targetDir);
        g_error = true;
    }

    const QStringList sourceList = { "Configuration.h", "Configuration_adv.h", "Version.h", "platformio.ini" };
    for (const QString &sourceFile : sourceList) {
        if (!customizeFile(machineName, root + "/Original Configs/", outputRoot + targetDir, mode, sourceFile)) {
            break;
        }
    }

    if (g_error) {
        appendLog("\nAn error was found while processing your request");
    } else {
        appendLog("\nConfiguration files correctly generated");
    }

    QFile logFile(outputRoot + targetDir + "log.txt");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << g_logLines;
        logFile.close();
    }

    return g_error;
}

bool ConfigGenerator::customizeFile(const QString &machineName,
                                    const QString &sourceDir,
                                    const QString &targetDir,
                                    const QStringList &mode,
                                    const QString &configName)
{
    const QString source = QDir::cleanPath(sourceDir + "/" + configName);
    const QString target = QDir::cleanPath(targetDir + "/" + configName);
    QFile sourceFile(source);
    if (!sourceFile.exists()) {
        appendLog("Source file: " + source + " not found");
        g_error = true;
        return false;
    }

    appendLog("-Process " + target);
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("Could not open source file: " + source);
        g_error = true;
        return false;
    }

    QString lines = normalizeNewlines(QString::fromUtf8(sourceFile.readAll()));
    sourceFile.close();

    g_currentLines = lines;
    const QString commonJson = QDir(findProjectRoot()).absoluteFilePath("_printers/Common.json");
    g_currentLines = processLines(commonJson, configName);
    lines = g_currentLines;

    const QString root = findProjectRoot();
    const QStringList pathList = { root + "/_printers/", root + "/_boards/", root + "/_leveling/", root + "/_displays/", root + "/_thermistor/", root + "/_features/" };
    QString jsonFile;
    for (const QString &value : mode) {
        bool found = false;
        for (const QString &path : pathList) {
            const QString candidate = QDir::cleanPath(path + "/" + value + ".json");
            QFile candidateFile(candidate);
            if (candidateFile.exists()) {
                jsonFile = candidate;
                g_currentLines = lines;
                g_currentLines = processLines(jsonFile, configName);
                lines = g_currentLines;
                found = true;
                break;
            }
        }
        if (!found) {
            appendLog(QString("%1.json was not found").arg(value));
            g_error = true;
            break;
        }
        if (g_error) {
            break;
        }
    }

    if (!g_error) {
        if (!machineName.isEmpty()) {
            lines.replace("//#define CUSTOM_MACHINE_NAME \"3D Printer\"",
                          "#define CUSTOM_MACHINE_NAME \"" + machineName + "\"");
            lines.replace("//#define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION",
                          "#if ENABLED(IS_DEMO)\n  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION \" DEMO, NOT FOR PRODUCTION\"\n#else\n  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION \" " + machineName + ", based on 2.1.2.8\"\n#endif");
        } else {
            QString joined = mode.join(" ");
            lines.replace("//#define CUSTOM_MACHINE_NAME \"3D Printer\"",
                          "#define CUSTOM_MACHINE_NAME \"" + joined + "\"");
            lines.replace("//#define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION",
                          "#if ENABLED(IS_DEMO)\n  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION \" DEMO, NOT FOR PRODUCTION\"\n#else\n  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION \" " + joined + ", based on 2.1.2.8\"\n#endif");
        }
    }

    QFile outFile(target);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        appendLog("Could not write target file: " + target);
        g_error = true;
        return false;
    }
    outFile.write(lines.toUtf8());
    outFile.close();
    return true;
}

QString ConfigGenerator::processLines(const QString &jsonFilePath, const QString &configName)
{
    if (g_currentLines.isEmpty() && QFileInfo::exists(jsonFilePath)) {
        g_currentLines = QString();
    }

    QFile file(jsonFilePath);
    if (!file.exists()) {
        return g_currentLines;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("Could not load " + jsonFilePath);
        g_error = true;
        return g_currentLines;
    }

    const QByteArray raw = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(raw, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        appendLog(QString("JSON parse error in %1: %2").arg(jsonFilePath, parseError.errorString()));
        g_error = true;
        return g_currentLines;
    }

    if (!document.isObject()) {
        return g_currentLines;
    }

    QJsonObject root = document.object();
    if (!root.contains(configName)) {
        return g_currentLines;
    }

    const QJsonValue configValue = root.value(configName);
    if (!configValue.isArray()) {
        return g_currentLines;
    }

    const QJsonArray operations = configValue.toArray();
    for (const QJsonValue &value : operations) {
        if (!value.isObject()) {
            continue;
        }

        const QJsonObject operation = value.toObject();
        bool localError = false;
        g_currentLines = applyOperation(g_currentLines, operation, localError, jsonFilePath);
        if (localError || g_error) {
            g_error = true;
            break;
        }
    }

    return g_currentLines;
}

QString ConfigGenerator::applyOperation(const QString &lines,
                                       const QJsonObject &operation,
                                       bool &hadError,
                                       const QString &sourceFile)
{
    hadError = false;
    QString content = lines;
    const QString op = operation.value("op").toString();
    const QString searchFor = operation.value("searchfor").toString();
    const QString newline = operation.value("newline").toString();
    const QString mask = operation.value("mask").toString();
    const QString value = operation.value("value").toString();
    const QString comment = operation.value("comment").toString();

    if (op == "InsertAfter") {
        if (searchFor.isEmpty()) {
            appendLog("InsertAfter requires a searchfor value.");
            hadError = true;
            return content;
        }
        const QRegularExpression regex(QString("%1(.*)").arg(QRegularExpression::escape(searchFor)));
        const QRegularExpressionMatch match = regex.match(content);
        if (match.hasMatch()) {
            const QString matched = match.captured(0);
            return content.replace(matched, matched + "\n" + newline);
        }
        appendLog("Not found " + searchFor + " in " + sourceFile);
        hadError = true;
        return content;
    }

    if (op == "Replace") {
        if (searchFor.isEmpty()) {
            appendLog("Replace requires a searchfor value.");
            hadError = true;
            return content;
        }
        QRegularExpression regex(searchFor);
        QRegularExpressionMatchIterator it = regex.globalMatch(content);
        int count = 0;
        while (it.hasNext()) {
            it.next();
            ++count;
        }
        if (count > 0) {
            return content.replace(regex, value);
        }
        appendLog("Not found " + searchFor + " in " + sourceFile);
        hadError = true;
        return content;
    }

    if (op == "Custom" || op == "CustomVal" || op == "Enable" || op == "Disable") {
        const QString definition = QString("#define %1").arg(searchFor);
        const QString escapedSearch = QRegularExpression::escape(searchFor);
        const QString effectiveMask = mask.isEmpty() ? "(.*)" : QString("(%1)").arg(mask);
        QString pattern;
        QString replacement;

        if (op == "Disable") {
            pattern = QString(R"((\n *)(//)?( *)(#define +%1\b.*))").arg(escapedSearch);
            replacement = QString("\\1// #define %1").arg(searchFor);
            if (!comment.isEmpty()) {
                replacement += QString("  // %1").arg(comment);
            }
            QRegularExpression regex(pattern);
            const QString updated = content.replace(regex, replacement);
            if (updated != content) {
                return updated;
            }
            if (content.contains(definition) || content.contains("#undef " + searchFor)) {
                return content;
            }
            appendLog("Optional disabled macro not present: " + searchFor + " in " + sourceFile);
            return content;
        }

        if (op == "Enable") {
			pattern = QString(R"((\n *)(//)?( *)(#define +%1\b.*))").arg(escapedSearch);
			replacement = comment.isEmpty() ? QStringLiteral("\\1\\4") : QString("\\1\\4  // %1").arg(comment);
            if (!comment.isEmpty()) {
                replacement += QString("  // %1").arg(comment);
            }
            QRegularExpression regex(pattern);
            const QString updated = content.replace(regex, replacement);
            if (updated != content) {
                return updated;
            }
            if (content.contains(definition)) {
                return content;
            }
            appendLog("Optional enabled macro not present: " + searchFor + " in " + sourceFile);
            return content;
        }

        pattern = QString(R"((#define +%1\b[ \t]*)(%2)(.*))").arg(escapedSearch, effectiveMask);
        QRegularExpression regex(pattern);
        const QRegularExpressionMatch match = regex.match(content);
        if (match.hasMatch()) {
            QString replacementText = QString("#define %1 %2").arg(searchFor, value);
            if (!comment.isEmpty()) {
                replacementText += QString("  // %1").arg(comment);
            }
            return content.replace(match.captured(0), replacementText);
        }

        appendLog("Optional config value not present: " + searchFor + " in " + sourceFile);
        return content;
    }

    appendLog("Invalid operation: " + op + " in " + sourceFile);
    hadError = true;
    return content;
}
