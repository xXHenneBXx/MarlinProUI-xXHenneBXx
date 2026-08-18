#ifndef CONFIGGENERATOR_H
#define CONFIGGENERATOR_H

#include <QJsonObject>
#include <QString>
#include <QStringList>

class ConfigGenerator
{
public:
    static QStringList availableValues(const QString &folderPath);
    static bool Generate(const QString &machineName, const QStringList &mode);
    static QString buildCommandString(const QString &machineName, const QStringList &mode);

private:
    static bool customizeFile(const QString &machineName,
                             const QString &sourceDir,
                             const QString &targetDir,
                             const QStringList &mode,
                             const QString &configName);
    static QString processLines(const QString &jsonFilePath, const QString &configName);
    static QString applyOperation(const QString &lines,
                                  const QJsonObject &operation,
                                  bool &hadError,
                                  const QString &sourceFile = QString());
};

#endif // CONFIGGENERATOR_H
