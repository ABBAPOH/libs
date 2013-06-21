#include "toolbar.h"

#include <QtCore/QDebug>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include "actionmanager.h"
#include "commandcontainer.h"

using namespace Parts;

/*!
    \class Parts::ToolBar

    This class is used to crete a tool bar populated with actions created by
    AbstractCommands.

    ToolBar stays in sync with the current CommandContainer and updates it's
    state when appropriate.
*/

/*!
    Creates a ToolBar with the given \a parent.
*/
ToolBar::ToolBar(QWidget *parent) :
    QToolBar(parent),
    m_container(0)
{
}

/*!
    Returns current container.
*/
CommandContainer * ToolBar::container() const
{
    return m_container;
}

/*!
    Sets current container and populates menu.

    If \a container is 0, tool bar is cleared.
*/
void ToolBar::setContainer(CommandContainer *container)
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
void ToolBar::setContainer(const QByteArray &containerId)
{
    ActionManager *manager = ActionManager::instance();
    CommandContainer *container = manager->container(containerId);
    if (!container) {
        qWarning() << "ToolBar::setContainer" << "Can't find container with id" << containerId;
        return;
    }

    setContainer(container);
}

/*!
    \internal
*/
void ToolBar::onCommandInserted(AbstractCommand *command, AbstractCommand *commandBefore)
{
    QAction *action = command->createAction(this);
    m_mapToAction.insert(command, action);
    QAction *actionBefore = m_mapToAction.value(commandBefore);
    insertAction(actionBefore, action);
}

/*!
    \internal
*/
void ToolBar::onCommandRemoved(AbstractCommand *command)
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
void ToolBar::populate()
{
    Q_ASSERT(m_container);
    foreach (AbstractCommand *command, m_container->commands()) {
        QAction *action = command->createAction(this);
        m_mapToAction.insert(command, action);
        addAction(action);
    }
}
