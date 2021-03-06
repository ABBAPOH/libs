#ifndef BOOKMARKSWIDGET_P_H
#define BOOKMARKSWIDGET_P_H

#include "bookmarkswidget.h"

#if QT_VERSION >= 0x050000
#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QAction>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QSplitter>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#endif

#include <Bookmarks/BookmarksModel>
#include <Bookmarks/BookmarksWidget>

namespace Bookmarks {

class FolderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FolderProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent) {}

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        QAbstractItemModel *source = sourceModel();
        if (!source)
            return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

        BookmarksModel *model = static_cast<BookmarksModel*>(source);
        QModelIndex index = model->index(source_row, 0, source_parent);
        return model->isFolder(index);
    }

    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
    {
        if (source_column == 0)
            return QSortFilterProxyModel::filterAcceptsColumn(source_column, source_parent);

        return false;
    }
};

class BookmarkListFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    BookmarkListFilterModel(QObject *parent = 0) :
        QSortFilterProxyModel(parent)
    {}

    QModelIndex rootIndex() const { return m_rootIndex; }
    void setRootIndex(const QModelIndex &index) { m_rootIndex = index; invalidateFilter(); }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);
        QModelIndex pidx = m_rootIndex;
        while (pidx.isValid()) {
            if (pidx == idx)
                return true;
            pidx = pidx.parent();
        }

        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }

    QPersistentModelIndex m_rootIndex;
};

class BookmarksWidgetPrivate
{
    Q_DECLARE_PUBLIC(BookmarksWidget)
    Q_DISABLE_COPY(BookmarksWidgetPrivate)

public:
    explicit BookmarksWidgetPrivate(BookmarksWidget *qq) : q_ptr(qq) {}

    QVBoxLayout *mainLayout;
    QToolBar *toolBar;

    // tool bar
    QWidget *spacer;
    QLineEdit *lineEdit;

    // splitter
    QTreeView *tableView;

    BookmarksModel *model;
    BookmarkListFilterModel *proxyModel;

    // actions

    QAction *actions[BookmarksWidget::ActionCount];

public:
    void init();
    void createActions();
    void createWidgets();
    void retranslateUi();

protected:
    BookmarksWidget *q_ptr;
};

} // namespace Bookmarks

#endif // BOOKMARKSWIDGET_P_H
