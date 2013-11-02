#include "filemanagerwidget.h"
#include "filemanagerwidget_p.h"

#include <QtCore/QDataStream>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QMetaProperty>

#include <QtGui/QContextMenuEvent>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#endif

#include <Parts/ActionManager>
#include <Parts/constants.h>
#include <IO/BoolLocker>
#include <IO/QDriveInfo>
#include <IO/QFileCopier>

#include "fileinfodialog.h"
#include "fileitemdelegate.h"
#include "filemanagerconstants.h"
#include "filemanagerhistory_p.h"
#include "filemanagerhistoryitem_p.h"
#include "filemanagersettings.h"
#include "filemanagersettings_p.h"
#include "openwithmenu.h"

using namespace Parts;
using namespace FileManager;

static const quint32 fmMagicNumber = 0x46313574; // "F15t"
static const quint8 fmVersion = 1;

static QDir::Filters mBaseFilters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs;

void FileManagerWidgetPrivate::init()
{
    Q_Q(FileManagerWidget);

    qRegisterMetaType<FileManagerWidget::ViewMode>("ViewMode");

    setupUi();
    createActions();
    retranslateUi();

    blockKeyEvent = false;
    currentView = 0;
    viewMode = (FileManagerWidget::ViewMode)-1; // to skip if in setView()
    fileSystemManager = 0;
    itemsExpandable = true;
    alternatingRowColors = true;
    showHiddenFiles = false;
    model = 0;

    q->setViewMode(FileManagerWidget::IconView);
    q->setModel(0);

    setFileSystemManager(FileSystemManager::instance());

    q->setFlow(FileManagerWidget::LeftToRight);
#ifdef Q_OS_MAC
    q->setIconSize(FileManagerWidget::IconView, QSize(64, 64));
    q->setGridSize(QSize(128, 128));
#else
    q->setIconSize(FileManagerWidget::IconView, QSize(32, 32));
    q->setGridSize(QSize(96, 96));
#endif
    q->setIconSize(FileManagerWidget::ColumnView, QSize(16, 16));
    q->setIconSize(FileManagerWidget::TreeView, QSize(16, 16));
    q->setItemsExpandable(true);
}

void FileManagerWidgetPrivate::setupUi()
{
    Q_Q(FileManagerWidget);

    layout = new QStackedLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);

    q->setLayout(layout);
    q->setFocusPolicy(Qt::StrongFocus);
    q->setMinimumSize(200, 200);
}

