#ifndef MODELCONTAINER_H
#define MODELCONTAINER_H

#include "commandcontainer.h"

#include <QtCore/QModelIndex>

class QAbstractItemModel;

namespace Parts {

class Separator;

class ModelContainerPrivate;
class PARTS_EXPORT ModelContainer : public CommandContainer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModelContainer)
    Q_DISABLE_COPY(ModelContainer)

public:
    explicit ModelContainer(const QByteArray &id, QObject *parent = 0);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model, const QModelIndex &rootIndex = QModelIndex());

    QModelIndex rootIndex() const;
    void setRootIndex(const QModelIndex &rootIndex);

    Separator *firstSeparator() const;

signals:
    void triggered(const QModelIndex &index);

protected slots:
    void onModelReset();
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onCommandTriggered();
};

} // namespace Parts

#endif // MODELCONTAINER_H
