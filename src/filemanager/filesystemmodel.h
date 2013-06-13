#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include "filemanager_global.h"

#include <QFileSystemModel>

namespace FileManager {

class FileThumbnails;
class FileSystemManager;

class FILEMANAGER_EXPORT FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FileSystemModel(QObject *parent = 0);

    FileSystemManager *fileSystemManager() const;
    void setFileSystemManager(FileSystemManager *manager);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent
                      );

    static QFileIconProvider *staticProvider();

signals:
    void updateIndex(const QModelIndex index);

private slots:
    void onThumbDone(const QString &path, const QImage &thumb);
    void loadThumbs(const QString &dir);

private:
    QHash<QString, QIcon> thumbs;
    FileThumbnails *thumbnails;

protected:
    FileSystemManager *m_manager; // can be safely changed to d_ptr
};

} // namespace FileManager

#endif // FILESYSTEMMODEL_H