void FileManagerWidgetPrivate::createActions()
{
    Q_Q(FileManagerWidget);

    actions[FileManagerWidget::Open] = new QAction(this);
    actions[FileManagerWidget::Open]->setEnabled(false);
    actions[FileManagerWidget::Open]->setObjectName(Constants::Actions::Open);
    connect(actions[FileManagerWidget::Open], SIGNAL(triggered()), q, SLOT(open()));

    actions[FileManagerWidget::NewFolder] = new QAction(this);
    actions[FileManagerWidget::NewFolder]->setObjectName(Constants::Actions::NewFolder);
    connect(actions[FileManagerWidget::NewFolder], SIGNAL(triggered()), q, SLOT(newFolder()));

    actions[FileManagerWidget::Rename] = new QAction(this);
    actions[FileManagerWidget::Rename]->setEnabled(false);
    actions[FileManagerWidget::Rename]->setObjectName(Constants::Actions::Rename);
    connect(actions[FileManagerWidget::Rename], SIGNAL(triggered()), q, SLOT(rename()));

    actions[FileManagerWidget::MoveToTrash] = new QAction(this);
    actions[FileManagerWidget::MoveToTrash]->setEnabled(false);
    actions[FileManagerWidget::MoveToTrash]->setObjectName(Constants::Actions::MoveToTrash);
    connect(actions[FileManagerWidget::MoveToTrash], SIGNAL(triggered()), q, SLOT(moveToTrash()));

    actions[FileManagerWidget::Remove] = new QAction(this);
    actions[FileManagerWidget::Remove]->setEnabled(false);
    actions[FileManagerWidget::Remove]->setObjectName(Constants::Actions::Remove);
    connect(actions[FileManagerWidget::Remove], SIGNAL(triggered()), q, SLOT(remove()));

    actions[FileManagerWidget::ShowFileInfo] = new QAction(this);
    actions[FileManagerWidget::ShowFileInfo]->setObjectName(Constants::Actions::ShowFileInfo);
    connect(actions[FileManagerWidget::ShowFileInfo], SIGNAL(triggered()), q, SLOT(showFileInfo()));

    actions[FileManagerWidget::Redo] = new QAction(this);
    actions[FileManagerWidget::Redo]->setEnabled(false);
    actions[FileManagerWidget::Redo]->setObjectName(Constants::Actions::Redo);
    connect(actions[FileManagerWidget::Redo], SIGNAL(triggered()), q, SLOT(redo()));

    actions[FileManagerWidget::Undo] = new QAction(this);
    actions[FileManagerWidget::Undo]->setEnabled(false);
    actions[FileManagerWidget::Undo]->setObjectName(Constants::Actions::Undo);
    connect(actions[FileManagerWidget::Undo], SIGNAL(triggered()), q, SLOT(undo()));

    actions[FileManagerWidget::Cut] = new QAction(this);
    actions[FileManagerWidget::Cut]->setEnabled(false);
    actions[FileManagerWidget::Cut]->setObjectName(Constants::Actions::Cut);
//    connect(actions[FileManagerWidget::Cut], SIGNAL(triggered()), q, SLOT(cut()));

    actions[FileManagerWidget::Copy] = new QAction(this);
    actions[FileManagerWidget::Copy]->setObjectName(Constants::Actions::Copy);
    connect(actions[FileManagerWidget::Copy], SIGNAL(triggered()), q, SLOT(copy()));

    actions[FileManagerWidget::Paste] = new QAction(this);
    actions[FileManagerWidget::Paste]->setObjectName(Constants::Actions::Paste);
    connect(actions[FileManagerWidget::Paste], SIGNAL(triggered()), q, SLOT(paste()));

    actions[FileManagerWidget::MoveHere] = new QAction(this);
    actions[FileManagerWidget::MoveHere]->setObjectName(Constants::Actions::MoveHere);
    connect(actions[FileManagerWidget::MoveHere], SIGNAL(triggered()), q, SLOT(moveHere()));

    actions[FileManagerWidget::SelectAll] = new QAction(this);
    actions[FileManagerWidget::SelectAll]->setObjectName(Constants::Actions::SelectAll);
    connect(actions[FileManagerWidget::SelectAll], SIGNAL(triggered()), q, SLOT(selectAll()));

    actions[FileManagerWidget::ShowHiddenFiles] = new QAction(this);
    actions[FileManagerWidget::ShowHiddenFiles]->setCheckable(true);
    actions[FileManagerWidget::ShowHiddenFiles]->setObjectName(Constants::Actions::ShowHiddenFiles);

    viewModeGroup = new QActionGroup(this);

    actions[FileManagerWidget::IconMode] = new QAction(viewModeGroup);
    actions[FileManagerWidget::ColumnMode] = new QAction(viewModeGroup);
    actions[FileManagerWidget::TreeMode] = new QAction(viewModeGroup);

    actions[FileManagerWidget::IconMode]->setCheckable(true);
    actions[FileManagerWidget::IconMode]->setChecked(true);
    actions[FileManagerWidget::ColumnMode]->setCheckable(true);
    actions[FileManagerWidget::TreeMode]->setCheckable(true);

    actions[FileManagerWidget::IconMode]->setData(FileManagerWidget::IconView);
    actions[FileManagerWidget::ColumnMode]->setData(FileManagerWidget::ColumnView);
    actions[FileManagerWidget::TreeMode]->setData(FileManagerWidget::TreeView);

    actions[FileManagerWidget::IconMode]->setObjectName(Constants::Actions::IconMode);
    actions[FileManagerWidget::ColumnMode]->setObjectName(Constants::Actions::ColumnMode);
    actions[FileManagerWidget::TreeMode]->setObjectName(Constants::Actions::TreeMode);

    connect(actions[FileManagerWidget::IconMode], SIGNAL(triggered(bool)), SLOT(toggleViewMode(bool)));
    connect(actions[FileManagerWidget::ColumnMode], SIGNAL(triggered(bool)), SLOT(toggleViewMode(bool)));
    connect(actions[FileManagerWidget::TreeMode], SIGNAL(triggered(bool)), SLOT(toggleViewMode(bool)));

    sortByGroup = new QActionGroup(this);

    actions[FileManagerWidget::SortByName] = new QAction(sortByGroup);
    actions[FileManagerWidget::SortBySize] = new QAction(sortByGroup);
    actions[FileManagerWidget::SortByType] = new QAction(sortByGroup);
    actions[FileManagerWidget::SortByDate] = new QAction(sortByGroup);
    actions[FileManagerWidget::SortDescendingOrder] = new QAction(this);

    actions[FileManagerWidget::SortByName]->setCheckable(true);
    actions[FileManagerWidget::SortBySize]->setCheckable(true);
    actions[FileManagerWidget::SortByType]->setCheckable(true);
    actions[FileManagerWidget::SortByDate]->setCheckable(true);
    actions[FileManagerWidget::SortDescendingOrder]->setCheckable(true);

    actions[FileManagerWidget::SortByName]->setChecked(true);

    actions[FileManagerWidget::SortByName]->setData(FileManagerModel::NameColumn);
    actions[FileManagerWidget::SortBySize]->setData(FileManagerModel::SizeColumn);
    actions[FileManagerWidget::SortByType]->setData(FileManagerModel::TypeColumn);
    actions[FileManagerWidget::SortByDate]->setData(FileManagerModel::DateColumn);

    actions[FileManagerWidget::SortByName]->setObjectName(Constants::Actions::SortByName);
    actions[FileManagerWidget::SortBySize]->setObjectName(Constants::Actions::SortBySize);
    actions[FileManagerWidget::SortByType]->setObjectName(Constants::Actions::SortByType);
    actions[FileManagerWidget::SortByDate]->setObjectName(Constants::Actions::SortByDate);
    actions[FileManagerWidget::SortDescendingOrder]->setObjectName(Constants::Actions::SortByDescendingOrder);

    connect(actions[FileManagerWidget::SortByName], SIGNAL(triggered(bool)), SLOT(toggleSortColumn(bool)));
    connect(actions[FileManagerWidget::SortBySize], SIGNAL(triggered(bool)), SLOT(toggleSortColumn(bool)));
    connect(actions[FileManagerWidget::SortByType], SIGNAL(triggered(bool)), SLOT(toggleSortColumn(bool)));
    connect(actions[FileManagerWidget::SortByDate], SIGNAL(triggered(bool)), SLOT(toggleSortColumn(bool)));
    connect(actions[FileManagerWidget::SortDescendingOrder], SIGNAL(triggered(bool)), SLOT(toggleSortOrder(bool)));

    for (int i = 0; i < FileManagerWidget::ActionCount; i++) {
        actions[i]->setShortcutContext(Qt::WidgetWithChildrenShortcut);
        q->addAction(actions[i]);
    }
}

