#ifndef SETTTINGSPAGE_H
#define SETTTINGSPAGE_H

#include "parts_global.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QIcon>

class QIcon;

namespace Parts {

class PARTS_EXPORT SettingsPage : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SettingsPage)

public:
    SettingsPage(const QString &id, const QString &category, QObject *parent = 0);

    inline QString id() const { return m_id; }
    inline QString category() const { return m_category; }

    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;

    virtual QString categoryName() const { return QString(); }
    virtual QIcon categoryIcon() const { return QIcon(); }

    virtual QWidget *createPage(QWidget *parent) = 0;

private:
    QString m_id;
    QString m_category;
};

} // namespace Parts

#endif // SETTTINGSPAGE_H
