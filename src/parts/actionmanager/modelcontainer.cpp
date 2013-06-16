#include "modelcontainer.h"
#include "modelcontainer_p.h"

#include "separator.h"

using namespace Parts;

void ModelContainerPrivate::clear()
{
    Q_Q(ModelContainer);

    int from = commands.indexOf(firstSeparator);
    int till = commands.count();
    for (int i = from + 1; i < till; ++i) {
        AbstractCommand * cmd = commands.at(from + 1);
        q->removeCommand(cmd);
    }
}

void ModelContainerPrivate::populate()
{
    QModelIndex root = rootIndex;

    for (int row = 0; row < model->rowCount(root); ++row) {
        QModelIndex index = model->index(row, column, root);
        addCommand(index, lastSeparator);
    }
}

void ModelContainerPrivate::addCommand(const QModelIndex &index, AbstractCommand *before)
{
    Q_Q(ModelContainer);

    ModelCommand *cmd = new ModelCommand(q);
    cmd->setText(index.data(Qt::DisplayRole).toString());
    cmd->setIcon(index.data(Qt::DecorationRole).value<QIcon>());
    QObject::connect(cmd, SIGNAL(triggered()), q, SLOT(onCommandTriggered()));
    q->addCommand(cmd, before);
}

/*!
    \class Parts::ModelContainer

    ModelContainer is a container that is populated based on a
    QAbstractItemModel data.
*/

/*!
    Creates a ModelContainer with the given \a parent.
*/
ModelContainer::ModelContainer(const QByteArray &id, QObject *parent) :
    CommandContainer(*new ModelContainerPrivate(this), id, parent)
{
    Q_D(ModelContainer);
    d->firstSeparator = new Separator(this);
    d->firstSeparator->setVisible(false);
    addCommand(d->firstSeparator);

    d->lastSeparator = new Separator(this);
    d->lastSeparator->setVisible(false);
    addCommand(d->lastSeparator);
}

/*!
    Returns current model.
*/
QAbstractItemModel * ModelContainer::model() const
{
    Q_D(const ModelContainer);
    return d->model;
}

/*!
    Sets current \a model and \a rootIndex.

    Container is populated by Commands created with model's data.
*/
void ModelContainer::setModel(QAbstractItemModel *model, const QModelIndex &rootIndex)
{
    Q_D(ModelContainer);

    if (d->model == model && d->rootIndex == rootIndex)
        return;

    d->model = model;
    d->rootIndex = rootIndex;

    if (model) {
        connect(model, SIGNAL(modelReset()), this, SLOT(onModelReset()));
        connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(onRowsInserted(QModelIndex,int,int)));
        connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(onRowsRemoved(QModelIndex,int,int)));
    }
    onModelReset();
}

/*!
    Returns current root index of the model.
*/
QModelIndex ModelContainer::rootIndex() const
{
    Q_D(const ModelContainer);
    return d->rootIndex;
}

/*!
    Sets current root index and repopulates containter.

    \note For performance reasons, it is better to set both model and
    root index using setModel() method.
*/
void ModelContainer::setRootIndex(const QModelIndex &rootIndex)
{
    Q_D(ModelContainer);

    if (d->rootIndex == rootIndex)
        return;
    if (!d->model)
        return;
    onModelReset();
}

/*!
    Returns pointer to the separator placed before Commands created with
    model data.

    You can insert actions before this Seprator. To add actions after the model
    commands, simply insert new commands at the end.

    By default, separator is invisible.
*/
Separator * ModelContainer::firstSeparator() const
{
    Q_D(const ModelContainer);
    return d->firstSeparator;
}

/*!
    \internal
*/
void ModelContainer::onModelReset()
{
    Q_D(ModelContainer);
    d->clear();
    d->populate();
}

/*!
    \internal
*/
void ModelContainer::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_D(ModelContainer);

    if (d->rootIndex != parent)
        return;

    AbstractCommand *before = d->commands.count() > last ? d->commands.at(last) : 0;
    for (int row = first; row <= last; ++row) {
        QModelIndex index = d->model->index(row, d->column, parent);
        d->addCommand(index, before);
    }
}

/*!
    \internal
*/
void ModelContainer::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_D(ModelContainer);

    if (d->rootIndex != parent)
        return;

    QList<AbstractCommand *> toRemove = d->commands.mid(first, last - first);
    foreach (AbstractCommand *cmd, toRemove) {
        removeCommand(cmd);
    }
}

/*!
    \internal
*/
void ModelContainer::onCommandTriggered()
{
    Q_D(ModelContainer);

    ApplicationCommand *cmd = qobject_cast<ApplicationCommand *>(sender());
    int row = d->commands.indexOf(cmd);
    QModelIndex index = d->model->index(row, d->column, d->rootIndex);
    emit triggered(index);
}