void FileManagerWidgetPrivate::retranslateUi()
{
    actions[FileManagerWidget::Open]->setText(tr("Open"));

    actions[FileManagerWidget::NewFolder]->setText(tr("New Folder"));
    actions[FileManagerWidget::Rename]->setText(tr("Rename"));
    actions[FileManagerWidget::MoveToTrash]->setText(tr("Move to trash"));
    actions[FileManagerWidget::Remove]->setText(tr("Remove"));
    actions[FileManagerWidget::ShowFileInfo]->setText(tr("File info"));

    actions[FileManagerWidget::Redo]->setText(tr("Redo"));
    actions[FileManagerWidget::Undo]->setText(tr("Undo"));
    actions[FileManagerWidget::Cut]->setText(tr("Cut"));
    actions[FileManagerWidget::Copy]->setText(tr("Copy"));
    actions[FileManagerWidget::Paste]->setText(tr("Paste"));
    actions[FileManagerWidget::MoveHere]->setText(tr("Move object(s) here"));
    actions[FileManagerWidget::SelectAll]->setText(tr("Select all"));

    actions[FileManagerWidget::ShowHiddenFiles]->setText(tr("Show hidden files"));

    actions[FileManagerWidget::IconMode]->setText(tr("Icon view"));
    actions[FileManagerWidget::ColumnMode]->setText(tr("Column view"));
    actions[FileManagerWidget::TreeMode]->setText(tr("Tree view"));

    actions[FileManagerWidget::SortByName]->setText(tr("Sort by name"));
    actions[FileManagerWidget::SortBySize]->setText(tr("Sort by size"));
    actions[FileManagerWidget::SortByType]->setText(tr("Sort by type"));
    actions[FileManagerWidget::SortByDate]->setText(tr("Sort by date"));
    actions[FileManagerWidget::SortDescendingOrder]->setText(tr("Descending order"));
}

class ListView : public QListView
{
public:
    ListView(QWidget *parent = 0) : QListView(parent) {}

    void dropEvent(QDropEvent *e)
    {
        QListView::dropEvent(e);
        doItemsLayout(); // fix for wrong layout after dropping
    }
};

void FileManagerWidgetPrivate::connectModel(FileManagerModel *model)
{
    Q_ASSERT(model);

    actions[FileManagerWidget::ShowHiddenFiles]->setChecked(model->showHiddenFiles());
    connect(actions[FileManagerWidget::ShowHiddenFiles], SIGNAL(triggered(bool)),
            model, SLOT(setShowHiddenFiles(bool)));
    connect(model, SIGNAL(showHiddenFilesChanged(bool)),
            actions[FileManagerWidget::ShowHiddenFiles], SLOT(setChecked(bool)));

    connect(model, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(model, SIGNAL(sortingChanged(int,Qt::SortOrder)),
            this, SLOT(onSortingChanged(int,Qt::SortOrder)));
    connect(model->itemModel(), SIGNAL(fileSystemManagerChanged()),
            this, SLOT(onFileSystemManagerChanged()));
}

void FileManagerWidgetPrivate::disconnectModel(FileManagerModel *model)
{
    Q_ASSERT(model);

    disconnect(currentView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
               this, SLOT(onSelectionChanged()));

    disconnect(actions[FileManagerWidget::ShowHiddenFiles], SIGNAL(triggered(bool)),
            model, SLOT(setShowHiddenFiles(bool)));
    disconnect(model, SIGNAL(showHiddenFilesChanged(bool)),
            actions[FileManagerWidget::ShowHiddenFiles], SLOT(setChecked(bool)));

    disconnect(model, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    disconnect(model, SIGNAL(sortingChanged(int,Qt::SortOrder)),
               this, SLOT(onSortingChanged(int,Qt::SortOrder)));

    disconnect(model->itemModel(), SIGNAL(fileSystemManagerChanged()),
            this, SLOT(onFileSystemManagerChanged()));
}

void FileManagerWidgetPrivate::setFileSystemManager(FileSystemManager *manager)
{
    if (fileSystemManager) {
        disconnect(fileSystemManager, SIGNAL(canRedoChanged(bool)),
                actions[FileManagerWidget::Redo], SLOT(setEnabled(bool)));
        disconnect(fileSystemManager, SIGNAL(canUndoChanged(bool)),
                actions[FileManagerWidget::Undo], SLOT(setEnabled(bool)));
    }

    fileSystemManager = manager;

    actions[FileManagerWidget::Redo]->setEnabled(fileSystemManager->canRedo());
    actions[FileManagerWidget::Undo]->setEnabled(fileSystemManager->canUndo());
    connect(fileSystemManager, SIGNAL(canRedoChanged(bool)),
            actions[FileManagerWidget::Redo], SLOT(setEnabled(bool)));
    connect(fileSystemManager, SIGNAL(canUndoChanged(bool)),
            actions[FileManagerWidget::Undo], SLOT(setEnabled(bool)));
}

QModelIndexList FileManagerWidgetPrivate::selectedIndexes() const
{
    Q_ASSERT(currentView);
    Q_ASSERT(currentView->selectionModel());
    if (viewMode == FileManagerWidget::ColumnView)
        return currentView->selectionModel()->selectedIndexes();

    return currentView->selectionModel()->selectedRows();
}

void FileManagerWidgetPrivate::paste(bool copy)
{
    Q_Q(FileManagerWidget);

    QClipboard * clipboard = QApplication::clipboard();
    const QMimeData * data = clipboard->mimeData();
    const QList<QUrl> & urls = data->urls();

    QStringList files;
    foreach (const QUrl &url, urls) {
        files.append(url.toLocalFile());
    }

    Q_ASSERT(q->url().isLocalFile());
    QString path = q->url().toLocalFile();
    if (copy)
        fileSystemManager->copy(files, path);
    else
        fileSystemManager->move(files, path);
}

bool FileManagerWidgetPrivate::hasFiles(const QStringList &paths)
{
    // TODO: use model for speedup
    foreach (const QString &path, paths) {
        if (!QFileInfo(path).isDir()) {
            return true;
        }
    }
    return false;
}

QAbstractItemView * FileManagerWidgetPrivate::createView(FileManagerWidget::ViewMode mode)
{
    Q_Q(FileManagerWidget);

    QAbstractItemView *view = 0;
    switch (mode) {
    case FileManagerWidget::IconView:
        view = createListView();
        break;
    case FileManagerWidget::ColumnView:
        view = createColumnView();
        break;
    case FileManagerWidget::TreeView:
        view = createTreeView();
        break;
    default:
        break;
    }

    view->setFocusProxy(q);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setDragDropMode(QAbstractItemView::DragDrop);
    view->setAcceptDrops(true);
    view->setDefaultDropAction(Qt::MoveAction);
    view->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    view->setItemDelegate(new FileItemDelegate(view));
    view->setIconSize(iconSizes[mode]);

    if (mode != FileManagerWidget::ColumnView) {
        connect(view, SIGNAL(activated(QModelIndex)),
                this, SLOT(onActivated(QModelIndex)),
                Qt::QueuedConnection);
    } else {
        connect(view, SIGNAL(doubleClicked(QModelIndex)),
                this, SLOT(onActivated(QModelIndex)),
                Qt::QueuedConnection);
    }

    setViewModel(view, model);

    return view;
}

QListView * FileManagerWidgetPrivate::createListView()
{
    Q_Q(FileManagerWidget);

    QListView *iconView = new ListView(q);

    iconView->setWordWrap(true);
    iconView->setWrapping(true);
    iconView->setFlow(QListView::LeftToRight);
    iconView->setViewMode(QListView::IconMode);
    iconView->setResizeMode(QListView::Adjust);
    iconView->setMovement(QListView::Static);

    updateListViewFlow(iconView);

    return iconView;
}

QColumnView *FileManagerWidgetPrivate::createColumnView()
{
    Q_Q(FileManagerWidget);

    return new QColumnView(q);
}

QTreeView * FileManagerWidgetPrivate::createTreeView()
{
    Q_Q(FileManagerWidget);

    QTreeView *treeView = new QTreeView(q);

    treeView->setUniformRowHeights(true); // huge speed up
    treeView->setAlternatingRowColors(alternatingRowColors);
    treeView->setExpandsOnDoubleClick(false);
    treeView->setItemsExpandable(itemsExpandable);
    treeView->setRootIsDecorated(itemsExpandable);
    treeView->setSortingEnabled(true);

    connect(treeView->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),
            this, SLOT(onSortIndicatorChanged(int,Qt::SortOrder)));

    return treeView;
}

QAbstractItemView *FileManagerWidgetPrivate::testCurrentView(FileManagerWidget::ViewMode mode)
{
    switch (mode) {
    case FileManagerWidget::IconView:
        return qobject_cast<QListView*>(currentView);
    case FileManagerWidget::ColumnView:
        return qobject_cast<QColumnView*>(currentView);
    case FileManagerWidget::TreeView:
        return qobject_cast<QTreeView*>(currentView);
    default:
        break;
    }
    return 0;
}

void FileManagerWidgetPrivate::setViewModel(QAbstractItemView *view, FileManagerModel *model)
{
    Q_Q(FileManagerWidget);

    Q_ASSERT(view);

    if (!model) {
        view->setModel(0);
        return;
    }

    FileSystemModel *itemModel = model->itemModel();
    view->setModel(itemModel);

    const QUrl url = q->url();
    Q_ASSERT(url.isLocalFile());
    QString path = url.toLocalFile();
    QModelIndex index = itemModel->index(path);
    view->setRootIndex(index);
    QTreeView *treeView = qobject_cast<QTreeView *>(view);
    if (treeView) {
        treeView->setColumnWidth(0, 250);
        treeView->sortByColumn(model->sortColumn(), model->sortOrder());
    }
    connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(onSelectionChanged()));
}

