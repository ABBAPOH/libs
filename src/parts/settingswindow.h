#ifndef SettingsWindow_H
#define SettingsWindow_H

#include "parts_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QStackedLayout>
#else
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QStackedLayout>
#endif

namespace Parts {

class SettingsPage;
class SettingsPageManager;

class Category;
class CategoryModel;

class SettingsWindowPrivate;
class PARTS_EXPORT SettingsWindow : public QMainWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SettingsWindow)
    Q_DISABLE_COPY(SettingsWindow)

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    SettingsPageManager *settingsPageManager() const;
    void setSettingsPageManager(SettingsPageManager *manager);

    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);

private slots:
    void onSelectionChanged(const QItemSelection &current);
    void onPageAdded(SettingsPage *page);
    void onPageRemoved(SettingsPage *page);
    void onActionTriggered(bool toggled);

protected:
    SettingsWindowPrivate *d_ptr;
};

} // namespace Parts

#endif // SettingsWindow_H
