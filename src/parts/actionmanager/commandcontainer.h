#ifndef COMMANDCONTAINER_H
#define COMMANDCONTAINER_H

#include "../parts_global.h"

#include "abstractcommand.h"

class QMenu;
class QMenuBar;
class QToolBar;

namespace Parts {

class ContextCommand;
class CommandContainerPrivate;
class PARTS_EXPORT CommandContainer : public AbstractCommand
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CommandContainer)
    Q_DISABLE_COPY(CommandContainer)

public:
    explicit CommandContainer(const QByteArray &id, QObject *parent = 0);
    ~CommandContainer();

    void addCommand(AbstractCommand *command, AbstractCommand *commandBefore = 0);
    void addSeparator(AbstractCommand *commandBefore = 0);
    void removeCommand(AbstractCommand *command);

    void clear();

    QList<AbstractCommand *> commands() const;

signals:
    void commandInserted(AbstractCommand *command, AbstractCommand *commandBefore);
    void commandRemoved(AbstractCommand *command);

private slots:
    void onDestroy(QObject *object);

protected:
    CommandContainer(CommandContainerPrivate &dd, const QByteArray &id, QObject *parent = 0);

    QAction *createAction(QObject *parent) const;
};

} // namespace Parts

#endif // COMMANDCONTAINER_H
