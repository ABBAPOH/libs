#include "commandcontainer.h"
#include "commandcontainer_p.h"

#include "actionmanager.h"
#include "command.h"
#include "separator.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#endif

using namespace Parts;

void CommandContainerPrivate::onTextChanged(const QString &text)
{
    if (menu)
        menu->setTitle(text);
}

void CommandContainerPrivate::onIconChanged(const QIcon &icon)
{
    if (menu)
        menu->setIcon(icon);
}

/*!
    \class Parts::CommandContainer

    \brief The CommandContainer is an abstraction over menus and toolbars which allows
    to store Commands.

    This class serves as a "model" for a Parts::Menu/Parts::MenuBar or a
    Parts::ToolBar. Those classes are wrappers around Qt classes that can
    monitor current container state and update actions.

    Typically, you should create a Container, fill it with Commands,
    instantiate one or many Menus and set container to them.
*/

/*!
    \brief Constructs CommandContainer with \a id and register it in ActionManager.
*/
CommandContainer::CommandContainer(const QByteArray &id, QObject *parent) :
    AbstractCommand(*new CommandContainerPrivate(this), id, parent)
{
    ActionManager::instance()->registerContainer(this);
}

/*
    \internal
*/
CommandContainer::CommandContainer(CommandContainerPrivate &dd, const QByteArray &id, QObject *parent) :
    AbstractCommand(dd, id, parent)
{
    ActionManager::instance()->registerContainer(this);
}

/*!
    \brief Destroys CommandContainer and unregister it in ActionManager.
*/
CommandContainer::~CommandContainer()
{
    ActionManager::instance()->unregisterContainer(this);
}

/*!
    \brief Adds \a command to a \a group.
*/
void CommandContainer::addCommand(AbstractCommand *command, AbstractCommand *commandBefore)
{
    if (!command)
        return;

    Q_D(CommandContainer);

    int index = d->commands.indexOf(commandBefore);
    if (index == -1)
        index = d->commands.count();

    d->commands.insert(index, command);

    QObject::connect(command, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroy(QObject*)));
    emit commandInserted(command, commandBefore);
}

void CommandContainer::addSeparator(AbstractCommand *commandBefore)
{
    addCommand(new Separator(this), commandBefore);
}

void CommandContainer::removeCommand(AbstractCommand *command)
{
    if (!command)
        return;

    onDestroy(command);
}

/*!
    \brief Destroys all gorups and remove added Commands.
*/
void CommandContainer::clear()
{
    Q_D(CommandContainer);

    foreach (AbstractCommand *c, d->commands) {
        emit commandRemoved(c);
        if (c->parent() == this)
            delete c;
    }
    d->commands.clear();
}

/*!
    \brief Returns list all Commands in this container.
*/
QList<AbstractCommand *> CommandContainer::commands() const
{
    Q_D(const CommandContainer);
    return d->commands;
}

/*!
    \internal
*/
void CommandContainer::onDestroy(QObject *object)
{
    Q_D(CommandContainer);

    AbstractCommand *command = static_cast<AbstractCommand *>(object);
    int index = d->commands.indexOf(command);
    d->commands.removeAt(index);
    emit commandRemoved(command);
}

/*!
    \reimp
*/
QAction * CommandContainer::createAction(QObject * /*parent*/) const
{
    Q_D(const CommandContainer);

    if (!d->menu) {
        d->menu = new Menu;
        d->menu->setContainer(const_cast<CommandContainer*>(this));
    }
    return d->menu->menuAction();
}
