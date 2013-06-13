#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include "../extensionsystem_global.h"

#include <QtCore/QModelIndex>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

namespace Ui {
    class PluginView;
}

namespace ExtensionSystem {

class FullPluginView;

class EXTENSIONSYSTEM_EXPORT PluginView : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(PluginView)

public:
    explicit PluginView(QWidget *parent = 0);
    ~PluginView();

private:
    Ui::PluginView *ui;

private slots:
    void showFullInfo(const QModelIndex &index = QModelIndex());
    void onSelectionChanged();
};

} // namespace ExtensionSystem

#endif // PLUGINVIEW_H
