#include "bookmarkswidget.h"
#include "bookmarkswidget_p.h"

#include <QtGui/QContextMenuEvent>
#include <QHeaderView>

using namespace Bookmarks;

void BookmarksWidgetPrivate::init()
{
    Q_Q(BookmarksWidget);

    model = 0;
    proxyModel = new BookmarkListFilterModel(q);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    createActions();
    createWidgets();
    retranslateUi();
}

void BookmarksWidgetPrivate::createActions()
{
    Q_Q(BookmarksWidget);
    actions[BookmarksWidget::ActionOpen] = new QAction(q);
    actions[BookmarksWidget::ActionOpen]->setObjectName("Open");
    actions[BookmarksWidget::ActionOpen]->setShortcut(QKeySequence::Open);
    q->connect(actions[BookmarksWidget::ActionOpen], SIGNAL(triggered()), q, SLOT(openTriggered()));

    actions[BookmarksWidget::ActionUp] = new QAction(q);
    actions[BookmarksWidget::ActionUp]->setObjectName("Up");
    actions[BookmarksWidget::ActionUp]->setShortcut(QKeySequence("Ctrl+Up"));
    q->connect(actions[BookmarksWidget::ActionUp], SIGNAL(triggered()), q, SLOT(up()));

    actions[BookmarksWidget::ActionRename] = new QAction(q);
    actions[BookmarksWidget::ActionRename]->setObjectName("Rename");
    q->connect(actions[BookmarksWidget::ActionRename], SIGNAL(triggered()), q, SLOT(rename()));

    actions[BookmarksWidget::ActionEditUrl] = new QAction(q);
    actions[BookmarksWidget::ActionEditUrl]->setObjectName("EditUrl");
    q->connect(actions[BookmarksWidget::ActionEditUrl], SIGNAL(triggered()), q, SLOT(editUrl()));

    actions[BookmarksWidget::ActionEditDescrition] = new QAction(q);
    actions[BookmarksWidget::ActionEditDescrition]->setObjectName("EditDescrition");
    q->connect(actions[BookmarksWidget::ActionEditDescrition], SIGNAL(triggered()), q, SLOT(editDescription()));

    actions[BookmarksWidget::ActionNewFolder] = new QAction(q);
    actions[BookmarksWidget::ActionNewFolder]->setObjectName("New");
    q->connect(actions[BookmarksWidget::ActionNewFolder], SIGNAL(triggered()), q, SLOT(addFolder()));

    actions[BookmarksWidget::ActionRemove] = new QAction(q);
    actions[BookmarksWidget::ActionRemove]->setObjectName("Remove");
    q->connect(actions[BookmarksWidget::ActionRemove], SIGNAL(triggered()), q, SLOT(remove()));
}

void BookmarksWidgetPrivate::createWidgets()
{
    Q_Q(BookmarksWidget);

    toolBar = new QToolBar(q);
    toolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    lineEdit = new QLineEdit(toolBar);
    lineEdit->setPlaceholderText(BookmarksWidget::tr("Filter"));
    lineEdit->setStyleSheet("QLineEdit { border-radius : 8px; }");
    lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lineEdit->setMinimumWidth(200);
    q->connect(lineEdit, SIGNAL(textEdited(QString)), proxyModel, SLOT(setFilterFixedString(QString)));

    toolBar->addAction(actions[BookmarksWidget::ActionNewFolder]);
    toolBar->addWidget(spacer);
    toolBar->addWidget(lineEdit);

    tableView = new QTreeView(q);
    tableView->setExpandsOnDoubleClick(false);
    tableView->setEditTriggers(QAbstractItemView::SelectedClicked);
    tableView->setDragDropMode(QAbstractItemView::DragDrop);
    // TODO: allow removing multiple indexes first
//    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setAcceptDrops(true);
    tableView->setDefaultDropAction(Qt::MoveAction);
    tableView->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setModel(proxyModel);
    q->connect(tableView, SIGNAL(activated(QModelIndex)), q, SLOT(onActivated(QModelIndex)));

    mainLayout = new QVBoxLayout(q);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(tableView);
    q->setLayout(mainLayout);
}

