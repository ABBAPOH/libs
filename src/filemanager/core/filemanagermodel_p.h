#ifndef FILEMANAGERMODEL_P_H
#define FILEMANAGERMODEL_P_H

#include "filemanagermodel.h"

namespace FileManager {

class FileManagerModelPrivate
{
    Q_DECLARE_PUBLIC(FileManagerModel)

    FileManagerModel *q_ptr;
public:
    FileManagerModelPrivate(FileManagerModel *qq) : q_ptr(qq) {}

    void init();

    QUrl url;
    FileSystemModel *model;
    FileManagerHistory *history;

    bool showHiddenFiles;
    FileManagerModel::Column sortColumn;
    Qt::SortOrder sortOrder;
};

} // namespace FileManager

#endif // FILEMANAGERMODEL_P_H