void FileManagerWidgetPrivate::updateListViewFlow(QListView *view)
{
    QSize s = gridSize;
    if (flow == FileManagerWidget::LeftToRight) {
        view->setFlow(QListView::LeftToRight);
        view->setViewMode(QListView::IconMode);
        view->update();
    } else if (flow == FileManagerWidget::TopToBottom) {
        view->setFlow(QListView::TopToBottom);
        view->setViewMode(QListView::ListMode);
        s.setWidth(256);
    }
    view->setGridSize(s);
    // fixes dragging
    view->setDragEnabled(true);
    view->viewport()->setAcceptDrops(true);
    view->setMouseTracking(true);
}

void FileManagerWidgetPrivate::onUrlChanged(const QUrl &url)
{
    Q_Q(FileManagerWidget);

    Q_ASSERT(url.isLocalFile());
    QString path = url.toLocalFile();

    QModelIndex index = model->itemModel()->index(path);
    currentView->selectionModel()->clear(); // to prevent bug with selecting dir we enter in
    currentView->setRootIndex(index);

    emit q->urlChanged(url);
}

void FileManagerWidgetPrivate::onSortingChanged(int column, Qt::SortOrder order)
{
    QTreeView *view = qobject_cast<QTreeView*>(currentView);
    if (view)
        view->sortByColumn(column, order);

    for (int i = FileManagerModel::NameColumn; i < FileManagerModel::ColumnCount; i++) {
        actions[FileManagerWidget::SortByName + i]->setChecked(column == FileManagerModel::NameColumn + i);
    }

    actions[FileManagerWidget::SortDescendingOrder]->setChecked(order == Qt::DescendingOrder);
}

void FileManagerWidgetPrivate::onActivated(const QModelIndex &index)
{
    Q_Q(FileManagerWidget);

    QString path = model->itemModel()->filePath(index);
    if (path.isEmpty())
        return;
    QUrl url = QUrl::fromLocalFile(path);
    Qt::KeyboardModifiers modifiers = qApp->keyboardModifiers();
    emit q->openRequested(QList<QUrl>() << url, modifiers);
}

void FileManagerWidgetPrivate::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    model->setSorting(FileManagerModel::Column(logicalIndex), order);
}

void FileManagerWidgetPrivate::onFileSystemManagerChanged()
{
    setFileSystemManager(model->itemModel()->fileSystemManager());
}

void FileManagerWidgetPrivate::toggleViewMode(bool toggled)
{
    Q_Q(FileManagerWidget);

    if (toggled) {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
            q->setViewMode((FileManagerWidget::ViewMode)action->data().toInt());
    }
}

void FileManagerWidgetPrivate::toggleSortColumn(bool toggled)
{
    if (toggled) {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
            model->setSortColumn((FileManagerModel::Column)action->data().toInt());
    }
}

