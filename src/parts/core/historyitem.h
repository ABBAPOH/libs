#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include "../parts_global.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtGui/QIcon>

namespace Parts {

class HistoryItemData;
class PARTS_EXPORT HistoryItem
{
public:
    HistoryItem(const QString &url = QString());
    HistoryItem(const HistoryItem &);
    HistoryItem &operator=(const HistoryItem &);
    ~HistoryItem();

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    bool isValid() const;

    QDateTime lastVisited() const;
    void setLastVisited(const QDateTime &visited);

    QString title() const;
    void setTitle(const QString &title);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QVariant userData(const QString &key) const;
    QVariantHash userData() const;
    void setUserData(const QString &key, const QVariant &data);

    bool operator==(const HistoryItem &other) const;

private:
    QSharedDataPointer<HistoryItemData> data;
};

} // namespace Parts

#endif // HISTORYITEM_H
