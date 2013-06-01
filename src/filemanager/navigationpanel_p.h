#ifndef NAVIGATIONPANEL_P_H
#define NAVIGATIONPANEL_P_H

#include "navigationmodel.h"

#include <QtCore/QEvent>
#include <QtCore/QRect>

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QTreeView>
#else
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtGui/QTreeView>
#endif

#include <IO/QDriveController>
#include <IO/QDriveInfo>

#define bublic public

namespace FileManager {

class NavigationPanelPrivate
{
bublic:
    QTreeView *treeView;
    NavigationModel *model;
    QString currentPath;

    QAction *openAction;
    QAction *removeAction;
    QMenu *contextMenu;

    QModelIndex selectedRow() const;
};

class NavigationPanelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    QIcon m_ejectIcon;

public:
    explicit NavigationPanelDelegate(QObject * parent = 0);
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

} // namespace FileManager

#endif // NAVIGATIONPANEL_P_H
