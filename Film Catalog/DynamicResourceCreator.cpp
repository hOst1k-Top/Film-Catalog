#include "include/DynamicResourceCreator.h"
DynamicResourceCreator* DynamicResourceCreator::instance = nullptr;

DynamicResourceCreator* DynamicResourceCreator::getInstance()
{
    if (instance == nullptr) instance = new DynamicResourceCreator();
    return instance;
}

DynamicResourceCreator::DynamicResourceCreator(QObject* parent)
    : QObject(parent)
{
    loadExistingPaths();
}

void DynamicResourceCreator::loadExistingPaths()
{
    availablePaths.clear();
    QDir currentDir(QDir::currentPath());
    QStringList directories = currentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& dir : directories)
    {
        if (dir.startsWith(prefix + "_")) {
            availablePaths.append(dir.mid(QString(prefix + "_").length()));
        }
    }
}

void DynamicResourceCreator::addPath(const QString& pathName)
{
    ensurePathExists(pathName);
    if (!availablePaths.contains(pathName)) {
        availablePaths.append(pathName);
    }
}

QStringList DynamicResourceCreator::getPaths() const
{
    return availablePaths;
}

bool DynamicResourceCreator::addFileToPath(const QString& pathName, const QString& filePath)
{
    QString fullPath = getFullPath(pathName);
    QFileInfo fileInfo(filePath);
    return QFile::copy(filePath, fullPath + "/" + fileInfo.fileName());
}

bool DynamicResourceCreator::removeFileFromPath(const QString& pathName, const QString& fileName)
{
    QString fullPath = getFullPath(pathName) + "/" + fileName;
    return QFile::remove(fullPath);
}

QStringList DynamicResourceCreator::getFilesInPath(const QString& pathName) const
{
    QString fullPath = getFullPath(pathName);
    QDir dir(fullPath);
    return dir.entryList(QDir::Files);
}

QPixmap DynamicResourceCreator::findFile(const QString& pathName, const QString& fileName) const
{
    QString fullPath = getFullPath(pathName) + "/" + fileName;
    if (QFile::exists(fullPath)) {
        return QPixmap(fullPath);
    }
    return QPixmap();
}

QString DynamicResourceCreator::getFullPath(const QString& pathName) const
{
    return QDir::currentPath() + "/" + prefix + "_" + pathName;
}

void DynamicResourceCreator::ensurePathExists(const QString& pathName)
{
    QString fullPath = getFullPath(pathName);
    QDir dir;
    if (!dir.exists(fullPath)) {
        dir.mkpath(fullPath);
    }
}