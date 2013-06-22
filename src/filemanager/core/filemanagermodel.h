#ifndef FILEMANAGERMODEL_H
#define FILEMANAGERMODEL_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

namespace FileManager {

class FileManagerHistory;
class FileSystemModel;
class FileSystemManager;

class FileManagerModelPrivate;
class FileManagerModel : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FileManagerModel)
    Q_DISABLE_COPY(FileManagerModel)

    Q_PROPERTY(bool showHiddenFiles READ showHiddenFiles WRITE setShowHiddenFiles NOTIFY showHiddenFilesChanged)
    Q_PROPERTY(Column sortColumn READ sortColumn WRITE setSortColumn NOTIFY sortingChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortingChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

public:
    enum Column { NameColumn = 0, SizeColumn, TypeColumn, DateColumn, ColumnCount };

    explicit FileManagerModel(QObject *parent = 0);
    ~FileManagerModel();

    bool showHiddenFiles() const;

    Column sortColumn() const;
    void setSortColumn(Column column);

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder order);

    void setSorting(Column column, Qt::SortOrder order);

    QUrl url() const;
    void setUrl(const QUrl &url);

    FileManagerHistory *history() const;
    FileSystemModel *itemModel() const;

    bool restoreState(const QByteArray &state);
    QByteArray saveState() const;

public slots:
    void setShowHiddenFiles(bool show);

signals:
    void showHiddenFilesChanged(bool show);
    void sortingChanged(int column, Qt::SortOrder order);
    void urlChanged(const QUrl &url);

protected slots:
    void onCurrentItemIndexChanged(int index);

protected:
    FileManagerModelPrivate *d_ptr;
};

} // namespace FileManager

#endif // FILEMANAGERMODEL_H