void FileManagerWidgetPrivate::toggleSortOrder(bool descending)
{
    model->setSortOrder(descending ? Qt::DescendingOrder : Qt::AscendingOrder);
}

void FileManagerWidgetPrivate::onSelectionChanged()
{
    Q_Q(FileManagerWidget);

    QList<QUrl> urls = q->selectedUrls();
    bool enabled = !urls.empty();

    actions[FileManagerWidget::Open]->setEnabled(enabled);
    actions[FileManagerWidget::Rename]->setEnabled(enabled);
    actions[FileManagerWidget::MoveToTrash]->setEnabled(enabled);
    actions[FileManagerWidget::Remove]->setEnabled(enabled);
//    actions[FileManagerWidget::Cut]->setEnabled(enabled);
    actions[FileManagerWidget::Copy]->setEnabled(enabled);

    if (!urls.isEmpty()) {
        if (urls.size() == 1) {
            QUrl url = urls.first();
            Q_ASSERT(url.isLocalFile());
            QFileInfo info(url.toLocalFile());
            actions[FileManagerWidget::Cut]->setText(tr("Cut \"%1\"").arg(info.fileName()));
            actions[FileManagerWidget::Copy]->setText(tr("Copy \"%1\"").arg(info.fileName()));
        } else {
            actions[FileManagerWidget::Cut]->setText(tr("Cut %1 items").arg(urls.size()));
            actions[FileManagerWidget::Copy]->setText(tr("Copy %1 items").arg(urls.size()));
        }
    } else {
        actions[FileManagerWidget::Cut]->setText(tr("Cut"));
        actions[FileManagerWidget::Copy]->setText(tr("Copy"));
    }

    emit q->selectedPathsChanged();
}

/*!
    \class FileManager::FileManagerWidget
    \brief FileManagerWidget is a main widget class for displaying filesystem.

    It provides basic functionality of a file manager - possibility to change
    view mode, copy/paste and undo/redo support, possbility to change files and
    folders sort order, etc.

    Also it provides actions for basic file manipulation - creating new folders,
    removing files, showing file info.

    \image html filemanagerwidget.png
*/

/*!
    \enum FileManager::FileManagerWidget::ViewMode
    This enum describes possible view modes can be used in FileManagerWidget.

    \var FileManager::FileManagerWidget::ViewMode FileManager::FileManagerWidget::IconView
    Is an icon or a list view, depending on file manager settings.

    \var FileManager::FileManagerWidget::ViewMode FileManager::FileManagerWidget::ColumnView
    Is a column view - each folder represented using a column.

    \var FileManager::FileManagerWidget::ViewMode FileManager::FileManagerWidget::TreeView
    Is a tree or a detailed list, depending on file manager settings.
*/

/*!
    \enum FileManager::FileManagerWidget::Flow
    This enum describes possible flow values for a list view mode.

    \var FileManager::FileManagerWidget::ViewMode FileManager::FileManagerWidget::LeftToRight
    The items are laid out in the view from the left to the right.

    \var FileManager::FileManagerWidget::ViewMode FileManager::FileManagerWidget::TopToBottom
    The items are laid out in the view from the top to the bottom.
*/

/*!
    \enum FileManager::FileManagerWidget::Column
    This enum describes column currently being sorted.

    \var FileManager::FileManagerWidget::Column FileManager::FileManagerWidget::NameColumn
    Name column.

    \var FileManager::FileManagerWidget::Column FileManager::FileManagerWidget::SizeColumn
    Size column.

    \var FileManager::FileManagerWidget::Column FileManager::FileManagerWidget::TypeColumn
    Type column.

    \var FileManager::FileManagerWidget::Column FileManager::FileManagerWidget::DateColumn
    Date column.
*/

/*!
    \enum FileManager::FileManagerWidget::Action
    This enum describes the types of actions which can be triggered in a file
    manager widget.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::NoAction
    Refers to an invalid action.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Open
    Open current file in an external editor or folder in current widget.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::NewFolder
    Create new folder in current dir.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Rename
    Rename selected file or folder.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::MoveToTrash
    Move selected files and/or folders to the trash.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Remove
    Permanently remove selected paths from filesystem.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::ShowFileInfo
    Show FileManager::FileInfoDialog for each selected path.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Redo
    Redo next operation.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Undo
    Undo previous operation.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Copy
    Copy selected paths to a clipboard.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::Paste
    Copy files and folders located at paths in a clipboard to a current folder.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::MoveHere
    Move files and folders located at paths in a clipboard to a current folder.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SelectAll
    Select all paths at a current location.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::ShowHiddenFiles
    Toggle showing of hidden files.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::IconMode
    Toggle icon view mode.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::ColumnMode
    Toggle column view mode.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::TreeMode
    Toggle tree view mode.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SortByName
    Toggle sorting by name.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SortBySize
    Toggle sorting by size.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SortByType
    Toggle sorting by type.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SortByDate
    Toggle sorting by date.

    \var FileManager::FileManagerWidget::Action FileManager::FileManagerWidget::SortDescendingOrder
    Toggle sort order.
*/

/*!
    Creates FileManagerWidget with the given \a parent.
*/
FileManagerWidget::FileManagerWidget(QWidget *parent) :
    QWidget(parent),
    d_ptr(new FileManagerWidgetPrivate(this))
{
    Q_D(FileManagerWidget);

    d->init();
}

/*!
    Destroys FileManagerWidget.
*/
FileManagerWidget::~FileManagerWidget()
{
    delete d_ptr;
}

/*!
    Returns action specified by \a action enum.
*/
QAction * FileManagerWidget::action(Action action) const
{
    Q_D(const FileManagerWidget);

    if (action <= NoAction || action >= ActionCount)
        return 0;

    return d->actions[action];
}

/*!
    \property FileManagerWidget::alternatingRowColors
    Holds if view should use alternatingRowColors.  It only affects tree view
    mode.

    Default value is true.
*/
bool FileManagerWidget::alternatingRowColors() const
{
    Q_D(const FileManagerWidget);

    return d->alternatingRowColors;
}

