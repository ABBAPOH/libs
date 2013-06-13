#ifndef SETTINGSPAGEMANAGER_H
#define SETTINGSPAGEMANAGER_H

#include "parts_global.h"

#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace Parts {

class SettingsPage;

class SettingsPageManagerPrivate;
class PARTS_EXPORT SettingsPageManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SettingsPageManager)
    Q_DISABLE_COPY(SettingsPageManager)

public:
    explicit SettingsPageManager(QObject *parent = 0);
    ~SettingsPageManager();

    SettingsPage *page(const QString &id) const;
    void addPage(SettingsPage* page);
    void removePage(SettingsPage* page);

    QStringList categories() const;

    QList<SettingsPage *> pages() const;
    QList<SettingsPage *> pages(const QString &category) const;

signals:
    void pageAdded(SettingsPage *);
    void pageRemoved(SettingsPage *);

private slots:
    void onDestroyed(QObject *o);

protected:
    SettingsPageManagerPrivate *d_ptr;
};

} // namespace Parts

#endif // SETTINGSPAGEMANAGER_H
