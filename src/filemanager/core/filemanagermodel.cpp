#include "filemanagermodel.h"
#include "filemanagermodel_p.h"

#include "filemanagerhistoryitem.h"
#include "filemanagerhistoryitem_p.h"
#include "filemanagerhistory.h"
#include "filemanagerhistory_p.h"
#include "filesystemmanager.h"
#include "filesystemmodel.h"

using namespace FileManager;

static const quint32 mMagicNumber = 0x4631346C; // "F14l"
static const quint8 mVersion = 1;
static const QDir::Filters mBaseFilters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs;

void FileManagerModelPrivate::init()
{
    Q_Q(FileManagerModel);
    model = new FileSystemModel(q);
#ifdef Q_OS_UNIX
    model->setRootPath("/");
#else
    model->setRootPath("");
#endif
    model->setFilter(mBaseFilters);
    model->setReadOnly(false);

    history = new FileManagerHistory(q);
    QObject::connect(history, SIGNAL(currentItemIndexChanged(int)), q, SLOT(onCurrentItemIndexChanged(int)));

    showHiddenFiles = false;

    sortColumn = (FileManagerModel::Column)-1;
    sortOrder = (Qt::SortOrder)-1;
    q->setSorting(FileManagerModel::NameColumn, Qt::AscendingOrder);

    q->setUrl(QUrl::fromLocalFile(QDir::homePath()));
}

/*!
    \class FileManagerModel
*/

/*!
    Creates a FileManagerModel with the given \a parent.
*/
FileManagerModel::FileManagerModel(QObject *parent) :
    QObject(parent),
    d_ptr(new FileManagerModelPrivate(this))
{
    Q_D(FileManagerModel);

    d->init();
}

/*!
    Destroys the FileManagerModel object.
*/
FileManagerModel::~FileManagerModel()
{
    delete d_ptr;
}

/*!
    \property FileManagerModel::showHiddenFiles
    Holds whether show hidden files or not.
*/

bool FileManagerModel::showHiddenFiles() const
{
    Q_D(const FileManagerModel);
    return d->showHiddenFiles;
}

void FileManagerModel::setShowHiddenFiles(bool show)
{
    Q_D(FileManagerModel);

    if (d->showHiddenFiles == show)
        return;

    d->showHiddenFiles = show;

    if (show)
        d->model->setFilter(mBaseFilters | QDir::Hidden);
    else
        d->model->setFilter(mBaseFilters);

    emit showHiddenFilesChanged(show);
}

/*!
    \property FileManagerModel::sortColumn
    This property holds the column which are currently sorted.
    Default value is FileManagerModel::NameColumn.
*/

FileManagerModel::Column FileManagerModel::sortColumn() const
{
    return d_func()->sortColumn;
}

void FileManagerModel::setSortColumn(FileManagerModel::Column column)
{
    Q_D(FileManagerModel);

    if (d->sortColumn == column)
        return;

    d->sortColumn = column;
    d->model->sort(d->sortColumn, d->sortOrder);

    emit sortingChanged(d->sortColumn, d->sortOrder);
}

/*!
    \property FileManagerModel::sortOrder
    This property holds the order in which FileManagerModel::sortingColumn is
    sorted.

    Default value is Qt::AscendingOrder.
*/

Qt::SortOrder FileManagerModel::sortOrder() const
{
    Q_D(const FileManagerModel);
    return d->sortOrder;
}

void FileManagerModel::setSortOrder(Qt::SortOrder order)
{
    Q_D(FileManagerModel);

    if (d->sortOrder == order)
        return;

    d->sortOrder = order;
    d->model->sort(d->sortColumn, d->sortOrder);

    emit sortingChanged(d->sortColumn, d->sortOrder);
}

/*!
    Sets both sort \a column and sort \a order.
*/
void FileManagerModel::setSorting(FileManagerModel::Column column, Qt::SortOrder order)
{
    Q_D(FileManagerModel);

    if (d->sortColumn == column && d->sortOrder == order)
        return;

    d->sortColumn = column;
    d->sortOrder = order;
    d->model->sort(d->sortColumn, d->sortOrder);
    emit sortingChanged(column, order);
}

/*!
    \fn void FileManagerModel::sortingChanged(int column, Qt::SortOrder order)

    This signal is emitted when FileManagerModel::sortColumn or
    FileManagerModel::sortOrder properties are changed.
*/

/*!
    \property FileManagerModel::url

    This property holds current url.
*/
QUrl FileManagerModel::url() const
{
    Q_D(const FileManagerModel);
    return d->url;
}

void FileManagerModel::setUrl(const QUrl &url)
{
    Q_D(FileManagerModel);

    Q_ASSERT(url.isLocalFile());

    if (d->url == url)
        return;

    if (!url.isLocalFile())
        return;

    QString path = url.toLocalFile();
    QFileInfo info(path);
    if (!info.isDir())
        return;

    d->url = url;

    FileManagerHistoryItemData item;
    item.url = url;
    item.title = QFileInfo(url.path()).fileName();
    item.lastVisited = QDateTime::currentDateTime();
    d->history->d_func()->appendItem(FileManagerHistoryItem(item));

    emit urlChanged(url);
}

/*!
    Returns pointer to the FileManagerHistory object.
*/
FileManagerHistory * FileManagerModel::history() const
{
    Q_D(const FileManagerModel);
    return d->history;
}

/*!
    Returns pointer to the FileSystemModel object.
*/
FileSystemModel * FileManagerModel::itemModel() const
{
    Q_D(const FileManagerModel);
    return d->model;
}

/*!
    Restores model state from \a state bytearray. Returns true on success.
*/
bool FileManagerModel::restoreState(const QByteArray &state)
{
    if (state.isEmpty())
        return false;

    QByteArray tmp = state;
    QDataStream s(&tmp, QIODevice::ReadOnly);

    quint32 magic;
    quint8 version;

    s >> magic;
    if (magic != mMagicNumber)
        return false;

    s >> version;
    if (version != mVersion)
        return false;

    QUrl url;
    bool showHiddenFiles;
    quint8 sortColumn, sortOrder;

    s >> url;
    s >> showHiddenFiles;
    s >> sortColumn;
    s >> sortOrder;

    if (s.status() != QDataStream::Ok)
        return false;

    setUrl(url);
    setShowHiddenFiles(showHiddenFiles);
    setSorting(FileManagerModel::Column(sortColumn), Qt::SortOrder(sortOrder));

    return true;
}

/*!
    Stores model's state and returns in a bytearray.
*/
QByteArray FileManagerModel::saveState() const
{
    Q_D(const FileManagerModel);

    QByteArray state;
    QDataStream s(&state, QIODevice::WriteOnly);

    s << mMagicNumber;
    s << mVersion;
    s << d->url;
    s << d->showHiddenFiles;
    s << (quint8)d->sortColumn;
    s << (quint8)d->sortOrder;

    return state;
}

void FileManagerModel::onCurrentItemIndexChanged(int index)
{
    Q_D(FileManagerModel);

    QUrl url = d->history->itemAt(index).url();
    if (d->url == url)
        return;

    d->url = url;
    emit urlChanged(url);
}
