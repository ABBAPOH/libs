#ifndef BOOKMARKSWIDGET_H
#define BOOKMARKSWIDGET_H

#include "../bookmarks_global.h"

#include <QtCore/QModelIndex>
#include <QtCore/QUrl>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class QMenu;
class QUrl;

namespace Bookmarks {

class BookmarksModel;

class BookmarksWidgetPrivate;
class BOOKMARKS_EXPORT BookmarksWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BookmarksWidget)
    Q_DISABLE_COPY(BookmarksWidget)

public:
    enum Action {
        ActionNone = -1,
        ActionOpen = 0,
        ActionUp,
        ActionRename,
        ActionEditUrl,
        ActionEditDescrition,
        ActionNewFolder,
        ActionRemove,
        ActionCount // should be last
    };

    explicit BookmarksWidget(QWidget *parent = 0);
    ~BookmarksWidget();

    QAction *action(Action action) const;

    BookmarksModel *model() const;
    void setModel(BookmarksModel *model);

    QModelIndexList selectedIndexes() const;

    bool restoreState(const QByteArray &state);
    QByteArray saveState() const;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    QMenu *createStandardContextMenu() const;

    static QList<QUrl> urlsForIndexes(const QModelIndexList &indexes);

public slots:
    void addFolder();
    void up();
    void rename();
    void editUrl();
    void editDescription();
    void remove();

signals:
    void openRequested(const QList<QUrl> &urls);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void onActivated(const QModelIndex &index);
    void openTriggered();

private:
    BookmarksWidgetPrivate *d_ptr;
};

} // namespace Bookmarks

#endif // BOOKMARKSWIDGET_H
