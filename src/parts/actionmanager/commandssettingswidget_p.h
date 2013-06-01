#ifndef COMMANDSSETTINGSWIDGET_P_H
#define COMMANDSSETTINGSWIDGET_P_H

#include "commandssettingswidget.h"

#if QT_VERSION >= 0x050000
#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QStyledItemDelegate>
#else
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QStyledItemDelegate>
#endif

namespace Parts {

class FolderProxyModel : public QSortFilterProxyModel
{
public:
    explicit FolderProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

class ShortcutDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ShortcutDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool eventFilter(QObject *object, QEvent *event);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private slots:
    void onFinishShortcut();
};

} // namespace Parts

#endif // COMMANDSSETTINGSWIDGET_P_H
