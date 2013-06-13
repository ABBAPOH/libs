#include "navigationmodel.h"
#include "navigationmodel_p.h"

#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include <QtGui/QFontMetrics>

#if QT_VERSION >= 0x050000
#include <QtCore/QStandardPaths>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileIconProvider>
#else
#include <IO/QStandardPaths>
#include <QtGui/QApplication>
#include <QtGui/QFileIconProvider>
#endif

#include <IO/QDriveController>

#include "filesystemmodel.h"
#include "navigationpanelsettings.h"

using namespace FileManager;

// todo: refactor model
NavigationModelPrivate::NavigationModelPrivate(NavigationModel *qq) :
    q_ptr(qq)
{
}

void NavigationModelPrivate::insertItem(NavigationModelItem *parentItem, const QString &name, const QString &path)
{
    Q_Q(NavigationModel);

    QModelIndex parent = q->createIndex(parentItem->row(), 0, parentItem);
    q->beginInsertRows(parent, parentItem->childCount(), parentItem->childCount());
    NavigationModelItem *item = new NavigationModelItem(parentItem, name, path);
    item->icon = iconProvider->icon(QFileInfo(path));
    mapToItem.insert(path, item);
    q->endInsertRows();
}

void NavigationModelPrivate::removeItem(const QString &path)
{
    Q_Q(NavigationModel);

    NavigationModelItem *item = mapToItem.value(path);
    if (!item)
        return;

    NavigationModelItem *parentItem = item->parent();

    QModelIndex parent = q->createIndex(parentItem->row(), 0, parentItem);
    q->beginRemoveRows(parent, item->row(), item->row());
    delete item;
    mapToItem.remove(path);
    q->endRemoveRows();
}

QString getDriveName(const QDriveInfo &info)
{
    QString name = info.name();

#if defined(Q_OS_WIN)
    QString path = info.rootPath();
    if (!name.isEmpty())
        name = QString("%1 (%2)").arg(path).arg(name);
    else
        name = QString("%1").arg(path);
#elif defined(Q_OS_LINUX)
    QString path = info.rootPath();
    if (name.isEmpty())
        name = path;
#endif

    return name;
}

void NavigationModelPrivate::onDriveAdded(const QString &path)
{
    QDriveInfo info(path);
    QString name = getDriveName(info);

    if (info.type() == QDriveInfo::RemoteDrive)
        insertItem(networkItem, name, path);
#ifndef Q_OS_WIN
    else if (info.type() != QDriveInfo::InvalidDrive)
#else
    else // awful hack for unmounting *.iso
#endif
        insertItem(drivesItem, name, path);

    NavigationModelItem* item = mapToItem.value(path);
    if (item)
        item->driveInfo = info;
}

void NavigationModelPrivate::onDriveRemoved(const QString &path)
{
    removeItem(path);
}

//#if QT_VERSION >= 0x050000
static QString locationToString(NavigationModel::StandardLocations locations)
{
    switch (locations) {
    case NavigationModel::DesktopLocation :
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    case NavigationModel::DocumentsLocation :
        return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    case NavigationModel::MusicLocation :
        return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    case NavigationModel::MoviesLocation :
        return QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    case NavigationModel::PicturesLocation :
        return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    case NavigationModel::HomeLocation :
        return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    case NavigationModel::ApplicationsLocation :
        return QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    case NavigationModel::DownloadsLocation :
        return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    default: return QString();
    }
}

NavigationModel::StandardLocation pathToLocation(const QString &path)
{
    QStringList locations;
    for (int i = NavigationModel::DesktopLocation; i <= NavigationModel::StandardLocationCount; i = (i << 1)) {
        locations.append(locationToString((NavigationModel::StandardLocation)i));
    }
    int index = locations.indexOf(path);
    if (index != -1)
        return (NavigationModel::StandardLocation)(NavigationModel::DesktopLocation << index);

    return NavigationModel::NoLocation;
}

/*!
    \class FileManager::NavigationModel
    \brief NavigationModel is a class for displaying a tree-like list of devices,
    standard and user folders.
*/

