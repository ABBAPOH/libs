#include "menu.h"

#include <QtCore/QDebug>

#include "actionmanager.h"
#include "commandcontainer.h"

using namespace Parts;

/*!
    \class Parts::Menu

    This class is used to crete a menu populated with actions created by
    AbstractCommands.

    Menu stays in sync with the current CommandContainer and updates it's state
    when appropriate.
*/

/*!
    Creates a Menu with the given \a parent.
*/
Menu::Menu(QWidget *parent) :
    QMenu(parent)
{
}

/*!
    Returns current container.
*/
CommandContainer * Menu::container() const
{
    return m_container;
}

/*!
    Sets current container and populates menu.

    If \a container is 0, menu is cleared.
*/
void Menu::setContainer(CommandContainer *container)
{
    if (m_container == container)
        return;

    if (m_container) {
        clear();
        m_mapToAction.clear();
        disconnect(container, SIGNAL(commandInserted(AbstractCommand*,AbstractCommand*)),
                   this, SLOT(onCommandInserted(AbstractCommand*,AbstractCommand*)));
        disconnect(container, SIGNAL(commandRemoved(AbstractCommand*)),
                   this, SLOT(onCommandRemoved(AbstractCommand*)));
        disconnect(container, SIGNAL(changed()), this, SLOT(onContainerChanged()));
    }

    m_container = container;

    if (container) {
        onContainerChanged();
        populate();
        connect(container, SIGNAL(commandInserted(AbstractCommand*,AbstractCommand*)),
                this, SLOT(onCommandInserted(AbstractCommand*,AbstractCommand*)));
        connect(container, SIGNAL(commandRemoved(AbstractCommand*)),
                this, SLOT(onCommandRemoved(AbstractCommand*)));
        connect(container, SIGNAL(changed()), this, SLOT(onContainerChanged()));
    }
}

/*!
    Searches for the container with the \a containerId in an ActionManager
    and sets current container if found.
*/
void Menu::setContainer(const QByteArray &containerId)
{
    ActionManager *manager = ActionManager::instance();
    CommandContainer *container = manager->container(containerId);
    if (!container) {
        qWarning() << "Menu::setContainer" << "Can't find container with id" << containerId;
        return;
    }

    setContainer(container);
}

/*!
    \internal
*/
void Menu::onCommandInserted(AbstractCommand *command, AbstractCommand *commandBefore)
{
    QAction *action = command->createAction(this);
    m_mapToAction.insert(command, action);
    QAction *actionBefore = m_mapToAction.value(commandBefore);
    insertAction(actionBefore, action);
}

/*!
    \internal
*/
void Menu::onCommandRemoved(AbstractCommand *command)
{
    QAction *action = m_mapToAction.take(command);
    if (!action)
        return;
    removeAction(action);

    if (action->parent() == this)
        delete action;
}

/*!
    \internal
*/
void Menu::onContainerChanged()
{
    setTitle(m_container->text());
    setIcon(m_container->icon());
}

/*!
    \internal
*/
void Menu::populate()
{
    Q_ASSERT(m_container);
    foreach (AbstractCommand *command, m_container->commands()) {
        QAction *action = command->createAction(this);
        m_mapToAction.insert(command, action);
        addAction(action);
    }
}
