#ifndef QNAVIGATIONPANEL_H
#define QNAVIGATIONPANEL_H

#include "filemanager_global.h"

#include <QtCore/QHash>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class QModelIndex;
class QTreeView;
class QTreeWidget;
class QTreeWidgetItem;

namespace FileManager {

class NavigationModel;

class NavigationPanelPrivate;
class FILEMANAGER_EXPORT NavigationPanel : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NavigationPanel)
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
public:
    explicit NavigationPanel(QWidget *parent = 0);
    ~NavigationPanel();

    void addFolder(const QString & path);
    void removeFolder(const QString & path);

    QString currentPath() const;

    NavigationModel *model() const;
    void setModel(NavigationModel *model);

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void currentPathChanged(const QString & path);
    void triggered(const QString & path);

private slots:
    void onClick(const QModelIndex &index);
    void onCustomContextMenuRequested(QPoint p);
    void onOpenTriggered();
    void onRemoveTriggered();

protected:
    NavigationPanelPrivate *d_ptr;
};

} // namespace FileManager

#endif // QNAVIGATIONPANEL_H
