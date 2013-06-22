#ifndef FILEMANAGERWIDGET_P_H
#define FILEMANAGERWIDGET_P_H

#include "filemanagerwidget.h"

#include <QtCore/QMimeData>
#include <QtCore/QPointer>
#include <QtCore/QUrl>

#include <QtGui/QClipboard>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QApplication>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QListView>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QUndoCommand>
#include <QtWidgets/QUndoStack>
#else
#include <QtGui/QActionGroup>
#include <QtGui/QApplication>
#include <QtGui/QColumnView>
#include <QtGui/QFileSystemModel>
#include <QtGui/QListView>
#include <QtGui/QStackedLayout>
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QUndoCommand>
#include <QtGui/QUndoStack>
#endif

#include <FileManager/FileSystemManager>
#include <FileManager/FileSystemModel>
#include <FileManager/FileManagerModel>

namespace FileManager {

class FileManagerWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(FileManagerWidget)
    Q_DISABLE_COPY(FileManagerWidgetPrivate)
public:
    FileManagerWidgetPrivate(FileManagerWidget *qq) : QObject(), q_ptr(qq) {}

    void init();
    void setupUi();
    void createActions();
    void retranslateUi();

    void connectModel(FileManagerModel *model);
    void disconnectModel(FileManagerModel *model);
    void setFileSystemManager(FileSystemManager *manager);

    QModelIndexList selectedIndexes() const;

    void paste(bool copy = true);

    static bool hasFiles(const QStringList &paths);

    QAbstractItemView *createView(FileManagerWidget::ViewMode mode);
    QListView *createListView();
    QColumnView *createColumnView();
    QTreeView *createTreeView();
    QAbstractItemView *testCurrentView(FileManagerWidget::ViewMode mode);
    void setViewModel(QAbstractItemView *view, FileManagerModel *model);

    void updateListViewFlow(QListView *view);

public slots:
    void onUrlChanged(const QUrl &url);
    void onSortingChanged(int column, Qt::SortOrder order);
    void onActivated(const QModelIndex &index);
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void onFileSystemManagerChanged();

    void toggleViewMode(bool);
    void toggleSortColumn(bool);
    void toggleSortOrder(bool descending);
    void onSelectionChanged();

public:
    FileManagerWidget::ViewMode viewMode;
    QAbstractItemView * currentView;
    QSize iconSizes[FileManagerWidget::MaxViews];
    QStackedLayout * layout;

    inline FileSystemModel *itemModel() const { return model ? model->itemModel() : 0; }
    FileManagerModel *model;

    FileSystemManager *fileSystemManager;

    bool blockKeyEvent;
    QSize gridSize;
    FileManagerWidget::Flow flow;

    bool alternatingRowColors;
    bool itemsExpandable;
    bool showHiddenFiles;

    QAction *actions[FileManagerWidget::ActionCount];

    QActionGroup *viewModeGroup;
    QActionGroup *sortByGroup;

private:
    FileManagerWidget *q_ptr;
};

} // namespace FileManager

#endif // FILEMANAGERWIDGET_P_H
