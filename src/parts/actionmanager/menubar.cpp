#include "menubar.h"

#include <QtCore/QDebug>

#include "actionmanager.h"
#include "commandcontainer.h"

using namespace Parts;

/*!
    \class Parts::MenuBar

    This class is used to crete a menu bar populated with actions created by
    AbstractCommands.

    MenuBar stays in sync with the current CommandContainer and updates it's
    state when appropriate.
*/

/*!
    Creates a MenuBar with the given \a parent.
*/
MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent),
    m_container(0)
{
}

/*!
    Returns current container.
*/
CommandContainer * MenuBar::container() const
{
    return m_container;
}

/*!
    Sets current container and populates menu.

    If \a container is 0, menu bar is cleared.
*/
void MenuBar::setContainer(CommandContainer *container)
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
    }

    m_container = container;

    if (container) {
        populate();
        connect(container, SIGNAL(commandInserted(AbstractCommand*,AbstractCommand*)),
                this, SLOT(onCommandInserted(AbstractCommand*,AbstractCommand*)));
        connect(container, SIGNAL(commandRemoved(AbstractCommand*)),
                this, SLOT(onCommandRemoved(AbstractCommand*)));
    }
}

/*!
    Searches for the container with the \a containerId in an ActionManager
    and sets current container if found.
*/
void MenuBar::setContainer(const QByteArray &containerId)
{
    ActionManager *manager = ActionManager::instance();
    CommandContainer *container = manager->container(containerId);
    if (!container) {
        qWarning() << "MenuBar::setContainer" << "Can't find container with id" << containerId;
        return;
    }

    setContainer(container);
}

/*!
    \internal
*/
void MenuBar::onCommandInserted(AbstractCommand *command, AbstractCommand *commandBefore)
{
    QAction *action = command->createAction(this);
    m_mapToAction.insert(command, action);
    QAction *actionBefore = m_mapToAction.value(commandBefore);
    insertAction(actionBefore, action);
}

/*!
    \internal
*/
void MenuBar::onCommandRemoved(AbstractCommand *command)
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
void MenuBar::populate()
{
    Q_ASSERT(m_container);
    foreach (AbstractCommand *command, m_container->commands()) {
        QAction *action = command->createAction(this);
        m_mapToAction.insert(command, action);
        addAction(action);
    }
}