/*!
    Creates NavigationModel with the given \a parent.
*/
NavigationModel::NavigationModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new NavigationModelPrivate(this))
{
    Q_D(NavigationModel);

    d->rootItem = new NavigationModelItem();

    d->foldersItem = new NavigationModelItem(d->rootItem, tr("Favorites"));
    d->networkItem = new NavigationModelItem(d->rootItem, tr("Network"));
    d->drivesItem = new NavigationModelItem(d->rootItem, tr("Devices"));

    d->iconProvider = FileSystemModel::staticProvider();

    d->driveController = new QDriveController(this);
    connect(d->driveController, SIGNAL(driveMounted(QString)), d, SLOT(onDriveAdded(QString)));
    connect(d->driveController, SIGNAL(driveUnmounted(QString)), d, SLOT(onDriveRemoved(QString)));

    QList<QDriveInfo> drives = QDriveInfo::drives();
    foreach (const QDriveInfo &info, drives) {
        QString name = getDriveName(info);
        QString path = info.rootPath();

        NavigationModelItem *item = 0;
        if (info.type() == QDriveInfo::RemoteDrive)
            item = new NavigationModelItem(d->networkItem, name, path);
        else if (info.type() != QDriveInfo::InvalidDrive)
            item = new NavigationModelItem(d->drivesItem, name, path);

        if (item) {
            item->icon = d->iconProvider->icon(QFileInfo(path));
            d->mapToItem.insert(path, item);
            item->driveInfo = info;
        }
    }

    NavigationPanelSettings *panelSettings = NavigationPanelSettings::globalSettings();
    panelSettings->globalSettings()->addModel(this);
    QSettings settings("NavigationModel");
    if (settings.contains("folders")) {
        QStringList folders = settings.value("folders").toStringList();
        foreach (const QString &folder, folders)
            addFolder(folder);
    } else {
        setStandardLocations(panelSettings->standardLocations());
    }
}

/*!
    Destroys NavigationModel.
*/
NavigationModel::~NavigationModel()
{
    Q_D(NavigationModel);

    NavigationPanelSettings *panelSettings = NavigationPanelSettings::globalSettings();
    panelSettings->globalSettings()->removeModel(this);

    QSettings settings("NavigationModel");
    QStringList folders;
    foreach (NavigationModelItem *item, d->foldersItem->m_children) {
        folders.append(item->path);
    }
    settings.setValue("folders", folders);

    delete d->rootItem;

    delete d_ptr;
}

/*!
    \reimp
*/
int NavigationModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

/*!
    \reimp
*/
QVariant NavigationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    NavigationModelItem *item = static_cast<NavigationModelItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return item->name;
    } else if (role == Qt::DecorationRole) {
        if (item->type == NavigationModelItem::ChildItem)
            return item->icon;
        else
            return QVariant();
    } else if (role == Qt::SizeHintRole) {
        if (item->type == NavigationModelItem::GroupItem) {
            QFontMetrics fm = qApp->fontMetrics();
            int h = fm.height();
            int w = fm.width(item->name);
            return QSize(w, 1.5*h);
        }
    }
    return QVariant();
}

/*!
    \reimp
*/
QStringList NavigationModel::mimeTypes() const
{
    return QStringList() << "text/uri-list";
}

/*!
    \reimp
*/
bool NavigationModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                   int row, int column, const QModelIndex &parent)
{
    if (!data->hasUrls())
        return false;

    Q_D(NavigationModel);

    NavigationModelItem *parentItem = static_cast<NavigationModelItem*>(parent.internalPointer());
    if (parentItem == d->foldersItem) {

        const QList<QUrl> & urls = data->urls();

        for (int i = 0; i < urls.size(); i++) {
            QString path = urls[i].toLocalFile();
            NavigationModelItem *item = d->mapToItem.value(path);
            if (item) {
                if (item->row() < row)
                    row--;
                beginRemoveRows(parent, item->row(), item->row());
                delete item;
                endRemoveRows();
            }
        }

        beginInsertRows(parent, row, row + urls.size() - 1);
        for (int i = 0; i < urls.size(); i++) {
            QString path = urls[i].toLocalFile();
            QFileInfo info(path);

            NavigationModelItem *item = new NavigationModelItem(d->foldersItem, row + i);
            item->type = NavigationModelItem::ChildItem;
            item->path = path;
            item->name = info.fileName();
            item->icon = d->iconProvider->icon(info);

            d->mapToItem.insert(path, item);

            StandardLocation loc = pathToLocation(path);
            if (loc != NoLocation) {
                d->locations = d->locations | loc;
                emit standardLocationsChanged(d->locations);
            }

        }
        endInsertRows();
    } else {
        if (!data->hasFormat("user/navigationModel")) {
            QStringList paths;
            foreach (const QUrl &url, data->urls())
                paths.append(url.toLocalFile());

            if (paths.isEmpty())
                return false;

            QDriveInfo srcDrive(parentItem->path);
            QDriveInfo dstDrive(paths[0]);

            //default actions
            if (srcDrive != dstDrive)
                action = Qt::CopyAction;
            else
                action = Qt::MoveAction;

            Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();
            //change action according to the keyboard modifier, if any.
            if (keyboardModifiers.testFlag(Qt::AltModifier))
                action = Qt::CopyAction;
            else if (keyboardModifiers.testFlag(Qt::ControlModifier))
                action = Qt::MoveAction;
            else if (keyboardModifiers.testFlag(Qt::ShiftModifier))
                action = Qt::LinkAction;

            emit pathsDropped(parentItem->path, paths, action);
        } else {
            return dropMimeData(data, action, parent.row(), column, parent.parent());
        }
    }
    return true;
}