void BookmarksWidgetPrivate::retranslateUi()
{
    actions[BookmarksWidget::ActionOpen]->setText(BookmarksWidget::tr("Open"));
    actions[BookmarksWidget::ActionUp]->setText(BookmarksWidget::tr("Up"));
    actions[BookmarksWidget::ActionRename]->setText(BookmarksWidget::tr("Rename"));
    actions[BookmarksWidget::ActionEditUrl]->setText(BookmarksWidget::tr("Edit url"));
    actions[BookmarksWidget::ActionEditDescrition]->setText(BookmarksWidget::tr("Edit description"));
    actions[BookmarksWidget::ActionNewFolder]->setText(BookmarksWidget::tr("Add folder"));
    actions[BookmarksWidget::ActionRemove]->setText(BookmarksWidget::tr("Remove"));
}

/*!
    \class Bookmarks::BookmarksWidget
    This widget displays BookmarksModel as a tree and provides actions for
    editing bookmarks.
*/

/*!
    Creates BookmarksWidget with the given \a parent.
*/
BookmarksWidget::BookmarksWidget(QWidget *parent) :
    QWidget(parent),
    d_ptr(new BookmarksWidgetPrivate(this))
{
    Q_D(BookmarksWidget);
    d->init();
}

/*!
    Destroys BookmarksWidget.
*/
BookmarksWidget::~BookmarksWidget()
{
    delete d_ptr;
}

/*!
    Returns pointer to an action for a given \a action enum or 0 if value is
    invalid.
*/
QAction *BookmarksWidget::action(BookmarksWidget::Action action) const
{
    Q_D(const BookmarksWidget);

    if (action <= BookmarksWidget::ActionNone || action >= BookmarksWidget::ActionCount)
        return 0;
    return d->actions[action];
}

/*!
    Returns current model.
*/
BookmarksModel * BookmarksWidget::model() const
{
    Q_D(const BookmarksWidget);
    return d->model;
}

/*!
    Sets current model.
*/
void BookmarksWidget::setModel(BookmarksModel *model)
{
    Q_D(BookmarksWidget);

    d->model = model;
    d->proxyModel->setSourceModel(model);
    d->tableView->setColumnWidth(0, 200);
    d->tableView->setColumnWidth(1, 200);
}

/*!
    Returns list of selected rows in a view.
*/
QModelIndexList BookmarksWidget::selectedIndexes() const
{
    Q_D(const BookmarksWidget);

    QModelIndexList result;
    QModelIndexList indexes = d->tableView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, indexes) {
        const QModelIndex sourceIndex = d->proxyModel->mapToSource(index);
        result.append(sourceIndex);
    }
    return result;
}

bool BookmarksWidget::restoreState(const QByteArray &/*state*/)
{
    return true;
}

QByteArray BookmarksWidget::saveState() const
{
    return QByteArray();
}

/*!
    \reimp
*/
QSize BookmarksWidget::minimumSizeHint() const
{
    return QSize(200, 200);
}

/*!
    \reimp
*/
QSize BookmarksWidget::sizeHint() const
{
    return QSize(800, 600);
}

/*!
    Creates and returns standard context menu. You can add or remove actions
    from the menu to customize widget's behaviour.

    \note The ownership of the created menu is transfered to caller of this
    funciton.
*/
QMenu *BookmarksWidget::createStandardContextMenu() const
{
    Q_D(const BookmarksWidget);

    const QModelIndexList indexes = selectedIndexes();
    QScopedPointer<QMenu> menu(new QMenu);

    if (indexes.isEmpty()) {
        menu->addAction(d->actions[BookmarksWidget::ActionNewFolder]);
    } else {
        menu->addAction(d->actions[BookmarksWidget::ActionOpen]);
        menu->addSeparator();
        if (indexes.count() == 1) {
            menu->addAction(d->actions[BookmarksWidget::ActionRename]);
            if (!d->model->isFolder(indexes.first())) {
                menu->addAction(d->actions[BookmarksWidget::ActionEditUrl]);
                menu->addAction(d->actions[BookmarksWidget::ActionEditDescrition]);
            }
            menu->addSeparator();
        }
        menu->addAction(d->actions[BookmarksWidget::ActionRemove]);
    }
    return menu.take();
}