void FileManagerWidget::setAlternatingRowColors(bool enable)
{
    Q_D(FileManagerWidget);

    if (d->alternatingRowColors == enable)
        return;

    QTreeView *view = qobject_cast<QTreeView*>(d->currentView);
    if (view)
        view->setAlternatingRowColors(enable);
}

/*!
    \property FileManagerWidget::url
    This property holds current folder displayed it the widget.
*/
QUrl FileManagerWidget::url() const
{
    Q_D(const FileManagerWidget);

    return d->model->url();
}

void FileManagerWidget::setUrl(const QUrl &url)
{
    Q_D(FileManagerWidget);

    d->model->setUrl(url);
}

/*!
    \fn void FileManagerWidget::currentPathChanged(const QString &path)
    This signal is emitted when currently displayed folder changes.
*/

/*!
    \property FileManagerWidget::flow
    This property holds which direction the items layout should flow. It only
    affects icon view mode.

    Default value is FileManagerWidget::LeftToRight.
*/
FileManagerWidget::Flow FileManagerWidget::flow() const
{
    Q_D(const FileManagerWidget);

    return d->flow;
}

void FileManagerWidget::setFlow(FileManagerWidget::Flow flow)
{
    Q_D(FileManagerWidget);

    if (d->flow == flow)
        return;

    d->flow = flow;

    QListView *view = qobject_cast<QListView*>(d->currentView);
    if (view)
        d->updateListViewFlow(view);

    emit flowChanged(flow);
}

/*!
    \property FileManagerWidget::gridSize
    This property holds the size of the layout grid. It only affects icon view
    mode.
*/
QSize FileManagerWidget::gridSize() const
{
    Q_D(const FileManagerWidget);

    return d->gridSize;
}

void FileManagerWidget::setGridSize(QSize s)
{
    Q_D(FileManagerWidget);

    if (d->gridSize == s)
        return;

    d->gridSize = s;

    QListView *view = qobject_cast<QListView*>(d->currentView);
    if (view) {
        if (d->flow == TopToBottom)
            s.setWidth(256);
        view->setGridSize(s);
    }

    emit gridSizeChanged(s);
}

QSize FileManagerWidget::iconSize() const
{
    return iconSize(IconView);
}

void FileManagerWidget::setIconSize(QSize size)
{
    setIconSize(IconView, size);
}

QSize FileManagerWidget::iconSizeColumn() const
{
    return iconSize(ColumnView);
}

void FileManagerWidget::setIconSizeColumn(QSize size)
{
    setIconSize(ColumnView, size);
}

QSize FileManagerWidget::iconSizeTree() const
{
    return iconSize(TreeView);
}

void FileManagerWidget::setIconSizeTree(QSize size)
{
    setIconSize(TreeView, size);
}

/*!
    Returns icon fixe for the view mode \a mode. Each view has indepentent icon
    size; that size is usually controlled by FileManagerSettings, i.e. when you
    change icon size using settings, it is changed in all views simultaneously.
*/
QSize FileManagerWidget::iconSize(FileManagerWidget::ViewMode mode) const
{
    Q_D(const FileManagerWidget);

    if (mode < 0 || mode >= FileManagerWidget::MaxViews)
        return QSize();

    return d->iconSizes[mode];
}

/*!
    Sets icon fixe for the view mode \a mode.
*/
void FileManagerWidget::setIconSize(FileManagerWidget::ViewMode mode, QSize size)
{
    Q_D(FileManagerWidget);

    if (mode < 0 && mode >= FileManagerWidget::MaxViews)
        return;

    if (d->iconSizes[mode] == size)
        return;

    d->iconSizes[mode] = size;

    QAbstractItemView *view = d->testCurrentView(mode);
    if (view)
        view->setIconSize(size);
    switch (mode) {
    case IconView:
        emit iconSizeChanged(size);
        break;
    case ColumnView:
        emit iconSizeColumnChanged(size);
        break;
    case TreeView:
        emit iconSizeTreeChanged(size);
        break;
    default:
        break;
    }
}

/*!
    \property FileManagerWidget::itemsExpandable
    This property holds whether the items are expandable by the user. It only
    affects tree view mode.

    Default value is true.
*/
bool FileManagerWidget::itemsExpandable() const
{
    Q_D(const FileManagerWidget);

    return d->itemsExpandable;
}

void FileManagerWidget::setItemsExpandable(bool expandable)
{
    Q_D(FileManagerWidget);

    if (d->itemsExpandable == expandable)
        return;

    d->itemsExpandable = expandable;

    QTreeView *view = qobject_cast<QTreeView *>(d->currentView);
    if (view) {
        if (!expandable) {
            view->collapseAll();
            view->setRootIsDecorated(false);
            view->setItemsExpandable(false);
        } else {
            view->setRootIsDecorated(true);
            view->setItemsExpandable(true);
        }
    }

    emit itemsExpandableChanged(expandable);
}

/*!
    \property FileManagerWidget::selectedUrls
    This property holds list of path (files or folders) currently selected by a user.
*/
QList<QUrl> FileManagerWidget::selectedUrls() const
{
    Q_D(const FileManagerWidget);

    QStringList paths;
    foreach (const QModelIndex &index, d->selectedIndexes()) {
        paths.append(d->model->itemModel()->filePath(index));
    }
    // TODO: remove hack when QColumnView will work correctly
    paths.removeDuplicates();

    QList<QUrl> result;
    foreach (const QString &path, paths) {
        result.append(QUrl::fromLocalFile(path));
    }
    return result;
}

/*!
    \fn void FileManagerWidget::selectedPathsChanged()
    This signal is emitted when user changes the selection in a view.
*/

/*!
    \property FileManagerWidget::viewMode
    This property holds current view mode.

    Default value is FileManagerWidget::IconView.

    \sa FileManagerWidget::ViewMode
*/
FileManagerWidget::ViewMode FileManagerWidget::viewMode() const
{
    Q_D(const FileManagerWidget);

    return d->viewMode;
}

