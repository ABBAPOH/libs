#ifndef FILEMANAGERHISTORYITEM_H
#define FILEMANAGERHISTORYITEM_H

#include "../filemanager_global.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtGui/QIcon>

namespace FileManager {

class FileManagerHistoryItemData;

class FILEMANAGER_EXPORT FileManagerHistoryItem
{
public:
    FileManagerHistoryItem();
    FileManagerHistoryItem(FileManagerHistoryItemData &dd);
    FileManagerHistoryItem(const FileManagerHistoryItem &other);
    FileManagerHistoryItem &operator=(const FileManagerHistoryItem &);
    ~FileManagerHistoryItem();

    QIcon icon() const;

    bool isValid() const;

    QDateTime lastVisited() const;
    QString title() const;
    QUrl url() const;

    bool operator==(const FileManagerHistoryItem &other) const;

private:
    QSharedDataPointer<FileManagerHistoryItemData> d;

    friend class FileManagerModel;
};

} // namespace FileManager

#endif // FILEMANAGERHISTORYITEM_H