QList<QUrl> BookmarksWidget::urlsForIndexes(const QModelIndexList &indexes)
{
    QModelIndexList list = indexes;
    QList<QUrl> urls;
    while (!list.isEmpty()) {
        QModelIndex index = list.takeFirst();
        const BookmarksModel *model = qobject_cast<const BookmarksModel*>(index.model());
        Q_ASSERT(model);
        if (model->isFolder(index)) {
            for (int i = 0; i < model->rowCount(index); i++) {
                QModelIndex idx = model->index(i, 0, index);
                list.append(idx);
            }
        } else {
            urls.append(model->data(index, BookmarksModel::UrlRole).toUrl());
        }
    }
    return urls;
}

/*!
    Creates new bookmark folder in the current floder.
*/
void BookmarksWidget::addFolder()
{
    Q_D(BookmarksWidget);

    QModelIndex index = d->tableView->rootIndex();
    QModelIndex sourceIndex = d->proxyModel->mapToSource(index);
    QModelIndex newIndex = d->model->addFolder("New bookmark folder", sourceIndex);
    d->tableView->edit(d->proxyModel->mapFromSource(newIndex));
}

/*!
    Goes to the parent folder.
*/
void BookmarksWidget::up()
{
    Q_D(BookmarksWidget);

    QModelIndex index = d->tableView->rootIndex();
    if (!index.isValid())
        return;
    d->tableView->setRootIndex(index.parent());
}

/*!
    Starts editing name of the selected bookmark of folder.
*/
void BookmarksWidget::rename()
{
    Q_D(BookmarksWidget);

    const QModelIndexList indexes = selectedIndexes();
    if (indexes.count() != 1)
        return;

    QModelIndex index = indexes.first();
    index = d->proxyModel->mapFromSource(index);
    index = index.sibling(index.row(), 0);
    d->tableView->edit(index);
}

/*!
    Starts editing url of the selected bookmark.
*/
void BookmarksWidget::editUrl()
{
    Q_D(BookmarksWidget);

    const QModelIndexList indexes = selectedIndexes();
    if (indexes.count() != 1)
        return;

    QModelIndex index = indexes.first();
    if (d->model->isFolder(index))
        return;
    index = d->proxyModel->mapFromSource(index);
    index = index.sibling(index.row(), 1);
    d->tableView->edit(index);
}

/*!
    Starts editing description of the selected bookmark.
*/
void BookmarksWidget::editDescription()
{
    Q_D(BookmarksWidget);

    const QModelIndexList indexes = selectedIndexes();
    if (indexes.count() != 1)
        return;

    QModelIndex index = indexes.first();
    if (d->model->isFolder(index))
        return;
    index = d->proxyModel->mapFromSource(index);
    index = index.sibling(index.row(), 2);
    d->tableView->edit(index);
}

/*!
    Removes selected bookmark or folder.
*/
void BookmarksWidget::remove()
{
    Q_D(BookmarksWidget);

    const QModelIndexList indexes = selectedIndexes();
    if (indexes.count() != 1)
        return;

    QModelIndex index = indexes.first();
    d->model->remove(index);
}

/*!
    \reimp
*/
void BookmarksWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QScopedPointer<QMenu> menu(createStandardContextMenu());
    if (!menu)
        return;
    menu->exec(event->globalPos());
}

/*!
    \internal
*/
void BookmarksWidget::onActivated(const QModelIndex &index)
{
    Q_D(BookmarksWidget);

    const QModelIndex sourceIndex = d->proxyModel->mapToSource(index);
    if (d->model->isFolder(sourceIndex)) {
        d->proxyModel->setRootIndex(sourceIndex);
        d->tableView->setRootIndex(index);
    } else {
        const QUrl url = d->model->data(sourceIndex, BookmarksModel::UrlRole).toUrl();
        emit openRequested(QList<QUrl>() << url);
    }
}

/*!
    \internal
*/
void BookmarksWidget::openTriggered()
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() == 1) {
        QModelIndex index = indexes.first();
        if (d_ptr->model->isFolder(index)) {
            onActivated(d_ptr->proxyModel->mapFromSource(index));
            return;
        }
    }

    emit openRequested(urlsForIndexes(indexes));
}