/*!
    \reimp
*/
QMimeData *NavigationModel::mimeData(const QModelIndexList &indexes) const
{
    Q_D(const NavigationModel);

    QMimeData *data = new QMimeData;
    QList<QUrl> urls;
    foreach (const QModelIndex &index, indexes) {
        NavigationModelItem *item = static_cast<NavigationModelItem*>(index.internalPointer());
        if (item->parent() == d->foldersItem) {
            urls.append(QUrl::fromLocalFile(item->path));
        }
    }
    data->setUrls(urls);
    data->setData("user/navigationModel", "true");
    return data;
}

/*!
    \reimp
*/
Qt::ItemFlags NavigationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Q_D(const NavigationModel);

    NavigationModelItem *item = static_cast<NavigationModelItem*>(index.internalPointer());
    if (item->type == NavigationModelItem::ChildItem && item->parent() == d->foldersItem)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    if (item == d->foldersItem)
        return Qt::ItemIsDropEnabled;

    if (item->type == NavigationModelItem::ChildItem)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::NoItemFlags;
}

/*!
    \reimp
*/
QModelIndex NavigationModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const NavigationModel);

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    NavigationModelItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<NavigationModelItem*>(parent.internalPointer());

    NavigationModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

/*!
    \reimp
*/
QModelIndex NavigationModel::parent(const QModelIndex &index) const
{
    Q_D(const NavigationModel);

    if (!index.isValid())
        return QModelIndex();

    NavigationModelItem *childItem = static_cast<NavigationModelItem*>(index.internalPointer());
    NavigationModelItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

/*!
    \reimp
*/
int NavigationModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const NavigationModel);

    NavigationModelItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<NavigationModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

/*!
    \reimp
*/
Qt::DropActions NavigationModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

/*!
    Returns index for the specified \a path or an invalid index, if there's no
    such path in a model.
*/
QModelIndex NavigationModel::index(const QString &path) const
{
    Q_D(const NavigationModel);

    NavigationModelItem *item = d->mapToItem.value(path);
    if (!item)
        return QModelIndex();
    else
        return createIndex(item->row(), 0, item);
}

/*!
    Returns path for the specified \a index or an empty string for an invalid
    or not leaf index.
*/
QString NavigationModel::path(const QModelIndex &index) const
{
    if (!index.isValid())
        return QString();

    NavigationModelItem *item = static_cast<NavigationModelItem*>(index.internalPointer());
    if (item->type == NavigationModelItem::ChildItem)
        return item->path;
    else
        return QString();
}

/*!
    Adds specified \a path to the list of user and standard paths.
*/
void NavigationModel::addFolder(const QString &path)
{
    Q_D(NavigationModel);

    QFileInfo info(path);
    if (/* !info.isDir() ||*/ !info.exists())
        return;

    QString canonicalPath = info.canonicalFilePath();
    QString name = info.fileName();

    if (d->mapToItem.contains(canonicalPath))
        return;

    d->insertItem(d->foldersItem, name, canonicalPath);

    StandardLocation loc = pathToLocation(canonicalPath);
    if (loc != NoLocation) {
        d->locations = d->locations | loc;
        emit standardLocationsChanged(d->locations);
    }
}

/*!
    Femoves specified \a path from the list of user and standard paths.
*/
void NavigationModel::removeFolder(const QString &path)
{
    Q_D(NavigationModel);

    d->removeItem(path);

    StandardLocation loc = pathToLocation(path);
    if (loc != NoLocation) {
        d->locations = d->locations & ~loc;
        emit standardLocationsChanged(d->locations);
    }

}

/*!
    Returns flags specifying standard locations added to the model.
*/
NavigationModel::StandardLocations NavigationModel::standardLocations() const
{
    Q_D(const NavigationModel);

    return d->locations;
}

/*!
    Adds or removes standard location \a loc, depending on \a on parameter.
*/
void NavigationModel::setStandardLocation(StandardLocation loc, bool on)
{
    QString path = locationToString(loc);
    if (on)
        addFolder(path);
    else
        removeFolder(path);
    emit standardLocationsChanged(standardLocations());
}

/*!
    Adds standard locations to the model specified by \a location flags.
*/
void NavigationModel::setStandardLocations(StandardLocations locations)
{
    Q_D(NavigationModel);

    if (d->locations == locations)
        return;

    d->locations = locations;

    for (int i = DesktopLocation; i <= StandardLocationCount; i = (i << 1)) {
//        setStandardLocation((StandardLocation)i, (locations & i));
        StandardLocation loc = (StandardLocation)i;
        bool on = (locations & i);
        QString path = locationToString(loc);
        if (on)
            addFolder(path);
        else
            removeFolder(path);

    }
    emit standardLocationsChanged(d->locations);
}

/*!
    Returns QDriveInfo object for the specified \a index. Works only for child
    indexes of a Drives branch.
*/
QDriveInfo NavigationModel::driveInfo(const QModelIndex& index) const
{
    NavigationModelItem *item = static_cast<NavigationModelItem*>(index.internalPointer());
    if (item)
        return item->driveInfo;

    return QDriveInfo();
}