void FileManagerWidget::setViewMode(ViewMode mode)
{
    Q_D(FileManagerWidget);

    if (mode < 0 || mode >= MaxViews)
            return;

    if (d->viewMode != mode) {
        d->viewMode = mode;
        bool focus = d->currentView ? d->currentView->hasFocus() : false;
        d->layout->setCurrentIndex(mode);
        delete d->currentView;
        d->currentView = d->createView(mode);
        d->layout->addWidget(d->currentView);
        if (focus) {
            d->currentView->setFocus();
        }

        d->actions[IconMode]->setChecked(mode == IconView);
        d->actions[ColumnMode]->setChecked(mode == ColumnView);
        d->actions[TreeMode]->setChecked(mode == TreeView);

        emit viewModeChanged(d->viewMode);
    }
}

/*!
    \fn void FileManagerWidget::viewModeChanged(FileManagerWidget::ViewMode mode)
    This signal is emitted when FileManagerWidget::viewMode property is changed;
    \a mode is a new view mode.
*/

/*!
    Returns pointer to a FileManager::FileSystemModel assotiated with this
    widget.
*/
FileManagerModel * FileManagerWidget::model() const
{
    Q_D(const FileManagerWidget);
    return d->model;
}

/*!
    Sets \a model as the current model of the widget.

    \note The widget does not take ownership of the model unless it is the
    models's parent object. The parent object of the provided model remains the
    owner of the object.

    \note Widget deletes current model if widget is the model parent.
*/
void FileManagerWidget::setModel(FileManagerModel *model)
{
    Q_D(FileManagerWidget);

    if (!model)
        model = new FileManagerModel(this);

    if (d->model == model)
        return;

    if (d->model) {
        d->disconnectModel(d->model);

        if (d->model->parent() == this)
            delete d->model;
    }

    d->model = model;
    Q_ASSERT(d->model);

    d->setViewModel(d->currentView, d->model);

    d->setFileSystemManager(d->model->itemModel()->fileSystemManager());
    d->connectModel(d->model);
    d->onUrlChanged(d->model->url());
}

/*!
    Restores widget's \a state. Returns true on success.
    \sa saveState()
*/
bool FileManagerWidget::restoreState(const QByteArray &state)
{
    if (state.isEmpty())
        return false;

    QByteArray tmpState = state;
    QDataStream s(&tmpState, QIODevice::ReadOnly);

    quint32 magic;
    quint8 version;

    s >> magic;
    if (magic != fmMagicNumber)
        return false;

    s >> version;
    if (version != fmVersion)
        return false;

    bool alternate, expandable;
    quint8 flow, viewMode;
    QSize gridSize, iconSizes[MaxViews];

    s >> alternate;
    s >> expandable;
    s >> flow;
    s >> gridSize;
    for (int i = 0; i < MaxViews; i++)
        s >> iconSizes[i];
    s >> viewMode;

    if (s.status() != QDataStream::Ok)
        return false;

    setAlternatingRowColors(alternate);
    setItemsExpandable(expandable);
    setFlow((Flow)flow);
    setGridSize(gridSize);
    for (int i = 0; i < MaxViews; i++)
        setIconSize(ViewMode(i), iconSizes[i]);
    setViewMode((FileManagerWidget::ViewMode)viewMode);
    return true;
}

/*!
    Stores widget's state to a byte array.
    \sa restoreState()
*/
QByteArray FileManagerWidget::saveState() const
{
    Q_D(const FileManagerWidget);

    QByteArray state;
    QDataStream s(&state, QIODevice::WriteOnly);

    s << fmMagicNumber;
    s << fmVersion;
    s << d->alternatingRowColors;
    s << d->itemsExpandable;
    s << (quint8)d->flow;
    s << d->gridSize;
    for (int i = 0; i < MaxViews; i++)
        s << d->iconSizes[ViewMode(i)];
    s << (quint8)d->viewMode;

    return state;
}

/*!
    Clears current path and history.
*/
void FileManagerWidget::clear()
{
    Q_D(FileManagerWidget);

    setUrl(QString());
    d->model->history()->clear();
}

QMenu * FileManagerWidget::createStandardMenu(const QList<QUrl> &urls)
{
    Q_D(FileManagerWidget);

    QMenu *menu = new QMenu;
    if (urls.isEmpty()) {
        menu->addAction(d->actions[NewFolder]);
        menu->addSeparator();
        menu->addAction(d->actions[ShowFileInfo]);
        menu->addSeparator();
        menu->addAction(d->actions[Paste]);
        menu->addAction(d->actions[MoveHere]);
        menu->addAction(d->actions[SelectAll]);
        menu->addSeparator();
        QMenu * viewModeMenu = menu->addMenu(tr("View Mode"));
        viewModeMenu->addAction(d->actions[IconMode]);
        viewModeMenu->addAction(d->actions[ColumnMode]);
        viewModeMenu->addAction(d->actions[TreeMode]);
        QMenu * sortByMenu = menu->addMenu(tr("Sort by"));
        sortByMenu->addAction(d->actions[SortByName]);
        sortByMenu->addAction(d->actions[SortByType]);
        sortByMenu->addAction(d->actions[SortBySize]);
        sortByMenu->addAction(d->actions[SortByDate]);
        sortByMenu->addSeparator();
        sortByMenu->addAction(d->actions[SortDescendingOrder]);
    } else {
        menu->addAction(d->actions[Open]);

        OpenWithMenu *openWithMenu = new OpenWithMenu(menu);
        openWithMenu->setUrls(urls);
        menu->addMenu(openWithMenu);

        menu->addSeparator();
        menu->addAction(d->actions[ShowFileInfo]);
        menu->addSeparator();
        menu->addAction(d->actions[Rename]);
        menu->addAction(d->actions[MoveToTrash]);
        menu->addAction(d->actions[Remove]);
        menu->addSeparator();
        menu->addAction(d->actions[Copy]);
    }
    return menu;
}

