#ifndef MODELCONTAINER_P_H
#define MODELCONTAINER_P_H

#include "applicationcommand.h"
#include "modelcontainer.h"
#include "commandcontainer_p.h"

//#include <QtCore/QPersistentModelIndex>

namespace Parts {

class ModelContainerPrivate : public CommandContainerPrivate
{
    Q_DECLARE_PUBLIC(ModelContainer)
    Q_DISABLE_COPY(ModelContainerPrivate)
public:
    explicit ModelContainerPrivate(ModelContainer *qq) :
        CommandContainerPrivate(qq),
        model(0),
        column(0)
    {}

    void clear();
    void populate();
    void addCommand(const QModelIndex &index, AbstractCommand *before = 0);

public:
    QAbstractItemModel *model;
    QPersistentModelIndex rootIndex;
    int column;
    Separator *firstSeparator;
    Separator *lastSeparator;
};


class ModelCommand : public ApplicationCommand
{
public:
    explicit ModelCommand(QObject *parent) :
        ApplicationCommand(QByteArray::number(quintptr(this)), parent)
    {}
};

} // namespace Parts

#endif // MODELCONTAINER_P_H
