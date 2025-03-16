#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QDir>
#include <QPixmap>
#include <QFileDialog>
#include <QMutex>
#include <QList>


class DynamicResourceCreator : public QObject
{
	Q_OBJECT

public:
    DynamicResourceCreator(DynamicResourceCreator& other) = delete;
    void operator=(DynamicResourceCreator other) = delete;
    static DynamicResourceCreator* getInstance();

    void addPath(const QString& pathName);
    QStringList getPaths() const;
    bool addFileToPath(const QString& pathName, const QString& filePath);
    bool removeFileFromPath(const QString& pathName, const QString& fileName);
    QStringList getFilesInPath(const QString& pathName) const;
    QPixmap findFile(const QString& pathName, const QString& fileName) const;

private:
    explicit DynamicResourceCreator(QObject* parent = nullptr);
    ~DynamicResourceCreator() = default;

    QString getFullPath(const QString& pathName) const;
    void ensurePathExists(const QString& pathName);
    void loadExistingPaths();

    QList<QString> availablePaths;
    const QString prefix = "dynamicresource";

    static DynamicResourceCreator* instance;
    Q_DISABLE_COPY(DynamicResourceCreator)
};
