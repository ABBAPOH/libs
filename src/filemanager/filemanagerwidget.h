#ifndef FILEMANAGERWIDGET_H
#define FILEMANAGERWIDGET_H

#include "filemanager_global.h"

#include <QtCore/QDir>
#include <QtCore/QUrl>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class QAbstractItemView;
class QFileSystemModel;
class QMenu;

namespace FileManager {

class FileManagerHistory;
class FileSystemUndoManager;
class FileSystemManager;
class FileManagerModel;

class FileManagerWidgetPrivate;
class FILEMANAGER_EXPORT FileManagerWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FileManagerWidget)
    Q_DISABLE_COPY(FileManagerWidget)

    Q_PROPERTY(bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(Flow flow READ flow WRITE setFlow NOTIFY flowChanged)
    Q_PROPERTY(QSize gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
    Q_PROPERTY(QSize iconSizeColumn READ iconSizeColumn WRITE setIconSizeColumn NOTIFY iconSizeColumnChanged)
    Q_PROPERTY(QSize iconSizeTree READ iconSizeTree WRITE setIconSizeTree NOTIFY iconSizeTreeChanged)
    Q_PROPERTY(bool itemsExpandable READ itemsExpandable WRITE setItemsExpandable NOTIFY itemsExpandableChanged)
    Q_PROPERTY(QList<QUrl> selectedUrls READ selectedUrls NOTIFY selectedPathsChanged)
    Q_PROPERTY(ViewMode viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)

public:
    enum ViewMode { IconView = 0, ColumnView, TreeView, MaxViews };
    enum Flow { LeftToRight = 0, TopToBottom = 1 };
    enum Action { NoAction = -1,
                  Open,
                  NewFolder,
                  Rename,
                  MoveToTrash,
                  Remove,
                  ShowFileInfo,

                  Redo,
                  Undo,
                  Cut,
                  Copy,
                  Paste,
                  MoveHere,
                  SelectAll,
                  ShowHiddenFiles,

                  IconMode,
                  ColumnMode,
                  TreeMode,

                  SortByName,
                  SortBySize,
                  SortByType,
                  SortByDate,
                  SortDescendingOrder,

                  ActionCount
                };

    Q_ENUMS(Flow)
    Q_ENUMS(ViewMode)
    Q_ENUMS(Action)

    explicit FileManagerWidget(QWidget *parent = 0);
    ~FileManagerWidget();

    QAction *action(Action action) const;

    bool alternatingRowColors() const;
    void setAlternatingRowColors(bool);

    QUrl url() const;

    Flow flow() const;
    void setFlow(Flow flow);

    QSize gridSize() const;
    void setGridSize(QSize s);

    QSize iconSize() const;
    void setIconSize(QSize size);

    QSize iconSizeColumn() const;
    void setIconSizeColumn(QSize size);

    QSize iconSizeTree() const;
    void setIconSizeTree(QSize size);

    QSize iconSize(FileManagerWidget::ViewMode mode) const;
    void setIconSize(FileManagerWidget::ViewMode mode, QSize size);

    bool itemsExpandable() const;

    QList<QUrl> selectedUrls() const;

    ViewMode viewMode() const;
    void setViewMode(ViewMode mode);

    FileManagerModel *model() const;
    void setModel(FileManagerModel *model);

    bool restoreState(const QByteArray &state);
    QByteArray saveState() const;

    void clear();

    QMenu *createStandardMenu(const QList<QUrl> &urls);

signals:
    void urlChanged(const QUrl &url);
    void flowChanged(Flow flow);
    void gridSizeChanged(QSize size);
    void iconSizeChanged(QSize size);
    void iconSizeColumnChanged(QSize size);
    void iconSizeTreeChanged(QSize size);
    void itemsExpandableChanged(bool expandable);
    void selectedPathsChanged();
    void viewModeChanged(FileManagerWidget::ViewMode mode);

    void openRequested(const QList<QUrl> &urls, Qt::KeyboardModifiers modifiers);

public slots:
    void setUrl(const QUrl &url);

    void newFolder();
    void open();
    void showFileInfo();
    void remove();
    void rename();
    void moveToTrash();

    void undo();
    void redo();
    void copy();
    void paste();
    void moveHere();
    void selectAll();

    void setItemsExpandable(bool expandable);
    void showContextMenu(QPoint pos);

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    FileManagerWidgetPrivate *d_ptr;
};

} // namespace FileManager

#endif // FILEMANAGERWIDGET_H