/*!
    Creates new folder at the current folder.
*/
void FileManagerWidget::newFolder()
{
    Q_D(FileManagerWidget);
    Q_ASSERT(url().isLocalFile());
    QString dir = url().toLocalFile();
    if (dir == "") {
//        emit error;
    } else {
        QString  folderName = tr("New Folder");
        QModelIndex index = d->itemModel()->mkdir(d->itemModel()->index(dir), folderName);

        if (index.isValid())
            d->currentView->edit(index);
    }
}

/*!
    Opens currently selected paths. If selected only one folder, opens it;
    otherwise emits openRequested() signal.
*/
void FileManagerWidget::open()
{
    emit openRequested(selectedUrls(), Qt::NoModifier);
}

/*!
    Shows FileManager::FileInfoDialog for each selected path.
*/
void FileManagerWidget::showFileInfo()
{
    QList<QUrl> urls = selectedUrls();
    if (urls.isEmpty())
        urls.append(url());

    foreach (const QUrl &url, urls) {
        Q_ASSERT(url.isLocalFile());
        QString path = url.toLocalFile();
        FileInfoDialog *dialog = new FileInfoDialog(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setFileInfo(QFileInfo(path));
        dialog->show();
    }
}

/*!
    Permanently removes selected paths from filesystem. Also shows warning dialog.
*/
void FileManagerWidget::remove()
{
    Q_D(FileManagerWidget);

    if (FileManagerSettings::globalSettings()->warnOnFileRemove()) {
        QMessageBox messageBox;
        messageBox.setWindowTitle(tr("Remove files"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setText(tr("Are you sure you want to delete selected item(s)?"));
        messageBox.setInformativeText(tr("This action can't be undone."));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        if (messageBox.exec() == QMessageBox::No)
            return;
    }

    QStringList paths;
    foreach (const QUrl &url, selectedUrls()) {
        Q_ASSERT(url.isLocalFile());
        paths.append(url.toLocalFile());
    }
#ifdef Q_OS_WIN
    int index = d->fileSystemManager->remove(paths);
    QFileCopier *copier = d->fileSystemManager->copier(index);
    QEventLoop loop;
    connect(copier, SIGNAL(done(bool)), &loop, SLOT(quit()));
    loop.exec();

    // this is slow, but otherwise QFSM bugs when we remove files via copier.
    foreach (const QString &path, paths) {
        d->model->remove(d->model->index(path));
    }
#else
    d->fileSystemManager->remove(paths);
#endif
}

/*!
    Starts editing of the selected path. Editing of multiple paths are not
    supported.
*/
void FileManagerWidget::rename()
{
    Q_D(FileManagerWidget);

    QModelIndexList indexes = d->selectedIndexes();
    if (indexes.count() != 1) {
//        emit error();
    } else {
        d->currentView->edit(indexes.first());
    }
}

/*!
    Moves selected path to the trash.
*/
void FileManagerWidget::moveToTrash()
{
    Q_D(FileManagerWidget);
    QStringList paths;
    foreach (const QUrl &url, selectedUrls()) {
        Q_ASSERT(url.isLocalFile());
        paths.append(url.toLocalFile());
    }
    d->fileSystemManager->moveToTrash(paths);
}

/*!
    Undoes last operation. Remove operation can't be undone.
*/
void FileManagerWidget::undo()
{
    Q_D(FileManagerWidget);
    d->fileSystemManager->undo();
}

/*!
    Redoes next operation.
*/
void FileManagerWidget::redo()
{
    Q_D(FileManagerWidget);
    d->fileSystemManager->redo();
}

/*!
    Copies selected paths to a clipboard.
*/
void FileManagerWidget::copy()
{
    QClipboard * clipboard = QApplication::clipboard();
    QMimeData * data = new QMimeData();
    data->setUrls(selectedUrls());
    clipboard->setMimeData(data);
}

/*!
    Copies files and folders located at paths in a clipboard to a current folder.
*/
void FileManagerWidget::paste()
{
    Q_D(FileManagerWidget);

    d->paste(true);
}

/*!
    Moves files and folders located at paths in a clipboard to a current folder.
*/
void FileManagerWidget::moveHere()
{
    Q_D(FileManagerWidget);

    d->paste(false);
}

/*!
    Selects all paths at a current location.
*/
void FileManagerWidget::selectAll()
{
    Q_D(FileManagerWidget);

    d->currentView->selectAll();
}

/*!
    Shows FileManagerWidget's context menu.
*/
void FileManagerWidget::showContextMenu(QPoint pos)
{
    QList<QUrl> urls = selectedUrls();
    QMenu *menu = createStandardMenu(urls);
    menu->exec(mapToGlobal(pos));
    delete menu;
}

/*!
    \reimp
*/
void FileManagerWidget::contextMenuEvent(QContextMenuEvent *e)
{
    showContextMenu(e->pos());
}

/*!
    \reimp
*/
void FileManagerWidget::keyPressEvent(QKeyEvent *event)
{
    Q_D(FileManagerWidget);

    if (d->blockKeyEvent) // prevent endless recursion
        return;

    switch (event->key()) {
    // we override default shortucts to handle activated() signal only on mouse events.
    // this is needed to handle Qt::KeyboardModifiers correctly
#ifndef Q_WS_MAC
    case Qt::Key_Enter:
    case Qt::Key_Return:
        open();
        break;
#endif
#ifdef Q_WS_MAC
    case Qt::Key_O:
#endif
    case Qt::Key_Down:
        if (event->modifiers() & Qt::ControlModifier) {
            open();
            break;
        }
    default: {
        BoolLocker l(&d->blockKeyEvent);
        qApp->sendEvent(d_func()->currentView, event);
        break;
    }
    }
}

/*!
    \reimp
*/
void FileManagerWidget::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(FileManagerWidget);

    if (d->blockKeyEvent) // prevent endless recursion
        return;

    BoolLocker l(&d->blockKeyEvent);
    qApp->sendEvent(d_func()->currentView, event);
}
